#ifndef  Delay_H
#define  Delay_H
#define DELAY(NUMBER) \
 for(int i = 0; i< NUMBER/5; i++)  \
  { \                              
  __asm__ volatile ("NOP");   \
   } 
  
#endif