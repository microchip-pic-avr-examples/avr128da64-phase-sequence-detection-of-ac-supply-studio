/*
 * application.c
 *
 * Created: 12/13/2019 1:58:18 PM
 *  Author: I20946
 */ 
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "adc_basic.h"
#include "application.h"
#include "dataVisualizer.h"
#include "atmel_start_pins.h"
#include "usart_basic.h"
#include "AD9833.h"
#include "AD5227.h"

WfclkSel_st wfclkSel_st;

uint8_t RxData[2] = {0};
uint8_t Lines[4] ={4,5,6};
uint8_t freq_sel = DeFHz;
uint16_t ADC_Results_L[4] = {0};
uint16_t numberofSamples = 0 ;
uint16_t tergatedSamples = 80;
float line_voltage[3] = {0};
float freqency = DEFAULT_FREQ1;
float Vrms[3] = {0.00};

extern volatile uint8_t PhaseLossFlag[3];
extern volatile uint8_t timer1OverflowFlag;
extern volatile uint16_t timerCounterValue;
extern volatile uint16_t timer1OvfCnt;
extern volatile uint32_t refreshCount;

extern DataStremerFrame_u dataStremerFrame_u;

static void read3PhaseSignal();
static void phaseLossDetection();
static void sineWaveGeneration(uint8_t AD9833,float frequency,float phase);
static uint8_t emulatorTaskProcess (uint8_t sw);
static uint8_t phaseReversalDetection(void);
static uint16_t waveformAproxPhasecalculation(float freqency);
static uint32_t waveformAproxFreqcalculation(float freqency);
static float calculateVrms();



/*******************************************************************************
 * void application(void)
 *
 * API to run the application, this API schedule to reproduce the simulated 3-p signal 
 * and receive user command and pass to process the command  
 *
 * @param void 
 * @return void 
 ******************************************************************************/
void application(void)
{
	uint16_t i = 0;
		
    if(timer1OverflowFlag  == 1)
    {
	   timer1OverflowFlag = 0;
	   read3PhaseSignal();                  //read three phase signal for calculate vrms voltage and reproduce emulator signal on data visualizer 
	   createDataStreamProtocolAndSend();   //Create data in to data stremer protocol format and send to data visualizer through USART 
	   phaseLossDetection();                //phase loss detection 
	   phaseReversalDetection();            //phase reversal detection 
	   calculateVrms();                     //calculate Vrms (*Note- timeOverflow counter is rested in this function)
    }
	
	if (refreshCount >= REFRESH_COUNT)
	{
	   refreshCount = 0;
	   emulatorTaskProcess(RECOVER_S);      //refreshing simulated 3phase signal for stabilized signal on screen	
	}
	
	if(USART_0_is_rx_ready())
	{
		for (i=0;i<=1;i++)                      //reading two bytes from the data visualizer
		{
			RxData[i] = USART_0_read();
		}
		refreshCount = 0;                      //reseting the refresh count when user is operating the system
		timer1OverflowFlag = 0;               //explicitly reseting to avoid case 7 when simulation command received
		emulatorTaskProcess(RxData[1]);                  
	}
}

/*******************************************************************************
 * static uint8_t emulatorTaskProcess (uint8_t sch_status)
 *
 * main scheduler API call phase loss function, phase reversal function, user input
 * simulation command process.  
 *
 * @param void 
 * @return void 
 ******************************************************************************/
static uint8_t emulatorTaskProcess (uint8_t sch_status)
{	                 
 switch(sch_status)
 {
    case RECOVER_S:
		sineWaveGeneration(WfClickSG_1,freqency,PHASE_SHIFT1);
		sineWaveGeneration(WfClickSG_2,freqency,PHASE_SHIFT2);
		sineWaveGeneration(WfClickSG_3,freqency,PHASE_SHIFT3);
	break;
 	case RMV_R_S:                                                         //remove third line from the 3-phase 
		sineWaveGeneration(WfClickSG_3,ZERO_FREQ,PHASE_SHIFT1);    
	break;
	case RMV_Y_S:                                                         //remove second line from the 3-phase
	    sineWaveGeneration(WfClickSG_2,ZERO_FREQ,PHASE_SHIFT1);
	break;
	case RMV_B_S:                                                         //remove first line from the 3-phase 
	    sineWaveGeneration(WfClickSG_1,ZERO_FREQ,PHASE_SHIFT1);
	break;
	case PHASE_RVRSL_S:                                                         //changing the third line for phase reversal simulation 
		sineWaveGeneration(WfClickSG_1,freqency,PHASE_SHIFT3);
		sineWaveGeneration(WfClickSG_2,freqency,PHASE_SHIFT2);
		sineWaveGeneration(WfClickSG_3,freqency,PHASE_SHIFT1);
	break;
	default: 
		return 1;
	break;
 }
return 0;
}
/*******************************************************************************
 * static void read3PhaseSignal(void)
 *
 * API to read 3-phase signals (R Y B) through ADC channels.
 *
 * @param void 
 * @return void 
 ******************************************************************************/
