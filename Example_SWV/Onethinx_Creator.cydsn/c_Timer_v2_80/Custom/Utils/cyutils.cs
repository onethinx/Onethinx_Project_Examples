/*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Windows.Forms;

namespace c_Timer_v2_80
{
    public class CyUtils
    {
        public const double EPS = 1E-5;

        #region Numeric
        /// <summary>
        /// Assigns minimum and maximum values to the NumericUpDown
        /// </summary>
        public static void SetNumericMinMax(NumericUpDown num, double min, double max)
        {
            num.Maximum = (decimal)max;
            num.Minimum = (decimal)min;
        }
        public static bool ValidateDoubleString(string text, double min, double max)
        {
            double value;
            bool isOK = Double.TryParse(text, out value);
            if (isOK)
            {
                if (value < min - EPS || value > max + EPS)
                    isOK = false;
            }
            return isOK;
        }

        /// <summary>
        /// Validates the text entered in the NumericUpDown and shows error in the error provider. 
        /// Return 'true' if validation passed.
        /// </summary>
        public static bool ValidateNumeric(NumericUpDown num, double min, double max, ErrorProvider errProvider)
        {
            bool isOK = ValidateDoubleString(num.Text, min, max);
            // Tag contains units of measurement
            string units = (num.Tag != null) ? num.Tag.ToString() : String.Empty;
            string msg = isOK ? String.Empty : String.Format(Resources.MsgValueRange, min, max, units);
            SetError(errProvider, num, msg);
            return isOK;
        }

        public static bool ValidateNumeric(NumericUpDown num, decimal min, decimal max, ErrorProvider errProvider)
        {
            return ValidateNumeric(num, (double)min, (double)max, errProvider);
        }

        /// <summary>
        /// Set value to NumericUpDown. Verifies if it doesn't exceed Minimum and Maximum and if so, sets either 
        /// minimum or maximum.
        /// </summary>
        /// <param name="nud">NumericUpDown</param>
        /// <param name="value">Value to set</param>
        public static void SetNudValue(NumericUpDown nud, double value)
        {
            decimal decimalValue = (decimal)value;
            if (decimalValue < nud.Minimum)
                decimalValue = nud.Minimum;
            else if (decimalValue > nud.Maximum)
                decimalValue = nud.Maximum;

            nud.Value = decimalValue;
        }
        #endregion Numeric

        #region ErrorProvider
        /// <summary>
        /// Set the error to the control using error provider. 
        /// Performs additional checks (enable state, previously assigned error)
        /// </summary>
        public static void SetError(ErrorProvider errorProvider, Control control, string message, bool overwrite)
        {
            if (control.Enabled == false)
            {
                // Hide error provider if the control is disabled
                errorProvider.SetError(control, String.Empty);
            }
            else if (String.IsNullOrEmpty(errorProvider.GetError(control)) == false && overwrite == false)
            {
                // Do nothing if some error has been assigned to this control earlier
            }
            else
            {
                errorProvider.SetError(control, message);
            }
        }

        /// <summary>
        /// Set the error to the control using error provider. 
        /// Performs additional checks (enable state)
        /// </summary>
        public static void SetError(ErrorProvider errorProvider, Control control, string message)
        {
            SetError(errorProvider, control, message, true);
        }
        #endregion
    }
}
