/*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using CyDesigner.Extensions.Gde;
using System.Windows.Forms;

namespace c_UART_v2_50
{
    #region Component Parameters Names
    public class CyParamNames
    {
        public const string ADDRESS1 = "Address1";
        public const string ADDRESS2 = "Address2";
        public const string BAUD_RATE = "BaudRate";
        public const string BREAK_BITS_RX = "BreakBitsRX";
        public const string BREAK_BITS_TX = "BreakBitsTX";
        public const string BREAK_DETECT = "BreakDetect";
        public const string CRC_OUTPUTS_EN = "CRCoutputsEn";
        public const string EN_INT_RX_INTERRUPT = "EnIntRXInterrupt";
        public const string EN_INT_TX_INTERRUPT = "EnIntTXInterrupt";
        public const string FLOW_CONTROL = "FlowControl";
        public const string HALF_DUPLEX_EN = "HalfDuplexEn";
        public const string HW_TX_EN_SIGNAL = "HwTXEnSignal";
        public const string INTERNAL_CLOCK = "InternalClock";
        public const string INTERRUPT_ON_TX_COMPLETE = "InterruptOnTXComplete";
        public const string INTERRUPT_ON_TX_FIFO_EMPTY = "InterruptOnTXFifoEmpty";
        public const string INTERRUPT_ON_TX_FIFO_FULL = "InterruptOnTXFifoFull";
        public const string INTERRUPT_ON_TX_FIFO_NOT_FULL = "InterruptOnTXFifoNotFull";
        public const string INT_ON_ADDRESS_DETECT = "IntOnAddressDetect";
        public const string INT_ON_ADDRESS_MATCH = "IntOnAddressMatch";
        public const string INT_ON_BREAK = "IntOnBreak";
        public const string INT_ON_BYTE_RCVD = "IntOnByteRcvd";
        public const string INT_ON_OVERRUN_ERROR = "IntOnOverrunError";
        public const string INT_ON_PARITY_ERROR = "IntOnParityError";
        public const string INT_ON_STOP_ERROR = "IntOnStopError";
        public const string NUM_DATA_BITS = "NumDataBits";
        public const string NUM_STOP_BITS = "NumStopBits";
        public const string OVER_SAMPLING_RATE = "OverSamplingRate";
        public const string PARITY_TYPE = "ParityType";
        public const string PARITY_TYPE_SW = "ParityTypeSw";
        public const string RX_ADDRESS_MODE = "RXAddressMode";
        public const string RX_BUFFER_SIZE = "RXBufferSize";
        public const string RX_ENABLE = "RXEnable";
        public const string TX_BIT_CLK_GEN_DP = "TXBitClkGenDP";
        public const string TX_BUFFER_SIZE = "TXBufferSize";
        public const string TX_ENABLE = "TXEnable";
        public const string USE23_POLLING = "Use23Polling";
    }
    #endregion

    #region Component types
    public enum CY_E_PARITY_TYPE
    {
        B_UART__None_revB = 0,
        B_UART__Even_revB = 1,
        B_UART__Odd_revB = 2,
        B_UART__Mark_Space_revB = 3,
    };
    #endregion

    public class CyUARTParameters
    {
        public ICyInstEdit_v1 m_instance;
        public CyUartControl m_basicTab;
        public CyUartAdvancedControl m_advancedTab;
        public float m_exClockFreq;
        public bool m_exClockExist;

        #region Private members
        private byte m_address1;
        private byte m_address2;
        private UInt16 m_rxBufferSize;
        private UInt16 m_txBufferSize;
        private bool m_use23Polling;
        private int m_breakBitsTX;
        private bool m_breakDetect;
        private bool m_crcOutputsEn;
        private bool m_rxEnable;
        private bool m_txEnable;
        private bool m_halfDuplexEnable;
        private bool m_fullEnable;
        private bool m_internalClock;
        private bool m_hwTXEnSignal;
        private bool m_interruptOnTXComplete;
        private bool m_interruptOnTXFifoEmpty;
        private bool m_interruptOnTXFifoFull;
        private bool m_interruptOnTXFifoNotFull;
        private bool m_intOnAddressDetect;
        private bool m_intOnAddressMatch;
        private bool m_intOnBreak;
        private bool m_intOnByteRcvd;
        private bool m_intOnOverrunError;
        private bool m_intOnParityError;
        private bool m_intOnStopError;
        private bool m_paritySoftware;
        private int m_baudRate;
        private string m_numDataBits;
        private string m_numStopBits;
        private string m_parityType;
        private string m_flowControl;
        private string m_rxAddressMode;
        private string m_overSamplingRate;
        #endregion

        #region Properties
        public string OverSamplingRate
        {
            get { return m_overSamplingRate; }
            set
            {
                if (m_overSamplingRate != value)
                {
                    m_overSamplingRate = value;
                    SaveParam(CyParamNames.OVER_SAMPLING_RATE, m_overSamplingRate);
                }
            }
        }

        public byte Address1
        {
            get { return m_address1; }
            set
            {
                if (m_address1 != value)
                {
                    m_address1 = value;
                    SaveParam(CyParamNames.ADDRESS1, m_address1.ToString());
                }
            }
        }

        public byte Address2
        {
            get { return m_address2; }
            set
            {
                if (m_address2 != value)
                {
                    m_address2 = value;
                    SaveParam(CyParamNames.ADDRESS2, m_address2.ToString());
                }
            }
        }

        public int BaudRate
        {
            get { return m_baudRate; }
            set
            {
                if (m_baudRate != value)
                {
                    m_baudRate = value;
                    SaveParam(CyParamNames.BAUD_RATE, m_baudRate.ToString());
                }
            }
        }

        public string FlowControl
        {
            get { return m_flowControl; }
            set
            {
                if (m_flowControl != value)
                {
                    m_flowControl = value;
                    SaveParam(CyParamNames.FLOW_CONTROL, m_flowControl);
                }
            }
        }

        public bool InternalClock
        {
            get { return m_internalClock; }
            set
            {
                if (m_internalClock != value)
                {
                    m_internalClock = value;
                    SaveParam(CyParamNames.INTERNAL_CLOCK, m_internalClock.ToString().ToLower());
                }
            }
        }

        public UInt16 TXBufferSize
        {
            get { return m_txBufferSize; }
            set
            {
                if (m_txBufferSize != value)
                {
                    m_txBufferSize = value;
                    SaveParam(CyParamNames.TX_BUFFER_SIZE, m_txBufferSize.ToString());
                }
            }
        }

        public UInt16 RXBufferSize
        {
            get { return m_rxBufferSize; }
            set
            {
                if (m_rxBufferSize != value)
                {
                    m_rxBufferSize = value;
                    SaveParam(CyParamNames.RX_BUFFER_SIZE, m_rxBufferSize.ToString());
                }
            }
        }

        public bool Use23Polling
        {
            get { return m_use23Polling; }
            set
            {
                if (m_use23Polling != value)
                {
                    m_use23Polling = value;
                    SaveParam(CyParamNames.USE23_POLLING, m_use23Polling.ToString().ToLower());
                }
            }
        }

        public int BreakBitsTX
        {
            get { return m_breakBitsTX; }
            set
            {
                if (m_breakBitsTX != value)
                {
                    m_breakBitsTX = value;
                    SaveParam(CyParamNames.BREAK_BITS_TX, m_breakBitsTX.ToString());
                }
            }
        }

        public int BreakBitsRX
        {
            set
            {
                SaveParam(CyParamNames.BREAK_BITS_RX, value.ToString());
            }
        }

        public bool BreakDetect
        {
            get { return m_breakDetect; }
            set
            {
                if (m_breakDetect != value)
                {
                    m_breakDetect = value;
                    SaveParam(CyParamNames.BREAK_DETECT, m_breakDetect.ToString().ToLower());
                }
            }
        }

        public bool CRCoutputsEn
        {
            get { return m_crcOutputsEn; }
            set
            {
                if (m_crcOutputsEn != value)
                {
                    m_crcOutputsEn = value;
                    SaveParam(CyParamNames.CRC_OUTPUTS_EN, m_crcOutputsEn.ToString().ToLower());
                }
            }
        }

        public bool RXEnable
        {
            get { return m_rxEnable; }
            set
            {
                if (m_rxEnable != value && m_instance != null)
                {
                    m_rxEnable = value;
                    m_instance.SetParamExpr(CyParamNames.RX_ENABLE, m_rxEnable.ToString().ToLower());
                }
            }
        }

        public bool TXEnable
        {
            get { return m_txEnable; }
            set
            {
                if (m_txEnable != value && m_instance != null)
                {
                    m_txEnable = value;
                    m_instance.SetParamExpr(CyParamNames.TX_ENABLE, m_txEnable.ToString().ToLower());
                }
            }
        }

        public bool HalfDuplexEnable
        {
            get { return m_halfDuplexEnable; }
            set
            {
                if (m_halfDuplexEnable != value && m_instance != null)
                {
                    m_halfDuplexEnable = value;
                    m_instance.SetParamExpr(CyParamNames.HALF_DUPLEX_EN, m_halfDuplexEnable.ToString().ToLower());
                }
            }
        }

        public bool FullEnable
        {
            get { return m_fullEnable; }
            set { m_fullEnable = value; }
        }

        public bool HwTXEnSignal
        {
            get { return m_hwTXEnSignal; }
            set
            {
                if (m_hwTXEnSignal != value)
                {
                    m_hwTXEnSignal = value;
                    SaveParam(CyParamNames.HW_TX_EN_SIGNAL, m_hwTXEnSignal.ToString());
                }
            }
        }

        public bool InterruptOnTXComplete
        {
            get { return m_interruptOnTXComplete; }
            set
            {
                if (m_interruptOnTXComplete != value)
                {
                    m_interruptOnTXComplete = value;
                    SaveParam(CyParamNames.INTERRUPT_ON_TX_COMPLETE, m_interruptOnTXComplete.ToString());
                }
            }
        }

        public bool InterruptOnTXFifoEmpty
        {
            get { return m_interruptOnTXFifoEmpty; }
            set
            {
                if (m_interruptOnTXFifoEmpty != value)
                {
                    m_interruptOnTXFifoEmpty = value;
                    SaveParam(CyParamNames.INTERRUPT_ON_TX_FIFO_EMPTY, m_interruptOnTXFifoEmpty.ToString());
                }
            }
        }

        public bool InterruptOnTXFifoFull
        {
            get { return m_interruptOnTXFifoFull; }
            set
            {
                if (m_interruptOnTXFifoFull != value)
                {
                    m_interruptOnTXFifoFull = value;
                    SaveParam(CyParamNames.INTERRUPT_ON_TX_FIFO_FULL, m_interruptOnTXFifoFull.ToString());
                }
            }
        }

        public bool InterruptOnTXFifoNotFull
        {
            get { return m_interruptOnTXFifoNotFull; }
            set
            {
                if (m_interruptOnTXFifoNotFull != value)
                {
                    m_interruptOnTXFifoNotFull = value;
                    SaveParam(CyParamNames.INTERRUPT_ON_TX_FIFO_NOT_FULL, m_interruptOnTXFifoNotFull.ToString());
                }
            }
        }

        public bool IntOnAddressDetect
        {
            get { return m_intOnAddressDetect; }
            set
            {
                if (m_intOnAddressDetect != value)
                {
                    m_intOnAddressDetect = value;
                    SaveParam(CyParamNames.INT_ON_ADDRESS_DETECT, m_intOnAddressDetect.ToString());
                }
            }
        }

        public bool IntOnAddressMatch
        {
            get { return m_intOnAddressMatch; }
            set
            {
                if (m_intOnAddressMatch != value)
                {
                    m_intOnAddressMatch = value;
                    SaveParam(CyParamNames.INT_ON_ADDRESS_MATCH, m_intOnAddressMatch.ToString());
                }
            }
        }

        public bool IntOnBreak
        {
            get { return m_intOnBreak; }
            set
            {
                if (m_intOnBreak != value)
                {
                    m_intOnBreak = value;
                    SaveParam(CyParamNames.INT_ON_BREAK, m_intOnBreak.ToString());
                }
            }
        }

        public bool IntOnByteRcvd
        {
            get { return m_intOnByteRcvd; }
            set
            {
                if (m_intOnByteRcvd != value)
                {
                    m_intOnByteRcvd = value;
                    SaveParam(CyParamNames.INT_ON_BYTE_RCVD, m_intOnByteRcvd.ToString());
                }
            }
        }

        public bool IntOnOverrunError
        {
            get { return m_intOnOverrunError; }
            set
            {
                if (m_intOnOverrunError != value)
                {
                    m_intOnOverrunError = value;
                    SaveParam(CyParamNames.INT_ON_OVERRUN_ERROR, m_intOnOverrunError.ToString());
                }
            }
        }

        public bool IntOnParityError
        {
            get { return m_intOnParityError; }
            set
            {
                if (m_intOnParityError != value)
                {
                    m_intOnParityError = value;
                    SaveParam(CyParamNames.INT_ON_PARITY_ERROR, m_intOnParityError.ToString());
                }
            }
        }

        public bool IntOnStopError
        {
            get { return m_intOnStopError; }
            set
            {
                if (m_intOnStopError != value)
                {
                    m_intOnStopError = value;
                    SaveParam(CyParamNames.INT_ON_STOP_ERROR, m_intOnStopError.ToString());
                }
            }
        }

        public string NumDataBits
        {
            get { return m_numDataBits; }
            set
            {
                if (m_numDataBits != value)
                {
                    m_numDataBits = value;
                    SaveParam(CyParamNames.NUM_DATA_BITS, m_numDataBits);
                }
            }
        }

        public string NumStopBits
        {
            get { return m_numStopBits; }
            set
            {
                if (m_numStopBits != value)
                {
                    m_numStopBits = value;
                    SaveParam(CyParamNames.NUM_STOP_BITS, m_numStopBits);
                }
            }
        }

        public string ParityType
        {
            get { return m_parityType; }
            set
            {
                if (m_parityType != value)
                {
                    m_parityType = value;
                    SaveParam(CyParamNames.PARITY_TYPE, m_parityType);
                }
            }
        }

        public string RXAddressMode
        {
            get { return m_rxAddressMode; }
            set
            {
                if (m_rxAddressMode != value)
                {
                    m_rxAddressMode = value;
                    SaveParam(CyParamNames.RX_ADDRESS_MODE, m_rxAddressMode);
                }
            }
        }

        public bool ParitySoftware
        {
            get { return m_paritySoftware; }
            set
            {
                if (m_paritySoftware != value)
                {
                    m_paritySoftware = value;
                    SaveParam(CyParamNames.PARITY_TYPE_SW, m_paritySoftware.ToString().ToLower());
                }
            }
        }
        #endregion

        #region Constructor(s)
        public CyUARTParameters(ICyInstEdit_v1 inst)
        {
            m_instance = inst;
            LoadParams();
        }
        #endregion

        #region Getting parameters
        public void LoadParams()
        {
            if (m_instance != null)
            {
                m_instance.GetCommittedParam(CyParamNames.ADDRESS1).TryGetValueAs<byte>(out m_address1);
                m_instance.GetCommittedParam(CyParamNames.ADDRESS2).TryGetValueAs<byte>(out m_address2);
                m_internalClock = GetBool(CyParamNames.INTERNAL_CLOCK);
                m_hwTXEnSignal = GetBool(CyParamNames.HW_TX_EN_SIGNAL);
                m_interruptOnTXComplete = GetBool(CyParamNames.INTERRUPT_ON_TX_COMPLETE);
                m_interruptOnTXFifoEmpty = GetBool(CyParamNames.INTERRUPT_ON_TX_FIFO_EMPTY);
                m_interruptOnTXFifoFull = GetBool(CyParamNames.INTERRUPT_ON_TX_FIFO_FULL);
                m_interruptOnTXFifoNotFull = GetBool(CyParamNames.INTERRUPT_ON_TX_FIFO_NOT_FULL);
                m_intOnAddressDetect = GetBool(CyParamNames.INT_ON_ADDRESS_DETECT);
                m_intOnAddressMatch = GetBool(CyParamNames.INT_ON_ADDRESS_MATCH);
                m_intOnBreak = GetBool(CyParamNames.INT_ON_BREAK);
                m_intOnByteRcvd = GetBool(CyParamNames.INT_ON_BYTE_RCVD);
                m_intOnOverrunError = GetBool(CyParamNames.INT_ON_OVERRUN_ERROR);
                m_intOnParityError = GetBool(CyParamNames.INT_ON_PARITY_ERROR);
                m_intOnStopError = GetBool(CyParamNames.INT_ON_STOP_ERROR);
                m_halfDuplexEnable = GetBool(CyParamNames.HALF_DUPLEX_EN);
                m_txEnable = GetBool(CyParamNames.TX_ENABLE);
                m_rxEnable = GetBool(CyParamNames.RX_ENABLE);
                m_breakDetect = GetBool(CyParamNames.BREAK_DETECT);
                int.TryParse(m_instance.GetCommittedParam(CyParamNames.BREAK_BITS_TX).Value, out m_breakBitsTX);
                m_use23Polling = GetBool(CyParamNames.USE23_POLLING);
                m_crcOutputsEn = GetBool(CyParamNames.CRC_OUTPUTS_EN);
                m_paritySoftware = GetBool(CyParamNames.PARITY_TYPE_SW);
                int.TryParse(m_instance.GetCommittedParam(CyParamNames.BAUD_RATE).Value, out m_baudRate);
                m_flowControl = m_instance.GetCommittedParam(CyParamNames.FLOW_CONTROL).Expr;
                m_numDataBits = m_instance.GetCommittedParam(CyParamNames.NUM_DATA_BITS).Expr;
                m_numStopBits = m_instance.GetCommittedParam(CyParamNames.NUM_STOP_BITS).Expr;

                try
                {
                    m_parityType = ((CY_E_PARITY_TYPE)Convert.ToByte(m_instance.GetCommittedParam(
                            CyParamNames.PARITY_TYPE).Value)).ToString();
                }
                catch { }

                m_rxAddressMode = m_instance.GetCommittedParam(CyParamNames.RX_ADDRESS_MODE).Expr;
                m_overSamplingRate = m_instance.GetCommittedParam(CyParamNames.OVER_SAMPLING_RATE).Value;
                m_instance.GetCommittedParam(CyParamNames.RX_BUFFER_SIZE).TryGetValueAs<UInt16>(out m_rxBufferSize);
                m_instance.GetCommittedParam(CyParamNames.TX_BUFFER_SIZE).TryGetValueAs<UInt16>(out m_txBufferSize);
            }
        }

        private bool GetBool(string pName)
        {
            bool res;
            m_instance.GetCommittedParam(pName).TryGetValueAs<bool>(out res);
            return res;
        }
        #endregion

        #region Setting parameters
        private void SaveParam(String paramName, String paramValue)
        {
            if (m_instance != null)
            {
                m_instance.SetParamExpr(paramName, paramValue);
                m_instance.CommitParamExprs();
            }
        }
        #endregion
    }
}
