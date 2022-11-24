
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
 * Created by: Rolf Nooteboom on 2021-05-27
 *
 * Sample project to demonstrate SWV debugging with the
 *  the Onethinx module using JLink debugger
 * 
 * For a description please see:
 * https://github.com/onethinx/Onethinx_Creator
 *
 ********************************************************************************/

#include <project.h>
#include <stdio.h>


// Use UART SWV emulation to output data on SWO port.
int _write(int file, char *ptr, int len) 
{
    for (int i = 0; i < ((len + 3) & ~3); i++) 
    {
		if ((i&3) == 0)
		{
			while((UART_1_TXSTATUS_REG & UART_1_TX_STS_FIFO_FULL) != 0u) {}	// Wait for room in FIFO
        	UART_1_TXDATA_REG = 0x03;
		}
		while((UART_1_TXSTATUS_REG & UART_1_TX_STS_FIFO_FULL) != 0u) {}	// Wait for room in FIFO
        UART_1_TXDATA_REG = (i < len)? ptr[i]:0;		
    }
    return len;
}

int main(void)
{
	uint32_t i = 0;
	CyDelay(200);
	UART_1_Start();
	while (1)
	{
		printf("\r\nTest count:%ld", i++);
        CyDelay(1000);
	}
}

/* [] END OF FILE */
