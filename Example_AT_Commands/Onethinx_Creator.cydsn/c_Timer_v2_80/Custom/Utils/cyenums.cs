/*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Diagnostics; 

namespace c_Timer_v2_80
{
    public enum CyECaptureModes
    {
        B_Timer__CM_None = 0,
        B_Timer__CM_RisingEdge = 1,
        B_Timer__CM_FallingEdge = 2,
        B_Timer__CM_EitherEdge = 3,
        B_Timer__CM_Software = 4,
    }

    public enum CyEEnableModes
    {
        B_Timer__EM_SoftwareOnly = 0,
        B_Timer__EM_HardwareOnly = 1,
        B_Timer__EM_SW_HW = 2,
    }

    public enum CyEResolutions
    {
        B_Timer__R8 = 8,
        B_Timer__R16 = 16,
        B_Timer__R24 = 24,
        B_Timer__R32 = 32
    }

    public enum CyERunModes
    {
        B_Timer__RM_Continuous = 0,
        B_Timer__RM_OneShot = 1,
        B_Timer__RM_OneShotHaltOnInterrupt = 2
    }

    public enum CyETriggerModes
    {
        B_Timer__TM_None = 0,
        B_Timer__TM_RisingEdge = 1,
        B_Timer__TM_FallingEdge = 2,
        B_Timer__TM_EitherEdge = 3,
        B_Timer__TM_Software = 4
    }


    internal class CyEnums
    {
        #region Enum to display name

        public static string ToDisplayName(CyECaptureModes enumItem)
        {
            switch (enumItem)
            {
                case CyECaptureModes.B_Timer__CM_None:
                    return "None";
                case CyECaptureModes.B_Timer__CM_RisingEdge:
                    return "Rising Edge";
                case CyECaptureModes.B_Timer__CM_FallingEdge:
                    return "Falling Edge";
                case CyECaptureModes.B_Timer__CM_EitherEdge:
                    return "Either Edge";
                case CyECaptureModes.B_Timer__CM_Software:
                    return "Software Controlled";
                default:
                    Debug.Fail("unhandled enum item.");
                    return String.Empty;
            }
        }

        public static string ToDisplayName(CyEEnableModes enumItem)
        {
            switch (enumItem)
            {
                case CyEEnableModes.B_Timer__EM_SoftwareOnly:
                    return "Software Only";
                case CyEEnableModes.B_Timer__EM_HardwareOnly:
                    return "Hardware Only";
                case CyEEnableModes.B_Timer__EM_SW_HW:
                    return "Software and Hardware";
                default:
                    Debug.Fail("unhandled enum item.");
                    return String.Empty;
            }
        }

        public static string ToDisplayName(CyEResolutions enumItem)
        {
            switch (enumItem)
            {
                case CyEResolutions.B_Timer__R8:
                    return "8";
                case CyEResolutions.B_Timer__R16:
                    return "16";
                case CyEResolutions.B_Timer__R24:
                    return "24";
                case CyEResolutions.B_Timer__R32:
                    return "32";
                default:
                    Debug.Fail("unhandled enum item.");
                    return String.Empty;
            }
        }

        public static string ToDisplayName(CyERunModes enumItem)
        {
            switch (enumItem)
            {
                case CyERunModes.B_Timer__RM_Continuous:
                    return "Continuous";
                case CyERunModes.B_Timer__RM_OneShot:
                    return "One Shot";
                case CyERunModes.B_Timer__RM_OneShotHaltOnInterrupt:
                    return "One Shot (Halt On Interrupt)";
                default:
                    Debug.Fail("unhandled enum item.");
                    return String.Empty;
            }
        }

        public static string ToDisplayName(CyETriggerModes enumItem)
        {
            switch (enumItem)
            {
                case CyETriggerModes.B_Timer__TM_None:
                    return "None";
                case CyETriggerModes.B_Timer__TM_RisingEdge:
                    return "Rising Edge";
                case CyETriggerModes.B_Timer__TM_FallingEdge:
                    return "Falling Edge";
                case CyETriggerModes.B_Timer__TM_EitherEdge:
                    return "Either Edge";
                case CyETriggerModes.B_Timer__TM_Software:
                    return "Software Controlled";
                default:
                    Debug.Fail("unhandled enum item.");
                    return String.Empty;
            }
        }
        #endregion
    }
}