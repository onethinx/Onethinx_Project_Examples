/*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;
using System.Reflection;

namespace c_UART_v2_50
{
    public delegate void ClockFrequencyChangeDelegate(float clockValue);

    [CyCompDevCustomizer()]
    public class CyCustomizer : ICyParamEditHook_v1, ICyBootLoaderSupport
    {
        public const string BASIC_TAB_NAME = "ConfigureTab";
        public const string ADVANCED_TAB_NAME = "AdvancedTab";

        #region ICyParamEditHook_v1 Members
        public DialogResult EditParams(ICyInstEdit_v1 edit, ICyTerminalQuery_v1 termQuery, ICyExpressMgr_v1 mgr)
        {
            CyUARTParameters parameters = new CyUARTParameters(edit);
            ICyTabbedParamEditor editor = edit.CreateTabbedParamEditor();

            CyUartAdvancedControl editControlAdv = new CyUartAdvancedControl(edit, termQuery, parameters);
            CyUartControl editControl = new CyUartControl(edit, parameters);

            CyParamExprDelegate paramChanged = delegate(ICyParamEditor custEditor, CyCompDevParam param)
            {
                parameters.LoadParams();
                switch (param.Name)
                {
                    case CyParamNames.INTERNAL_CLOCK:
                        editControl.UpdateExternalClockLabel();
                        editControlAdv.UpdateInternalClock();
                        break;

                    case CyParamNames.RX_ENABLE:
                    case CyParamNames.TX_ENABLE:
                    case CyParamNames.HALF_DUPLEX_EN:
                        if (param.ErrorCount == 0)
                            if (editControl.InExpView) editControl.UpdateRXTXButtons();
                        editControlAdv.CheckRXTXEnable();
                        break;

                    case CyParamNames.BAUD_RATE:
                        editControl.UpdateBaudRate();
                        break;

                    case CyParamNames.NUM_DATA_BITS:
                        editControl.UpdateNumDataBits();
                        break;

                    case CyParamNames.PARITY_TYPE:
                        editControl.UpdateParityType();
                        break;

                    case CyParamNames.NUM_STOP_BITS:
                        editControl.UpdateNumStopBits();
                        break;

                    case CyParamNames.FLOW_CONTROL:
                        editControl.UpdateFlowControl();
                        break;

                    case CyParamNames.INT_ON_ADDRESS_DETECT:
                        editControlAdv.UpdateOnAddressDetect();
                        break;

                    case CyParamNames.INT_ON_ADDRESS_MATCH:
                        editControlAdv.UpdateOnAddressMatch();
                        break;

                    case CyParamNames.INT_ON_BREAK:
                        editControlAdv.UpdateOnBreak();
                        break;

                    case CyParamNames.INT_ON_BYTE_RCVD:
                        editControlAdv.UpdateOnByteRcvd();
                        break;

                    case CyParamNames.INT_ON_OVERRUN_ERROR:
                        editControlAdv.UpdateOnOverrrunError();
                        break;

                    case CyParamNames.INT_ON_PARITY_ERROR:
                        editControlAdv.UpdateOnParityError();
                        break;

                    case CyParamNames.INT_ON_STOP_ERROR:
                        editControlAdv.UpdateOnStopError();
                        break;

                    case CyParamNames.INTERRUPT_ON_TX_COMPLETE:
                        editControlAdv.UpdateOnTXComplete();
                        break;

                    case CyParamNames.INTERRUPT_ON_TX_FIFO_EMPTY:
                        editControlAdv.UpdateOnTXFifoEmpty();
                        break;

                    case CyParamNames.INTERRUPT_ON_TX_FIFO_FULL:
                        editControlAdv.UpdateOnTXFifoFull();
                        break;

                    case CyParamNames.INTERRUPT_ON_TX_FIFO_NOT_FULL:
                        editControlAdv.UpdateOnTXFifoNotFull();
                        break;

                    case CyParamNames.RX_ADDRESS_MODE:
                        editControlAdv.UpdateRXAddressMode();
                        break;

                    case CyParamNames.ADDRESS1:
                        editControlAdv.UpdateAddress1();
                        break;

                    case CyParamNames.ADDRESS2:
                        editControlAdv.UpdateAddress2();
                        break;

                    case CyParamNames.RX_BUFFER_SIZE:
                        editControlAdv.UpdateRXBufferSize();
                        break;

                    case CyParamNames.TX_BUFFER_SIZE:
                        editControlAdv.UpdateTXBufferSize();
                        break;

                    case CyParamNames.BREAK_BITS_TX:
                        editControlAdv.UpdateBreakBitsTX();
                        break;

                    case CyParamNames.BREAK_DETECT:
                        editControlAdv.UpdateBreakBitsTX();
                        break;

                    case CyParamNames.USE23_POLLING:
                        editControlAdv.UpdateUse23Polling();
                        break;

                    case CyParamNames.CRC_OUTPUTS_EN:
                        editControlAdv.UpdateCRCoutputsEn();
                        break;

                    case CyParamNames.HW_TX_EN_SIGNAL:
                        editControlAdv.UpdateHwTXEnSignal();
                        break;

                    case CyParamNames.PARITY_TYPE_SW:
                        editControl.UpdateParitySoftware();
                        break;

                    case CyParamNames.OVER_SAMPLING_RATE:
                        editControl.UpdateExternalClockFrequency(parameters.m_exClockFreq);
                        editControlAdv.UpdateOverSamplingRate();
                        break;
                }
            };

            editControlAdv.ExClockFrequencyChanged += editControl.UpdateExternalClockFrequency;

            editor.AddCustomPage(Resources.ConfigureTabCaption, editControl, paramChanged,
                BASIC_TAB_NAME);
            editor.AddCustomPage(Resources.AdvancedTabCaption, editControlAdv, paramChanged,
                ADVANCED_TAB_NAME);
            editor.AddDefaultPage(Resources.BuiltInTabCaption, "Built-in");
            edit.NotifyWhenDesignUpdates(new CyDesignUpdated_v1(editControlAdv.UpdateClock));

            DialogResult result = editor.ShowDialog();

            editControlAdv.ExClockFrequencyChanged -= editControl.UpdateExternalClockFrequency;

            return result;
        }

        public bool EditParamsOnDrop
        {
            get { return false; }
        }

        public CyCompDevParamEditorMode GetEditorMode()
        {
            return CyCompDevParamEditorMode.COMPLETE;
        }
        #endregion

        #region ICyBootLoaderSupport Members
        const UInt16 MIN_BUFFER_SIZE_SUPPORT = 64;

        public CyCustErr IsBootloaderReady(ICyInstQuery_v1 inst)
        {
            bool txEnabled;
            bool rxEnabled;
            UInt16 rxBufSize;

            CyCustErr err = inst.GetCommittedParam(CyParamNames.RX_ENABLE).TryGetValueAs<bool>(out txEnabled);
            if (err.IsNotOK)
            {
                return err;
            }

            err = inst.GetCommittedParam(CyParamNames.TX_ENABLE).TryGetValueAs<bool>(out rxEnabled);
            if (err.IsNotOK)
            {
                return err;
            }

            if (txEnabled ^ rxEnabled)
            {
                return new CyCustErr(Resources.BootloaderSupportModeFailError);
            }

            err = inst.GetCommittedParam(CyParamNames.RX_BUFFER_SIZE).TryGetValueAs<UInt16>(out rxBufSize);
            if (err.IsNotOK)
            {
                return err;
            }

            if (rxBufSize < MIN_BUFFER_SIZE_SUPPORT)
            {
                return new CyCustErr(string.Format(Resources.BootloaderSupportBufferSizeFailError,
                    MIN_BUFFER_SIZE_SUPPORT));
            }

            return CyCustErr.OK;
        }
        #endregion
    }
}
