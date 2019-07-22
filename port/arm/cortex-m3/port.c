#include <port.h>
#include <dos_config.h>

#ifndef DOS_SYSTEM_CLOCK_HZ
    #error "DOS_SYSTEM_CLOCK_HZ must be defined as the system frequency, such as '#define DOS_SYSTEM_CLOCK_HZ (SystemCoreClock)' "
#endif // !DOS_SYSTEM_CLOCK_HZ


extern void Dos_TaskExit(void);

dos_uint32 *Dos_StackInit(dos_uint32 *sp , 
                          void *task_entry,
                          dos_void *parameter)
{
	*--sp = 0x01000000L;	/* xPSR */
	*--sp = ((dos_uint32)task_entry);	/* PC */
	*--sp = (dos_uint32)Dos_TaskExit;	/* LR */
	/* R12, R3, R2 and R1. */
	sp -=4;	
	*--sp = ( dos_uint32 ) parameter;	/* R0 */
	/* R11, R10, R9, R8, R7, R6, R5 and R4. */
	sp -=8;	
  
	return sp;
}



dos_uint32 Dos_StartScheduler( void )
{
	/* 配置PendSV 和 SysTick 的中断优先级为最低 */
	SYSPRI2_REG |= (PENDSV_PRI | SYSTICK_PRI);


	SYSTICK_LOAD_REG = ( DOS_SYSTEM_CLOCK_HZ / DOS_SYSTICK_CLOCK_HZ ) - 1UL;

	SYSTICK_CTRL_REG = ( SYSTICK_CLK_BIT |
                       SYSTICK_INT_BIT |
                       SYSTICK_ENABLE_BIT );

	/* 启动第一个任务，不再返回 */
	Dos_StartFirstTask();

	return 0;
}


/**
 * Is the context environment interrupted?
 */
dos_bool Dos_ContextIsInt(void)
{
	return (__get_IPSR() != 0) ? DOS_TRUE : DOS_FALSE; 
}





