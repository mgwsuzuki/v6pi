// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_uart.h
//
// written by  : mgwsuzuki
//
// date        : Tue Apr 29 17:43:46 2014 +0900
//
////////////////////////////////////////////////////////////

#ifndef VP_UART_H
#define VP_UART_H

#include "vp_typedef.h"

// レジスタアドレス
#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

// miniUARTの初期化
void vp_uart_init();

// 1文字取得
u8_t vp_uart_get_char();

// 1文字送信
void vp_uart_put_char(u8_t c);

//////////////////////////////////////////////////
//
// UARTバッファリング
//

// 入力バッファのサイズ(バイト数)
#define VP_UARTBUF_ISIZE 1024

// 出力バッファのサイズ(バイト数)
#define VP_UARTBUF_OSIZE 16368

// デリミタキャラクタ
#define VP_UARTBUF_DELIM 0x0d

// バックスペース
#define VP_UARTBUF_BS 0x08

typedef struct {
  u32_t tx_count;
  u32_t tx_widx;
  u32_t tx_ridx;
  u8_t  tx_buf[VP_UARTBUF_OSIZE];
  u32_t rx_count;
  u32_t rx_widx;
  u32_t rx_ridx;
  u8_t  rx_buf[VP_UARTBUF_ISIZE];
  u32_t rx_delim_count;
} vp_uartbuf_t;

// uartbufの初期化
void vp_uartbuf_init();

// 定期的にコールする関数
void vp_uartbuf_task();

// 送信バッファに1文字追加する
void vp_uartbuf_put_char(u8_t c);

// 送信バッファに文字列を追加する
u32_t vp_uartbuf_put_string(char* str);

// 1行を取得する
u32_t vp_uartbuf_getline(char* buf);

// 受信バッファをクリアする
void vp_uartbuf_clear_rxbuf();

#endif	// !VP_UART_H
