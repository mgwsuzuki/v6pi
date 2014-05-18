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
#include "vp_string.h"

int main(){

  char strbuf[1024];
  u32_t v;
  char putbuf[1024];

  vp_uart_init();
  vp_uartbuf_init();
  vp_uartbuf_put_string("vp_string test\n");

  while(1){
    vp_uartbuf_task();
    if (vp_uartbuf_getline(strbuf) > 0){
      // echo
      vp_uartbuf_put_string("line: ");
      vp_uartbuf_put_string(strbuf);
      vp_uartbuf_put_char('\n');

      v = vp_a2i(strbuf);

      vp_i2dec(v, putbuf);
      vp_uartbuf_put_string("dec: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      // hex
      vp_u2hex(v, 6, putbuf);
      vp_uartbuf_put_string("hex: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      vp_u2hex(v, 0, putbuf);
      vp_uartbuf_put_string("hex: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      vp_u2hex(v, -4, putbuf);
      vp_uartbuf_put_string("hex: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      // oct
      vp_u2oct(v, 10, putbuf);
      vp_uartbuf_put_string("oct: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      vp_u2oct(v, 0, putbuf);
      vp_uartbuf_put_string("oct: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      vp_u2oct(v, -5, putbuf);
      vp_uartbuf_put_string("oct: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      // bin
      vp_u2bin(v, 28, putbuf);
      vp_uartbuf_put_string("bin: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      vp_u2bin(v, 0, putbuf);
      vp_uartbuf_put_string("bin: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");

      vp_u2bin(v, -18, putbuf);
      vp_uartbuf_put_string("bin: (");
      vp_uartbuf_put_string(putbuf);
      vp_uartbuf_put_string(")\n");
    }
  }

  return 0;
}
