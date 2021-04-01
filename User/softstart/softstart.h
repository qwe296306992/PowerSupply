#ifndef SOFTSTART_H
#define SOFTSTART_H



#include "stm32f10x.h"


extern void SoftStart_Init(void);

void Power_AC_On(void);
void Power_AC_Off(void);

	
extern void SS_DISADBE_Init(void);

extern void SS_DISADBE_On(void);
extern void SS_DISADBE_Off(void);
extern void DISADBE_On_Or_Off(void);

extern void TIM1_SoftStatr_IT(void);













#endif /* SOFTSTART_H */




