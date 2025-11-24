/*
 * fuelcell_init.c
 *
 *  Created on: Nov 7, 2025
 *      Author: User
 */

#include "fuelcell_can.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

extern FullCell_t fuelcell;


FullCell_t fuelcell_dummy;

/* prototype function
 */

static void fc_can_pack_measurements(FullCell_t *fc, uint8_t *data_buf);
static void fc_can_pack_status(FullCell_t *fc, uint8_t *data_buf);
static void fc_can_parse_command(FullCell_t *fc, uint8_t *data_buf);

//CAN_RxHeaderTypeDef RxHeader;
//uint8_t RxData[8];
//
//void lcancallback(CAN_HandleTypeDef *hcan) {
//	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData) != HAL_OK) {
//		Error_Handler();
//	}
//}
//
//void gcancallback(CAN_HandleTypeDef *hcan) {
//	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
//		Error_Handler();
//	}
//}

/* helper: encode a 29-bit EXT ID into FilterIdHigh/Low for 32-bit filter registers.
 Based on common STM32 encoding: see reference / community notes.
 - id must be 29-bit (0 .. 0x1FFFFFFF)
 - returns high/low 16-bit halves for CAN_FilterTypeDef
 */

/* Initialize CAN1 (global) and CAN2 (local).
 - CAN1 filter set to accept only extended ID 0x0CF11200 (exact match).
 - CAN2 filter kept as "accept all" (your original mask).
 */

HAL_StatusTypeDef fc_can_init(void) {
	HAL_StatusTypeDef ret;
	CAN_FilterTypeDef sFilterConfig;

	/*

	Rumus 32-bit untuk filter ID adalah: Nilai_Filter_32bit = (ID_Extended << 3) | (Bit_IDE << 2) | (Bit_RTR << 1)

	Bit_IDE: 1 (Karena ini adalah Extended ID, bit ini harus 1)
	Bit_RTR: 0 (Karena kita memfilter Data Frame, bukan Remote Frame. Bit ini harus 0)

	Langkah 1: Geser ID (Shift)
	Pertama, kita geser ID 29-bit ke kiri sebanyak 3 bit:

	0x0CF11200 << 3

	Hasilnya adalah: 0x67889000

	Langkah 2: Tambahkan Bit Status
	Sekarang, kita gabungkan (OR) hasil di atas dengan bit status:

	Nilai ID yang digeser: 0x67889000
	Nilai Bit IDE: 1 << 2 = 4 (atau 0x4)
	Nilai Bit RTR: 0 << 1 = 0 (atau 0x0)
	0x67889000 | 0x4 | 0x0 = 0x67889004

	Langkah 3: Bagi Menjadi Register High & Low
	Nilai 0x67889004 ini adalah nilai 32-bit yang akan dilihat oleh filter. Sekarang, kita bagi menjadi dua register 16-bit:
	sFilterConfig.FilterIdHigh (16 bit atas): 0x6788
	sFilterConfig.FilterIdLow (16 bit bawah): 0x9004

	*/

	uint16_t id_high_1 = 0x6788;
	uint16_t id_low_1 = 0x9004;

	sFilterConfig.FilterBank = 0; // Menggunakan bank 0
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = id_high_1;
	sFilterConfig.FilterIdLow = id_low_1;
	sFilterConfig.FilterMaskIdHigh = 0xFFFF; // Mask presisi
	sFilterConfig.FilterMaskIdLow = 0xFFFC; // Mask presisi (abaikan RTR)
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; // Kirim ke FIFO 0
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14; // Titik pemisah antara CAN1 dan CAN2

	ret = HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	if (ret != HAL_OK)
		return ret;

	sFilterConfig.FilterBank = 14; /* next bank (avoid overlap) */
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000; /* mask 0 => accept all */
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	ret = HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
	if (ret != HAL_OK)
		return ret;

	ret = HAL_CAN_Start(&hcan1);
	if (ret != HAL_OK)
		return ret;

	ret = HAL_CAN_Start(&hcan2);
	if (ret != HAL_OK)
		return ret;

	/* enable RX notification for CAN1 (optional) */
	ret = HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	if (ret != HAL_OK)
		return ret;

	/* enable RX notification for CAN2 (optional) */
	ret = HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);
	if (ret != HAL_OK)
		return ret;

	return ret;
}



CAN_TxHeaderTypeDef TxHeader_Status;
CAN_TxHeaderTypeDef TxHeader_Data;

uint8_t TxData_Data[8];
uint8_t TxData_Status[8];

uint32_t TxMailbox1;
uint32_t TxMailbox2;

void fc_can_gcan_transmit() {

	TxHeader_Status.ExtId = 0x18F11381;
	TxHeader_Status.IDE = CAN_ID_EXT;
	TxHeader_Status.RTR = CAN_RTR_DATA;
	TxHeader_Status.DLC = 8;

	fc_can_pack_status(&fuelcell, TxData_Status);
//	fc_can_pack_status(&fuelcell_dummy, TxData_Status);


	TxHeader_Data.ExtId = 0x18F11481;
	TxHeader_Data.IDE = CAN_ID_EXT;
	TxHeader_Data.RTR = CAN_RTR_DATA;
	TxHeader_Data.DLC = 8;

	fc_can_pack_measurements(&fuelcell, TxData_Data);
//	fc_can_pack_measurements(&fuelcell_dummy, TxData_Data);


	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader_Status, TxData_Status,
			&TxMailbox1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader_Data, TxData_Data, &TxMailbox2)
			!= HAL_OK) {
		Error_Handler();
	}

}

