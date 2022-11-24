/*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided. 
********************************************************************************/

using System;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using CyDesigner.Extensions.Gde;
using CyDesigner.Extensions.Common;

namespace c_Timer_v2_80
{
    internal class CyTabBase : UserControl, ICyParamEditingControl
    {
        #region Protected members
        protected readonly CyParameters m_params = null;
        protected readonly ErrorProvider m_errorProvider = null;
        protected readonly ErrorProvider m_warningProvider = null;
        #endregion

        #region Constructor(s)
        /// <summary>
        /// This constructor intended for correct control appearance in the designer mode.
        /// </summary>
        protected CyTabBase() { }

        public CyTabBase(CyParameters parameters, string tabName, string tabDisplayName)
        {
            TabName = tabName;
            TabDisplayName = tabDisplayName;
            m_params = parameters;

            m_errorProvider = new ErrorProvider();
            m_errorProvider.BlinkStyle = ErrorBlinkStyle.NeverBlink;

            m_warningProvider = new ErrorProvider();
            m_warningProvider.BlinkStyle = ErrorBlinkStyle.NeverBlink;
            m_warningProvider.Icon = Resources.Symbol_Warning_2;

            Dock = DockStyle.Fill;
            AutoScroll = true;
        }
        #endregion

        #region ICyParamEditingControl members
        public Control DisplayControl
        {
            get { return this; }
        }

        /// <summary>
        /// Gets error provider and parameters errors.
        /// </summary>
        public IEnumerable<CyCustErr> GetErrors()
        {
            List<CyCustErr> errs = new List<CyCustErr>();

            errs.AddRange(CheckControlForErrors(this.Controls));

            // Look for errors in parameters
            string tabName = TabName;
            foreach (string paramName in m_params.InstQuery.GetParamNames())
            {
                CyCompDevParam param = m_params.InstQuery.GetCommittedParam(paramName);
                if (param.TabName.Equals(tabName))
                {
                    if (param.ErrorCount > 0)
                    {
                        errs.AddRange(param.Errors.Select(errMsg => new CyCustErr(errMsg)));
                    }
                }
            }
            return errs;
        }
        #endregion

        #region Controls errors check method(s)
        /// <summary>
        /// Clears the error provider error for the specified control.
        /// </summary>
        protected void ClearError(Control control)
        {
            m_errorProvider.SetError(control, String.Empty);
        }

        /// <summary>
        /// Sets the error provider error for the specified control.
        /// </summary>
        protected void SetError(Control control, string errorMessage)
        {
            m_errorProvider.SetError(control, errorMessage);
        }

        /// <summary>
        /// Recursively grabs error messages from error provider for all controls
        /// </summary>
        /// <param name="controls">List of controls to check</param>
        /// <returns>List of found errors</returns>
        protected List<CyCustErr> CheckControlForErrors(ControlCollection controls)
        {
            string errorMessage = string.Empty;
            List<CyCustErr> errs = new List<CyCustErr>();
            foreach (Control control in controls)
            {
                errorMessage = m_errorProvider.GetError(control);
                if (string.IsNullOrEmpty(errorMessage) == false)
                    errs.Add(new CyCustErr(errorMessage));

                // Recursively call itself again to make sure that nested controls does not contain errors
                if (control.Controls.Count > 0)
                    errs.AddRange(CheckControlForErrors(control.Controls));
            }
            return errs;
        }

        #endregion

        #region ICyTab members
        public string TabName { set; get; }
        public string TabDisplayName { set; get; }
        public virtual void UpdateUI()
        {
            Debug.Fail("Calling of the CyTabBase virtual method");
        }
        #endregion
    }
}
