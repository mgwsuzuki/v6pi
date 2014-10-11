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
#include "vp_uart.h"

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
  i32_t val = 0;

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

  do {
    r = val & mask;
    val >>= shift;
    *sp++ = hexstr[r];
    c++;
  } while(val != 0);

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

////////////////////////////////////////
// 文字列のコピー
u32_t vp_strncpy(char* src, char* dst, u32_t n){

  u32_t c;

  for (c = 0; c < n; c++){
    *dst++ = *src;
    if (*src == 0){
      return c;
    }
    src++;
  }
  return c;
}

////////////////////////////////////////
// 文字列の比較
i32_t vp_strncmp(char* s1, char* s2, u32_t n){

  char c1, c2;

  for (; n > 0; n--){
    c1 = *s1++;
    c2 = *s2++;

    if (c1 == 0 && c2 == 0){
      return 0;
    }else if (c1 == 0 && c2 != 0){
      return -1;
    }else if (c1 != 0 && c2 == 0){
      return 1;
    }else if (c1 > c2){
      return 1;
    }else if (c1 < c2){
      return -1;
    }
  }
  return 0;
}

char* vp_strtok(char* p, char* delim){

  static char* prev_ptr = 0;
  char* s;
  char* tk;
  char* d;

  s = (p != 0 ? p : prev_ptr);

  // 連続するデリミタをスキップ
  while(*s != 0){
    d = delim;
    while(1){
      // デリミタが1文字でも現れたらループを抜けてs++する
      if (*s == *d){		// デリミタなのでスキップ
	break;
      }else if (*d == 0){	// すべてのデリミタ文字と比較した、つまりデリミタじゃなった
	goto pass1;
      }
      d++;
    }
    s++;
  }

  // sの文字列の終端まできたのでトークンなし
  return 0;

 pass1:
  tk = s;
  // デリミタ以外の文字が連続するのをチェック
  while(*s != 0){
    d = delim;
    while(1){
      // デリミタが1文字でも現れたら終了
      if (*s == *d){		// デリミタなので終了する
	*s++ = 0;
	goto pass2;
      }else if (*d == 0){	// すべてのデリミタ文字と比較した、つまりデリミタじゃなかった
	break;
      }
      d++;
    }
    s++;
  }

  // 文字列の終端に達したので、sを更新せずに終端文字列を指し示した状態にする
  // 次回この関数がコールされるとき、トークンなしとして処理される

 pass2:
  prev_ptr = s;
  return tk;
}

// printfみたいな関数

typedef enum {
  ST_NORMAL,
  ST_WIDTH,
  ST_PARCENT
} format_st_t;

typedef enum {
  FMT_NONE,	// 出力可能文字
  FMT_WAIT,	// 出力文字未確定
  FMT_PARCENT,	// '%'文字確定
  FMT_CHAR,	// 'c'確定
  FMT_STRING,	// 's'確定
  FMT_DEC,	// 'd'確定
  FMT_OCT,	// 'o'確定
  FMT_HEX,	// 'x'確定
  FMT_BIN	// 'b'確定
} format_result_t;

typedef struct {
  format_st_t state;
  format_result_t result;
  u32_t zeropad;
  u32_t width;
} printf_obj_t;

