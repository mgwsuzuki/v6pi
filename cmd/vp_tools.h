// -*- coding: utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_tools.h
//
// written by  : mgwsuzuki
//
// date        : Sun May 11 15:37:00 2014 +0900
//
////////////////////////////////////////////////////////////

#ifndef VP_TOOLS_H
#define VP_TOOLS_H

#include "vp_typedef.h"

// 32bitアクセスでaddrにvalを書き込む
inline void vp_set32(u32_t addr, u32_t val){
  volatile u32_t* a = (volatile u32_t*)addr;
  *a = val;
}

// 32bitアクセスでaddrから値を読んで返す
inline u32_t vp_get32(u32_t addr){
  volatile u32_t* a = (volatile u32_t*)addr;
  return *a;
}

#endif	// !VP_TOOLS_H
