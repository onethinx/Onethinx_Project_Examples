/*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using CyDesigner.Extensions.Gde;

namespace c_Timer_v2_80
{
    public class CyClock
    {
        /// <summary>
        /// Returns external clock value
        /// </summary>
        public static double GetExternalClockHz(ICyTerminalQuery_v1 termQuery)
        {
            return GetClockHz(termQuery, "clock", false);
        }

        private static double GetClockHz(ICyTerminalQuery_v1 termQuery, string termName, bool internalClock)
        {
            List<CyClockData> clkdata = internalClock ? termQuery.GetClockData(termName, "clock", 0) :
                                                        termQuery.GetClockData(termName, 0);
            if (clkdata.Count == 1)
            {
                if (clkdata[0].IsFrequencyKnown)
                {
                    return clkdata[0].Frequency * Math.Pow(10, clkdata[0].UnitAsExponent);
                }
            }
            return -1;
        }

        public static bool IsFrequencyKnown(double frequency)
        {
            return frequency > 0;
        }
    }
}