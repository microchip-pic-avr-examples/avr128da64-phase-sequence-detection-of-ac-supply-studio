/*
 * application_isr.c
 *
 * Created: 6/21/2020 5:54:21 PM
 *  Author: I20946
 */ 
#include "application_isr.h"


volatile uint16_t timerCounterValue = {0};
volatile uint8_t timer1OverflowFlag = {0};
volatile uint8_t ZCDPhaseLossFlag[3] = {0};
volatile uint8_t ZCDPhaseRvrslFlag[3] = {0};
volatile uint8_t PhaseLossFlag[3] = {0};
volatile uint32_t refreshCount = 0;
volatile uint16_t timer1OvfCnt = 0;

ISR(ZCD0_ZCD_vect)
{
	/* Insert your ZCD interrupt handling code here */
	if (ZCDPhaseLossFlag[1] == 1)
	{
		PhaseLossFlag[2] = 1;
	}
	TCA0_SINGLE_CNT = 0;
	ZCDPhaseLossFlag[2] = 0;
	
	ZCDPhaseLossFlag[0] = 1;
	PhaseLossFlag[0] = 0;
	
	ZCDPhaseRvrslFlag[0] = 1;
	ZCD0.STATUS = ZCD_CROSSIF_bm;
}

ISR(ZCD1_ZCD_vect)
{
	/* Insert your ZCD interrupt handling code here */
	if (ZCDPhaseLossFlag[2] == 1)
	{
		PhaseLossFlag[0] = 1;
	}
	if (ZCDPhaseRvrslFlag[0] == 1)
	{
		ZCDPhaseRvrslFlag[0] = 0;
		timerCounterValue = TCA0_SINGLE_CNT;
	}
	ZCDPhaseLossFlag[0] = 0;
	
	TCA0_SINGLE_CNT = 0;
	ZCDPhaseLossFlag[1] = 1;
	PhaseLossFlag[1] = 0;
	
	ZCDPhaseRvrslFlag[1] = 1;
	ZCD1.STATUS = ZCD_CROSSIF_bm;
}

ISR(ZCD2_ZCD_vect)
{
	/* Insert your ZCD interrupt handling code here */
	if (ZCDPhaseLossFlag[0] == 1)
	{
		PhaseLossFlag[1] = 1;
	}
	ZCDPhaseLossFlag[1] = 0;
	//timer_counter_value[1] = TCA0_SINGLE_CNT;
	
	ZCDPhaseLossFlag[2] = 1;
	PhaseLossFlag[2] = 0;
	
	ZCD2.STATUS = ZCD_CROSSIF_bm;
}

ISR(TCA0_OVF_vect)
{
	/* Insert your TCA overflow interrupt handling code */

	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

ISR(TCA1_OVF_vect)
{
	/* The interrupt flag has to be cleared manually */
	TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
	/* Insert your TCA overflow interrupt handling code */
	timer1OverflowFlag = 1;
	timer1OvfCnt = timer1OvfCnt + 1;
	refreshCount += 1;
}
