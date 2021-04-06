/********************************************************************************
 *    ___             _   _     _			
 *   / _ \ _ __   ___| |_| |__ (_)_ __ __  __
 *  | | | | '_ \ / _ \ __| '_ \| | '_ \\ \/ /
 *  | |_| | | | |  __/ |_| | | | | | | |>  < 
 *   \___/|_| |_|\___|\__|_| |_|_|_| |_/_/\_\
 *
 ********************************************************************************
 *
 * Copyright (c) 2020 Onethinx BV <info@onethinx.com>
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
 * Created by: Rolf Nooteboom
 *
 * Extended Library to use with the Open stack Onethinx Core LoRaWAN module.
 * 
 * For a description please see:
 * https://github.com/onethinx/OnethinxCoreAPI
 *
 ********************************************************************************/

#include "OnethinxCore01.h"
#include "OnethinxExt01.h"
#include "cy_gpio.h"

extern  coreStatus_t coreComm(coreFunctions_e function, WaitMode_e waitMode);
extern  volatile coreArguments_t coreArguments;

coreStatus_t LoRa_RX(RadioParams_t * RadioParams, RadioStatus_t * RadioStatus, uint8_t * payload, uint8_t payloadSize, uint16_t timeOutMS, WaitMode_e waitMode)
{
	coreArguments.arg1 = (uint32_t) RadioParams;
	coreArguments.arg2 = (uint32_t) RadioStatus;
	coreArguments.arg3 = (uint32_t) payload;
	coreArguments.arg4 = (payloadSize << 16) | timeOutMS; 
	coreStatus_t coreStatus = coreComm(coreFunction_L_RX, waitMode);
	coreStatus.mac.bytesToRead = coreArguments.arg4;
	return coreStatus;
}

coreStatus_t LoRa_TX(RadioParams_t * RadioParams, RadioStatus_t * RadioStatus, uint8_t * payload, uint8_t payloadSize, uint16_t timeOutMS, WaitMode_e waitMode)
{
	coreArguments.arg1 = (uint32_t) RadioParams;
	coreArguments.arg2 = (uint32_t) RadioStatus;
	coreArguments.arg3 = (uint32_t) payload;
	coreArguments.arg4 = (payloadSize << 16) | timeOutMS; 
	return coreComm(coreFunction_L_TX, waitMode);
}

void LoRaWAN_Debug(bool debugLedsOn, uint32_t * coreStatePNT)
{
	if (debugLedsOn)		// Debug LEDs should be connected to P12_4 and P12_5
	{
		Cy_GPIO_Pin_FastInit(P12_4_PORT, P12_4_NUM, CY_GPIO_DM_STRONG, 1UL, HSIOM_SEL_GPIO);
		Cy_GPIO_Pin_FastInit(P12_5_PORT, P12_5_NUM, CY_GPIO_DM_STRONG, 1UL, HSIOM_SEL_GPIO);
	}
	coreArguments.arg1 = debugLedsOn? 1:0;
	coreArguments.arg2 = (uint32_t) coreStatePNT;
	coreArguments.arg3 = 0;
	coreArguments.arg4 = 0; 
	coreComm(coreFunction_Debug, false);
}

// Use the Unlock function before using any other extended functions. Unlocking may void LoRa Alliance Certification by Similarity.
void LoRaWAN_Unlock()
{
	coreArguments.arg1 = 0x4B1D;
	coreComm(coreFunction_Unlock, true);
}