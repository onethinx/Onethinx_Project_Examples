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
using System.Diagnostics;
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;
using System.Threading;

namespace c_UART_v2_50
{
    public partial class CyUartControl : UserControl, ICyParamEditingControl
    {
        private ICyInstEdit_v1 m_instance;
        private CyUARTParameters m_parameters;

        public bool InExpView
        {
            get { return !this.Visible; }
        }

        #region Constructor(s)
        public CyUartControl(ICyInstEdit_v1 instance, CyUARTParameters parameters)
        {
            m_instance = instance;
            m_parameters = parameters;

            InitializeComponent();
            this.Dock = DockStyle.Fill;
            this.AutoScroll = true;
            parameters.m_basicTab = this;

            FillComboBox(m_cbDataBits, CyParamNames.NUM_DATA_BITS);
            FillComboBox(m_cbParity, CyParamNames.PARITY_TYPE);
            FillComboBox(m_cbStopBits, CyParamNames.NUM_STOP_BITS);
            FillComboBox(m_cbFlowControl, CyParamNames.FLOW_CONTROL);

            UpdateRXTXButtons();
            UpdateBaudRate();
            UpdateNumDataBits();
            UpdateParityType();
            UpdateNumStopBits();
            UpdateFlowControl();
            UpdateParitySoftware();
            UpdateExternalClockLabel();
            UpdateExternalClockFrequency(m_parameters.m_exClockFreq);
        }
        #endregion

        #region ICyParamEditingControl Members
        public Control DisplayControl
        {
            get { return this; }
        }

        public IEnumerable<CyCustErr> GetErrors()
        {
            if (m_parameters.m_instance != null)
            {
                foreach (string paramName in m_parameters.m_instance.GetParamNames())
                {
                    CyCompDevParam param = m_parameters.m_instance.GetCommittedParam(paramName);
                    if (param.TabName.Equals(CyCustomizer.BASIC_TAB_NAME))
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

        #region UpdateControls
        private void FillComboBox(ComboBox box, String enumName)
        {
            IEnumerable<string> enumValues = m_instance.GetPossibleEnumValues(enumName);
            IEnumerator<string> e = enumValues.GetEnumerator();

            while (e.MoveNext())
            {
                box.Items.Add(e.Current);
            }
        }

        public void UpdateExternalClockLabel()
        {
            m_lblExternalClockMessage.Visible = !m_parameters.InternalClock;
            m_cbBitsPerSecond.Visible = m_parameters.InternalClock;
        }

        public void UpdateExternalClockFrequency(float externalClockFrequency)
        {
            int oversamplingRate;
            int.TryParse(m_parameters.OverSamplingRate, out oversamplingRate);

            if (!m_parameters.InternalClock && m_parameters.m_exClockExist)
            {
                decimal frq = 0;
                string units = string.Empty;

                if (externalClockFrequency > 999999)
                {
                    frq = Decimal.Divide((Decimal)(externalClockFrequency), (Decimal)(1000000));
                    units = "MBaud";
                }
                else
                {
                    frq = Decimal.Divide((Decimal)(externalClockFrequency), (Decimal)(1000));
                    units = "KBaud";
                }

                if (oversamplingRate != 0)
                {
                    frq /= oversamplingRate;
                }
                m_lblExternalClockMessage.Text = String.Format(Resources.ExternalClockParamMsg,
                    oversamplingRate, frq, units);
            }
            else
            {
                m_lblExternalClockMessage.Text = String.Format(Resources.ExternalClockMsg, oversamplingRate);
            }
        }

        public void UpdateRXTXButtons()
        {
            if (m_parameters.HalfDuplexEnable)
            {
                m_rbHALF.Checked = true;
            }
            else if (m_parameters.RXEnable && m_parameters.TXEnable)
            {
                m_rbRXTX.Checked = true;
            }
            else if (m_parameters.RXEnable)
            {
                m_rbRXOnly.Checked = true;
            }
            else if (m_parameters.TXEnable)
            {
                m_rbTXOnly.Checked = true;
            }
        }

        private void SelectCBItem(ComboBox box, string enumName, string currValue)
        {
            box.SelectedItem = m_instance.ResolveEnumIdToDisplay(enumName, currValue);
        }

        public void UpdateBaudRate()
        {
            bool finded = false;
            for (int i = 0; i < m_cbBitsPerSecond.Items.Count; i++)
            {
                if (m_cbBitsPerSecond.Items[i].ToString() == m_parameters.BaudRate.ToString())
                {
                    finded = true;
                    m_cbBitsPerSecond.SelectedIndex = i;
                }
            }
            if (!finded)
            {
                string nItm = m_parameters.BaudRate.ToString();
                m_cbBitsPerSecond.Items.Add(nItm);
                m_cbBitsPerSecond.SelectedItem = nItm;
            }
        }

        public void UpdateFlowControl()
        {
            SelectCBItem(m_cbFlowControl, CyParamNames.FLOW_CONTROL, m_parameters.FlowControl);
        }

        public void UpdateNumStopBits()
        {
            SelectCBItem(m_cbStopBits, CyParamNames.NUM_STOP_BITS, m_parameters.NumStopBits);
        }

        public void UpdateParityType()
        {
            SelectCBItem(m_cbParity, CyParamNames.PARITY_TYPE, m_parameters.ParityType);
        }

        public void UpdateNumDataBits()
        {
            SelectCBItem(m_cbDataBits, CyParamNames.NUM_DATA_BITS, m_parameters.NumDataBits);
        }

        public void UpdateParitySoftware()
        {
            m_chbSoftware.Checked = m_parameters.ParitySoftware;
        }
        #endregion

        #region Event Handlers
        private void m_cbBitsPerSecond_SelectedIndexChanged(object sender, EventArgs e)
        {
            int iVal;
            int.TryParse(m_cbBitsPerSecond.Text, out iVal);
            m_parameters.BaudRate = iVal;

            string errStr = (iVal <= 4000000) ? string.Empty : Resources.BitsPerSecErrMsg;
            errorProvider1.SetError(m_cbBitsPerSecond, errStr);
        }

        private void m_cbDataBits_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_cbDataBits.SelectedItem != null && m_cbParity.SelectedItem != null)
            {
                if (m_cbDataBits.SelectedItem.Equals("9") && !m_cbParity.SelectedItem.Equals("Mark/Space"))
                {
                    errorProvider1.SetError(m_cbParity, Resources.MarkSpaceParityMsg);
                    errorProvider1.SetError(m_cbDataBits, string.Empty);
                }
                else if (m_cbParity.SelectedItem.Equals("Mark/Space") && !m_cbDataBits.SelectedItem.Equals("9"))
                {
                    errorProvider1.SetError(m_cbDataBits, Resources.DataBitsMsg);
                }
                else
                {
                    errorProvider1.SetError((ComboBox)sender, string.Empty);
                    errorProvider1.SetError(m_cbParity, string.Empty);
                }
            }
            m_parameters.NumDataBits = m_instance.ResolveEnumDisplayToId(
                        CyParamNames.NUM_DATA_BITS, m_cbDataBits.Text);
        }

        private void m_cbParity_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_cbDataBits.SelectedItem != null && m_cbParity.SelectedItem != null)
            {
                if (m_cbParity.SelectedItem.Equals("Mark/Space") && !m_cbDataBits.SelectedItem.Equals("9"))
                {
                    errorProvider1.SetError(m_cbDataBits, Resources.DataBitsMsg);
                    errorProvider1.SetError(m_cbParity, string.Empty);
                }
                else if (m_cbDataBits.SelectedItem.Equals("9") && !m_cbParity.SelectedItem.Equals("Mark/Space"))
                {
                    errorProvider1.SetError(m_cbParity, Resources.MarkSpaceParityMsg);
                }
                else
                {
                    errorProvider1.SetError((ComboBox)sender, string.Empty);
                    errorProvider1.SetError(m_cbDataBits, string.Empty);
                }
            }
            m_parameters.ParityType = m_instance.ResolveEnumDisplayToId(
                        CyParamNames.PARITY_TYPE, m_cbParity.Text);
        }

