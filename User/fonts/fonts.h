#ifndef __FONTS_H
#define __FONTS_H

#include "stm32f10x.h"


extern u8 Fonts_AsicII_8x16[];
extern u8 Fonts_AsicII_16x32[];
extern u8 Fonts_AsicII_12x24[];
extern int strcmp(const char *s1,const char *s2);
extern u8 Fonts_16x16Chinese[];
extern u8 Chinese_Pinyin_to_16x16Fonts(char Character[]);
#endif /* __FONTS_H*/