static void read3PhaseSignal()
{
	uint16_t i= 0;
	
	for (i= 0;i<=2;i++)                     // read signal for reproducing the simulated 3-phase input and Vrms calculation
	{
#if(!ADC_MODE)
	  ADC_Results_L[i] = ADC_0_get_conversion(Lines[i]);
#else
	  ADC_Results_L[i] = ADC_0_get_diff_conversion(Lines[i],ADC_MUXNEG_GND_gc);
#endif
	  if(tergatedSamples >= timer1OvfCnt)
	  {
		  if(ADC_Results_L[i] > ADC_OFFSET_VALUE)
		  {
			numberofSamples++;
			line_voltage[i] += pow((ADC_STEPS_PER_VOLTAGE * ADC_Results_L[i]),2);
		  }
	  }
	}
}

/*******************************************************************************
 * static uint8_t phaseReversalDetection(void)
 *
 * API to calculate time in between the two phases and update the phase reversal flag 
 * field in the data streamer buffer.
 *
 * @param void 
 * @return uint8_t No phase reversal if 0, phase reversal if 1
 ******************************************************************************/
static uint8_t phaseReversalDetection(void)
{
	volatile float phaseShiftTime = 0.0;
	
	phaseShiftTime = timerCounterValue * TIMER_TIME_PER_CLOCK;
	if ((phaseShiftTime > 0.003) && (phaseShiftTime < 0.008))				//Check the Vrms voltage to detect the phase loss by checking the voltage in between 0.003 to 0.008
	{
		dataStremerFrame_u.dataStreamer_st.phaseReversalFlag = 0x00;    
		return 0;                 //No reversal identified
	}
	else
	{
		dataStremerFrame_u.dataStreamer_st.phaseReversalFlag = 0x01;
		return 1;                //Phase is reversal in between L2 and L3
	}
}

/*******************************************************************************
 * static void phaseLossDetection(uint8_t line)
 *
 * API to calculate the 3 phase input Vrms and update the phase loss flag field of 
 * data stream buffer
 *
 * @param which signal Vrms calculation has to be done
 * @return void
 ******************************************************************************/
static void phaseLossDetection()
{	
	uint8_t j = 0;

	for (j=0;j<=2;j++)
	{
	  dataStremerFrame_u.dataStreamer_st.phaseLossFlag[j] = PhaseLossFlag[j];
	}

}

/*******************************************************************************
 * static float calculateVrms(uint16_t channel)
 *
 * API to calculate the Vrms for the requested line
 *
 * @param channel - which signal Vrms calculation has to be done
 * @return return calculated rms voltage 
 ******************************************************************************/
static float calculateVrms()
{
   float voltage = 0.0;
   uint8_t channel = 0;
   
  	if (!(tergatedSamples >= timer1OvfCnt))
  	{
	  	timer1OvfCnt = 0;
	  	for (channel= 0;channel<=2;channel++)                                          //Loop 3 time for Three-Phases
	  	{
		  	voltage = (line_voltage[channel] / numberofSamples);    //averaging the cumulative raw adc addition value
		  	line_voltage[channel] = 0;
		  	Vrms[channel] = sqrt(voltage);
		  	Vrms[channel] = (uint16_t)(Vrms[channel] * 100 + .5);    //this calculation is to make two digit after decimal point
		  	Vrms[channel] /= 100;
	  	}
		numberofSamples =0;
  	}
  	return (Vrms[channel]);
}
/*******************************************************************************
 * void AD9833Select(uint8_t selAD9833)
 *
 * API to select the SPI slave device (AD9833 device)
 *
 * @param which slave to select
 * @return void
 ******************************************************************************/
void AD9833Select(uint8_t selAD9833)
{
	wfclkSel_st.AD9833_sel = selAD9833;
}

/*******************************************************************************
 * void AD5227Select(uint8_t selAD5227)
 *
 * API to select the SPI slave device(AD5277)
 *
 * @param which slave to select
 * @return void
 ******************************************************************************/
void AD5227Select(uint8_t selAD5227)
{
	wfclkSel_st.AD5227_sel = selAD5227;
}

/*******************************************************************************
 *static void sineWaveGeneration(uint8_t AD9833,float frequency, float phas)
 *
 * API to generate sine wave on selected SPI module 
 *
 * @param  AD9833 -SPI slave number, frequency - frequency of the signal, phas - 
 * phase of the signal 
 * @return void
 ******************************************************************************/
