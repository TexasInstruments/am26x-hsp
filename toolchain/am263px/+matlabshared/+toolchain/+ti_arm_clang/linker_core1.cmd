

 /* This is the stack that is used by code running within main()
  * In case of NORTOS,
  * - This means all the code outside of ISR uses this stack
  * In case of FreeRTOS
  * - This means all the code until vTaskStartScheduler() is called in main()
  *   uses this stack.
  * - After vTaskStartScheduler() each task created in FreeRTOS has its own stack
  */

 --stack_size=16384
/* This is the heap size for malloc() API in NORTOS and FreeRTOS
* This is also the heap used by pvPortMalloc in FreeRTOS
*/
 --heap_size=32768
-e_vectors  /* This is the entry of the application, _vector MUST be placed starting address 0x0 */

/* This is the size of stack when R5 is in IRQ mode
 * In NORTOS,
 * - Here interrupt nesting is enabled
 * - This is the stack used by ISRs registered as type IRQ
 * In FreeRTOS,
 * - Here interrupt nesting is enabled
 * - This is stack that is used initally when a IRQ is received
 * - But then the mode is switched to SVC mode and SVC stack is used for all user ISR callbacks
 * - Hence in FreeRTOS, IRQ stack size is less and SVC stack size is more
 */
__IRQ_STACK_SIZE = 256;
/* This is the size of stack when R5 is in IRQ mode
 * - In both NORTOS and FreeRTOS nesting is disabled for FIQ
 */
__FIQ_STACK_SIZE = 256;
__SVC_STACK_SIZE = 4096; /* This is the size of stack when R5 is in SVC mode */
__ABORT_STACK_SIZE = 256;  /* This is the size of stack when R5 is in ABORT mode */
__UNDEFINED_STACK_SIZE = 256;  /* This is the size of stack when R5 is in UNDEF mode */



SECTIONS
{
    .vectors:{} palign(8) > R5F_VECS


    GROUP {
        .text.hwi: 
        .text.cache: 
        .text.mpu: 
        .text.boot: 
        .text:abort:  /* this helps in loading symbols when using XIP mode */
    } >> R5F_TCMA | R5F_TCMB | OCRAM


    GROUP {
        .text:   {}    /* This is where code resides */
        .rodata: {}    /* This is where const's go */
    } >> R5F_TCMA | R5F_TCMB | OCRAM 


    GROUP {

        .data:   {}   /* This is where initialized globals and static go */
    } >> R5F_TCMA | R5F_TCMB | OCRAM


    GROUP {
        .bss:    {} palign(8)   /* This is where uninitialized globals go */
    RUN_START(__BSS_START)
    RUN_END(__BSS_END)
        .sysmem: {} palign(8)   /* This is where the malloc heap goes */
        .stack:  {} palign(8)   /* This is where the main() stack goes */
    } > OCRAM 


    /* This is where the stacks for different R5F modes go */
    GROUP {
        .irqstack: {. = . + __IRQ_STACK_SIZE;} align(8)
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
        .fiqstack: {. = . + __FIQ_STACK_SIZE;} align(8)
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
        .svcstack: {. = . + __SVC_STACK_SIZE;} align(8)
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)
        .abortstack: {. = . + __ABORT_STACK_SIZE;} align(8)
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
        .undefinedstack: {. = . + __UNDEFINED_STACK_SIZE;} align(8)
    RUN_START(__UNDEFINED_STACK_START)
    RUN_END(__UNDEFINED_STACK_END)
    } >> R5F_TCMA | R5F_TCMB | OCRAM


    GROUP {
        .ARM.exidx:  {} palign(8)   /* Needed for C++ exception handling */
        .init_array: {} palign(8)   /* Contains function pointers called before main */
        .fini_array: {} palign(8)   /* Contains function pointers called after main */
    } > OCRAM 

    /* General purpose user shared memory, used in some examples */
    .bss.user_shared_mem (NOLOAD) : {} > USER_SHM_MEM
    /* this is used when Debug log's to shared memory are enabled, else this is not used */
    .bss.log_shared_mem  (NOLOAD) : {} > LOG_SHM_MEM
    /* this is used only when IPC RPMessage is enabled, else this is not used */
    .bss.ipc_vring_mem   (NOLOAD) : {} > RTOS_NORTOS_IPC_SHM_MEM
    /* this is used only when Secure IPC is enabled */
    .bss.sipc_hsm_queue_mem   (NOLOAD) : {} > MAILBOX_HSM
    .bss.sipc_r5f_queue_mem   (NOLOAD) : {} > MAILBOX_R5F   


}


MEMORY
{
    R5F_VECS   : ORIGIN = 0x0 , LENGTH = 0x40 
    R5F_TCMA   : ORIGIN = 0x40 , LENGTH = 0x7FC0 
    R5F_TCMB   : ORIGIN = 0x80000 , LENGTH = 0x8000 
    OCRAM   : ORIGIN = 0x70080000 , LENGTH = 0x40000 
    FLASH   : ORIGIN = 0x60180000 , LENGTH = 0x80000 

    /* For memory Regions not defined in this core but shared by other cores with the current core */

    USER_SHM_MEM   : ORIGIN = 0x701D0000 , LENGTH = 0x4000  
    LOG_SHM_MEM   : ORIGIN = 0x701D4000 , LENGTH = 0x4000  
    RTOS_NORTOS_IPC_SHM_MEM   : ORIGIN = 0x72000000 , LENGTH = 0x3E80  
    MAILBOX_HSM   : ORIGIN = 0x44000000 , LENGTH = 0x3CE  
    MAILBOX_R5F   : ORIGIN = 0x44000400 , LENGTH = 0x3CE  

}
