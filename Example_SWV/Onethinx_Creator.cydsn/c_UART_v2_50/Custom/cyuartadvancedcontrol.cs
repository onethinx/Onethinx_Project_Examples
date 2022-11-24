/*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;

namespace c_UART_v2_50
{
    public partial class CyUartAdvancedControl : UserControl, ICyParamEditingControl
    {
        private ICyInstEdit_v1 m_instance;
        private ICyTerminalQuery_v1 m_termQuery;
        private CyUARTParameters m_params;
        private bool m_loading;

        public event ClockFrequencyChangeDelegate ExClockFrequencyChanged;

        #region Constructor(s)
        public CyUartAdvancedControl(ICyInstEdit_v1 inst, ICyTerminalQuery_v1 termquery, CyUARTParameters parameters)
        {
            m_instance = inst;
            m_termQuery = termquery;
            m_params = parameters;

            InitializeComponent();
            this.Dock = DockStyle.Fill;
            parameters.m_advancedTab = this;
            //Set the RX Address Modes Combo Box
            List<string> RXAddressModes = new List<string>(m_instance.GetPossibleEnumValues(
                CyParamNames.RX_ADDRESS_MODE));
            for (int i = 0; i < RXAddressModes.Count; i++)
            {
                m_cbRXAddressMode.Items.Add(RXAddressModes[i]);
            }
            SetFrequencyLabel();

            m_numRXBufferSize.Minimum = 4;
            m_numRXBufferSize.Maximum = UInt16.MaxValue;
            m_numTXBufferSize.Minimum = 4;
            m_numTXBufferSize.Maximum = UInt16.MaxValue;

            UpdateFormFromParams(inst);
            CheckRXTXEnable();

            m_numRXBufferSize.TextChanged += new EventHandler(m_numeric_TextChanged);
            m_numTXBufferSize.TextChanged += new EventHandler(m_numeric_TextChanged);
        }
        #endregion

        #region ICyParamEditingControl Members
        public Control DisplayControl
        {
            get { return this; }
        }

        public IEnumerable<CyCustErr> GetErrors()
        {
            if (m_params.m_instance != null)
            {
                foreach (string paramName in m_params.m_instance.GetParamNames())
                {
                    CyCompDevParam param = m_params.m_instance.GetCommittedParam(paramName);
                    if (param.TabName.Equals(CyCustomizer.ADVANCED_TAB_NAME))
                    {
                        if (param.ErrorCount > 0)
                        {
                            foreach (string errMsg in param.Errors)
                            {
                                yield return new CyCustErr(errMsg);
                            }
                        }
                    }
                }
            }
        }
        #endregion

        #region Event Handlers
        private void m_numeric_TextChanged(object sender, EventArgs e)
        {
            UpdateRxTxControlsState();

            if (sender == m_numRXBufferSize)
                SetBufferSize(sender, CyParamNames.RX_BUFFER_SIZE, CyParamNames.EN_INT_RX_INTERRUPT);
            if (sender == m_numTXBufferSize)
                SetBufferSize(sender, CyParamNames.TX_BUFFER_SIZE, CyParamNames.EN_INT_TX_INTERRUPT);
        }

        public void UpdateRxTxControlsState()
        {
            double rxValue = 0;
            double.TryParse(m_numRXBufferSize.Text, out rxValue);
            double txValue = 0;
            double.TryParse(m_numTXBufferSize.Text, out txValue);

            if (m_numRXBufferSize.Enabled)
            {
                if (rxValue >= 5)
                {
                    m_lblRXStatus.Text = "enabled";
                    m_chbxInterruptOnByteReceived.Checked = true;
                    m_chbxInterruptOnByteReceived.Enabled = false;
                }
                else
                {
                    m_lblRXStatus.Text = "disabled";
                    m_chbxInterruptOnByteReceived.Enabled = true;
                }
            }

            if (m_numTXBufferSize.Enabled && m_params.HalfDuplexEnable == false)
            {
                if (txValue >= 5)
                {
                    m_lblTXStatus.Text = "enabled";
                    m_chbxInterruptOnTXFifoEmpty.Checked = true;
                    m_chbxInterruptOnTXFifoEmpty.Enabled = false;
                }
                else
                {
                    m_lblTXStatus.Text = "disabled";
                    m_chbxInterruptOnTXFifoEmpty.Enabled = true;
                }
            }

            // Show/Hide error provider for RX Buffer Size
            if (rxValue < (double)m_numRXBufferSize.Minimum || rxValue > (double)m_numRXBufferSize.Maximum)
                ep_Errors.SetError(m_numRXBufferSize, string.Format(Resources.InvalidRXBufferSize,
                    m_numRXBufferSize.Minimum, m_numRXBufferSize.Maximum));
            else
                ep_Errors.SetError(m_numRXBufferSize, string.Empty);

            // Show/Hide error provider for TX Buffer Size
            if (txValue < (double)m_numTXBufferSize.Minimum || txValue > (double)m_numTXBufferSize.Maximum)
            {
                if (m_params.HalfDuplexEnable)
                    ep_Errors.SetError(m_numTXBufferSize, string.Format(Resources.InvalidTXBufferSizeHalfDuplex,
                        m_numTXBufferSize.Minimum));
                else
                    ep_Errors.SetError(m_numTXBufferSize, string.Format(Resources.InvalidTXBufferSize,
                            m_numTXBufferSize.Minimum, m_numTXBufferSize.Maximum));
            }
            else
                ep_Errors.SetError(m_numTXBufferSize, string.Empty);
        }

        /// <summary>
        /// Enables/disables Advanced tab controls, according to selected mode
        /// </summary>
        public void CheckRXTXEnable()
        {
            bool rx_or_hd = (m_params.RXEnable || m_params.HalfDuplexEnable);
            bool tx_or_hd = (m_params.TXEnable || m_params.HalfDuplexEnable);

            bool rxInterrupt = rx_or_hd;
            bool txInterrupt = m_params.TXEnable;
            bool rxAddressConf = rx_or_hd;
            bool rxBufferSize = rx_or_hd;
            bool txBufferSize = tx_or_hd;
            bool rs485 = tx_or_hd;
            bool voting = rx_or_hd;

            // Assigning values to controls
            // RX Enable
            m_chbxInterruptOnAddressDetect.Enabled = rxInterrupt;
            m_chbxInterruptOnAddressMatch.Enabled = rxInterrupt;
            m_chbxInterruptOnBreak.Enabled = rxInterrupt;
            m_chbxInterruptOnByteReceived.Enabled = rxInterrupt;
            m_chbxInterruptOnOverrunError.Enabled = rxInterrupt;
            m_chbxInterruptOnParityError.Enabled = rxInterrupt;
            m_chbxInterruptOnStopError.Enabled = rxInterrupt;

            if (rxInterrupt == false)
            {
                m_chbxInterruptOnAddressDetect.Checked = rxInterrupt;
                m_chbxInterruptOnAddressMatch.Checked = rxInterrupt;
                m_chbxInterruptOnBreak.Checked = rxInterrupt;
                m_chbxInterruptOnByteReceived.Checked = rxInterrupt;
                m_chbxInterruptOnOverrunError.Checked = rxInterrupt;
                m_chbxInterruptOnParityError.Checked = rxInterrupt;
                m_chbxInterruptOnStopError.Checked = rxInterrupt;
            }

            // TX Enable
            m_chbxInterruptOnTXComplete.Enabled = txInterrupt;
            m_chbxInterruptOnTXFifoEmpty.Enabled = txInterrupt;
            m_chbxInterruptOnTXFifoFull.Enabled = txInterrupt;
            m_chbxInterruptOnTXFifoNotFull.Enabled = txInterrupt;

            if (txInterrupt == false)
            {
                m_chbxInterruptOnTXComplete.Checked = txInterrupt;
                m_chbxInterruptOnTXFifoEmpty.Checked = txInterrupt;
                m_chbxInterruptOnTXFifoFull.Checked = txInterrupt;
                m_chbxInterruptOnTXFifoNotFull.Checked = txInterrupt;
            }

            // RX Address Configuration
            m_cbRXAddressMode.Enabled = rxAddressConf;
            m_numRXAddress1.Enabled = rxAddressConf;
            m_numRXAddress2.Enabled = rxAddressConf;

            if (m_params.HalfDuplexEnable)
            {
                m_lblRXStatusLabel.Enabled = false;
                m_lblRXStatus.Enabled = false;
                m_lblTXStatusLabel.Enabled = false;
                m_lblTXStatus.Enabled = false;
            }
            else
            {
                // Buffers size
                m_numRXBufferSize.Enabled = rxBufferSize;
                m_lblRXStatusLabel.Enabled = rxBufferSize;
                m_lblRXStatus.Enabled = rxBufferSize;
                m_numTXBufferSize.Enabled = txBufferSize;
                m_lblTXStatusLabel.Enabled = txBufferSize;
                m_lblTXStatus.Enabled = txBufferSize;
            }

            // RS-485 Configuration Options
            m_chbxHWTxEnable.Enabled = rs485;

            // Enable 2 out of 3 voting per bit
            m_chbUse23Polling.Enabled = voting;

            SetRXAddress12Enabled();
        }

        private void SetRXAddress12Enabled()
        {
            bool notTXOnly = m_params.RXEnable || m_params.HalfDuplexEnable || m_params.FullEnable;
            m_numRXAddress1.Enabled = notTXOnly && (m_cbRXAddressMode.Text != "None");
            m_numRXAddress2.Enabled = notTXOnly && (m_cbRXAddressMode.Text != "None");
        }

        private void UpdateInterrupts()
        {
            //RX
            UpdateOnAddressDetect();
            UpdateOnAddressMatch();
            UpdateOnBreak();
            UpdateOnByteRcvd();
            UpdateOnOverrrunError();
            UpdateOnParityError();
            UpdateOnStopError();

            //TX
            UpdateOnTXComplete();
            UpdateOnTXFifoEmpty();
            UpdateOnTXFifoFull();
            UpdateOnTXFifoNotFull();
        }

        public void UpdateOnTXFifoNotFull()
        {
            m_chbxInterruptOnTXFifoNotFull.Checked = m_params.InterruptOnTXFifoNotFull;
        }

        public void UpdateOnTXFifoFull()
        {
            m_chbxInterruptOnTXFifoFull.Checked = m_params.InterruptOnTXFifoFull;
        }

        public void UpdateOnTXFifoEmpty()
        {
            m_chbxInterruptOnTXFifoEmpty.Checked = m_params.InterruptOnTXFifoEmpty;
        }

        public void UpdateOnTXComplete()
        {
            m_chbxInterruptOnTXComplete.Checked = m_params.InterruptOnTXComplete;
        }

        public void UpdateOnStopError()
        {
            m_chbxInterruptOnStopError.Checked = m_params.IntOnStopError;
        }

        public void UpdateOnParityError()
        {
            m_chbxInterruptOnParityError.Checked = m_params.IntOnParityError;
        }

        public void UpdateOnOverrrunError()
        {
            m_chbxInterruptOnOverrunError.Checked = m_params.IntOnOverrunError;
        }

        public void UpdateOnByteRcvd()
        {
            m_chbxInterruptOnByteReceived.Checked = m_params.IntOnByteRcvd;
        }

        public void UpdateOnBreak()
        {
            m_chbxInterruptOnBreak.Checked = m_params.IntOnBreak;
        }

        public void UpdateOnAddressMatch()
        {
            m_chbxInterruptOnAddressMatch.Checked = m_params.IntOnAddressMatch;
        }

        public void UpdateOnAddressDetect()
        {
            m_chbxInterruptOnAddressDetect.Checked = m_params.IntOnAddressDetect;
        }

        public void UpdateFormFromParams(ICyInstEdit_v1 inst)
        {
            m_loading = true;
            UpdateInterrupts();
            UpdateBreakBitsTX();
            UpdateUse23Polling();
            UpdateCRCoutputsEn();
            UpdateRXBufferSize();
            UpdateTXBufferSize();
            UpdateInternalClock();
            UpdateHwTXEnSignal();
            UpdateAddress1();
            UpdateAddress2();
            UpdateRXAddressMode();
            UpdateOverSamplingRate();
            SetFrequencyLabel();
            m_loading = false;
        }

        public void UpdateOverSamplingRate()
        {
            m_rb8x.Checked = (m_params.OverSamplingRate == "8");
            m_rb16x.Checked = (m_params.OverSamplingRate == "16");
        }

        public void UpdateHwTXEnSignal()
        {
            m_chbxHWTxEnable.Checked = m_params.HwTXEnSignal;
        }

        public void UpdateCRCoutputsEn()
        {
            m_chbCRCoutputsEn.Checked = m_params.CRCoutputsEn;
        }

        public void UpdateUse23Polling()
        {
            m_chbUse23Polling.Checked = m_params.Use23Polling;
        }

        public void UpdateBreakBitsTX()
        {
            if (m_params.BreakDetect)
            {
                bool finded = false;
                for (int i = 0; i < m_cbBreakSignal.Items.Count; i++)
                {
                    if (m_cbBreakSignal.Items[i].ToString() == m_params.BreakBitsTX.ToString())
                    {
                        finded = true;
                        m_cbBreakSignal.SelectedIndex = i;
                    }
                }
                if (!finded)
                {
                    string nItm = m_params.BreakBitsTX.ToString();
                    if (nItm == "0")
                    {
                        m_cbBreakSignal.SelectedItem = "None";
                    }
                    else
                    {
                        m_cbBreakSignal.Items.Add(nItm);
                        m_cbBreakSignal.SelectedItem = nItm;
                    }
                }
            }
            else
            {
                m_cbBreakSignal.SelectedItem = "None";
            }
        }

        public void UpdateTXBufferSize()
        {
            try
            {
                m_numTXBufferSize.Value = m_params.TXBufferSize;
            }
            catch (Exception)
            {
                m_numTXBufferSize.Value = m_numTXBufferSize.Minimum;
            }
        }

        public void UpdateRXBufferSize()
        {
            try
            {
                m_numRXBufferSize.Value = m_params.RXBufferSize;
            }
            catch (Exception)
            {

                m_numRXBufferSize.Value = m_numTXBufferSize.Minimum;
            }
        }

        public void UpdateRXAddressMode()
        {
            //Set the RXAddressmode Combo Box
            List<string> RXAddressModes = 
                            new List<string> (m_instance.GetPossibleEnumValues(CyParamNames.RX_ADDRESS_MODE));
            for (int i = 0; i < RXAddressModes.Count; i++)
            {
                string paramcompare = m_instance.ResolveEnumIdToDisplay(CyParamNames.RX_ADDRESS_MODE,
                    m_params.RXAddressMode);
                if (paramcompare == RXAddressModes[i])
                {
                    m_cbRXAddressMode.SelectedItem = paramcompare;
                }
            }
        }

        public void UpdateAddress2()
        {
            m_numRXAddress2.Value = m_params.Address2;
        }

        public void UpdateAddress1()
        {
            m_numRXAddress1.Value = m_params.Address1;
        }

        public void UpdateInternalClock()
        {
            //Set the Clock Selection Radio Buttons
            m_rbInternalClock.Checked = m_params.InternalClock;
            m_rbExternalClock.Checked = !m_params.InternalClock;
        }

        private void SetAParameter(string parameter, string value)
        {
            m_instance.SetParamExpr(parameter, value);
            m_instance.CommitParamExprs();
        }

        private void m_chbxHWTxEnable_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.HW_TX_EN_SIGNAL, m_chbxHWTxEnable.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnByteReceived_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INT_ON_BYTE_RCVD, m_chbxInterruptOnByteReceived.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnParityError_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INT_ON_PARITY_ERROR, m_chbxInterruptOnParityError.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnStopError_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INT_ON_STOP_ERROR, m_chbxInterruptOnStopError.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnBreak_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INT_ON_BREAK, m_chbxInterruptOnBreak.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnOverrunError_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INT_ON_OVERRUN_ERROR,
                m_chbxInterruptOnOverrunError.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnAddressMatch_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INT_ON_ADDRESS_MATCH,
                m_chbxInterruptOnAddressMatch.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnAddressDetect_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INT_ON_ADDRESS_DETECT,
                m_chbxInterruptOnAddressDetect.Checked.ToString().ToLower());
        }

        private void m_numRXAddress1_ValueChanged(object sender, EventArgs e)
        {
            m_params.Address1 = (byte)m_numRXAddress1.Value;
        }

        private void m_numRXAddress2_ValueChanged(object sender, EventArgs e)
        {
            m_params.Address2 = (byte)m_numRXAddress2.Value;
        }

        private void m_cbRXAddressMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_params.RXAddressMode = m_instance.ResolveEnumDisplayToId(CyParamNames.RX_ADDRESS_MODE,
                m_cbRXAddressMode.Text);
            SetRXAddress12Enabled();
        }

        private void m_chbxInterruptOnTXComplete_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INTERRUPT_ON_TX_COMPLETE,
                m_chbxInterruptOnTXComplete.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnTXFifoEmpty_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INTERRUPT_ON_TX_FIFO_EMPTY,
                m_chbxInterruptOnTXFifoEmpty.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnTXFifoFull_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INTERRUPT_ON_TX_FIFO_FULL,
                m_chbxInterruptOnTXFifoFull.Checked.ToString().ToLower());
        }

        private void m_chbxInterruptOnTXFifoNotFull_CheckedChanged(object sender, EventArgs e)
        {
            SetAParameter(CyParamNames.INTERRUPT_ON_TX_FIFO_NOT_FULL,
                m_chbxInterruptOnTXFifoNotFull.Checked.ToString().ToLower());
        }

        private void m_chbUse23Polling_CheckedChanged(object sender, EventArgs e)
        {
            m_params.Use23Polling = m_chbUse23Polling.Checked;
        }

        private void m_chbCRCoutputsEn_CheckedChanged(object sender, EventArgs e)
        {
            m_params.CRCoutputsEn = m_chbCRCoutputsEn.Checked;
        }

        private void SetFrequencyLabel()
        {
            if (m_rbExternalClock.Checked)
            {
                m_lblCalcFrequency.Visible = true;
                List<CyClockData> clkdata = new List<CyClockData>();
                clkdata = m_termQuery.GetClockData("clock", 0);
                if (clkdata[0].IsFrequencyKnown)
                {
                    double infreq = clkdata[0].Frequency;
                    switch (clkdata[0].Unit)
                    {
                        case CyClockUnit.kHz: infreq = infreq * 1000; break;
                        case CyClockUnit.MHz: infreq = infreq * 1000000; break;
                    }
                    m_params.m_exClockFreq = (float)infreq;
                    if (ExClockFrequencyChanged != null)
                    {
                        ExClockFrequencyChanged((int)infreq);
                    }
                    m_lblCalcFrequency.Text = string.Format(Resources.SourceFreqMsg,
                        Math.Round(clkdata[0].Frequency, 3), clkdata[0].Unit);
                    m_params.m_exClockExist = true;
                }
                else
                {
                    m_params.m_exClockExist = false;
                    m_lblCalcFrequency.Text = Resources.UnknownSourceFreqMsg;
                }
            }
            else
            {
                m_lblCalcFrequency.Visible = false;
            }
        }

        public void UpdateClock(ICyInstQuery_v1 edit, ICyTerminalQuery_v1 termQuery)
        {
            SetFrequencyLabel();
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Enter)
            {
                SendKeys.Send("{TAB}");
                return true;
            }
            else
                return base.ProcessCmdKey(ref msg, keyData);
        }

        private void SetBufferSize(object sender, string buffSizeName, string interruptName)
        {
            NumericUpDown currentNumericUpDown = (NumericUpDown)sender;
            string str = ep_Errors.GetError(currentNumericUpDown);
            UInt16 rxValue = 0;
            UInt16.TryParse(currentNumericUpDown.Text, out rxValue);
            SetAParameter(buffSizeName, rxValue.ToString());
            bool paramValue = rxValue < 5 ? false : true;
            SetAParameter(interruptName, paramValue.ToString().ToLower());
        }

        private void m_rbInternalClock_CheckedChanged(object sender, EventArgs e)
        {
            m_params.InternalClock = m_rbInternalClock.Checked;
            m_rbExternalClock.Checked = !m_rbInternalClock.Checked;

            SetFrequencyLabel();
            if (m_params.m_basicTab != null) m_params.m_basicTab.UpdateExternalClockLabel();
        }

        private void m_cbBreakSignal_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_cbBreakSignal.SelectedItem.ToString() == "None")
            {
                m_params.BreakDetect = false;
            }
            else
            {
                int breakBits;
                int.TryParse(m_cbBreakSignal.SelectedItem.ToString(), out breakBits);
                m_params.BreakBitsTX = breakBits;
                if (!m_loading) m_params.BreakBitsRX = breakBits;
                m_params.BreakDetect = true;
            }

            int iVal;
            int.TryParse(m_cbBreakSignal.SelectedItem.ToString(), out iVal);
            string errStr = (iVal > 15) ? Resources.BreakSignalErrMsg : string.Empty;
            ep_Errors.SetError(m_cbBreakSignal, errStr);
        }

        private void m_rbOverSamplingRate_CheckedChanged(object sender, EventArgs e)
        {
            if (m_rb8x.Checked && !m_rb16x.Checked) m_params.OverSamplingRate = "8";
            if (!m_rb8x.Checked && m_rb16x.Checked) m_params.OverSamplingRate = "16";

            string errStr = string.Empty;
            if ((m_rb8x.Checked && m_rb16x.Checked) || (!m_rb8x.Checked && !m_rb16x.Checked))
            {
                errStr = Resources.OverSamplingRateErrMsg;
            }
            ep_Errors.SetError(m_rb8x, errStr);
            ep_Errors.SetError(m_rb16x, errStr);
        }

        private void CyUartAdvancedControl_Load(object sender, EventArgs e)
        {
            m_lblRXStatus.Text = (m_numRXBufferSize.Value >= 5) ? "enabled" : "disabled";
            m_lblTXStatus.Text = (m_numTXBufferSize.Value >= 5) ? "enabled" : "disabled";
        }

        public void UpdateBufferSizeValues()
        {
            SetBufferSize(m_numTXBufferSize, CyParamNames.TX_BUFFER_SIZE, CyParamNames.EN_INT_TX_INTERRUPT);
            SetBufferSize(m_numRXBufferSize, CyParamNames.RX_BUFFER_SIZE, CyParamNames.EN_INT_RX_INTERRUPT);
        }

        private void m_numBufferSize_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Do not allow input point and comma characters
            if (e.KeyChar == 46 || e.KeyChar == 44)
            {
                e.Handled = true;
            }
        }
        #endregion
    }
}