static void sineWaveGeneration(uint8_t AD9833,float frequency,float phas)
{
  uint32_t freq = 0;
  uint16_t phase = 0;
	
	 freq= waveformAproxFreqcalculation(frequency);        //Generate sine wave with provided characteristic input 
	 phase = waveformAproxPhasecalculation(phas);
	 AD9833Select(AD9833);
	 AD9833_Reset();
	 AD9833_SetFrequency(AD9833_REG_FREQ0, freq); 
	 AD9833_SetPhase(AD9833_REG_PHASE0,phase);
	 AD9833_ClearReset();
	 AD9833_Setup(AD9833_FSEL0, AD9833_PSEL0, AD9833_OUT_SINUS);
	 
}

/*******************************************************************************
 * void threePhaseAmplitudeMax()
 *
 * API to increase the amplitude of the 3 phase signals (L1 L2 L3)
 *
 * @param void
 * @return void
 ******************************************************************************/
void threePhaseAmplitudeMax()
{
	uint8_t i,j = 0;
	
	for (i=1;i<=3;i++)						//Increase L1 L2 L3 phase amplitude to maximum 
	{
		for (j=0;j<=20;j++)
		{
			AD5227Select(i);
			AD5227DigipotInc();
		}
	}
}
/*******************************************************************************
 * void threePhaseAmplitudeMin()
 *
 * API to increase the amplitude of the 3 phase signals (L1 L2 L3)
 *
 * @param void
 * @return void
 ******************************************************************************/
void threePhaseAmplitudeMin()
{
	uint8_t i,j = 0;
	
	for (i=1;i<=3;i++)					//Decrease L1 L2 L3 phase amplitude to minimum 
	{
		for (j=0;j<=9;j++)
		{
			AD5227Select(i);
			AD5227DigipotDec();
		}
	}
}

/*******************************************************************************
 * uint32_t waveformAproxFreqcalculation(float freqency)
 *
 * API to calculate the frequency register value of the AD9833
 *
 * @param frequency 
 * @return frequency register value 
 ******************************************************************************/
static uint32_t waveformAproxFreqcalculation(float freqency)
{
	uint32_t calculation = 0;
	
	calculation =(uint32_t) (freqency * ( WAVEFORM_CONSTANT / WAVEFORM_OSC_FREQ ) + 0.5);
	
	return calculation;
}

/*******************************************************************************
 * uint16_t waveformAproxPhasecalculation(float phase)
 *
 * API to calculate the phase register value of the AD9833
 *
 * @param phase
 * @return phase register value 
 ******************************************************************************/
static uint16_t waveformAproxPhasecalculation(float phase)
{
	uint16_t calculation = 0;
	
	calculation = ((phase * BITS_PER_DEG) );  
	
	return calculation;
}

/*******************************************************************************
 * void FrequencySelectionCallBack(uint8_t sel)
 *
 * API to select the frequency for application 
 *
 * @param sel - user input for selecting the frequency  
 * @return void  
 ******************************************************************************/
void FrequencySelectionCallBack(uint8_t sel)
{
	if (sel)
	{
		freq_sel = F60Hz;	
		freqency = DEFAULT_FREQ2;	
		tergatedSamples = NUMBER_OF_SAMPLES_60Hz; 
	}
	else
	{
		freq_sel = F50Hz;
		freqency = DEFAULT_FREQ1;
		tergatedSamples = NUMBER_OF_SAMPLES_50Hz;
	}
	createDataStreamProtocolAndSend();
}

/*******************************************************************************
 * void defaultFrequencyGeneration()
 *
 * API to generate default signals for 3PSD input 
 *
 * @param void
 * @return void 
 ******************************************************************************/
void defaultSignalGeneration()
{
	sineWaveGeneration(WfClickSG_1,freqency,PHASE_SHIFT1);			//Default frequency generation with user desired frequency 
	
	sineWaveGeneration(WfClickSG_2,freqency,PHASE_SHIFT2);
		
	sineWaveGeneration(WfClickSG_3,freqency,PHASE_SHIFT3);
}

/*******************************************************************************
 * void readSwToSelectFrequency()
 *
 * API to read switch to select frequency (user command) 
 *
 * @param void
 * @return void 
 ******************************************************************************/
void readSwToSelectFrequency()
{
	uint8_t freq_toggle_flag = 0;
	while (timer1OvfCnt <= SWITCH_TIME_COUNT)         //using timer1 and waiting for user to select the frequency
	{												  //if user not select any frequency, default (50Hz) will be used
		if (SW_get_level() == false)				  //Read switch 
		{
			timer1OvfCnt = 0;
			freq_toggle_flag = ~freq_toggle_flag;
			FrequencySelectionCallBack(freq_toggle_flag);
			_delay_ms(500);                                     //de-bounce delay
		}
	} 
}

