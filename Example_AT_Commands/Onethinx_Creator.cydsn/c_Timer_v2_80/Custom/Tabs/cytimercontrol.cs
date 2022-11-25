/*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;

namespace c_Timer_v2_80
{
    internal partial class CyTimerControl : CyTabBase
    {
        private const string PARAM_TAB_NAME = "Basic";

        private List<CyECaptureModes> m_cbDataSourceCaptureModeUDB;
        private List<CyECaptureModes> m_cbDataSourceCaptureModeFF;
        private List<CyEEnableModes> m_cbDataSourceEnableModeUDB;
        private List<CyEEnableModes> m_cbDataSourceEnableModeFF;
        private List<CyERunModes> m_cbDataSourceRunModeUDB;
        private List<CyERunModes> m_cbDataSourceRunModeFF;

        public CyTimerControl() :
            base(null, PARAM_TAB_NAME, Resources.ConfigureTabDisplayName)
        {
            InitializeComponent();
        }

        public CyTimerControl(CyParameters parameters) :
            base(parameters, PARAM_TAB_NAME, Resources.ConfigureTabDisplayName)
        {
            InitializeComponent();
            InitControls();
        }

        #region Form Initialization
        private void InitControls()
        {
            m_cbTriggerMode.DataSource = Enum.GetValues(typeof(CyETriggerModes));
            m_cbRunMode.DataSource = Enum.GetValues(typeof(CyERunModes));

            CyUtils.SetNumericMinMax(m_numPeriod, CyConstants.PERIOD_MIN, (ulong)UInt32.MaxValue + 1);
            CyUtils.SetNumericMinMax(m_numCaptureCount, Math.Min(CyConstants.CAPTURE_COUNT_MIN, m_params.CaptureCount), 
                CyConstants.CAPTURE_COUNT_MAX);
            CyUtils.SetNumericMinMax(m_numIntOnCaptureCount, CyConstants.INT_CAPTURE_COUNT_MIN, 
                CyConstants.INT_CAPTURE_COUNT_MAX);

            HookupUpdateEvents();

            m_params.DesignUpdates += DesignUpdates;
        }

        /// <summary>
        /// Initializes a drop-down items list of the m_cbCaptureMode combobox depending on the implementation: FF or UDB.
        /// </summary>
        private void UpdateComboboxCaptureMode()
        {
            UpdateCombobox<CyECaptureModes>(m_cbCaptureMode, ref m_cbDataSourceCaptureModeUDB, ref m_cbDataSourceCaptureModeFF,
               new [] { CyECaptureModes.B_Timer__CM_FallingEdge, CyECaptureModes.B_Timer__CM_EitherEdge, 
                   CyECaptureModes.B_Timer__CM_Software }, m_params.CaptureMode);
        }

        /// <summary>
        /// Initializes a drop-down items list of the m_cbEnableMode combobox depending on the implementation: FF or UDB.
        /// </summary>
        private void UpdateComboboxEnableMode()
        {
            UpdateCombobox<CyEEnableModes>(m_cbEnableMode, ref m_cbDataSourceEnableModeUDB, ref m_cbDataSourceEnableModeFF, 
                new []{CyEEnableModes.B_Timer__EM_HardwareOnly}, m_params.EnableMode);
        }

        /// <summary>
        /// Initializes a drop-down items list of the m_cbRunMode combobox depending on the implementation: FF or UDB.
        /// </summary>
        private void UpdateComboboxRunMode()
        {
            UpdateCombobox<CyERunModes>(m_cbRunMode, ref m_cbDataSourceRunModeUDB, ref m_cbDataSourceRunModeFF,
                new []{CyERunModes.B_Timer__RM_OneShotHaltOnInterrupt}, m_params.RunMode);
        }

        /// <summary>
        /// Initializes a drop-down items list of the combobox depending on the implementaiton: FF or UDB.
        /// </summary>
        private void UpdateCombobox<T>(ComboBox cb, ref List<T> udbDataSource, ref List<T> ffDataSource, T[] diffItems, 
            T selValue)
        {
            cb.SelectedIndexChanged -= ComboBox_SelectedIndexChanged;

            // Init data sources
            if (udbDataSource == null)
            {
                udbDataSource = new List<T>((T[])Enum.GetValues(typeof(T)));
            }
            if (ffDataSource == null)
            {
                ffDataSource = new List<T>(udbDataSource);
                for (int i = 0; i < diffItems.Length; i++)
                {
                    ffDataSource.Remove(diffItems[i]);
                }
            }

            // UDB mode
            if (m_params.FixedFunction == false && cb.DataSource != udbDataSource && udbDataSource.Contains(selValue))
            {
                cb.DataSource = udbDataSource;
                cb.SelectedItem = selValue;
            }
            // FF mode
            else if (m_params.FixedFunction && cb.DataSource != ffDataSource && ffDataSource.Contains(selValue))
            {
                cb.DataSource = ffDataSource;
                cb.SelectedItem = selValue;
            }
            // If DataSource still isn't initialized (selected value is unavailable for mode), init with a full list
            else if (cb.DataSource == null) 
            {
                cb.DataSource = udbDataSource;
                cb.SelectedItem = selValue;
            }

            cb.SelectedIndexChanged += ComboBox_SelectedIndexChanged;
        }

        void HookupUpdateEvents()
        {
            m_cbCaptureMode.SelectedIndexChanged += ComboBox_SelectedIndexChanged;
            m_cbTriggerMode.SelectedIndexChanged += ComboBox_SelectedIndexChanged;
            m_cbRunMode.SelectedIndexChanged += ComboBox_SelectedIndexChanged;
            m_cbEnableMode.SelectedIndexChanged += ComboBox_SelectedIndexChanged;

            m_cbCaptureMode.Format += m_cb_Format;
            m_cbTriggerMode.Format += m_cb_Format;
            m_cbRunMode.Format += m_cb_Format;
            m_cbEnableMode.Format += m_cb_Format;

            m_chbxIntOnCapture.CheckedChanged += CheckBox_CheckedChanged;
            m_chbxIntOnTC.CheckedChanged += CheckBox_CheckedChanged;
            m_chbxIntOnFifoFull.CheckedChanged += CheckBox_CheckedChanged;
            m_chbxEnableCaptureCounter.CheckedChanged += CheckBox_CheckedChanged;

            m_numIntOnCaptureCount.TextChanged += NumericUpDown_TextChanged;
            m_numCaptureCount.TextChanged += NumericUpDown_TextChanged;
            m_numPeriod.TextChanged += NumericUpDown_TextChanged;

            m_rbResolution8.CheckedChanged += m_rbResolution_CheckedChanged;
            m_rbResolution16.CheckedChanged += m_rbResolution_CheckedChanged;
            m_rbResolution24.CheckedChanged += m_rbResolution_CheckedChanged;
            m_rbResolution32.CheckedChanged += m_rbResolution_CheckedChanged;
            m_rbFixedFunction.CheckedChanged += m_rbFFUDB_CheckedChanged;
            m_rbUDB.CheckedChanged += m_rbFFUDB_CheckedChanged;
        }

        void UnhookUpdateEvents()
        {
            m_cbCaptureMode.SelectedIndexChanged -= ComboBox_SelectedIndexChanged;
            m_cbTriggerMode.SelectedIndexChanged -= ComboBox_SelectedIndexChanged;
            m_cbRunMode.SelectedIndexChanged -= ComboBox_SelectedIndexChanged;
            m_cbEnableMode.SelectedIndexChanged -= ComboBox_SelectedIndexChanged;

            m_cbCaptureMode.Format -= m_cb_Format;
            m_cbTriggerMode.Format -= m_cb_Format;
            m_cbRunMode.Format -= m_cb_Format;
            m_cbEnableMode.Format -= m_cb_Format;

            m_chbxIntOnCapture.CheckedChanged -= CheckBox_CheckedChanged;
            m_chbxIntOnTC.CheckedChanged -= CheckBox_CheckedChanged;
            m_chbxIntOnFifoFull.CheckedChanged -= CheckBox_CheckedChanged;
            m_chbxEnableCaptureCounter.CheckedChanged -= CheckBox_CheckedChanged;

            m_numIntOnCaptureCount.TextChanged -= NumericUpDown_TextChanged;
            m_numCaptureCount.TextChanged -= NumericUpDown_TextChanged;
            m_numPeriod.TextChanged -= NumericUpDown_TextChanged;

            m_rbResolution8.CheckedChanged -= m_rbResolution_CheckedChanged;
            m_rbResolution16.CheckedChanged -= m_rbResolution_CheckedChanged;
            m_rbResolution24.CheckedChanged -= m_rbResolution_CheckedChanged;
            m_rbResolution32.CheckedChanged -= m_rbResolution_CheckedChanged;
            m_rbFixedFunction.CheckedChanged -= m_rbFFUDB_CheckedChanged;
            m_rbUDB.CheckedChanged -= m_rbFFUDB_CheckedChanged;
        }
        #endregion

        #region Form Update Methods
        public override void UpdateUI()
        {
            UnhookUpdateEvents();

            // Resolution
            switch (m_params.Resolution)
            {
                case CyEResolutions.B_Timer__R8:
                    m_rbResolution8.Checked = true;
                    break;
                case CyEResolutions.B_Timer__R16:
                    m_rbResolution16.Checked = true;
                    break;
                case CyEResolutions.B_Timer__R24:
                    m_rbResolution24.Checked = true;
                    break;
                case CyEResolutions.B_Timer__R32:
                    m_rbResolution32.Checked = true;
                    break;
                default:
                    Debug.Fail("unhandled value: " + m_params.Resolution);
                    break;
            }

            // Implementation (Fixed Function or UDB)
            if (m_params.FixedFunction)
            {
                m_rbFixedFunction.Checked = true;
            }
            else
            {
                m_rbUDB.Checked = true;
            }
            
            // Period
            CyUtils.SetNudValue(m_numPeriod, m_params.Period);
            UpdateCalculatedPeriodLabel();

            // Trigger Mode
            m_cbTriggerMode.SelectedItem = m_params.TriggerMode;
            
            // Capture Mode
            UpdateComboboxCaptureMode();
            m_chbxEnableCaptureCounter.Checked = m_params.CaptureCounterEnabled;
            CyUtils.SetNudValue(m_numCaptureCount, m_params.CaptureCount);

            // Enable Mode
            UpdateComboboxEnableMode();
            
            // Run Mode
            UpdateComboboxRunMode();
            
            // Interrupts
            m_chbxIntOnTC.Checked = m_params.InterruptOnTC;
            m_chbxIntOnFifoFull.Checked = m_params.InterruptOnFIFOFull;
            m_chbxIntOnCapture.Checked = m_params.InterruptOnCapture;
            CyUtils.SetNudValue(m_numIntOnCaptureCount, m_params.InterruptCaptures);

            ValidateAll();
            UpdateControlsState();

            HookupUpdateEvents();
        }

        /// <summary>
        /// Creates the text of the Period calculation value displayed.
        /// Calculation is based on period value and a query of the input clock frequency.
        /// </summary>
        /// <param name="period">Integer value of the current period setting in the text box </param>
        public string GetCalculatedPeriod(ulong period)
        {
            double infreq = CyClock.GetExternalClockHz(m_params.TermQuery);
            if (CyClock.IsFrequencyKnown(infreq) && period > 0)
            {
                try
                {
                    double periodfreq = infreq / period;
                    double periodtime = 1 / periodfreq;

                    int i = 0;
                    while (periodtime < 1)
                    {
                        periodtime = periodtime * 1000;
                        i += 3;
                    }
                    string time = "s";
                    switch (i)
                    {
                        case 0:
                            time = "s";
                            break;
                        case 3:
                            time = "ms";
                            break;
                        case 6:
                            time = "us";
                            break;
                        case 9:
                            time = "ns";
                            break;
                        case 12:
                            time = "ps";
                            break;
                        default:
                            Debug.Fail("unhandled value: " + i);
                            break;
                    }
                    return (string.Format(Resources.PeriodFormattedValue, Math.Round(periodtime, 3), time));
                }
                catch (Exception ex)
                {
                    Debug.Fail(ex.Message);
                }
            }
            return Resources.ErrorMsgClockFreqUnknown;
        }

        private void UpdateControlsState()
        {
            bool udbMode = m_params.FixedFunction == false;

            // Enable
            SetEnableState(m_rbResolution24, udbMode);
            SetEnableState(m_rbResolution32, udbMode);

            SetEnableState(m_rbFixedFunction, m_params.InstQuery.DeviceQuery.IsPSoC4 == false);

            SetEnableState(m_numCaptureCount, udbMode && m_params.CaptureCounterEnabled);
            SetEnableState(m_numIntOnCaptureCount, udbMode && m_params.InterruptOnCapture);

            SetEnableState(m_cbTriggerMode, udbMode);

            SetEnableState(m_chbxIntOnFifoFull, udbMode);

            SetEnableState(m_chbxEnableCaptureCounter, udbMode && m_params.CaptureMode != CyECaptureModes.B_Timer__CM_None);
            SetEnableState(m_numCaptureCount, m_params.CaptureCounterEnabled);
            SetEnableState(m_chbxIntOnCapture, m_params.CaptureMode != CyECaptureModes.B_Timer__CM_None);
            SetEnableState(m_numIntOnCaptureCount, udbMode && m_params.InterruptOnCapture);

            SetVisibleState(m_chbxEnableCaptureCounter, udbMode);
            SetVisibleState(m_numCaptureCount, udbMode);
            SetVisibleState(m_numIntOnCaptureCount, udbMode);

            m_chbxIntOnCapture.Text = udbMode ? Resources.LblTitleIntOnCaptureUdb : Resources.LblTitleIntOnCaptureFF;
        }

        private void SetEnableState(Control control, bool enable)
        {
            // If there are errors in the control, enable it so that a user can change the value
            control.Enabled = String.IsNullOrEmpty(m_errorProvider.GetError(control)) == false || enable;
        }

        private void SetVisibleState(Control control, bool visible)
        {
            // If there are errors in the control, make it visible so that a user can change the value
            control.Visible = String.IsNullOrEmpty(m_errorProvider.GetError(control)) == false || visible;
        }

        private void DesignUpdates(object sender, EventArgs e)
        {
            UpdateCalculatedPeriodLabel();
        }
        
        public void UpdateCalculatedPeriodLabel()
        {
            m_lblCalcPeriod.Text = GetCalculatedPeriod(m_params.Period);
        }
        #endregion

        #region Validation
        private void ValidateAll()
        {
            ValidateResolution();
            ValidateImplementation();
            ValidateNumerics();
            ValidatePeriod();
            ValidateRunMode();
            ValidateCapture();
            ValidateTriggerMode();
            ValidateEnableMode();
            ValidateInterrupts();
        }

        /// <summary>
        /// Validates Resolution value.
        /// 24-bit and 32-bit values are unavailable in the Fixed Function mode.
        /// </summary>
        private void ValidateResolution()
        {
            CyUtils.SetError(m_errorProvider, m_rbResolution24, String.Empty);
            CyUtils.SetError(m_errorProvider, m_rbResolution32, String.Empty);

            if (m_params.FixedFunction && m_params.Resolution == CyEResolutions.B_Timer__R24)
            {
                CyUtils.SetError(m_errorProvider, m_rbResolution24, Resources.ErrorMsgResolution);
            }
            else if (m_params.FixedFunction && m_params.Resolution == CyEResolutions.B_Timer__R32)
            {
                CyUtils.SetError(m_errorProvider, m_rbResolution32, Resources.ErrorMsgResolution);
            }
        }

        /// <summary>
        /// Validates Implementation value.
        /// Fixed Function mode is unavailable for PSoC 4 devices.
        /// </summary>
        private void ValidateImplementation()
        {
            if (m_params.FixedFunction && m_params.IsPSoC4)
            {
                CyUtils.SetError(m_errorProvider, m_rbFixedFunction, Resources.ErrorMsgImplementation);
            }
            else 
            {
                CyUtils.SetError(m_errorProvider, m_rbFixedFunction, String.Empty);
            }
        }

        /// <summary>
        /// Validates Period value.
        /// The max value depends on the Resolution value.
        /// </summary>
        private void ValidatePeriod()
        {
            ulong maxValue = (ulong)Math.Round(Math.Pow(2, (int)m_params.Resolution));

            bool isOk = CyUtils.ValidateNumeric(m_numPeriod, m_numPeriod.Minimum, maxValue, m_errorProvider);
            if (isOk == false)
            {   CyUtils.SetError(m_errorProvider, m_numPeriod, String.Format(Resources.ErrorMsgPeriod,
                   (int)m_params.Resolution, maxValue));
            }
        }

        /// <summary>
        /// Validates numerics min/max range.
        /// </summary>
        private void ValidateNumerics()
        {
            CyUtils.ValidateNumeric(m_numCaptureCount, (decimal)CyConstants.CAPTURE_COUNT_MIN, 
               (decimal)CyConstants.CAPTURE_COUNT_MAX, m_errorProvider);
            CyUtils.ValidateNumeric(m_numIntOnCaptureCount, (decimal)CyConstants.INT_CAPTURE_COUNT_MIN,
               (decimal)CyConstants.INT_CAPTURE_COUNT_MAX, m_errorProvider);
        }

        /// <summary>
        /// Validates EnableMode and RunMode conflicts. Shows a warning.
        /// </summary>
        private void ValidateRunMode()
        {
            CyUtils.SetError(m_warningProvider, m_cbRunMode, String.Empty);
            CyUtils.SetError(m_errorProvider, m_cbRunMode, String.Empty);

            if (m_params.FixedFunction && (m_params.RunMode == CyERunModes.B_Timer__RM_OneShotHaltOnInterrupt))
            {
                CyUtils.SetError(m_errorProvider, m_cbRunMode, Resources.RunModeDrcError);
            }
            else if ((m_params.FixedFunction == false) && 
                (m_params.EnableMode == CyEEnableModes.B_Timer__EM_HardwareOnly) &&
                (m_params.RunMode == CyERunModes.B_Timer__RM_OneShot || 
                 m_params.RunMode == CyERunModes.B_Timer__RM_OneShotHaltOnInterrupt))
            {
                CyUtils.SetError(m_warningProvider, m_cbRunMode, Resources.WarnMsgRunMode);
            }
        }

        /// <summary>
        /// Validates Capture mode related controls.
        /// </summary>
        private void ValidateCapture()
        {
            CyUtils.SetError(m_errorProvider, m_cbCaptureMode, String.Empty);
            CyUtils.SetError(m_errorProvider, m_chbxEnableCaptureCounter, String.Empty);
            CyUtils.SetError(m_errorProvider, m_chbxIntOnCapture, String.Empty);

            if (m_params.CaptureCounterEnabled)
            {
                if (m_params.FixedFunction)
                {
                    CyUtils.SetError(m_errorProvider, m_chbxEnableCaptureCounter, Resources.ErrorMsgEnableCaptureCounterFF);
                }
                else if (m_params.CaptureMode == CyECaptureModes.B_Timer__CM_None)
                {
                    CyUtils.SetError(m_errorProvider, m_chbxEnableCaptureCounter, Resources.ErrorMsgEnableCaptureCounter);
                }
            }
            if (m_params.InterruptOnCapture)
            {
                if (m_params.CaptureMode == CyECaptureModes.B_Timer__CM_None)
                {
                    CyUtils.SetError(m_errorProvider, m_chbxIntOnCapture, Resources.ErrorMsgIntOnCapture);
                }
            }
         
            if (m_params.FixedFunction && (m_params.CaptureMode != CyECaptureModes.B_Timer__CM_None && 
                                           m_params.CaptureMode != CyECaptureModes.B_Timer__CM_RisingEdge))
            {
                CyUtils.SetError(m_errorProvider, m_cbCaptureMode, Resources.ErrorMsgCaptureMode);
            }
        }

        /// <summary>
        /// Validates Trigger mode.
        /// </summary>
        private void ValidateTriggerMode()
        {
            if (m_params.FixedFunction && m_params.TriggerMode != CyETriggerModes.B_Timer__TM_None)
            {
                CyUtils.SetError(m_errorProvider, m_cbTriggerMode, Resources.ErrorMsgTriggerMode);
            }
            else
            {
                CyUtils.SetError(m_errorProvider, m_cbTriggerMode, String.Empty);
            }
        }

        /// <summary>
        /// Validates Enable mode.
        /// </summary>
        private void ValidateEnableMode()
        {
            if (m_params.FixedFunction && m_params.EnableMode == CyEEnableModes.B_Timer__EM_HardwareOnly)
            {
                CyUtils.SetError(m_errorProvider, m_cbEnableMode, Resources.ErrorMsgEnableMode);
            }
            else
            {
                CyUtils.SetError(m_errorProvider, m_cbEnableMode, String.Empty);
            }
        }

        /// <summary>
        /// Validates Interrupts.
        /// </summary>
        private void ValidateInterrupts()
        {
            if (m_params.FixedFunction && m_params.InterruptOnFIFOFull)
            {
                CyUtils.SetError(m_errorProvider, m_chbxIntOnFifoFull, Resources.ErrorMsgIntFifoFull);
            }
            else
            {
                CyUtils.SetError(m_errorProvider, m_chbxIntOnFifoFull, String.Empty);
            }
        }
        #endregion

        #region Event Handlers

        private void NumericUpDown_TextChanged(object sender, EventArgs e)
        {
            NumericUpDown num = sender as NumericUpDown;
            string strValue = num.Text;
            ulong value;
            
            if (UInt64.TryParse(strValue, out value) &&
                CyUtils.ValidateDoubleString(strValue, (double) num.Minimum, (double) num.Maximum))
            {
                if (sender == m_numPeriod)
                {
                    m_params.Period = value;
                    UpdateCalculatedPeriodLabel();
                }
                else if (sender == m_numCaptureCount)
                {
                    m_params.CaptureCount = (byte)value;
                }
                else if (sender == m_numIntOnCaptureCount)
                {
                    m_params.InterruptCaptures = (byte)value;
                }
                else
                {
                    Debug.Fail("Unexpected sender");
                }
            }
            ValidateAll();
            UpdateControlsState();
        }

        private void CheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (sender == m_chbxIntOnCapture)
            {
                m_params.InterruptOnCapture = m_chbxIntOnCapture.Checked;
            }
            else if (sender == m_chbxIntOnTC)
            {
                m_params.InterruptOnTC = m_chbxIntOnTC.Checked;
            }
            else if (sender == m_chbxIntOnFifoFull)
            {
                m_params.InterruptOnFIFOFull = m_chbxIntOnFifoFull.Checked;
            }
            else if (sender == m_chbxEnableCaptureCounter)
            {
                m_params.CaptureCounterEnabled = m_chbxEnableCaptureCounter.Checked;
            }
            else
            {
                Debug.Fail("Unexpected sender");
            }

            ValidateAll();
            UpdateControlsState();
        }

        private void ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Tx comboboxes
            if (sender == m_cbCaptureMode)
            {
                m_params.CaptureMode = (CyECaptureModes)m_cbCaptureMode.SelectedItem;
                UpdateComboboxCaptureMode();
            }
            else if (sender == m_cbTriggerMode)
            {
                m_params.TriggerMode = (CyETriggerModes)m_cbTriggerMode.SelectedItem;
            }
            else if (sender == m_cbEnableMode)
            {
                m_params.EnableMode = (CyEEnableModes)m_cbEnableMode.SelectedItem;
                UpdateComboboxEnableMode();
            }
            else if (sender == m_cbRunMode)
            {
                m_params.RunMode = (CyERunModes)m_cbRunMode.SelectedItem;
                UpdateComboboxRunMode();
            }

            ValidateAll();
            UpdateControlsState();
        }
       

        private void m_cb_Format(object sender, System.Windows.Forms.ListControlConvertEventArgs e)
        {
            if (sender == m_cbCaptureMode)
            {
                CyECaptureModes item = (CyECaptureModes)e.ListItem;
                e.Value = CyEnums.ToDisplayName(item);
            }
            else if (sender == m_cbEnableMode)
            {
                CyEEnableModes item = (CyEEnableModes)e.ListItem;
                e.Value = CyEnums.ToDisplayName(item);
            }
            else if (sender == m_cbRunMode)
            {
                CyERunModes item = (CyERunModes)e.ListItem;
                e.Value = CyEnums.ToDisplayName(item);
            }
            else if (sender == m_cbTriggerMode)
            {
                CyETriggerModes item = (CyETriggerModes)e.ListItem;
                e.Value = CyEnums.ToDisplayName(item);
            }
            else
            {
                Debug.Fail("Unexpected sender");
            }
        }

        private void m_rbResolution_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton rb = sender as RadioButton;
            if (rb.Checked == false) return;

            if (m_rbResolution8.Checked)
                m_params.Resolution = CyEResolutions.B_Timer__R8;
            else if (m_rbResolution16.Checked)
                m_params.Resolution = CyEResolutions.B_Timer__R16;
            else if (m_rbResolution24.Checked)
                m_params.Resolution = CyEResolutions.B_Timer__R24;
            else if (m_rbResolution32.Checked)
                m_params.Resolution = CyEResolutions.B_Timer__R32;

            ValidateAll();
            UpdateControlsState();
        }

        private void m_rbFFUDB_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton rb = sender as RadioButton;
            if (rb.Checked == false) return;

            m_params.FixedFunction = m_rbFixedFunction.Checked;

            UpdateComboboxCaptureMode();
            UpdateComboboxEnableMode();
            UpdateComboboxRunMode();

            // Validation
            ValidateAll();
            UpdateControlsState();
        }

        private void m_bMaxPeriod_Click(object sender, EventArgs e)
        {
            m_numPeriod.Value = (ulong)Math.Round(Math.Pow(2, (int)m_params.Resolution));
        }
        #endregion
    }
}
