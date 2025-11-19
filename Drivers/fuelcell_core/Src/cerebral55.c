#include "cerebral55.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// helper: find first numeric token in string and parse to float (simple)
static float parse_first_number(const char *s){
  while(*s && *s!='+' && *s!='-' && !isdigit((unsigned char)*s)) s++;
  if(!*s) return 0.0f;
  double sign = 1.0;
  if(*s=='+'||*s=='-'){ if(*s=='-') sign=-1.0; s++; }
  long long ip = 0;
  while(isdigit((unsigned char)*s)){ ip = ip*10 + (*s - '0'); s++; }
  double val = (double)ip;
  if(*s=='.'){
    s++;
    double place = 0.1;
    while(isdigit((unsigned char)*s)){
      val += (*s - '0') * place;
      place *= 0.1;
      s++;
    }
  }
  return (float)(sign * val);
}

static float parse_after_colon(const char *token){
  const char *p = strchr(token, ':');
  if(!p) return 0.0f;
  return parse_first_number(p+1);
}

static void parse_operational_payload(const char *payload, Cerebral55_OperatingData_t *out){
  // zero fields first
  memset(out, 0, sizeof(*out));
  // copy into local buffer because strtok modifies
  char local[C55_BUF_SIZE];
  strncpy(local, payload, sizeof(local)-1);
  local[sizeof(local)-1] = '\0';

  char *saveptr = NULL;
  char *tok = strtok_r(local, "|", &saveptr);
  while(tok){
    while(*tok && isspace((unsigned char)*tok)) tok++;
    if(strncmp(tok, "V_in", 4)==0) out->V_in = parse_after_colon(tok);
    else if(strstr(tok, "I_in")) out->I_in = parse_after_colon(tok);
    else if(strstr(tok, "P_in")) out->P_in = parse_after_colon(tok);
    else if(strstr(tok, "Temp")) out->Temp = parse_after_colon(tok);
    else if(strstr(tok, "V_out")) out->V_out = parse_after_colon(tok);
    else if(strstr(tok, "I_out")) out->I_out = parse_after_colon(tok);
    else if(strstr(tok, "P_out")) out->P_out = parse_after_colon(tok);
    else if(strstr(tok, "V_set")) out->V_set = parse_after_colon(tok);
    else if(strstr(tok, "I_set")) out->I_set = parse_after_colon(tok);
    else if(strstr(tok, "Ramp")){
      const char *p = strchr(tok, ':');
      if(p){
        p++;
        while(*p && isspace((unsigned char)*p)) p++;
        out->RampBit = atoi(p);
        const char *c = strchr(p, ',');
        if(c) out->RampRate = parse_first_number(c+1);
      }
    }
    tok = strtok_r(NULL, "|", &saveptr);
  }
}

void Cerebral55_Init(Cerebral55_Handle_t *h){
  if(!h) return;
  memset(h, 0, sizeof(*h));
  h->state = CEREBRAL55_STATE_UNKNOWN;
  h->idx = 0;
  h->prev = 0;
  h->in_msg = 0;
}

bool Cerebral55_ProcessByte(Cerebral55_Handle_t *h, uint8_t b){
  if(!h) return false;

  // waiting for start sequence: previous == '$' and now one of '@','^','%'
  if(!h->in_msg){
    if(h->prev == '$' && (b == '@' || b == '^' || b == '%')){
      // begin message; store start two chars
      h->in_msg = 1;
      h->idx = 0;
      if(h->idx < C55_BUF_SIZE-1) h->buf[h->idx++] = '$';
      if(h->idx < C55_BUF_SIZE-1) h->buf[h->idx++] = b;
    }
    h->prev = b;
    return false;
  }

  // collecting
  if(h->in_msg){
    if(h->idx < C55_BUF_SIZE-1){
      h->buf[h->idx++] = b;
    } else {
      // overflow -> reset
      h->in_msg = 0;
      h->idx = 0;
      h->prev = 0;
      return false;
    }

    // end of message marked by '!'
    if(b == '!'){
      h->buf[h->idx] = '\0';
      if(h->idx >= 2 && h->buf[0] == '$'){
        char typ = h->buf[1];
        // set state variable
        if(typ == '@') h->state = CEREBRAL55_STATE_STANDBY;
        else if(typ == '^') h->state = CEREBRAL55_STATE_RUN;
        else if(typ == '%') h->state = CEREBRAL55_STATE_SHUT;
        else h->state = CEREBRAL55_STATE_UNKNOWN;

        // if RUN, parse payload after "$^"
        if(typ == '^'){
          const char *payload = (const char*)(h->buf + 2);
          parse_operational_payload(payload, &h->data);
        } else {
          // for STANDBY/SHUT we clear data (optional)
          memset(&h->data, 0, sizeof(h->data));
        }

        // reset for next message
        h->in_msg = 0;
        h->idx = 0;
        h->prev = 0;
        return true; // completed a message
      } else {
        // invalid framing: reset
        h->in_msg = 0;
        h->idx = 0;
        h->prev = 0;
        return false;
      }
    }
  }

  return false;
}
