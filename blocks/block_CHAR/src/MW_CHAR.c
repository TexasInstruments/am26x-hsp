#include <stdint.h>
#include <stdbool.h>
#include "MW_CHAR.h"

void CHAR_step1(const void *dataInput, uint8_t dataType, const void *numOutputElements, uint8_t inputLen, const void *dataOutput)
{
    const void* ptr;
    uint8_t Buffer[1024];
    
    uint32_t b;
    uint8_t j = 0;
    uint8_t a;
    uint8_t *data_output = (uint8_t *)dataOutput;
    uint8_t *ptr1 = (uint8_t *)numOutputElements;
    *ptr1 = 0;

    for(uint8_t i = 0; i< inputLen; i++)
    {
        ptr = &Buffer[0U];
    if(dataType == 0)
    {
        b = (double)b;
        b = *(double *)dataInput;
        dataInput+=8;
    }
    else if(dataType == 1)
    {
        b = (float)b;
        b = *(float *)dataInput;
        dataInput+=4;
    }
    else if(dataType == 2)
    {
        b = (uint32_t)b;
        b = *(uint32_t *)dataInput;
        dataInput+=4;
    }
    else if(dataType == 3)
    {
        b = (int32_t)b;
        b = *(int32_t *)dataInput;
        dataInput+=4;
    }
    else if(dataType == 4)
    {
        b = (int16_t)b;
        b = *(int16_t *)dataInput;
        dataInput+=2;
    }
    else if(dataType == 5)
    {
        b = (uint16_t)dataInput;
        b = *(uint16_t *)dataInput;
        dataInput+=2;
    }
    else if(dataType == 6)
    {
        b = (int8_t)b;
        b = *(int8_t *)dataInput;
        dataInput++;
    }
    else if(dataType == 7)
    {
        b = (uint8_t)b;
        b = *(uint8_t *)dataInput;
        dataInput++;
    }
    else if(dataType == 8)
    {
        b = (bool)b;
        b = *(bool *)dataInput;
        dataInput++;
    }
    
    uint8_t n1 = sprintf(ptr,"%d", b);
    
    
    a = 0;
    while(a < (n1))
    {
        *data_output = Buffer[a];
        data_output++;
        a++;
    }
    *data_output = (uint8_t)(32);
    data_output++;
    *ptr1 = (uint8_t)((uint8_t)*ptr1 + (n1+1));

    }

}