static format_result_t parse_format(printf_obj_t* st, char c){

  switch(st->state){
  case ST_NORMAL:
    switch(c){
    case '%':
      st->state = ST_PARCENT;
      st->result = FMT_WAIT;
      break;
    default:
      st->result = FMT_NONE;
      break;
    }
    break;

  case ST_PARCENT:
    if (c == '0'){
      st->zeropad = TRUE;
      st->state = ST_WIDTH;
      st->width = 0;
      st->result = FMT_WAIT;
      break;
    }else if (c >= '1' && c <= '9'){
      st->zeropad = FALSE;
      st->state = ST_WIDTH;
      st->width = (u32_t)(c - '0');
      st->result = FMT_WAIT;
      break;
    }
    switch(c){
    case '%': st->state = ST_NORMAL;  st->result = FMT_PARCENT; break;
    case 'c': st->state = ST_NORMAL;  st->result = FMT_CHAR;    break;
    case 's': st->state = ST_NORMAL;  st->result = FMT_STRING;  break;
    case 'd': st->state = ST_NORMAL;  st->result = FMT_DEC;     break;
    case 'o': st->state = ST_NORMAL;  st->result = FMT_OCT;     break;
    case 'x': st->state = ST_NORMAL;  st->result = FMT_HEX;     break;
    case 'b': st->state = ST_NORMAL;  st->result = FMT_BIN;     break;
    default:  st->state = ST_NORMAL;  st->result = FMT_NONE;    break;
    }

  case ST_WIDTH:
    if (c >= '0' && c <= '9'){
      st->width = st->width * 10 + (u32_t)(c - '0');
      st->result = FMT_WAIT;
      break;
    }
    switch(c){
    case 'c': st->state = ST_NORMAL;  st->result = FMT_CHAR;    break;
    case 's': st->state = ST_NORMAL;  st->result = FMT_STRING;  break;
    case 'd': st->state = ST_NORMAL;  st->result = FMT_DEC;     break;
    case 'o': st->state = ST_NORMAL;  st->result = FMT_OCT;     break;
    case 'x': st->state = ST_NORMAL;  st->result = FMT_HEX;     break;
    case 'b': st->state = ST_NORMAL;  st->result = FMT_BIN;     break;
    default:  st->state = ST_NORMAL;  st->result = FMT_NONE;    break;
    }

  default:
    break;
  }
  return st->result;
}

u32_t vp_vsprintf(char* buf, char* format, va_list arg){
  printf_obj_t obj;
  char c;
  u32_t len;
  char* b = buf;

  obj.state = ST_NORMAL;
  obj.zeropad = FALSE;
  obj.width = 0;

  while(*format != 0){
    c = *format++;
    parse_format(&obj, c);
    switch(obj.result){
    case FMT_PARCENT:
      *buf++ = '%';
      break;
    case FMT_CHAR:{
      char* cp;
      //*buf++ = 'C';
      cp = va_arg(arg, char*);
      *buf++ = *cp;
      break;
    }
    case FMT_STRING:{
      char* sp;
      //*buf++ = 'S';
      sp = va_arg(arg, char*);
      len = vp_strncpy(sp, buf, -1);
      buf += len;
      break;
    }
    case FMT_DEC:{
      int v;
      //*buf++ = 'D';
      v = va_arg(arg, int);
      len = vp_i2dec(v, buf);
      buf += len;
      break;
    }
    case FMT_OCT:{
      u32_t v;
      //*buf++ = 'O';
      v = va_arg(arg, int);
      len = vp_u2oct(v, (obj.zeropad == TRUE ? obj.width : -obj.width), buf);
      buf += len;
      break;
    }
    case FMT_HEX:{
      u32_t v;
      //*buf++ = 'H';
      v = va_arg(arg, int);
      len = vp_u2hex(v, (obj.zeropad == TRUE ? obj.width : -obj.width), buf);
      buf += len;
      break;
    }
    case FMT_BIN:{
      u32_t v;
      //*buf++ = 'H';
      v = va_arg(arg, int);
      len = vp_u2bin(v, (obj.zeropad == TRUE ? obj.width : -obj.width), buf);
      buf += len;
      break;
    }
    case FMT_NONE:
      *buf++ = c;
      break;

    default:
      break;
    }
  }
  *buf = 0;
  return buf - b;
}

void vp_printf(char* format, ...){
  char strbuf[1024];
  va_list arg;
  va_start(arg, format);
  vp_vsprintf(strbuf, format, arg);
  vp_uartbuf_put_string(strbuf);
  va_end(arg);
}

u32_t vp_sprintf(char* buf, char* format, ...){
  u32_t len;
  va_list arg;
  va_start(arg, format);
  len = vp_vsprintf(buf, format, arg);
  va_end(arg);
  return len;
}

