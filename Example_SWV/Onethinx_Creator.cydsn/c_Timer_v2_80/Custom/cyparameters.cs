/*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using CyDesigner.Extensions.Gde;
using CyDesigner.Toolkit;

namespace c_Timer_v2_80
{
    #region Component Parameters Names

    public class CyParamNames
    {
        public const string INSTANCE_NAME = "INSTANCE_NAME";
        public const string TRIGGER_MODE = "TriggerMode";
        public const string CAPTURE_MODE = "CaptureMode";
        public const string ENABLE_MODE = "EnableMode";
        public const string RUN_MODE = "RunMode";
        public const string CAPTURE_COUNT = "CaptureCount";
        public const string CAPTURE_COUNTER_ENABLED = "CaptureCounterEnabled";
        public const string RESOLUTION = "Resolution";
        public const string FIXED_FUNCTION = "FixedFunction";
        public const string CAPTURE_ALTERNATING_RISE = "CaptureAlternatingRise";
        public const string CAPTURE_ALTERNATING_FALL = "CaptureAlternatingFall";
        public const string PERIOD = "Period";
        public const string INTERRUPT_ON_CAPTURE = "InterruptOnCapture";
        public const string INTERRUPT_ON_TC = "InterruptOnTC";
        public const string INTERRUPT_ON_FIFO_FULL = "InterruptOnFIFOFull";
        public const string NUMBER_OF_CAPTURES = "NumberOfCaptures";
    }
    #endregion

    class CyParameters
    {
        #region Private fields
        private readonly ICyInstQuery_v1 m_instQuery;
        private readonly ICyInstEdit_v1 m_instEdit;
        private readonly ICyTerminalQuery_v1 m_termQuery;
        #endregion

        #region Events declaration
        /// <summary>
        /// An event that clients can use to be notified whenever the PSoC Creator design updates.
        /// </summary>
        public event EventHandler DesignUpdates;
        #endregion

        #region Constructor(s)
        private CyParameters(ICyInstQuery_v1 instQuery, ICyInstEdit_v1 instEdit, ICyTerminalQuery_v1 termQuery)
        {
            m_instQuery = instQuery;
            m_instEdit = instEdit;
            m_termQuery = termQuery;
        }

        public CyParameters(ICyInstQuery_v1 instQuery)
            : this(instQuery, null, null)
        {
        }

        public CyParameters(ICyInstEdit_v1 instEdit, ICyTerminalQuery_v1 termQuery)
            : this(instEdit, instEdit, termQuery)
        {
        }

        public CyParameters(ICyInstQuery_v1 instQuery, ICyTerminalQuery_v1 termQuery)
            : this(instQuery, null, termQuery)
        {
        }
        #endregion

        #region Properties
        //
        // Common properties
        //
        /// <summary>
        /// Indicates whether it is allowed to edit the parameters.
        /// During first getting of parameters this variable is false, what means that assigning
        /// values to form controls will not immediately overwrite parameters with the same values.
        /// </summary>
        public bool GlobalEditMode { get; set; }

        public ICyInstQuery_v1 InstQuery
        {
            get { return m_instQuery; }
        }

        public ICyTerminalQuery_v1 TermQuery
        {
            get { return m_termQuery; }
        }

        public bool IsPSoC4
        {
            get { return m_instQuery != null && m_instQuery.DeviceQuery.IsPSoC4; }
        }

        // Parameters from the symbol
        public bool CaptureAlternatingRise
        {
            get { return GetValue<bool>(CyParamNames.CAPTURE_ALTERNATING_RISE); }
            set { SetValue(CyParamNames.CAPTURE_ALTERNATING_RISE, value); }
        }

        public bool CaptureAlternatingFall
        {
            get { return GetValue<bool>(CyParamNames.CAPTURE_ALTERNATING_FALL); }
            set { SetValue(CyParamNames.CAPTURE_ALTERNATING_FALL, value); }
        }

        public byte CaptureCount
        {
            get { return GetValue<byte>(CyParamNames.CAPTURE_COUNT); }
            set { SetValue(CyParamNames.CAPTURE_COUNT, value); }
        }

        public bool CaptureCounterEnabled
        {
            get { return GetValue<bool>(CyParamNames.CAPTURE_COUNTER_ENABLED); }
            set { SetValue(CyParamNames.CAPTURE_COUNTER_ENABLED, value); }
        }

        public CyECaptureModes CaptureMode
        {
            get { return GetValue<CyECaptureModes>(CyParamNames.CAPTURE_MODE); }
            set { SetValue(CyParamNames.CAPTURE_MODE, value); }
        }

        public CyEEnableModes EnableMode
        {
            get { return GetValue<CyEEnableModes>(CyParamNames.ENABLE_MODE); }
            set { SetValue(CyParamNames.ENABLE_MODE, value); }
        }

        public bool FixedFunction
        {
            get { return GetValue<bool>(CyParamNames.FIXED_FUNCTION); }
            set { SetValue(CyParamNames.FIXED_FUNCTION, value); }
        }

        public bool InterruptOnCapture
        {
            get { return GetValue<bool>(CyParamNames.INTERRUPT_ON_CAPTURE); }
            set { SetValue(CyParamNames.INTERRUPT_ON_CAPTURE, value); }
        }

        public bool InterruptOnTC
        {
            get { return GetValue<bool>(CyParamNames.INTERRUPT_ON_TC); }
            set { SetValue(CyParamNames.INTERRUPT_ON_TC, value); }
        }

        public bool InterruptOnFIFOFull
        {
            get { return GetValue<bool>(CyParamNames.INTERRUPT_ON_FIFO_FULL); }
            set { SetValue(CyParamNames.INTERRUPT_ON_FIFO_FULL, value); }
        }

        public byte InterruptCaptures
        {
            get { return GetValue<byte>(CyParamNames.NUMBER_OF_CAPTURES); }
            set { SetValue(CyParamNames.NUMBER_OF_CAPTURES, value); }
        }

        // Period value stored in the symbol is less by 1
        public ulong Period
        {
            get { return ((ulong)GetValue<uint>(CyParamNames.PERIOD) + 1); } 
            set { SetValue(CyParamNames.PERIOD, (uint)(value - 1)); }
        }

        public CyEResolutions Resolution
        {
            get { return GetValue<CyEResolutions>(CyParamNames.RESOLUTION); }
            set { SetValue(CyParamNames.RESOLUTION, value); }
        }

        public CyERunModes RunMode
        {
            get { return GetValue<CyERunModes>(CyParamNames.RUN_MODE); }
            set { SetValue(CyParamNames.RUN_MODE, value); }
        }

        public CyETriggerModes TriggerMode
        {
            get { return GetValue<CyETriggerModes>(CyParamNames.TRIGGER_MODE); }
            set { SetValue(CyParamNames.TRIGGER_MODE, value); }
        }

        #endregion

        #region Getting / Setting Parameters
        public T GetValue<T>(string paramName)
        {
            return CyParamUtils.GetValue<T>(m_instQuery, paramName);
        }

        public void SetValue<T>(string paramName, T value)
        {
            if (GlobalEditMode)
            {
                CyParamUtils.SetValue(m_instEdit, paramName, value);
            }
        }
        #endregion

        #region Events
        // Invoke the DesignUpdates event; called whenever PSoC Creator design updates.
        protected virtual void OnDesignUpdates(EventArgs e)
        {
            if (DesignUpdates != null)
                DesignUpdates(this, e);
        }

        public void UpdateDesigns(ICyInstQuery_v1 query, ICyTerminalQuery_v1 termQuery)
        {
            OnDesignUpdates(EventArgs.Empty);
        }
        #endregion
    }
}
