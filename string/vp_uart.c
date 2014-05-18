// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_uart.c
//
// written by  : mgwsuzuki
//
// date        : Tue Apr 29 18:00:20 2014 +0900
//
////////////////////////////////////////////////////////////

#include "vp_uart.h"
#include "vp_gpio.h"
#include "vp_tools.h"

// miniUARTの初期化
void vp_uart_init(){

  // TXD0であるGPIO14をpull-up/downなしにする
  vp_gpio_set_pud(14, VP_GPIO_PUD_OFF);

  // RXD0であるGPIO15をpull-upする
  vp_gpio_set_pud(15, VP_GPIO_PUD_UP);

  // GPIO14, GPIO15をTXD1, RXD1になるようにALT5にする
  // TXD0, RXD0ではないようだ
  vp_gpio_set_fsel(14, VP_GPIO_FSEL_ALT5);
  vp_gpio_set_fsel(15, VP_GPIO_FSEL_ALT5);

  // miniUARTをenableする
  vp_set32(AUX_ENABLES, 1);

  // tx, rxを停止する
  vp_set32(AUX_MU_CNTL_REG,0);
  // 割り込みはすべてdisable
  vp_set32(AUX_MU_IER_REG,0);
  // data sizeを8bitにする
  // SoC-Peripheral.pdf 14pによると[1]はreservedだが、1を書き込まないといけないようだ
  vp_set32(AUX_MU_LCR_REG,3);
  // RTSの設定
  vp_set32(AUX_MU_MCR_REG,0);
  // rx FIFO, tx FIFOをクリアする
  vp_set32(AUX_MU_IIR_REG,0x6);
  // baudrate設定
  vp_set32(AUX_MU_BAUD_REG,270);

  // tx, rxをenableにする
  vp_set32(AUX_MU_CNTL_REG, 3);
}

// 1文字取得
u8_t vp_uart_get_char(){
  return (vp_get32(AUX_MU_IO_REG) & 0xff);
}

// 1文字送信
void vp_uart_put_char(u8_t c){
  vp_set32(AUX_MU_IO_REG, (u32_t)c);
}

//////////////////////////////////////////////////
//
// UARTバッファリング
//
static vp_uartbuf_t uartbuf;

// uartbufの初期化
void vp_uartbuf_init(){
  uartbuf.tx_count = 0;
  uartbuf.tx_widx = 0;
  uartbuf.tx_ridx = 0;
  uartbuf.rx_count = 0;
  uartbuf.rx_widx = 0;
  uartbuf.rx_ridx = 0;
  uartbuf.rx_delim_count = 0;
}

// 定期的にコールする関数
void vp_uartbuf_task(){

  u8_t c;

  // Rx
  while((vp_get32(AUX_MU_LSR_REG) & 1) && (uartbuf.rx_count < VP_UARTBUF_ISIZE)){
    c = (u8_t)vp_get32(AUX_MU_IO_REG);
    vp_uartbuf_put_char(c);
    // バックスペースでないか
    if (c != VP_UARTBUF_BS){
      uartbuf.rx_buf[uartbuf.rx_widx++] = c;
      uartbuf.rx_count++;
      if (uartbuf.rx_widx >= VP_UARTBUF_ISIZE){
	uartbuf.rx_widx = 0;
      }
      // デリミタチェック
      if (c == VP_UARTBUF_DELIM){
	uartbuf.rx_delim_count++;
      }

    // バックスペースだった
    }else if (uartbuf.rx_count > 0){
      uartbuf.rx_count--;
      if (uartbuf.rx_widx == 0){
	uartbuf.rx_widx = VP_UARTBUF_ISIZE-1;
      }else{
	uartbuf.rx_widx--;
      }
    }
  }

  // Tx
  while((uartbuf.tx_count > 0) && (vp_get32(AUX_MU_LSR_REG) & 0x20)){
    c = uartbuf.tx_buf[uartbuf.tx_ridx++];
    vp_set32(AUX_MU_IO_REG, (u32_t)c);
    uartbuf.tx_count--;
    if (uartbuf.tx_ridx == VP_UARTBUF_OSIZE){
      uartbuf.tx_ridx = 0;
    }
  }
}

// 送信バッファに1文字追加する
void vp_uartbuf_put_char(u8_t c){

  if (uartbuf.tx_count < VP_UARTBUF_OSIZE){
    uartbuf.tx_buf[uartbuf.tx_widx++] = c;
    uartbuf.tx_count++;
    if (uartbuf.tx_widx >= VP_UARTBUF_OSIZE){
      uartbuf.tx_widx = 0;
    }
  }
}

// 送信バッファに文字列を追加する
u32_t vp_uartbuf_put_string(char* str){

  u32_t c = 0;
  while(*str != 0 && uartbuf.tx_count < VP_UARTBUF_OSIZE){
    uartbuf.tx_buf[uartbuf.tx_widx++] = *str++;
    uartbuf.tx_count++;
    c++;
    if (uartbuf.tx_widx >= VP_UARTBUF_OSIZE){
      uartbuf.tx_widx = 0;
    }
  }
  return c;
}

// 1行を取得する
u32_t vp_uartbuf_getline(char* buf){

  u8_t c;
  u32_t len = 0;

  if (uartbuf.rx_delim_count == 0){
    return 0;
  }

  while(uartbuf.rx_count > 0){
    c = uartbuf.rx_buf[uartbuf.rx_ridx++];
    uartbuf.rx_count--;
    if (uartbuf.rx_ridx >= VP_UARTBUF_ISIZE){
      uartbuf.rx_ridx = 0;
    }
    if (c == VP_UARTBUF_DELIM){
      *buf = 0;
      uartbuf.rx_delim_count--;
      return len;
    }else{
      *buf++ = c;
      len++;
    }
  }
  return 0;
}

// 受信バッファをクリアする
void vp_uartbuf_clear_rxbuf(){
  uartbuf.rx_count = 0;
  uartbuf.rx_widx = 0;
  uartbuf.rx_ridx = 0;
  uartbuf.rx_delim_count = 0;
}
