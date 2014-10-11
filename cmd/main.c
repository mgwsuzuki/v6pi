// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : main.c
//
// written by  : mgwsuzuki
//
// date        : Wed Jun 18 23:39:49 2014 +0900
//
////////////////////////////////////////////////////////////

#include "vp_string.h"
#include "vp_cmd.h"

// "hello world"と出力するコマンド
static u32_t cmd_hello(char* cmdstr){
  vp_printf("Hello World\n");
  return 0;
}

// コマンド文字列をさらに解析して加算処理を行って文字列出力するコマンド
static u32_t cmd_add(char* cmdstr){

  char* p;
  i32_t ans = 0;

  while((p = vp_strtok(0, " ,")) != 0){
    ans += vp_a2i(p);
  }
  vp_printf("ans: %d (0x%x)\n", ans, ans);
  return TRUE;
}

//// メモリ内容を表示する
static u32_t cmd_mr(char* cmdstr, u32_t width){
  // mr8 addr [len]

  char* p;
  u8_t* addr;
  u32_t len;
  u32_t c;

  if ((p = vp_strtok(0, " ,")) == 0){
    return FALSE;
  }
  addr = (u8_t*)vp_a2u(p);

  if ((p = vp_strtok(0, " ,")) != 0){
    len = vp_a2i(p);
  }else{
    len = 64 / width;
  }

  c = 0;
  while (len != 0){
    if (c == 0){
      vp_printf("%08x:", addr);
    }else if (c == 8){
      vp_printf(" ");
    }

    if (width == 4){
      vp_printf(" %08x", *(u32_t*)addr);
    }else if (width == 2){
      vp_printf(" %04x", *(u16_t*)addr);
    }else{
      vp_printf(" %02x", *addr);
    }
    addr += width;
    c += width;
    len--;

    if (c == 16){
      vp_printf("\n");
      c = 0;
    }
  }

  if (c != 0){
    vp_printf("\n");
  }

  return TRUE;
}

static u32_t cmd_mr8(char* cmdstr){
  // mr8 addr [len]
  return cmd_mr(cmdstr, 1);
}

static u32_t cmd_mr16(char* cmdstr){
  // mr16 addr [len]
  return cmd_mr(cmdstr, 2);
}

static u32_t cmd_mr32(char* cmdstr){
  // mr16 addr [len]
  return cmd_mr(cmdstr, 4);
}


//// メモリに指定された値を書き込む
static u32_t cmd_mw(char* cmdstr, u32_t width){

  char* p;
  u8_t* addr8;
  u16_t* addr16;
  u32_t* addr32;
  u8_t data8;
  u16_t data16;
  u32_t data32;

  if ((p = vp_strtok(0, " ,")) == 0){
    return FALSE;
  }

  if (width == 1){
    addr8 = (u8_t*)vp_a2u(p);
    while((p = vp_strtok(0, " ,")) != 0){
      data8 = (u8_t)vp_a2u(p);
      *addr8++ = data8;
    }
  }else if(width == 2){
    addr16 = (u16_t*)vp_a2u(p);
    while((p = vp_strtok(0, " ,")) != 0){
      data16 = (u16_t)vp_a2u(p);
      *addr16++ = data16;
    }
  }else{
    addr32 = (u32_t*)vp_a2u(p);
    while((p = vp_strtok(0, " ,")) != 0){
      data32 = vp_a2u(p);
      *addr32++ = data32;
    }
  }
  return TRUE;
}

static u32_t cmd_mw8(char* cmdstr){
  return cmd_mw(cmdstr, 1);
}

static u32_t cmd_mw16(char* cmdstr){
  return cmd_mw(cmdstr, 2);
}

static u32_t cmd_mw32(char* cmdstr){
  return cmd_mw(cmdstr, 4);
}

int main(){

  char strbuf[1024];

  // setup UART
  vp_uart_init();
  vp_uartbuf_init();

  // setup vp_cmd
  vp_cmd_init();
  vp_cmd_add("hw", cmd_hello);
  vp_cmd_add("add", cmd_add);
  vp_cmd_add("mr8", cmd_mr8);
  vp_cmd_add("mr16", cmd_mr16);
  vp_cmd_add("mr32", cmd_mr32);
  vp_cmd_add("mw8", cmd_mw8);
  vp_cmd_add("mw16", cmd_mw16);
  vp_cmd_add("mw32", cmd_mw32);

  while(1){
    vp_uartbuf_task();
    if (vp_uartbuf_getline(strbuf) > 0){
      vp_cmd_parse(strbuf);
    }
  }

  return 0;
}
