// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_string.c
//
// written by  : mgwsuzuki
//
// date        : Sun May 18 20:48:57 2014 +0900
//
////////////////////////////////////////////////////////////

#include "vp_string.h"

static char hexstr[] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static u32_t vp_u2str(u32_t val, i32_t len, u32_t shift, char* strbuf);

//////////////////////////////////////////////////
// 文字列からi32_tへ
i32_t vp_a2i(char* str){

  i32_t flag;
  i32_t t;
  i32_t val = 0;

  // 1文字以上なければ終了
  if (str[0] == 0){
    return 0;

  // 基数のプリフィックスチェック
  }else if(str[0] != '0'){
    // 10進数文字列として変換

    if (*str == '-'){
      flag = -1;
      str++;
    }else{
      flag = 1;
    }

    while(*str != 0){
      if (*str >= '0' && *str <= '9'){
	t = (u32_t)(*str - '0');
	val = val * 10 + t;
      }else{
	return val * flag;	// 変換不能文字がでたのでそれまで変換できた数値を返す
      }
      str++;
    }
    return val * flag;

  }else{
    // 負数を考慮しない変換なのでvp_a2u()に処理を任せる
    return (i32_t)vp_a2u(str);
  }
}

//////////////////////////////////////////////////
// 文字列からu32_tへ
u32_t vp_a2u(char* str){

  i32_t t;
  i32_t val;

  // 1文字以上なければ終了
  if (str[0] == 0){
    return 0;

  // 基数のプリフィックスチェック
  }else if(str[0] != '0'){
    // 10進数文字列として変換
    while(*str != 0){
      if (*str >= '0' && *str <= '9'){
	t = (u32_t)(*str - '0');
	val = val * 10 + t;
      }else{
	return val;		// 変換不能文字がでたのでそれまで変換できた数値を返す
      }
      str++;
    }
    return val;

  // 2文字以上なければ終了
  }else if (str[1] == 0){
    return 0;

  }else if (str[1] == 'x'){
    // 16進数文字列として変換
    return vp_hex2u(&str[2]);

  }else if (str[1] == 'b'){
    // 2進数文字列として変換
    return vp_bin2u(&str[2]);

  }else{
    // 8進数文字列として変換
    return vp_oct2u(&str[1]);
  }

  return 0;
}

//////////////////////////////////////////////////
// 16進数文字列からu32_tへ
u32_t vp_hex2u(char* str){

  u32_t val = 0;

  while(*str != 0){
    if (*str >= '0' && *str <= '9'){
      val = (val << 4) + (u32_t)(*str - '0');
    }else if (*str >= 'a' && *str <= 'f'){
      val = (val << 4) + (u32_t)(*str - 'a') + 10;
    }else if (*str >= 'A' && *str <= 'F'){
      val = (val << 4) + (u32_t)(*str - 'A') + 10;

    }else if (*str == '_'){	// '_'は無視する
      ;
    }else{
      return val;
    }
    str++;
  }
  return val;
}

// 8進数文字列からu32_tへ
u32_t vp_oct2u(char* str){

  u32_t val = 0;

  while(*str != 0){
    if (*str >= '0' && *str <= '7'){
      val = (val << 3) + (u32_t)(*str - '0');

    }else if (*str == '_'){	// '_'は無視する
      ;
    }else{
      return val;
    }
    str++;
  }
  return val;
}

//////////////////////////////////////////////////
// 2進数文字列からu32_tへ
u32_t vp_bin2u(char* str){

  u32_t val = 0;

  while(*str != 0){
    if (*str == '0' || *str == '1'){
      val = (val << 1) + (u32_t)(*str - '0');

    }else if (*str == '_'){	// '_'は無視する
      ;
    }else{
      return val;
    }
    str++;
  }
  return val;
}

//////////////////////////////////////////////////
// 符号付き数値から10進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_i2dec(i32_t val, char* strbuf){

  u32_t c;
  if (val < 0){
    c = vp_u2dec((u32_t)(-val), &strbuf[1]);
    strbuf[0] = '-';
    c++;
    return c;
  }else{
    return vp_u2dec((u32_t)val, strbuf);
  }
}

//////////////////////////////////////////////////
// 符号なし数値から10進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2dec(u32_t val, char* strbuf){

  u32_t r;
  u32_t c = 0;
  char str[16];
  char* sp = str;

  if (val == 0){
    *strbuf++ = '0';
    *strbuf = 0;
    return 1;
  }

  // 剰余なので下の桁から文字列が決定する
  while(val != 0){
    r = val % 10;
    val /= 10;
    *sp++ = hexstr[r];
    c++;
  }

  while(sp != str){
    sp--;
    *strbuf++ = *sp;
  }

  *strbuf = 0;

  return c;
}

//////////////////////////////////////////////////
// 符号なし数値から16進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2hex(u32_t val, i32_t len, char* strbuf){
  return vp_u2str(val, len, 4, strbuf);
}

//////////////////////////////////////////////////
// 符号なし数値から8進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2oct(u32_t val, i32_t len, char* strbuf){
  return vp_u2str(val, len, 3, strbuf);
}

//////////////////////////////////////////////////
// 符号なし数値から2進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2bin(u32_t val, i32_t len, char* strbuf){
  return vp_u2str(val, len, 1, strbuf);
}

//////////////////////////////////////////////////
// (1-4)ビット幅を1キャラクタとして文字列へ
u32_t vp_u2str(u32_t val, i32_t len, u32_t shift, char* strbuf){

  u32_t r;
  i32_t c = 0;
  i32_t d = 0;
  char str[33];
  char* sp = str;
  i32_t n;
  char pad;

  i32_t maxlen;
  u32_t mask;

  switch(shift){
  case 1:
    maxlen = 32; mask = 0x1; break;
  case 2:
    maxlen = 16; mask = 0x3; break;
  case 3:
    maxlen = 11; mask = 0x7; break;
  case 4:
  default:
    maxlen = 8; mask = 0xf; break;
  }

  // 出力文字列長とpad文字の決定
  if (len > maxlen){
    len = maxlen;
    pad = '0';
  }else if (len > 0){
    pad = '0';
  }else if (len == 0){
    pad = ' ';
  }else if (len >= -maxlen){
    len = -len;
    pad = ' ';
  }else{
    len = 8;
    pad = ' ';
  }

  while(val != 0){
    r = val & mask;
    val >>= shift;
    *sp++ = hexstr[r];
    c++;
  }

  sp--;
  if (len == 0){
    // MSB側の0は出力しない
    for (n = 0; n < c; n++){
      *strbuf++ = *sp--;
      d++;
    }

  }else{
    // MSB側の0を指定された文字でパッドする
    for (n = 0 ; n < len - c; n++){
      *strbuf++ = pad;
      d++;
    }
    for (n = 0; n < c; n++){
      *strbuf++ = *sp--;
      d++;
    }
  }

  *strbuf = 0;

  return d;
}