        private void m_cbStopBits_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_parameters.NumStopBits = m_instance.ResolveEnumDisplayToId(CyParamNames.NUM_STOP_BITS, m_cbStopBits.Text);
        }

        private void m_cbFlowControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_parameters.FlowControl = m_instance.ResolveEnumDisplayToId(CyParamNames.FLOW_CONTROL,
                m_cbFlowControl.Text);
        }

        private void m_chbSoftware_CheckedChanged(object sender, EventArgs e)
        {
            m_parameters.ParitySoftware = m_chbSoftware.Checked;
        }

        private void ModeRB_CheckedChanged(object sender, EventArgs e)
        {
            m_parameters.FullEnable = m_rbRXTX.Checked;
            m_parameters.HalfDuplexEnable = m_rbHALF.Checked;
            m_parameters.RXEnable = m_rbRXOnly.Checked || m_rbRXTX.Checked;
            m_parameters.TXEnable = m_rbTXOnly.Checked || m_rbRXTX.Checked;
            if (m_parameters.m_instance != null) m_parameters.m_instance.CommitParamExprs();
            UpdateRXTXButtons();

            if (m_parameters.HalfDuplexEnable)
            {
                m_parameters.m_advancedTab.m_numTXBufferSize.Maximum = 4;
            }
            else
            {
                m_parameters.m_advancedTab.m_numTXBufferSize.Maximum = UInt16.MaxValue;
            }

            m_parameters.m_advancedTab.CheckRXTXEnable();
            m_parameters.m_advancedTab.UpdateRxTxControlsState();
        }

        private void CyUartControl_Load(object sender, EventArgs e)
        {
            // Update RX and TX Buffer Size with new value if invalid value came from previous version
            m_parameters.m_advancedTab.UpdateBufferSizeValues();
        }
        #endregion
    }
}
