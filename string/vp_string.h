// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_string.h
//
// written by  : mgwsuzuki
//
// date        : Sun May 18 20:44:23 2014 +0900
//
////////////////////////////////////////////////////////////

#ifndef VP_STRING_H
#define VP_STRING_H

#include "vp_typedef.h"

// 文字列からi32_tへ
i32_t vp_a2i(char* str);

// 文字列からu32_tへ
u32_t vp_a2u(char* str);

// 16進数文字列からu32_tへ
u32_t vp_hex2u(char* str);

// 8進数文字列からu32_tへ
u32_t vp_oct2u(char* str);

// 2進数文字列からu32_tへ
u32_t vp_bin2u(char* str);

// 符号付き数値から10進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_i2dec(i32_t val, char* strbuf);

// 符号なし数値から10進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2dec(u32_t val, char* strbuf);

// 符号なし数値から16進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2hex(u32_t val, i32_t len, char* strbuf);

// 符号なし数値から8進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2oct(u32_t val, i32_t len, char* strbuf);

// 符号なし数値から2進数文字列へ
// strbufは十分な領域が確保されていること
u32_t vp_u2bin(u32_t val, i32_t len, char* strbuf);

#endif	// !VP_STRING_H
