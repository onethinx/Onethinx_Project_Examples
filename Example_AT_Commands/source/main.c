/********************************************************************************
 *    ___             _   _     _			
 *   / _ \ _ __   ___| |_| |__ (_)_ __ __  __
 *  | | | | '_ \ / _ \ __| '_ \| | '_ \\ \/ /
 *  | |_| | | | |  __/ |_| | | | | | | |>  < 
 *   \___/|_| |_|\___|\__|_| |_|_|_| |_/_/\_\
 *
 ********************************************************************************
 *
 * Copyright (c) 2019 Onethinx BV <info@onethinx.com>
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
 * Created by: Rolf Nooteboom on 2019-08-03
 *
 * Library to use AT commands with the Onethinx Core LoRaWAN module
 * 
 * For a description please see:
 * https://github.com/onethinx/OT_ATCommandSet_VScode/blob/master/README.md
 *
 ********************************************************************************/

#include <project.h>
#include "OnethinxCore01.h"
#include "OnethinxExt01.h"
#include "ATinterface.h"


int main(void)
{
		
	__enable_irq();  /* enable global interrupts */
	Cy_GPIO_Write(LED_R_PORT, LED_R_NUM, 0);
	Cy_GPIO_Write(LED_G_PORT, LED_G_NUM, 0);
	Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 0);

	UART_Start();

	while (1) {
		ATcomm();
	}
}

/* [] END OF FILE */
