// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_cmd.c
//
// written by  : mgwsuzuki
//
// date        : Sat Jun 07 23:01:09 2014 +0900
//
////////////////////////////////////////////////////////////

#include "vp_cmd.h"

// コマンド名を記憶する文字列バッファサイズ
#define VP_CMD_CMD_BUF_SIZE 1024

// 登録できるコマンドのサイズ
#define VP_CMD_TABLE_SIZE 128

static char cmdbuf[VP_CMD_CMD_BUF_SIZE];
static u32_t cmdbuf_widx;

// 1つのコマンドを記憶する構造体
typedef struct {
  char* namep;		// cmdbufに記録されているコマンド名へのポインタ
  vp_cmd_fptr_t fptr;	// 実行する関数へのポインタ
} vp_cmd_table_t;

vp_cmd_table_t cmd_table[VP_CMD_TABLE_SIZE];
static u32_t cmd_table_size;

// 初期化
void vp_cmd_init(){
  u32_t n;
  for (n = 0; n < VP_CMD_CMD_BUF_SIZE; n++){
    cmdbuf[n] = 0;
  }
  cmdbuf_widx = 0;

  for (n = 0; n < VP_CMD_TABLE_SIZE; n++){
    cmd_table[n].namep = 0;
    cmd_table[n].fptr = 0;
  }
  cmd_table_size = 0;
}

// コマンドを追加する
u32_t vp_cmd_add(char* cmdname, vp_cmd_fptr_t fptr){

  u32_t n;
  u32_t copy = FALSE;

  // cmd_tableがfullなら登録しない
  if (cmd_table_size == VP_CMD_TABLE_SIZE){
    return FALSE;
  }

  // cmdnameをcmdbufにコピーする
  // コピー中にbuffer fullになったら登録失敗とする
  for (n = cmdbuf_widx; n < VP_CMD_CMD_BUF_SIZE; n++){
    cmdbuf[n] = *cmdname;
    if (*cmdname == 0){
      copy = TRUE;
      break;
    }
    cmdname++;
  }

  if (copy == FALSE){
    return FALSE;
  }

  // cmd_tableを作る
  cmd_table[cmd_table_size].namep = &cmdbuf[cmdbuf_widx];
  cmd_table[cmd_table_size].fptr = fptr;

  cmdbuf_widx = ++n;
  cmd_table_size++;

  return FALSE;
}

// コマンドラインを解析する
u32_t vp_cmd_parse(char* cmdline){

  u32_t n;
  char* p;

  p = vp_strtok(cmdline, " ");

  if (p == 0){
    return FALSE;
  }

  for (n = 0; n < cmd_table_size; n++){
    if (vp_strncmp(p, cmd_table[n].namep, -1) == 0){
      cmd_table[n].fptr(p);
    }
  }

  return FALSE;
}