CAN_RxHeaderTypeDef local_RxHeader;
uint8_t local_RxData[8];

CAN_RxHeaderTypeDef global_RxHeader;
uint8_t global_RxData[8];

FloatPack_t lcan_data;


volatile uint32_t g_last_can_rx_time = 0;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &global_RxHeader,
			global_RxData) != HAL_OK) {
		Error_Handler();
	}
	if ((global_RxHeader.ExtId == 0x0CF11200)) {
		fc_can_parse_command(&fuelcell, global_RxData);
//		fc_can_parse_command(&fuelcell_dummy, global_RxData);

		g_last_can_rx_time = HAL_GetTick();
	}
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &local_RxHeader, local_RxData)
			!= HAL_OK) {
		Error_Handler();
	}
	if ((local_RxHeader.StdId == 0x111)) {
		lcan_data.bytes[0] = local_RxData[0];
		lcan_data.bytes[1] = local_RxData[1];
		lcan_data.bytes[2] = local_RxData[2];
		lcan_data.bytes[3] = local_RxData[3];
	}
}

static void fc_can_pack_status(FullCell_t *fc, uint8_t *data_buf) {
	if (fc == NULL || data_buf == NULL)
		return;

	// 1. Inisialisasi byte status ke 0
	uint8_t status_byte = 0;

	// 2. Set bit-bit berdasarkan spesifikasi

	/* Bit 0: Controller running/disabled (0=running, 1=disabled) */
	// Ini adalah logika terbalik: JIKA state-nya *bukan* RUNNING,
	// maka kita set bit-nya ke 1 (disabled).
	if (fc->operating_status != 0) {
		status_byte |= (1 << 0); // Set Bit 0 (Disabled)
	}

	/* Bit 1: Over current error (0=no error, 1=error) */
	if (fc->fault_over_current != 0) {
		status_byte |= (1 << 1); // Set Bit 1 (Error)
	}

	/* Bit 2: Over voltage error (0=no error, 1=error) */
	if (fc->fault_over_voltage != 0) {
		status_byte |= (1 << 2); // Set Bit 2 (Error)
	}

	/* Bit 3: Over temperature error (0=no error, 1=error) */
	if (fc->fault_over_temperature != 0) {
		status_byte |= (1 << 3); // Set Bit 3 (Error)
	}

	// Bit 4-7 tidak digunakan (range 0 to 0x0F)

	// 3. Tulis hasil akhir ke buffer
	data_buf[0] = status_byte;

	// 4. Byte 1-7: unimplemented (diisi 0x00)
	data_buf[1] = 0x00;
	data_buf[2] = 0x00;
	data_buf[3] = 0x00;
	data_buf[4] = 0x00;
	data_buf[5] = 0x00;
	data_buf[6] = 0x00;
	data_buf[7] = 0x00;
}

/**
 * @brief Mengemas data pengukuran (0x18F11481) ke dalam buffer 8-byte.
 */
static void fc_can_pack_measurements(FullCell_t *fc, uint8_t *data_buf) {
	if (fc == NULL || data_buf == NULL)
		return;

	// Byte 0-1: Tegangan (word)
	// Resolusi: 0.01 V per bit, tanpa offset
	// Raw = Physical / 0.01
	uint16_t raw_voltage = (uint16_t) (fc->voltage / 0.01f);
	data_buf[0] = (uint8_t) (raw_voltage & 0xFF); // LSB
	data_buf[1] = (uint8_t) (raw_voltage >> 8);   // MSB

	// Byte 2-3: Arus (word)
	// Resolusi: 0.1 A per bit, offset 3212.7 A
	// Raw = (Physical + 3212.7) / 0.1
	uint16_t raw_current = (uint16_t) ((fc->current + 3212.7f) / 0.1f);
	data_buf[2] = (uint8_t) (raw_current & 0xFF); // LSB
	data_buf[3] = (uint8_t) (raw_current >> 8);   // MSB


	// Byte 4: Kapasitas Hidrogen
	// Format: 0-200, Resolusi: 0.5%
	// fc->tank_pressure adalah 0-200 bar

	// Logika: 0-200 bar -> 0-100% (Fisik) -> 0-200 (Raw)
	// 1. Ubah bar (0-200) ke persen (0-100)
	float physical_percent = (fc->tank_pressure / 200.0f) * 100.0f;

	// 2. Ubah persen (0-100) ke raw (0-200)
	// Rumus: Raw = Physical * 2
	uint8_t raw_h2 = (uint8_t)(physical_percent * 2.0f);
	data_buf[4] = raw_h2;

	// Byte 5: Temperature
	// Format: 0-240, Resolusi: 1 C per bit, offset -40 C
	// Raw = Physical + 40
	uint8_t raw_temp = (uint8_t) (fc->temp[0] + 40.0f);
	data_buf[5] = raw_temp;

	// Byte 6-7: unimplemented (diisi 0x00)
	data_buf[6] = 0x00;
	data_buf[7] = 0x00;
}

static void fc_can_parse_command(FullCell_t *fc, uint8_t *data_buf) {
	if (fc == NULL || data_buf == NULL)
		return;

	fc->enable_command = data_buf[0] & 0x01; // (Masking untuk keamanan)
}

int fc_can_check_timeout(void)
{
	if (HAL_GetTick() - g_last_can_rx_time > CAN_TIMEOUT_MS) {
			return 1;
		}
	return 0;
}
