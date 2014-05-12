// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : main.c
//
// written by  : mgwsuzuki
//
// date        : Tue Apr 29 18:01:33 2014 +0900
//
////////////////////////////////////////////////////////////

#include "vp_uart.h"

int main(){

  // simple echoback
#if 0
  volatile u32_t* LSR_REG = (volatile u32_t*)AUX_MU_LSR_REG;
  u8_t c;
  vp_uart_init();
  while(1){
    if ((*LSR_REG & 1) == 1){
      c = vp_uart_get_char();
      vp_uart_put_char(c);
    }
  }
#endif

#if 1
  // buffered echoback
  char strbuf[1024];
  vp_uart_init();
  vp_uartbuf_init();
  vp_uartbuf_put_string("hello world\n");
  while(1){
    vp_uartbuf_task();
    if (vp_uartbuf_getline(strbuf) > 0){
      vp_uartbuf_put_string("line: ");
      vp_uartbuf_put_string(strbuf);
    }
  }
#endif
  return 0;
}
