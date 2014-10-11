// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_cmd.h
//
// written by  : mgwsuzuki
//
// date        : Sat Jun 07 22:50:50 2014 +0900
//
////////////////////////////////////////////////////////////

#ifndef VP_CMD_H
#define VP_CMD_H

#include "vp_typedef.h"
#include "vp_uart.h"
#include "vp_string.h"

// コマンドを処理する関数の型
typedef u32_t (*vp_cmd_fptr_t)(char* cmdlinep);

// 初期化
void vp_cmd_init();

// コマンドを追加する
// cmdname  コマンドの文字列
// fptr     cmdnameが検出されたときにコールされる関数へのポインタ
u32_t vp_cmd_add(char* cmdname, vp_cmd_fptr_t fptr);

// コマンドラインを解析する
// cmdline  コマンド入力文字列
u32_t vp_cmd_parse(char* cmdline);

#endif	// !VP_CMD_H
