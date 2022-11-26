
/********************************************************************************
 *    ___             _   _     _			
 *   / _ \ _ __   ___| |_| |__ (_)_ __ __  __
 *  | | | | '_ \ / _ \ __| '_ \| | '_ \\ \/ /
 *  | |_| | | | |  __/ |_| | | | | | | |>  < 
 *   \___/|_| |_|\___|\__|_| |_|_|_| |_/_/\_\
 *
 ********************************************************************************
 *
 * Copyright (c) 2019-2022 Onethinx BV <info@onethinx.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ********************************************************************************
 *
 * Created by: Onethinx on 2021-02-12
 *
 * Sample project to demostrate the integration of a PSoC Creator project
 * into Visual Studio Code
 * 
 * For a description please see:
 * https://github.com/onethinx/Onethinx_Creator
 *
 ********************************************************************************/

#include "project.h"
#include "OnethinxCore01.h"
#include "LoRaWAN_keys.h"

/* Go to ../OnethinxCore/LoRaWAN_keys.h and fill in the fields of the TTN_OTAAkeys structure */

coreConfiguration_t	coreConfig = {
	.Join.KeysPtr = 		&Keys_0,
	.Join.DataRate =		DR_AUTO,
	.Join.Power =			PWR_MAX,
	.Join.MAXTries = 		100,
    .Join.SubBand_1st =     EU_SUB_BANDS_DEFAULT,
	.Join.SubBand_2nd =     EU_SUB_BANDS_DEFAULT,
	.TX.Confirmed = 		false,
	.TX.DataRate = 			DR_ADR,		// Adaptive Data Rate
	.TX.Power = 			PWR_ADR,	// Adaptive Data Rate
	.TX.FPort = 			1,
	.System.Idle.Mode = 	M0_DeepSleep,
	.System.Idle.BleEcoON = false,
	.System.Idle.DebugON =  true,
	.RX.Boost			  = false,
};

sleepConfig_t sleepConfig =
{
	.sleepMode = modeDeepSleep,
	.BleEcoON = false,
	.DebugON = true,
	.sleepCores = coresBoth,
	.wakeUpPin = wakeUpPinHigh(true),
	.wakeUpTime = wakeUpDelay(0, 0, 0, 10), // day, hour, minute, second
};

/*******************************************************************************arm-none-eabi-gcc
* Function Name: main()
********************************************************************************
* Summary:
*  Sends analog data using LoRaWAN.
* Details:
*  Go to ../OnethinxCore/LoRaWAN_keys.h and fill in the fields of the TTN_OTAAkeys structure
*******************************************************************************/

/* OnethinxCore uses the following structures and variables, which can be defined globally */
coreStatus_t 	coreStatus;
coreInfo_t 		coreInfo;

uint8_t RXbuffer[64];
uint8_t TXbuffer[64];

#include <cyfitter_cfg.h>

int main(void)
{
	volatile int32_t adcResult = 0; 
	volatile int32_t voltage = 0;
	uint8_t j = 0;

	/* enable global interrupts */
	__enable_irq();

	/* initialize radio with parameters in coreConfig */
	coreStatus = LoRaWAN_Init(&coreConfig);

	/* send join using parameters in coreConfig, blocks until either success or MAXtries */
	coreStatus = LoRaWAN_Join(M4_WaitDeepSleep);

	/* check for successful join */
	if (!coreStatus.mac.isJoined)
		while(1);

	ADC_Start();

	/* main loop */
	for(;;)
	{
		/* Read the voltage value */
		ADC_StartConvert();
		ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT);
		adcResult = ADC_GetResult32(0);
		if (adcResult > 65535) adcResult = 65535;
        if (adcResult < 0) adcResult = 0; 
		voltage = ADC_CountsTo_mVolts(0, adcResult);

		/* Create the message */
		j = 0;
		TXbuffer[j++] = (voltage >> 24) & 0xFF;
		TXbuffer[j++] = (voltage >> 16) & 0xFF;
		TXbuffer[j++] = (voltage >>  8) & 0xFF;
		TXbuffer[j++] = (voltage >>  0) & 0xFF;

		/* Send message over LoRaWAN */
        coreStatus = LoRaWAN_Send(TXbuffer, j, M4_WaitDeepSleep);

		/* Sleep before sending next message, wake up with a button as well */
		LoRaWAN_Sleep(&sleepConfig);
	}
}


/* [] END OF FILE */
