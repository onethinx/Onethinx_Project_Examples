/*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

using System.Collections.Generic;
using System.Windows.Forms;
using CyDesigner.Extensions.Common;
using CyDesigner.Extensions.Gde;

namespace c_Timer_v2_80
{
    [CyCompDevCustomizer()]
    public class CyCustomizer : ICyParamEditHook_v1, ICyDRCProvider_v1
    {
        #region ICyParamEditHook_v1 Members

        public DialogResult EditParams(ICyInstEdit_v1 edit, ICyTerminalQuery_v1 termQuery, ICyExpressMgr_v1 mgr)
        {
            // Create class that works with parameters defined in the component symbol 
            CyParameters parameters = new CyParameters(edit, termQuery);

            // Create the top-level editor control
            ICyTabbedParamEditor editor = edit.CreateTabbedParamEditor();
            editor.UseBigEditor = true;

            // Create a custom tab that is displayed in the customizer
            CyTimerControl configureTab = new CyTimerControl(parameters);

            // These delegates are called when a parameter is changed in the Expression view 
            CyParamExprDelegate dataChanged = delegate(ICyParamEditor custEditor, CyCompDevParam param)
            {
                parameters.GlobalEditMode = false;
                configureTab.UpdateUI();
                parameters.GlobalEditMode = true;
            };

            // Add a custom tab to the editor
            editor.AddCustomPage(configureTab.TabDisplayName, configureTab, dataChanged, configureTab.TabName);
            // This is a standard page that is added to all customizers
            editor.AddDefaultPage(Resources.BuiltInTabDisplayName, "Built-in");

            // Initialize controls on a tab based on the configuration stored in the component symbol 
            configureTab.UpdateUI();
            // When this parameter is set, changed parameter values are committed to the symbol 
            parameters.GlobalEditMode = true;

            edit.NotifyWhenDesignUpdates(parameters.UpdateDesigns);

            return editor.ShowDialog();
        }

        /// <summary>
        /// Defines if the customizer is launched as soon as a component is dropped on to the schematic 
        /// from the Component Catalog
        /// </summary>
        public bool EditParamsOnDrop
        {
            get { return false; }
        }

        /// <summary>
        /// Defines the level of customization provided by the implementation of ICyParamEditHook_v1 interface
        /// </summary>
        public CyCompDevParamEditorMode GetEditorMode()
        {
            return CyCompDevParamEditorMode.COMPLETE;
        }

        #endregion

        #region ICyDRCProvider_v1 Members
        public IEnumerable<CyDRCInfo_v1> GetDRCs(ICyDRCProviderArgs_v1 args)
        {
            CyParameters parameters = new CyParameters(args.InstQueryV1, args.TermQueryV1);

            List<CyCustErr> err = new List<CyCustErr>();
            err.Add(VerifyRunMode(parameters));
            err.Add(VerifyImplementation(parameters));

            for (int i = 0; i < err.Count; i++)
            {
                if (err[i].IsNotOK)
                    yield return new CyDRCInfo_v1(CyDRCInfo_v1.CyDRCType_v1.Error, err[i].Message);
            }
        }

        CyCustErr VerifyRunMode(CyParameters parameters)
        {
            if (parameters.RunMode == CyERunModes.B_Timer__RM_OneShotHaltOnInterrupt && parameters.FixedFunction)
            {
                return new CyCustErr(Resources.RunModeDrcError);
            }
            return CyCustErr.OK;
        }

        CyCustErr VerifyImplementation(CyParameters parameters)
        {
            if (parameters.InstQuery.DeviceQuery.IsPSoC4 && parameters.FixedFunction)
            {
                return new CyCustErr(Resources.ImplementationDrcError);
            }
            return CyCustErr.OK;
        }
        #endregion
    }
}
