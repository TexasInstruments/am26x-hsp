#ifndef CHAR_MW_H_
#define CHAR_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

extern void
CHAR_step(const void *dataInput, const void *dataOutput, uint8_t dataType,  uint8_t digits);
extern 
void CHAR_step1(const void *dataInput, uint8_t dataType, const void *numOutputElements, uint8_t inputLen, const void *dataOutput);
#endif