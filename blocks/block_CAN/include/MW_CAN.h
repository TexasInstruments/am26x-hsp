#ifndef CAN_MW_H_
#define CAN_MW_H_

#include <stdint.h>
#include <stdbool.h>
#include <drivers/mcan.h>
#include <drivers/soc.h>
#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif

typedef struct{
    uint32_t base;
    uint32_t instance;
    bool Interrupt_status;
    bool RXFIFO0newmsg;
    bool RXFIFO0wtrmark;
    bool RXFIFO0full;
    bool RXFIFO0msglost;
    bool RXFIFO1newmsg;
    bool RXFIFO1wtrmark;
    bool RXFIFO1full;
    bool RXFIFO1msglost;
    bool Highprioritymsg;
    bool TXcomplete;
    bool TXcancellationfin;
    bool TXFIFOempty;
    bool TXEventFIFOnewentry;
    bool TXEventFIFOwatermark;
    bool TXEventFIFOfull;
    bool TXEventFIFOelementlost;
    bool Timestampwraparound;
    bool MsgRAMacsfail;
    bool Timeoutoccured;
    bool MsgstrdedRxbuf;
    bool Biterrorcorr;
    bool Biterruncorr;
    bool Errorloggingovf;
    bool Errorpassive;
    bool Warningstatus;
    bool Busoffstatus;
    bool Watchdogintr;
    bool Protocolerrarbitphase;
    bool Protocolerrdataphase;
    bool Accsresaddr; 
}CANINTSTATUS_step_struct;

extern void 
CANINTSTATUS_step(CANINTSTATUS_step_struct *ptr, const void* intstatus, const void* IR_reg);

extern void
CANWRITE_step(uint32_t base, uint32_t instance, uint32_t txID, uint32_t len, const void* datainput, bool enable_blockingtx);

extern void
CANWRITE_release(uint32_t base);
          

#endif