/***************************************************************************//**
* \file CapSense_Processing.c
* \version 3.0
*
* \brief
*   This file provides the source code for the Data Processing module APIs
*   of the Component. The Data Processing module is responsible for the
*   low level raw counts processing provided by the sensing module, maintaining
*   baseline and difference values and performing high-level widget processing
*   like updating button status or calculating slider position.
*
* \see CapSense v3.0 Datasheet
*
*//*****************************************************************************
* Copyright (2016-2017), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#include "syslib/cy_syslib.h"
#include <stddef.h>
#include <string.h>
#include "CapSense_Configuration.h"
#include "CapSense_Processing.h"
#include "CapSense_Filter.h"

#if (0u != CapSense_CENTROID_EN)
    #include "CapSense_Centroid_LL.h"
#endif /* (0 != CapSense_CENTROID_EN) */

#if (0 != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN))
    #include "CapSense_SmartSense_LL.h"
#endif /* (0 != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN)) */

#if (CapSense_ENABLE == CapSense_SELF_TEST_EN)
    #include "CapSense_SelfTest.h"
#endif

#if (CapSense_ENABLE == CapSense_CENTROID_5X5_CSD_EN)
    #include "CapSense_AdvancedCentroid_LL.h"
#endif

#if (CapSense_ENABLE == CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
    #include "CapSense_AdaptiveFilter_LL.h"
#endif

#if(CapSense_ENABLE == CapSense_BALLISTIC_MULTIPLIER_EN)
    #include "CapSense_Gesture.h"
#endif

/***********************************************************************************************************************
* Local definition
***********************************************************************************************************************/

/* UINT32 constant with all odd bits set. It is used by the processing function of the Proximity widget to
*  determine type of the status bit: odd bits report proximity status; even bits report touch status;
*/
#define UINT32_ODD_BITS_CONST       (0x55555555u)

/* Default value for the Off Debounce counter when the Off Debounce is disabled */
#define DEFAULT_OFF_DEBOUNCE_VALUE  (1u)
#define PERCENTAGE_100              (100u)
#define TEMP_BUFFER_MSB_SHIFT       (16)

/***********************************************************************************************************************
* Local variables
***********************************************************************************************************************/
#if (0u != CapSense_CSX_MULTIPHASE_TX_EN)
    static uint16 deconvRowData[CapSense_CSX_MAX_TX_PHASE_LENGTH];
#endif


/***********************************************************************************************************************
* Static Function Prototypes
***********************************************************************************************************************/
/**
* \cond SECTION_CYSENSE_INTERNAL
* \addtogroup group_cysense_internal
* \{
*/
#if (0u != CapSense_TOTAL_LINEAR_SLIDERS)
    __STATIC_INLINE void DpUpdateLinSlider(
                        const uint16 newPos[], uint32 numTouches,
                        CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt,
                        CapSense_RAM_WD_SLIDER_STRUCT *ptrRamWdgt);
#endif /* #if (0u != CapSense_TOTAL_LINEAR_SLIDERS) */

#if (0u != CapSense_TOTAL_RADIAL_SLIDERS)
    __STATIC_INLINE void DpUpdateRadSlider(
                        const uint16 newPos[], uint32 numTouches,
                        CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt,
                        CapSense_RAM_WD_SLIDER_STRUCT *ptrRamWdgt);
#endif /* #if (0u != CapSense_TOTAL_RADIAL_SLIDERS) */

#if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN)
    __STATIC_INLINE void CapSense_DpInitCsdTouchpad(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt);
#endif /* #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN) */

#if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN)
    __STATIC_INLINE void CapSense_DpInitCsxTouchpad(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt);
#endif /* #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN) */

#if (0u != CapSense_SLIDER_WIDGET_EN)
    __STATIC_INLINE void CapSense_DpInitSlider(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt);
#endif /* #if (0u != CapSense_SLIDER_WIDGET_EN) */

#if (0u != CapSense_CSD_MATRIX_WIDGET_EN)
    __STATIC_INLINE void CapSense_DpInitCsdMatrix(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt);
#endif /* #if (0u != CapSense_CSD_MATRIX_WIDGET_EN) */

/** \}
* \endcond */

#if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpInitCsdTouchpad
****************************************************************************//**
*
* \brief
*  Performs initialization of the CSD touchpad widget.
*   - Resets the position data to NONE
*   - Initializes the position filter history if enabled.
*
* \details
*  The position data is always reset to the CapSense_TOUCHPAD_POS_NONE value.
*  The same happens to the position filter history data if enabled.
*
*******************************************************************************/
__STATIC_INLINE void CapSense_DpInitCsdTouchpad(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt)
{
    #if (CapSense_ENABLE == CapSense_CENTROID_5X5_CSD_EN)
        uint32 touchIndex;
    #endif

    CapSense_RAM_WD_CSD_TOUCHPAD_STRUCT *ptrWdTouchpad;

    #if (0u != CapSense_POSITION_FILTER_EN)
        CapSense_TOUCHPAD_POS_HISTORY_STRUCT *ptrHistory;
    #endif  /* #if (0u != CapSense_POSITION_FILTER_EN) */

    #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
        ADAPTIVE_FILTER_CONTEXT_STRUCT adpContext;
    #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */

    ptrWdTouchpad = ptrFlashWdgt->ptr2WdgtRam;

    /* Reset position data */
    #if (CapSense_ENABLE == CapSense_CENTROID_5X5_CSD_EN)
        #if (CapSense_ENABLE == CapSense_CENTROID_3X3_CSD_EN)
            if (0u != (ptrFlashWdgt->staticConfig & CapSense_CENTROID_5X5_MASK))
            {
        #endif

        ptrWdTouchpad->position.touchNum = 0u;

        #if (0u != CapSense_POSITION_FILTER_EN)
            ptrHistory = ptrFlashWdgt->ptr2PosHistory;
        #endif /* (0u != CapSense_POSITION_FILTER_EN) */

        for (touchIndex = 0u; touchIndex < ADVANCED_CENTROID_MAX_TOUCHES; touchIndex++)
        {
            ptrWdTouchpad->position.pos[touchIndex].x = ADVANCED_CENTROID_POSITION_NONE;
            ptrWdTouchpad->position.pos[touchIndex].y = ADVANCED_CENTROID_POSITION_NONE;
            ptrWdTouchpad->position.pos[touchIndex].zX = ADVANCED_CENTROID_POSITION_NONE;
            ptrWdTouchpad->position.pos[touchIndex].zY = ADVANCED_CENTROID_POSITION_NONE;

            /* Clear position filter history data if available */
            #if (0u != CapSense_POSITION_FILTER_EN)
                if (NULL != ptrHistory)
                {
                    CapSense_InitPosFiltersDd(&ptrHistory[touchIndex], CapSense_TOUCHPAD_POS_NONE, CapSense_TOUCHPAD_POS_NONE);
                    #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
                        AdaptiveFilter_Initialize(&ptrWdTouchpad->aiirConfig, &adpContext);
                        ptrHistory[touchIndex].xPos.posAIIRCoeff = adpContext.coefficient;
                        ptrHistory[touchIndex].yPos.posAIIRCoeff = adpContext.coefficient;
                    #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */
                }
            #endif /* (0u != CapSense_POSITION_FILTER_EN) */
        }

        #if (CapSense_ENABLE == CapSense_CENTROID_3X3_CSD_EN)
            }
        #endif
    #endif

    #if (CapSense_ENABLE == CapSense_CENTROID_3X3_CSD_EN)
        if (0u != (ptrFlashWdgt->staticConfig & CapSense_CENTROID_3X3_MASK))
        {
            ptrWdTouchpad->posX = CapSense_TOUCHPAD_POS_NONE;
            ptrWdTouchpad->posY = CapSense_TOUCHPAD_POS_NONE;

            /* Clear position filter history data if available */
            #if (0u != CapSense_POSITION_FILTER_EN)
                ptrHistory = ptrFlashWdgt->ptr2PosHistory;
                CapSense_InitPosFiltersDd(ptrHistory, CapSense_TOUCHPAD_POS_NONE, CapSense_TOUCHPAD_POS_NONE);

                #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
                    if (NULL != ptrHistory)
                    {
                        AdaptiveFilter_Initialize(&ptrWdTouchpad->aiirConfig, &adpContext);
                        ptrHistory->xPos.posAIIRCoeff = adpContext.coefficient;
                        ptrHistory->yPos.posAIIRCoeff = adpContext.coefficient;
                    }
                #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */
            #endif /* (0u != CapSense_POSITION_FILTER_EN) */
        }
    #endif
}
#endif /* #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN) */

#if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpInitCsxTouchpad
****************************************************************************//**
*
* \brief
*  Performs initialization of the CSX touchpad widget i.e. Resets the position data to NONE.
*
* \details
*  The position data is always reset to the CapSense_TOUCHPAD_POS_NONE value.
*
*******************************************************************************/
__STATIC_INLINE void CapSense_DpInitCsxTouchpad(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt)
{
    CapSense_RAM_WD_CSX_TOUCHPAD_STRUCT *ptrWdTouchpad;

    #if (0u != CapSense_POSITION_FILTER_EN)
        uint32 touchIndex;
        CapSense_TOUCHPAD_POS_HISTORY_STRUCT *ptrHistory;
    #endif  /* #if (0u != CapSense_POSITION_FILTER_EN) */

    #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
        ADAPTIVE_FILTER_CONTEXT_STRUCT adpContext;
    #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */

    ptrWdTouchpad = ptrFlashWdgt->ptr2WdgtRam;

    #if (0u != CapSense_POSITION_FILTER_EN)
        ptrHistory = ptrFlashWdgt->ptr2PosHistory;
    #endif /* (0u != CapSense_POSITION_FILTER_EN) */

    /* Reset position data for finger */
    ptrWdTouchpad->touch[0u].x = CapSense_TOUCHPAD_POS_NONE;
    ptrWdTouchpad->touch[0u].y = CapSense_TOUCHPAD_POS_NONE;
    ptrWdTouchpad->touch[0u].z = CY_LO8(CapSense_TOUCHPAD_POS_NONE);

    #if (CapSense_CSX_MAX_FINGERS > 1u)
        ptrWdTouchpad->touch[1u].x = CapSense_TOUCHPAD_POS_NONE;
        ptrWdTouchpad->touch[1u].y = CapSense_TOUCHPAD_POS_NONE;
        ptrWdTouchpad->touch[1u].z = CY_LO8(CapSense_TOUCHPAD_POS_NONE);
    #endif /* #if (CapSense_CSX_MAX_FINGERS > 1u) */

    #if (CapSense_CSX_MAX_FINGERS > 2u)
        ptrWdTouchpad->touch[2u].x = CapSense_TOUCHPAD_POS_NONE;
        ptrWdTouchpad->touch[2u].y = CapSense_TOUCHPAD_POS_NONE;
        ptrWdTouchpad->touch[2u].z = CY_LO8(CapSense_TOUCHPAD_POS_NONE);
    #endif /* #if (CapSense_CSX_MAX_FINGERS > 2u) */

    #if (0u != CapSense_POSITION_FILTER_EN)
        for (touchIndex = 0u; touchIndex < CapSense_CSX_MAX_FINGERS; touchIndex++)
        {
            if (NULL != ptrHistory)
            {
                /* Clear position filter history data if available */
                CapSense_InitPosFiltersDd(&ptrHistory[touchIndex], CapSense_TOUCHPAD_POS_NONE, CapSense_TOUCHPAD_POS_NONE);
                #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
                    AdaptiveFilter_Initialize(&ptrWdTouchpad->aiirConfig, &adpContext);
                    ptrHistory[touchIndex].xPos.posAIIRCoeff = adpContext.coefficient;
                    ptrHistory[touchIndex].yPos.posAIIRCoeff = adpContext.coefficient;
                #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */
            }
        }
    #endif /* (0u != CapSense_POSITION_FILTER_EN) */
}
#endif /* #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN) */

#if (0u != CapSense_SLIDER_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpInitSlider
****************************************************************************//**
*
* \brief
*  Performs initialization of the Radial and Linear Slider widget.
*   - Resets the position data to NONE
*   - Initializes the position filter history to NONE if enabled.
*
* \details
*  Position data is always reset to the CapSense_SLIDER_POS_NONE value.
*  The same happens to the position filter history data if enabled.
*
*******************************************************************************/
__STATIC_INLINE void CapSense_DpInitSlider(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt)
{
    uint32 idx;
    CapSense_RAM_WD_SLIDER_STRUCT *ptrWdSlider;

    #if (0u != CapSense_POSITION_FILTER_EN)
        CapSense_SLIDER_POS_HISTORY_STRUCT *ptrHistory;
    #endif /* #if (0u != CapSense_POSITION_FILTER_EN) */

    #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
        ADAPTIVE_FILTER_CONTEXT_STRUCT context;
    #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */

    ptrWdSlider = ptrFlashWdgt->ptr2WdgtRam;

    /* Reset slider position data to NONE */
    for (idx = 0u; idx < CapSense_NUM_CENTROIDS; idx++)
    {
        ptrWdSlider->position[idx] = CapSense_SLIDER_POS_NONE;

        #if (0u != CapSense_POSITION_FILTER_EN)
            ptrHistory = ptrFlashWdgt->ptr2PosHistory;
            if (NULL != ptrHistory)
            {
                CapSense_InitPosFiltersSd(&ptrHistory[idx], CapSense_SLIDER_POS_NONE);
                #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
                    AdaptiveFilter_Initialize(&ptrWdSlider->aiirConfig, &context);
                    ptrHistory->posAIIRCoeff = context.coefficient;
                #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */
            }

        #endif /* #if (0u != CapSense_POSITION_FILTER_EN) */
    }
}
#endif /* #if (0u != CapSense_SLIDER_WIDGET_EN) */

#if (0u != CapSense_CSD_MATRIX_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpInitCsdMatrix
****************************************************************************//**
*
* \brief
*  Performs initialization of the CSD Matrix Buttons widget.
*   - Resets the active button position data to NONE.
*
* \details
*  Position data is always reset to the CapSense_MATRIX_POS_NONE value.
*
*******************************************************************************/
__STATIC_INLINE void CapSense_DpInitCsdMatrix(CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt)
{
    CapSense_RAM_WD_CSD_MATRIX_STRUCT *ptrRamWdgt;

    ptrRamWdgt = (CapSense_RAM_WD_CSD_MATRIX_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;

    /* Reset sensor id, row and col registers in data structure */
    ptrRamWdgt->posRow   = CapSense_MATRIX_POS_NONE;
    ptrRamWdgt->posCol   = CapSense_MATRIX_POS_NONE;
    ptrRamWdgt->posSnsId = CapSense_MATRIX_POS_NONE;
}
#endif /* #if (0u != CapSense_CSD_MATRIX_WIDGET_EN) */

/*******************************************************************************
* Function Name: CapSense_DpInitialize
****************************************************************************//**
*
* \brief
*  Performs initialization of the data processing FW module:
*   - Resets the de-bounce counters of all the widgets
*   - Resets the position filter history for slider and touchpad widgets
*   - Clears widget and sensor statuses
*   - Enables all the widgets.
*
* \details
*  The Button and Matrix Button widgets have individual de-bounce counter per
*  sensor for the CSD widgets and per node for the CSX widgets.
*
*  The Slider and Touchpad widgets have a single de-bounce counter per widget.
*
*  The Proximity widget has two de-bounce counters per sensor. One for the
*  proximity event and second for the touch event.
*
*  All debounce counters during initialization are set to the value of the
*  onDebounce widget parameter.
*
*******************************************************************************/
void CapSense_DpInitialize(void)
{
    uint32 wdgtId;
    CapSense_WD_TYPE_ENUM widgetType;
    CapSense_RAM_WD_BASE_STRUCT * ptrRamWdgt;
    CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt;

    for (wdgtId = 0u; wdgtId < CapSense_WDGT_STATUS_WORDS; wdgtId++)
    {
        /* Clear widget status */
        CapSense_dsRam.wdgtStatus[wdgtId] = 0u;

        /* Enable all the widgets */
        CapSense_dsRam.wdgtEnable[wdgtId] = 0xFFFFFFFFLu;

        /* Set all the widgets to working state if the Self-Test is not enabled */
        #if (0u != CapSense_SELF_TEST_EN)
            CapSense_dsRam.wdgtWorking[wdgtId] = 0xFFFFFFFFLu;
        #endif /* #if (0u == CapSense_SELF_TEST_EN) */
    }

    ptrFlashWdgt = CapSense_dsFlash.wdgtArray;

    for (wdgtId = CapSense_TOTAL_WIDGETS; wdgtId-- > 0u; )
    {
        ptrRamWdgt = (CapSense_RAM_WD_BASE_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
        widgetType = (CapSense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType;

        /* Clear all sensor statuses */
        CapSense_dsRam.snsStatus[wdgtId] = 0u;

        /* Reset de-bounce counters */
        switch (widgetType)
        {
        #if (0u != CapSense_MATRIX_WIDGET_EN)
            case CapSense_WD_MATRIX_BUTTON_E:
                #if (0u != CapSense_CSD_MATRIX_WIDGET_EN)
                    if (CapSense_SENSE_METHOD_CSD_E == CapSense_GET_SENSE_METHOD(ptrFlashWdgt))
                    {
                        CapSense_DpInitCsdMatrix(ptrFlashWdgt);
                    }
                #endif /* #if (0u != CapSense_CSD_MATRIX_WIDGET_EN) */

                if (NULL != ptrFlashWdgt->ptr2DebounceArr)
                {
                    /* Initialize array of debounce counters */
                    (void)memset(ptrFlashWdgt->ptr2DebounceArr,
                                 (int32)ptrRamWdgt->onDebounce,
                                 (size_t)ptrFlashWdgt->totalNumSns);
                }
                break;
        #endif /* #if (0u != CapSense_MATRIX_WIDGET_EN) */

        #if (0u != CapSense_BUTTON_WIDGET_EN)
            case CapSense_WD_BUTTON_E:
                if (NULL != ptrFlashWdgt->ptr2DebounceArr)
                {
                    /* Initialize array of debounce counters */
                    (void)memset(ptrFlashWdgt->ptr2DebounceArr,
                                 (int32)ptrRamWdgt->onDebounce,
                                 (size_t)ptrFlashWdgt->totalNumSns);
                }
                break;
        #endif /* #if (0u != CapSense_BUTTON_WIDGET_EN) */

        #if (0u != CapSense_SLIDER_WIDGET_EN)
            case CapSense_WD_LINEAR_SLIDER_E:
            case CapSense_WD_RADIAL_SLIDER_E:
                if (NULL != ptrFlashWdgt->ptr2DebounceArr)
                {
                    /* Initialize single debounce counter of slider */
                    *(uint8 *)(ptrFlashWdgt->ptr2DebounceArr) = ptrRamWdgt->onDebounce;
                }

                CapSense_DpInitSlider(ptrFlashWdgt);

                break;
        #endif /* #if (0u != CapSense_SLIDER_WIDGET_EN) */

        #if (0u != CapSense_TOUCHPAD_WIDGET_EN)
            case CapSense_WD_TOUCHPAD_E:
                if (NULL != ptrFlashWdgt->ptr2DebounceArr)
                {
                    /* Initialize single debounce counter of touchpad */
                    *(uint8 *)(ptrFlashWdgt->ptr2DebounceArr) = ptrRamWdgt->onDebounce;
                }

                #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN)
                    if (CapSense_SENSE_METHOD_CSD_E == CapSense_GET_SENSE_METHOD(ptrFlashWdgt))
                    {
                        CapSense_DpInitCsdTouchpad(ptrFlashWdgt);
                    }
                #endif /* #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN) */

                #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN)
                    if (CapSense_SENSE_METHOD_CSX_E == CapSense_GET_SENSE_METHOD(ptrFlashWdgt))
                    {
                        CapSense_DpInitCsxTouchpad(ptrFlashWdgt);
                    }
                #endif /* #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN) */

                break;
        #endif /* #if (0u != CapSense_TOUCHPAD_WIDGET_EN) */

        #if (0u != CapSense_PROXIMITY_WIDGET_EN)
            case CapSense_WD_PROXIMITY_E:
                if (NULL != ptrFlashWdgt->ptr2DebounceArr)
                {
                    /* Proximity widgets have 2 de-bounce counters per sensor.
                     * Therefore the number of sensors is multiplied by 2.
                     */
                   (void)memset(ptrFlashWdgt->ptr2DebounceArr,
                                (int32)ptrRamWdgt->onDebounce,
                                (size_t)ptrFlashWdgt->totalNumSns * 2u);
                }
                break;
        #endif /* #if (0u != CapSense_PROXIMITY_WIDGET_EN) */

            default:
                break;
        }

        /* Move to next widget */
        ptrFlashWdgt++;
    }
}

#if (0u != CapSense_TOTAL_CSX_WIDGETS)
/*******************************************************************************
* Function Name: CapSense_DpProcessCsxWidgetRawCounts
****************************************************************************//**
*
* \brief
*  Performs default processing of the CSX raw counts.
*
* \details
*  Performs default processing of the CSX raw counts:
*   - Run Filters
*   - Update Baselines
*   - Update Differences
*   - Update Status.
*  The same process is applied to all the sensors of the specified widget.
*
* \param ptrFlashWdgt The pointer to the Flash widget structure.
*
* \return
*  Returns the status of the specified widget processing operation:
*  - CY_RET_SUCCESS if operation was successfully completed;
*  - CapSense_PROCESS_BASELINE_FAILED if baseline processing of any
*    sensor of the specified widget failed. The result is concatenated with the index
*    of failed sensor.
*
*******************************************************************************/
uint32 CapSense_DpProcessCsxWidgetRawCounts(CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 result = CY_RET_SUCCESS;
    uint32 widgetType;
    uint32 snsCount;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp;
    CapSense_RAM_WD_BASE_STRUCT *ptrRamWdgt;

    #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
        CapSense_PTR_FILTER_VARIANT fltrHistV;

        fltrHistV.ptr = ptrFlashWdgt->ptr2FltrHistory;
    #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */

    ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
    snsCount = ptrFlashWdgt->totalNumSns;
    ptrRamWdgt = (CapSense_RAM_WD_BASE_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
    widgetType = ptrFlashWdgt->wdgtType;

    /* Run deconvolution algorithm for the widgets with multiphase TX support */
    #if (0u != CapSense_CSX_MULTIPHASE_TX_EN)
        if (0u != (ptrFlashWdgt->staticConfig & CapSense_MULTIPHASE_TX_MASK))
        {
            CapSense_DpDeconvolution(ptrFlashWdgt);
        }
    #endif  /* #if (0u != CapSense_CSX_MULTIPHASE_TX_EN) */

    for (;snsCount-- > 0u;)
    {
        #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
            CapSense_FtRunEnabledFiltersInternal(fltrHistV, ptrSnsTmp, widgetType);
            #if (CapSense_ALP_FILTER_EN)
                CapSense_ConfigRunALPInternal(fltrHistV, ptrRamWdgt, ptrSnsTmp, widgetType);
            #endif
        #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */

        result = CapSense_FtUpdateBaseline(ptrRamWdgt, ptrSnsTmp, widgetType);

        #if (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN)
            if (CY_RET_SUCCESS != result)
            {
                result = snsCount | CapSense_PROCESS_BASELINE_FAILED;
            }
        #endif /* (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN) */

        CapSense_DpUpdateDifferences(ptrRamWdgt, ptrSnsTmp);

        /* Move pointers to the next sensor and filter history objects */
        ptrSnsTmp++;
        
        #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
            CapSense_INC_REG_FLTR_OBJ(fltrHistV);
        #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */
    }
    return result;
}

/*******************************************************************************
* Function Name: CapSense_DpProcessCsxSensorRawCountsExt
****************************************************************************//**
*
* \brief
*   Performs customized processing of the CSX sensor raw counts.
*
* \details
*   Performs customized processing of the CSX sensor raw counts.
*
*   If all bits are set at once the default processing order will take place.
*   For the custom order this API can be called multiple times and execute one task at a time.
*
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
* \param curSnsPtr    The pointer to the specific RAM Sensor Object.
* \param fltrHistV    The pointer to the Filter History Object in RAM associated with specific sensor.
* \param mode         The bit-mask with the data processing tasks to be executed.
*   The mode parameters can take the following values:
*    - CapSense_PROCESS_FILTER     (0x01) Run Firmware Filter
*    - CapSense_PROCESS_BASELINE   (0x02) Update Baselines
*    - CapSense_PROCESS_DIFFCOUNTS (0x04) Update Difference Counts
*    - CapSense_PROCESS_ALL               Execute all tasks
*
* \return
*  Returns the status of the specified sensor processing operation:
*  - CY_RET_SUCCESS if operation was successfully completed;
*  - CapSense_PROCESS_BASELINE_FAILED if baseline processing of any
*    sensor of the specified widget failed. The result is concatenated with the index
*    of failed sensor.
*
*******************************************************************************/
uint32 CapSense_DpProcessCsxSensorRawCountsExt(
                CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt,
                CapSense_RAM_SNS_STRUCT *curSnsPtr,
                CapSense_PTR_FILTER_VARIANT fltrHistV,
                uint32 mode)
{
    uint32  result = CY_RET_SUCCESS;
    uint32  widgetType;
    CapSense_RAM_WD_BASE_STRUCT *ptrRamWdgt;

    widgetType = ptrFlashWdgt->wdgtType;
    ptrRamWdgt = (CapSense_RAM_WD_BASE_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;

    #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
        if (0u != (mode & CapSense_PROCESS_FILTER))
        {
            CapSense_FtRunEnabledFiltersInternal(fltrHistV, curSnsPtr, widgetType);
            #if (CapSense_ALP_FILTER_EN)
                CapSense_ConfigRunALPInternal(fltrHistV, ptrRamWdgt, curSnsPtr, widgetType);
            #endif
        }
    #else
        (void)fltrHistV; /* This parameter is unused in such configurations */
    #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */

    if (0u != (mode & CapSense_PROCESS_BASELINE))
    {
        result = CapSense_FtUpdateBaseline(ptrRamWdgt, curSnsPtr, widgetType);
    }

    if (0u != (mode & CapSense_PROCESS_DIFFCOUNTS))
    {
        CapSense_DpUpdateDifferences(ptrRamWdgt, curSnsPtr);
    }
    return result;
}


/*******************************************************************************
* Function Name: CapSense_DpProcessCsxWidgetStatus
****************************************************************************//**
*
* \brief
*   Updates the status of the CSX widget in the Data Structure.
*
* \details
*  This API determines the type of widget and runs the appropriate function
*  that implements the status update algorithm for this type of widget.
*
*  When the widget-specific processing completes this API updates the snsStatus
*  and wdgtStatus registers in the data structure. The wdgtStatus register
*  is a bitmask were each bit corresponds to the specific widget identified by
*  the widget index. When the bit is set the widget is active (touched);
*
*  Depending on the type of widget the snsStatus register may contain either
*  status of each widget node (active/non-active) or in case of the CSX Touchpad
*  is contains the number of touches. In both cases 0 means that widget is not
*  active.
*
* \param widgetId     The widget ID number.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
*******************************************************************************/
void CapSense_DpProcessCsxWidgetStatus(uint32 widgetId, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 sensorStatus;
    uint32 wdStatusIndex;
    uint32 wdStatusMask;
    CapSense_WD_TYPE_ENUM widgetType;

    widgetType = (CapSense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType;
    sensorStatus = CapSense_dsRam.snsStatus[widgetId];

    switch (widgetType)
    {
    #if (0u != CapSense_BUTTON_WIDGET_EN)
        case CapSense_WD_BUTTON_E:
            sensorStatus = CapSense_DpProcessButton(sensorStatus, ptrFlashWdgt);
            break;
    #endif /* #if (0u != CapSense_BUTTON_WIDGET_EN) */

    #if (0u != CapSense_CSX_MATRIX_WIDGET_EN)
        case CapSense_WD_MATRIX_BUTTON_E:
            sensorStatus = CapSense_DpProcessButton(sensorStatus, ptrFlashWdgt);
            break;
    #endif /* #if (0u != CapSense_CSX_MATRIX_WIDGET_EN) */

    #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN)
        case CapSense_WD_TOUCHPAD_E:
            sensorStatus = CapSense_DpProcessCsxTouchpad(sensorStatus, ptrFlashWdgt);
            break;
    #endif /* #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN) */

    default:
        CY_ASSERT(0 != 0);
        break;
    }

    /* Update sensor status in the data structure */
    CapSense_dsRam.snsStatus[widgetId] = (CapSense_SNS_STS_TYPE)sensorStatus;

    /* The next two lines calculate index and bitmask of the widget status bit in the wdgtStatus array member */
    wdStatusIndex = CapSense_GET_WDGT_STATUS_INDEX(widgetId);
    wdStatusMask = CapSense_GET_WDGT_STATUS_MASK(widgetId);

    /* Check if there are active sensors and update widget status accordingly */
    if (0u == sensorStatus)
    {
        CapSense_dsRam.wdgtStatus[wdStatusIndex] &= ~wdStatusMask;
    }
    else
    {
        CapSense_dsRam.wdgtStatus[wdStatusIndex] |= wdStatusMask;
    }
}
#endif /* #if CapSense_TOTAL_CSX_WIDGETS */

#if (0u != CapSense_TOTAL_CSD_WIDGETS)
/*******************************************************************************
* Function Name: CapSense_DpProcessCsdWidgetRawCounts
****************************************************************************//**
*
* \brief
*   Performs default processing for the CSD widget raw counts.
*
* \details
*   Performs default processing for the CSD widget raw counts:
*    - Run Filters
*    - Update Thresholds (FW Auto-tuning)
*    - Update Baselines
*    - Update Differences.
*   The process is applied to all widget's sensors.
*
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
* \return
*  Returns the status of the specified widget processing operation:
*  - CY_RET_SUCCESS if operation was successfully completed;
*  - CapSense_PROCESS_BASELINE_FAILED if baseline processing of any
*    sensor of the specified widget failed. The result is concatenated with the index
*    of failed sensor.
*
*******************************************************************************/
uint32 CapSense_DpProcessCsdWidgetRawCounts(CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32  result = CY_RET_SUCCESS;

    #if (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN)
        uint32 bslnTstResult = CY_RET_SUCCESS;
    #endif /* (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN) */

    uint32  widgetType;
    uint32  snsId;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp;
    CapSense_RAM_WD_BASE_STRUCT *ptrRamWdgt;

    #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN))
        /* The noise envelope data is available only when Full CSD Auto-Tuning is enabled */
        SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT *ptrNoiseEnvelope;
        #if (CapSense_ENABLE == CapSense_PROXIMITY_WIDGET_EN)
            uint32  proximityThreshold;
        #endif /* (0 != CapSense_PROXIMITY_WIDGET_EN) */
    #endif /* #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN)) */

    #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
        CapSense_PTR_FILTER_VARIANT fltrHistV;

        /* Determine if widget type is proximity */
        uint32 isProxWdgt = (CapSense_GET_WIDGET_TYPE(ptrFlashWdgt) == CapSense_WD_PROXIMITY_E) ? 1u : 0u;

        /* Get pointer to the filter history object */
        fltrHistV.ptr = ptrFlashWdgt->ptr2FltrHistory;
    #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */

    ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
    ptrRamWdgt = (CapSense_RAM_WD_BASE_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
    widgetType = ptrFlashWdgt->wdgtType;

    #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN))
        ptrNoiseEnvelope = ptrFlashWdgt->ptr2NoiseEnvlp;
    #endif /* #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN)) */

    /* Iterate through all widget's sensors */
    for(snsId = 0u; snsId < ptrFlashWdgt->totalNumSns; snsId++)
    {
        /* Run all enabled filter on specified widget's sensor */
        #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
            CapSense_FtRunEnabledFiltersInternal(fltrHistV, ptrSnsTmp, widgetType);
            #if (CapSense_ALP_FILTER_EN)
                CapSense_ConfigRunALPInternal(fltrHistV, ptrRamWdgt, ptrSnsTmp, widgetType);
            #endif
        #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */

        /* Run auto-tuning activities */
        #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN))
            SmartSense_RunNoiseEnvelope(ptrSnsTmp->raw[0u], ptrRamWdgt->sigPFC, ptrNoiseEnvelope);
            CapSense_DpUpdateThresholds(ptrRamWdgt, ptrNoiseEnvelope, snsId);

            /* Calculate Proximity Touch Threshold in SmartSense mode based on Finger Threshold and PROX_TOUCH_COEFF */
            #if (CapSense_ENABLE == CapSense_PROXIMITY_WIDGET_EN)
                if (CapSense_WD_PROXIMITY_E == CapSense_GET_WIDGET_TYPE(ptrFlashWdgt))
                {
                    proximityThreshold = ((uint32)ptrRamWdgt->fingerTh * CapSense_PROX_TOUCH_COEFF) / PERCENTAGE_100;
                    if ((((1Lu) << CapSense_THRESHOLD_SIZE) - 1u) < proximityThreshold)
                    {
                        proximityThreshold = ((1Lu) << CapSense_THRESHOLD_SIZE) - 1u;
                    }
                    ((CapSense_RAM_WD_PROXIMITY_STRUCT *)ptrFlashWdgt->ptr2WdgtRam)->proxTouchTh =
                        (CapSense_THRESHOLD_TYPE)proximityThreshold;
                }
            #endif /* (0 != CapSense_PROXIMITY_WIDGET_EN) */

            /* Move to next noise-envelope object */
            ptrNoiseEnvelope++;
        #endif /* (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN)) */

        #if (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN)
            bslnTstResult = CapSense_FtUpdateBaseline(ptrRamWdgt, ptrSnsTmp, widgetType);
            if ((CY_RET_SUCCESS != bslnTstResult) && (CY_RET_SUCCESS == result))
            {
                result = snsId | CapSense_PROCESS_BASELINE_FAILED;
            }
        #else /* (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN) */
            result = CapSense_FtUpdateBaseline(ptrRamWdgt, ptrSnsTmp, widgetType);
        #endif /* (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN) */

        CapSense_DpUpdateDifferences(ptrRamWdgt, ptrSnsTmp);

        /* Move to next sensor object */
        ptrSnsTmp++;

        #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
            /* Move to the next filter history object */
            CapSense_INC_FLTR_OBJ_VARIANT(isProxWdgt, fltrHistV);
        #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */
    }
    return result;
}


/*******************************************************************************
* Function Name: CapSense_DpProcessCsdSensorRawCountsExt
****************************************************************************//**
*
* \brief
*   Performs customized processing of the CSD sensor raw counts.
*
* \details
*   Performs customized processing of the CSD sensor raw counts.
*
*   Note that this API updates data associated only with the sensor. It cannot be used to update
*   widget status.
*
* \param ptrFlashWdgt The pointer to the Flash widget structure.
* \param curSnsPtr    The pointer to the sensor object in RAM.
* \param fltrHistV    The pointer to the Filter History Object in RAM.
* \param mode         The bit-mask with the data processing tasks to be executed.
*   The mode parameters can take the following values:
*    - CapSense_PROCESS_FILTER     (0x01) Run Firmware Filter
*    - CapSense_PROCESS_BASELINE   (0x02) Update Baselines
*    - CapSense_PROCESS_DIFFCOUNTS (0x04) Update Difference Counts
*    - CapSense_PROCESS_CALC_NOISE (0x08) Calculate Noise (only if FW Tuning is enabled)
*    - CapSense_PROCESS_THRESHOLDS (0x10) Update Thresholds (only if FW Tuning is enabled)
*    - CapSense_PROCESS_ALL               Execute all tasks
*
* \return
*  Returns the status of the specified sensor processing operation:
*  - CY_RET_SUCCESS if operation was successfully completed;
*  - CapSense_PROCESS_BASELINE_FAILED if baseline processing of any
*    sensor of the specified widget failed. The result is concatenated with the index
*    of failed sensor.
*
*******************************************************************************/
uint32 CapSense_DpProcessCsdSensorRawCountsExt(
                CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt,
                CapSense_RAM_SNS_STRUCT *curSnsPtr,
                CapSense_PTR_FILTER_VARIANT fltrHistV,
                 uint32 mode)
{
    uint32 result = CY_RET_SUCCESS;
    uint32  widgetType;
    CapSense_RAM_WD_BASE_STRUCT *ptrRamWdgt;

    #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN))
        SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT *ptrNoiseEnvelope;
        #if (CapSense_ENABLE == CapSense_PROXIMITY_WIDGET_EN)
            uint32  proximityThreshold;
        #endif /* (0 != CapSense_PROXIMITY_WIDGET_EN) */

        /* Need to specify pointer to corresponding sensor's noise-envelope object.*/
        ptrNoiseEnvelope = ptrFlashWdgt->ptr2NoiseEnvlp;
    #endif /* (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN)) */

    widgetType = ptrFlashWdgt->wdgtType;
    ptrRamWdgt = (CapSense_RAM_WD_BASE_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;

    #if (CapSense_ENABLE == CapSense_RC_FILTER_EN)
        if (0u != (mode & CapSense_PROCESS_FILTER))
        {
            CapSense_FtRunEnabledFiltersInternal(fltrHistV, curSnsPtr, widgetType);
            #if (CapSense_ALP_FILTER_EN)
                CapSense_ConfigRunALPInternal(fltrHistV, ptrRamWdgt, curSnsPtr, widgetType);
            #endif
        }
    #else
        (void)fltrHistV; /* This parameter is unused in such configurations */
    #endif /* #if (CapSense_ENABLE == CapSense_RC_FILTER_EN) */

    #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN))
        if (0u != (mode & CapSense_PROCESS_CALC_NOISE))
        {
            SmartSense_RunNoiseEnvelope(curSnsPtr->raw[0u], ptrRamWdgt->sigPFC, ptrNoiseEnvelope);
        }

        if (0u != (mode & CapSense_PROCESS_THRESHOLDS))
        {
            CapSense_DpUpdateThresholds(ptrRamWdgt, ptrNoiseEnvelope, 0u);

            /* Calculate Proximity Touch Threshold in SmartSense mode based on Finger Threshold and PROX_TOUCH_COEFF */
            #if (CapSense_ENABLE == CapSense_PROXIMITY_WIDGET_EN)
                if (CapSense_WD_PROXIMITY_E == CapSense_GET_WIDGET_TYPE(ptrFlashWdgt))
                {
                    proximityThreshold = ((uint32)ptrRamWdgt->fingerTh * CapSense_PROX_TOUCH_COEFF) / PERCENTAGE_100;
                    if ((((1Lu) << CapSense_THRESHOLD_SIZE) - 1u) < proximityThreshold)
                    {
                        proximityThreshold = ((1Lu) << CapSense_THRESHOLD_SIZE) - 1u;
                    }
                    ((CapSense_RAM_WD_PROXIMITY_STRUCT *)ptrFlashWdgt->ptr2WdgtRam)->proxTouchTh =
                        (CapSense_THRESHOLD_TYPE)proximityThreshold;
                }
            #endif /* (0 != CapSense_PROXIMITY_WIDGET_EN) */
        }
    #endif /* #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN)) */

    if (0u != (mode & CapSense_PROCESS_BASELINE))
    {
        result = CapSense_FtUpdateBaseline(ptrRamWdgt, curSnsPtr, widgetType);
    }

    if (0u != (mode & CapSense_PROCESS_DIFFCOUNTS))
    {
        CapSense_DpUpdateDifferences(ptrRamWdgt, curSnsPtr);
    }
    return result;
}

/*******************************************************************************
* Function Name: CapSense_DpProcessCsdWidgetStatus
****************************************************************************//**
*
* \brief
*  Runs the status processing of the CSD widget and updates it in the Data Structure.
*
* \details
*  This API determines the widget type and calls the appropriate low-level processing
*  function. Based on the returned results it updates the widget and sensor
*  status in the data structure. Any widget specific registers like the centroid
*  position are updated directly in the low-level processing function.
*
*  When the widget-specific processing completes this API updates the snsStatus
*  and wdgtStatus registers in the data structure. The wdgtStatus register
*  is a bitmask were each bit corresponds to the specific widget identified by
*  the widget index. When the bit is set the widget is active (touched);
*
*  The snsStatus register for CSD widgets contains status of each widget sensor
*  (active/non-active). Each bit corresponds to one sensor. In case of the
*  Proximity widget two bits are used per sensor: one for proximity status and
*  second for the touch status.
*
* \param widgetId      The widget index number.
* \param ptrFlashWdgt  The pointer to the Flash widget structure.
*
*******************************************************************************/
void CapSense_DpProcessCsdWidgetStatus(uint32 widgetId, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 sensorStatus;
    uint32 wdStatusIndex;
    uint32 wdStatusMask;
    CapSense_WD_TYPE_ENUM widgetType;

    widgetType = (CapSense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType;
    sensorStatus = CapSense_dsRam.snsStatus[widgetId];

    switch (widgetType)
    {
    #if (0u != CapSense_BUTTON_WIDGET_EN)
        case CapSense_WD_BUTTON_E:
            sensorStatus = CapSense_DpProcessButton(sensorStatus, ptrFlashWdgt);
            break;
    #endif /* #if (0u != CapSense_BUTTON_WIDGET_EN) */

    #if (0u != CapSense_SLIDER_WIDGET_EN)
        case CapSense_WD_LINEAR_SLIDER_E:
        case CapSense_WD_RADIAL_SLIDER_E:
            sensorStatus = CapSense_DpProcessSlider(sensorStatus, ptrFlashWdgt);
            break;
    #endif /* #if (0u != CapSense_SLIDER_WIDGET_EN) */

    #if (0u != CapSense_CSD_MATRIX_WIDGET_EN)
        case CapSense_WD_MATRIX_BUTTON_E:
            sensorStatus = CapSense_DpProcessCsdMatrix(sensorStatus, ptrFlashWdgt);
            break;
    #endif /* #if (0u != CapSense_CSD_MATRIX_WIDGET_EN) */

    #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN)
        case CapSense_WD_TOUCHPAD_E:
            sensorStatus = CapSense_DpProcessCsdTouchpad(sensorStatus, ptrFlashWdgt);
            #if (0u != CapSense_BALLISTIC_MULTIPLIER_EN)
                CapSense_RunBallisticMultiplier(widgetId);
            #endif /* (0u != CapSense_BALLISTIC_MULTIPLIER_EN) */
            break;
    #endif /* #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN) */

    #if (0u != CapSense_PROXIMITY_WIDGET_EN)
        case CapSense_WD_PROXIMITY_E:
            sensorStatus = CapSense_DpProcessProximity(sensorStatus, ptrFlashWdgt);
            break;
    #endif /* #if (0u != CapSense_PROXIMITY_WIDGET_EN) */

    default:
        CY_ASSERT(0 != 0);
        break;
    }

    CapSense_dsRam.snsStatus[widgetId] = (CapSense_SNS_STS_TYPE)sensorStatus;

    /* The next two lines calculate index and bitmask of the widget status bit in the wdgtStatus array member */
    wdStatusIndex = CapSense_GET_WDGT_STATUS_INDEX(widgetId);
    wdStatusMask = CapSense_GET_WDGT_STATUS_MASK(widgetId);

    /* Check if there are active sensors and update widget status accordingly */
    if (0u == sensorStatus)
    {
        CapSense_dsRam.wdgtStatus[wdStatusIndex] &= ~wdStatusMask;
    }
    else
    {
        CapSense_dsRam.wdgtStatus[wdStatusIndex] |= wdStatusMask;
    }
}
#endif /* #if (0u != CapSense_TOTAL_CSD_WIDGETS) */


#if (0 != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN))
/*******************************************************************************
* Function Name: CapSense_DpUpdateThresholds
****************************************************************************//**
*
* \brief
*   Updates noise and finger thresholds for a specified widget.
*
* \details
*   Updates noise and finger thresholds for a specified widget.
*   This algorithm is a part of Auto-tuning feature.
*
* \param ptrRamWdgt       The pointer to the RAM widget object.
* \param ptrNoiseEnvelope The pointer to the noise-envelope object in RAM.
* \param startFlag        The flag indicates when new widget is processed.
*
*******************************************************************************/
void CapSense_DpUpdateThresholds(CapSense_RAM_WD_BASE_STRUCT *ptrRamWdgt,
                                         SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT *ptrNoiseEnvelope,
                                         uint32 startFlag)
{
    SMARTSENSE_UPDATE_THRESHOLDS_STRUCT thresholds;
    SMARTSENSE_UPDATE_THRESHOLDS_STRUCT *ptrThresholds = &thresholds;
    ptrThresholds->fingerTh = ptrRamWdgt->fingerTh;

    SmartSense_UpdateThresholds(ptrNoiseEnvelope, ptrThresholds, ptrRamWdgt->sigPFC, startFlag);

    ptrRamWdgt->fingerTh = ptrThresholds->fingerTh;
    ptrRamWdgt->noiseTh = ptrThresholds->noiseTh;
    ptrRamWdgt->nNoiseTh = ptrThresholds->nNoiseTh;
    ptrRamWdgt->hysteresis = ptrThresholds->hysteresis;
}
#endif /* #if (0u != (CapSense_CSD_AUTOTUNE & CapSense_CSD_SS_TH_EN)) */


/*******************************************************************************
* Function Name: CapSense_DpUpdateDifferences
****************************************************************************//**
*
* \brief
*   Calculates new difference values.
*   Also, executes the multi-frequency scan algorithm if it is enabled.
*
* \details
*   This API calculates the difference between the baseline and raw counts.
*   If the difference is positive (raw > baseline), and higher than the widget
*   noise threshold, it is saved into the data structure for further processing.
*   Otherwise the difference is set to zero. The final difference value is saved
*   with the subtracted noise threshold value.
*
*   If the multi-frequency scan algorithm is enabled, the difference is calculated
*   for the three sets of data available for each sensor. Then
*   the median filter selects the final difference value.
*
* \param ptrRamWdgt The pointer to the RAR Widget Object.
* \param curSnsPtr  The pointer to the sensor object in RAM.
*
*******************************************************************************/
void CapSense_DpUpdateDifferences(
                        CapSense_RAM_WD_BASE_STRUCT *ptrRamWdgt,
                        CapSense_RAM_SNS_STRUCT *curSnsPtr)
{
    register int32 diffValue = 0;

    #if (0u != CapSense_MULTI_FREQ_SCAN_EN)
        register int32 diffValue_0 = 0;
        register int32 diffValue_2 = 0;
    #endif /* #if (0u != CapSense_MULTI_FREQ_SCAN_EN) */

    /* Calculate difference */
    diffValue = (int32)curSnsPtr->raw[0u] - (int32)curSnsPtr->bsln[0u];

    #if (0u != CapSense_MULTI_FREQ_SCAN_EN)
        diffValue_0 = (int32)curSnsPtr->raw[1u] - (int32)curSnsPtr->bsln[1u];
        diffValue_2 = (int32)curSnsPtr->raw[2u] - (int32)curSnsPtr->bsln[2u];

        /* Find median and put it into [0u] element */
        if (diffValue_0 > diffValue)
        {
            register int32 swapv = diffValue_0;
            diffValue_0 = diffValue;
            diffValue = swapv;
        }

        if (diffValue > diffValue_2)
        {
            register int32 swapv = diffValue;
            diffValue = diffValue_2;
            diffValue_2 = swapv;
        }

        if (diffValue_0 > diffValue)
        {
            register int32 swapv = diffValue_0;
            diffValue_0 = diffValue;
            diffValue = swapv;
        }
    #endif /* #if (0u != CapSense_MULTI_FREQ_SCAN_EN) */

    curSnsPtr->diff = 0u;

    if (diffValue > (int32)ptrRamWdgt->noiseTh)
    {
        curSnsPtr->diff = (CapSense_THRESHOLD_TYPE) diffValue;
    }
}


#if ((0u != CapSense_BUTTON_WIDGET_EN) || (0u != CapSense_CSX_MATRIX_WIDGET_EN))
/*******************************************************************************
* Function Name: CapSense_DpProcessButton
****************************************************************************//**
*
* \brief
*   Processes the status of the Button widget.
*
* \details
*   This API processes the status of the CSD/CSX Button widget and
*   CSX Matrix Button widget. It applies the hysteresis and de-bounce algorithm
*   to each sensor difference value. This API is expected to be called
*   after each new widget scan. If it is called multiple times for the same
*   scan data, the de-bounce algorithm will not work properly.
*
*   This API does not update any Data Structure registers
*   except the de-bounce counters.
*
* \param currStatus   The current status of the widget sensors.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
* \return Returns the status of the widget's sensors. Bit #0 - corresponds to Sensor #0.
*
*******************************************************************************/
uint32 CapSense_DpProcessButton(uint32 currStatus, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 snsCnt;
    uint32 snsMask;
    uint32 newStatus;
    uint32 touchTh;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp;
    CapSense_RAM_WD_BASE_STRUCT *ptrRamWdgt;
    uint8 * ptrDebounceCnt;

    snsCnt = ptrFlashWdgt->totalNumSns;
    ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
    ptrRamWdgt = (CapSense_RAM_WD_BASE_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
    ptrDebounceCnt = ptrFlashWdgt->ptr2DebounceArr;

    /* Go through all widget's sensors */
    snsMask = 1Lu;
    for (; snsCnt-- > 0u; )
    {
        /* Calculate touch threshold taking into account hysteresis
           and current sensor state. */
        touchTh = (0u == (currStatus & snsMask)) ?
                  ((uint32)ptrRamWdgt->fingerTh + ptrRamWdgt->hysteresis) :
                  ((uint32)ptrRamWdgt->fingerTh - ptrRamWdgt->hysteresis);

        /* Determine new potential sensor state */
        newStatus = (ptrSnsTmp->diff > touchTh) ? snsMask : 0u;

        /* Sensor state changed ? */
        if (newStatus != (currStatus & snsMask))
        {
            /* Decrement de-bounce counter */
            (*ptrDebounceCnt)--;
        }

        /*
        *  This piece of code has two objectives:
        *  1. Reset de-bounce counter if the sensor state did not change.
        *     (It is possible that during previous processing cycles sensor
        *     was active and de-bounce counter was decremented.)
        *  2. Set new sensor state after successful de-bounce.
        */
        if ((newStatus == (currStatus & snsMask)) || (0u == (*ptrDebounceCnt)))
        {
            /* Set new sensor status */
            currStatus &= ~snsMask;
            currStatus |= newStatus;

            /* Reset de-bounce counter */
            #if (0u != CapSense_OFF_DEBOUNCE_EN)
                *ptrDebounceCnt = (0Lu == newStatus) ? ptrRamWdgt->onDebounce : ptrRamWdgt->offDebounce;
            #else
                *ptrDebounceCnt = (0Lu == newStatus) ? ptrRamWdgt->onDebounce : DEFAULT_OFF_DEBOUNCE_VALUE;
            #endif /* #if (0u != CapSense_OFF_DEBOUNCE_EN) */
        }

        /* Move mask to the next sensor */
        snsMask <<= 1u;

        ptrSnsTmp++;
        ptrDebounceCnt++;
    }

    return currStatus;
}
#endif /* #if ((0u != CapSense_BUTTON_WIDGET_EN) || (0u != CapSense_CSX_MATRIX_WIDGET_EN)) */


#if (0u != CapSense_PROXIMITY_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpProcessProximity
****************************************************************************//**
*
* \brief
*   Processes the status of the Proximity widget.
*
* \details
*   This API processes the status of the CSD Proximity widget. It applies the
*   hysteresis and debounce algorithm to each sensor difference value.
*   The proximity and touch events are considered independently so debounce and
*   hysteresis are also applied independently.
*
*   This API is expected to be called after each new widget scan. If it is
*   called multiple times for the same scan data the debounce algorithm
*   will not work properly.
*
*   This API does not update any Data Structure registers
*   except the debounce counters.
*
* \param currStatus   The value with the current widget status.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
* \return
*  Returns the status of the proximity sensors. Each sensor is represented by two bits
*  that can take the following values:
*   - 00b not active
*   - 01b proximity is detected (signal is above finger threshold)
*   - 11b finger touch is detected (signal is above the touch threshold)
*
*******************************************************************************/
uint32 CapSense_DpProcessProximity(uint32 currStatus, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 currTh;
    uint32 snsCnt;
    uint32 snsMask;
    uint32 newStatus;
    uint32 difference;

    uint8 *ptrDebounceCnt;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp;
    CapSense_RAM_WD_PROXIMITY_STRUCT *ptrRamWdgt;

    /* Mask for proximity status bits. (All odd bits.) */
    const uint32 proxMask = UINT32_ODD_BITS_CONST;

    ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
    ptrRamWdgt = (CapSense_RAM_WD_PROXIMITY_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
    ptrDebounceCnt = ptrFlashWdgt->ptr2DebounceArr;

    /* Get number of sensors and multiply it by 2 - proximity and touch statuses per sensor. */
    snsCnt = (uint32)ptrFlashWdgt->totalNumSns << 1u;

    /* Go through all sensor's status bits */
    snsMask = 1Lu;
    for (; snsCnt-- > 0u;)
    {
        difference = ptrSnsTmp->diff;

        /* Determine current event (proximity - odd; touch - even) and get appropriate threshold. */
        currTh = ptrRamWdgt->fingerTh;

        if (0u == (snsMask & proxMask))
        {
            currTh = ptrRamWdgt->proxTouchTh;
            ptrSnsTmp++;
        }

        /* Calculate finger threshold applying hysteresis based on current sensor state. */
        currTh = (0u == (currStatus & snsMask)) ? (currTh + ptrRamWdgt->hysteresis) : (currTh - ptrRamWdgt->hysteresis);

        /* Determine new potential sensor state before de-bounce */
        newStatus = (difference > currTh) ? snsMask : 0Lu;

        /* Sensor state changed ? */
        if (newStatus != (currStatus & snsMask))
        {
            /* Decrement de-bounce counter */
            (*ptrDebounceCnt)--;
        }

        /*
        *  This piece of code has two objectives:
        *  1. Reset de-bounce counter if new sensor state has not changed.
        *     (It is possible that during previous processing cycles the sensor
        *     state was different which caused de-bounce counter change.)
        *  2. Set new sensor state after successful de-bounce.
        */
        if ((newStatus == (currStatus & snsMask)) || (0u == (*ptrDebounceCnt)))
        {
            /* Set new sensor status */
            currStatus &= ~snsMask;
            currStatus |= newStatus;

            /* Reset de-bounce counter */
            #if (0u != CapSense_OFF_DEBOUNCE_EN)
                *ptrDebounceCnt = (0Lu == newStatus) ? ptrRamWdgt->onDebounce : ptrRamWdgt->offDebounce;
            #else
                *ptrDebounceCnt = (0Lu == newStatus) ? ptrRamWdgt->onDebounce : DEFAULT_OFF_DEBOUNCE_VALUE;
            #endif /* #if (0u != CapSense_OFF_DEBOUNCE_EN) */
        }

        ptrDebounceCnt++;

        /* Move sensor mask to the next sensor */
        snsMask <<= 1u;
    }

    return currStatus;
}
#endif /* #if (0u != CapSense_PROXIMITY_WIDGET_EN) */


#if (0u != CapSense_SLIDER_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpProcessSlider
****************************************************************************//**
*
* \brief
*   Processes the status of the Linear/Radial Slider widget.
*   The process involves running the Linear/Radial Centroid algorithm.
*
* \details
*   This API processes the status of the CSD Slider widget (Linear and Radial).
*   It applies the hysteresis and de-bounce algorithm to the widget ignoring
*   the individual states of the sensors.
*
*   This API is expected to be called after each new widget scan. If it is
*   called multiple times for the same scan data, the debounce algorithm
*   will not work properly.
*
*   Slider widgets do not support the ON to OFF debounce.
*
*   This API does not update any Data Structure registers
*   except the debounce counters.
*
* \param currStatus   The current sensor status of the Slider widget.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
* \return Returns the status of the widget's sensors. Bit #0 - corresponds to Sensor #0.
*
*******************************************************************************/
uint32 CapSense_DpProcessSlider(uint32 currStatus, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 snsCnt;
    uint32 snsMask;
    uint32 touchTh;
    uint32 newStatus = 0Lu;
    uint8 * ptrDebounceCnt;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp;
    CapSense_RAM_WD_SLIDER_STRUCT *ptrRamWdgt;

    #if (0u != CapSense_POSITION_FILTER_EN)
        CapSense_SLIDER_POS_HISTORY_STRUCT *ptrHistory;
    #endif /* #if (0u != CapSense_POSITION_FILTER_EN) */

    uint32 touchIdx;
    uint32 touchCnt = 0Lu;
    uint16 newPos[CapSense_NUM_CENTROIDS];

    snsCnt = ptrFlashWdgt->totalNumSns;
    ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
    ptrRamWdgt = (CapSense_RAM_WD_SLIDER_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
    ptrDebounceCnt = ptrFlashWdgt->ptr2DebounceArr;

    #if (0u != CapSense_POSITION_FILTER_EN)
        ptrHistory = ptrFlashWdgt->ptr2PosHistory;
    #endif /* #if (0u != CapSense_POSITION_FILTER_EN) */

    /* Calculate touch threshold based on current slider state */
    touchTh = (0Lu == currStatus ) ?
              ((uint32)ptrRamWdgt->fingerTh + ptrRamWdgt->hysteresis) :
              ((uint32)ptrRamWdgt->fingerTh - ptrRamWdgt->hysteresis);

    /* Go through all widget's sensors */
    snsMask = 1Lu;
    for (; snsCnt-- > 0Lu; )
    {
        /* Determine new potential sensor state */
        newStatus |= (ptrSnsTmp->diff > touchTh) ? snsMask : 0Lu;
        ptrSnsTmp++;

        snsMask <<= 1u;
    }

    /* If widget is active, check whether debounce is needed */
    if (0Lu != newStatus)
    {
        if (0u < (*ptrDebounceCnt))
        {
            /* Decrement debounce counter */
            (*ptrDebounceCnt)--;
        }

        if (0u == (*ptrDebounceCnt))
        {
            /* Update slider sensor state when debounce completes */
            currStatus = newStatus;
        }
    }
    else
    {
        /* Reset debounce counter */
        *ptrDebounceCnt = ptrRamWdgt->onDebounce;

        /* Clear slider sensor state */
        currStatus = 0Lu;
    }

    /* Centroid processing */
    /* If there are active sensors after debounce run the centroid algorithm */
    if (0u != currStatus)
    {
        /* Centroid always uses the lowest finger threshold (minus hysteresis)
         * to produce same results for first and all consecutive invocations */
        touchTh = (uint32)ptrRamWdgt->fingerTh - ptrRamWdgt->hysteresis;
        snsCnt = ptrFlashWdgt->totalNumSns;

        #if (0u != CapSense_TOTAL_DIPLEXED_SLIDERS)
            if ((0u != (ptrFlashWdgt->staticConfig & CapSense_DIPLEXING_MASK)))
            {
                /* Run local maximum search for duplexed slider */
                touchCnt = CapSense_DpFindLocalMaxDiplex(
                                ptrFlashWdgt->ptr2SnsRam, snsCnt, ptrFlashWdgt->ptr2DiplexTable, touchTh);

                /* Centroid calculation API requires double sensor count for the diplexed sliders. */
                snsCnt *= 2u;
            }
            else
            {
                /* Run local maximum search for NON-duplexed slider */
                #if (CapSense_ENABLE == CapSense_4PTS_LOCAL_MAX_EN)
                    touchCnt = CapSense_DpFindLocalMaxSd4pts(ptrFlashWdgt, ptrFlashWdgt->ptr2SnsRam, snsCnt, touchTh);
                #else
                    touchCnt = CapSense_DpFindLocalMaxSd(ptrFlashWdgt->ptr2SnsRam, snsCnt, touchTh);
                #endif
            }
        #else
            /* Run local maximum search for NON-duplexed slider */
            #if (CapSense_ENABLE == CapSense_4PTS_LOCAL_MAX_EN)
                touchCnt = CapSense_DpFindLocalMaxSd4pts(ptrFlashWdgt, ptrFlashWdgt->ptr2SnsRam, snsCnt, touchTh);
            #else
                touchCnt = CapSense_DpFindLocalMaxSd(ptrFlashWdgt->ptr2SnsRam, snsCnt, touchTh);
            #endif
        #endif /* #if (0u != CapSense_TOTAL_DIPLEXED_SLIDERS) */

        /* If at least one touch is detected */
        if (0u != touchCnt)
        {
            /* Determine slider type and calculate new position(s) */
            #if (0u != CapSense_TOTAL_LINEAR_SLIDERS)
                if ((CapSense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType == CapSense_WD_LINEAR_SLIDER_E)
                {
                    #if (CapSense_ENABLE == CapSense_SLIDER_MULT_METHOD)
                        touchCnt = CapSense_DpCalcLinearCentroid(newPos, (uint32)ptrFlashWdgt->xCentroidMultiplier,
                                    snsCnt, (uint32)(ptrFlashWdgt->xCentroidMultiplier >> 1));
                    #else
                        touchCnt = CapSense_DpCalcLinearCentroid(
                                        newPos, (uint32)ptrFlashWdgt->xCentroidMultiplier, snsCnt, 0u);
                    #endif
                    DpUpdateLinSlider(newPos, touchCnt, ptrFlashWdgt, ptrRamWdgt);
                }
            #endif /* #if (0u != CapSense_TOTAL_LINEAR_SLIDERS) */

            #if (0u != CapSense_TOTAL_RADIAL_SLIDERS)
                if ((CapSense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType == CapSense_WD_RADIAL_SLIDER_E)
                {
                    touchCnt = CapSense_DpCalcRadialCentroid(
                                    newPos, (uint32)ptrFlashWdgt->xCentroidMultiplier, snsCnt);
                    DpUpdateRadSlider(newPos, touchCnt, ptrFlashWdgt, ptrRamWdgt);
                }
            #endif /* #if (0u != CapSense_TOTAL_RADIAL_SLIDERS) */
        }
    }

    /* Clear not active position data slots */
    for (touchIdx = touchCnt; touchIdx < CapSense_NUM_CENTROIDS; touchIdx++)
    {
        ptrRamWdgt->position[touchIdx] = CapSense_SLIDER_POS_NONE;

        #if (0u != CapSense_POSITION_FILTER_EN)
            if (NULL != ptrHistory)
            {
                CapSense_InitPosFiltersSd(&ptrHistory[touchIdx], CapSense_SLIDER_POS_NONE);
            }
        #endif /* #if (0u != CapSense_POSITION_FILTER_EN) */
    }

    return currStatus;
}
#endif /* #if (0u != CapSense_SLIDER_WIDGET_EN) */

#if (0u != CapSense_TOTAL_LINEAR_SLIDERS)
/*******************************************************************************
* Function Name: DpUpdateLinSlider
****************************************************************************//**
*
* \brief
*   Updates the touch status of the Linear Slider widget.
*
* \details
*   This function updates the position data of the Linear Slider
*   in the Data Structure. Also, this function runs position filters for the
*   position data if any enabled.
*
* \param newPos       The pointer to the array with the new finger position coordinates.
* \param numTouches   The number of the coordinates in the newPos array.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
* \param ptrRamWdgt   The pointer to the RAM Widget Object.
*
*******************************************************************************/
__STATIC_INLINE void DpUpdateLinSlider(
                        const uint16 newPos[],
                        uint32 numTouches,
                        CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt,
                        CapSense_RAM_WD_SLIDER_STRUCT *ptrRamWdgt)
{
    uint32 touchIdx;

    #if (0u != CapSense_POS_AVERAGE_FILTER_EN)
        uint16 tempPosition;
    #endif /* (0u != CapSense_POS_AVERAGE_FILTER_EN) */

    #if (0u == CapSense_POSITION_FILTER_EN)
        /* This parameter is unused in such configurations */
        (void)ptrFlashWdgt;
    #else
        uint16 filteredPosition;
        /* Get bitmask with the enabled filters */
        uint32 filterConfig = (uint32)ptrFlashWdgt->staticConfig;
        /* Get pointer to the position filter history */
        CapSense_SLIDER_POS_HISTORY_STRUCT *ptrHistory = ptrFlashWdgt->ptr2PosHistory;
    #endif /* #if (0u == CapSense_POSITION_FILTER_EN) */

    #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
        ADAPTIVE_FILTER_CONTEXT_STRUCT contextAIIR;
        uint16 emptyPosition = 0;
    #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */

    /* Run Filter and update the position registers with the filtered data */
    for (touchIdx = 0u; touchIdx < numTouches; touchIdx++)
    {
        #if (0u == CapSense_POSITION_FILTER_EN)
            /* If filters are not enabled just copy the position data */
            ptrRamWdgt->position[touchIdx] = newPos[touchIdx];
        #else
            filteredPosition = newPos[touchIdx];

            /* If there were no touch initialize the current position with new position data */
            if (ptrRamWdgt->position[touchIdx] == CapSense_SLIDER_POS_NONE)
            {
                CapSense_InitPosFiltersSd(ptrHistory, filteredPosition);
            }
            else
            {
                #if (0u != CapSense_POS_MEDIAN_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_MEDIAN_FILTER_MASK))
                    {
                        filteredPosition = (uint16)CapSense_FtMedian((uint32)ptrHistory->posMedianZ2,
                                            (uint32)ptrHistory->posMedianZ1, (uint32)newPos[touchIdx]);
                        ptrHistory->posMedianZ2 = ptrHistory->posMedianZ1;
                        ptrHistory->posMedianZ1 = newPos[touchIdx];
                    }
                #endif /* #if (0u != CapSense_POS_MEDIAN_FILTER_EN) */

                #if (0u != CapSense_POS_IIR_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_IIR_FILTER_MASK))
                    {
                        filteredPosition = (uint16)CapSense_FtIIR1stOrder((uint32)filteredPosition, (uint32)ptrHistory->posIIR,
                                            (uint32)ptrFlashWdgt->iirFilterCoeff, 0uL);
                        ptrHistory->posIIR = filteredPosition;
                    }
                #endif /* #if (0u != CapSense_POS_IIR_FILTER_EN) */

                #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
                    if (0u != (filterConfig & CapSense_AIIR_FILTER_MASK))
                    {
                        contextAIIR.previousX = ptrHistory->posAIIR;
                        contextAIIR.previousY = emptyPosition;
                        contextAIIR.coefficient = ptrHistory->posAIIRCoeff;
                        AdaptiveFilter_Run(&ptrRamWdgt->aiirConfig, &contextAIIR, &filteredPosition, &emptyPosition);
                        ptrHistory->posAIIR = contextAIIR.previousX;
                        ptrHistory->posAIIRCoeff = contextAIIR.coefficient;
                    }
                #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */

                #if (0u != CapSense_POS_AVERAGE_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_AVERAGE_FILTER_MASK))
                    {
                        tempPosition = filteredPosition;
                        filteredPosition = (uint16)(filteredPosition + ptrHistory->posAverage) >> 1u;
                        ptrHistory->posAverage = tempPosition;
                    }
                #endif /* #if (0u != CapSense_POS_AVERAGE_FILTER_EN) */

                #if (0u != CapSense_POS_JITTER_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_JITTER_FILTER_MASK))
                    {
                        filteredPosition = (uint16)CapSense_FtJitter((uint32)filteredPosition, (uint32)ptrHistory->posJitter);
                        ptrHistory->posJitter = filteredPosition;
                    }
                #endif /* #if (0u != CapSense_POS_JITTER_FILTER_EN) */
            }

            /* Update position data */
            ptrRamWdgt->position[touchIdx] = filteredPosition;

            ptrHistory = (NULL != ptrHistory) ? (ptrHistory + 1) : NULL;
        #endif /* (0u == CapSense_POSITION_FILTER_EN) */
    }
}
#endif /* #if (0u != CapSense_TOTAL_LINEAR_SLIDERS) */


#if (0u != CapSense_TOTAL_RADIAL_SLIDERS)
/*******************************************************************************
* Function Name: DpUpdateRadSlider
****************************************************************************//**
*
* \brief
*   Filters the Radial Slider widget position.
*
* \details
*   This function updates the position data of the radial slider
*   in the Data Structure. Also, this function runs position filters for the
*   position data if any enabled.
*
*   The filtering algorithm takes into account that it is possible that new value
*   will cross the zero position so it adjusts the position data appropriately to
*   get the correct filtered value.
*
* \param newPos[]     The pointer to the array with new centroid position data.
* \param numTouches   The number of elements in the newPos array.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
* \param ptrRamWdgt   The pointer to the RAM Widget Object .
*
*******************************************************************************/
__STATIC_INLINE void DpUpdateRadSlider(
                        const uint16 newPos[], uint32 numTouches,
                        CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt,
                        CapSense_RAM_WD_SLIDER_STRUCT *ptrRamWdgt)
{
    uint32 touchIdx;

    #if (0u != CapSense_POS_MEDIAN_FILTER_EN)
        uint16 z1;
        uint16 z2;
    #endif

    #if ((0u != CapSense_POS_IIR_FILTER_EN) || \
         (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN))
        uint16 delta;
    #endif

    #if (0u != CapSense_POS_AVERAGE_FILTER_EN)
        uint16 tempPosition;
    #endif

    #if (0u != CapSense_POSITION_FILTER_EN)
        uint16 filteredPosition;
        /* Get resolution. The register contains max position value, so therefore it is increased by 1 */
        uint16 tempResolution = (ptrFlashWdgt->xResolution + 1u);
        uint16 resolutionHafl = (ptrFlashWdgt->xResolution + 1u) >> 1u;
        /* Get bitmask with the enabled filters */
        uint32 filterConfig = (uint32)ptrFlashWdgt->staticConfig;
        /* Get pointer to the position filter history */
        CapSense_SLIDER_POS_HISTORY_STRUCT *ptrHistory = ptrFlashWdgt->ptr2PosHistory;
    #endif

    #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
        ADAPTIVE_FILTER_CONTEXT_STRUCT contextAIIR;
        uint16 emptyPosition = 0;
    #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */

    /* Run Filter and update the position registers with the filtered data */
    for (touchIdx = 0u; touchIdx < numTouches; touchIdx++)
    {
        #if (0u == CapSense_POSITION_FILTER_EN)
            /* If filters are not enabled just copy the position data */
            ptrRamWdgt->position[touchIdx] = newPos[touchIdx];
        #else
            filteredPosition = newPos[touchIdx];

            /* If there were no touch initialize the current position with new position data */
            if (ptrRamWdgt->position[touchIdx] == CapSense_SLIDER_POS_NONE)
            {
                CapSense_InitPosFiltersSd(ptrHistory, filteredPosition);
            }
            else
            {
                /*
                 * If new position crosses zero point in one or another direction the position variable with smaller value
                 * is increased by the slider resolution. It is done for the proper filtering. For example,
                 * yResolution = 100, currPosition = 95, newPosition = 5. If no actions are taken then the average filter
                 * will give value 50 - which is wrong. But if the position values are adusted as mentioned here we will get
                 * newPosition eqaul 105 and the average will be 100. Later this filtered value will be adusted further to not
                 * cross the yResolution and it will end up with 0u - which is correct average result for the provided example.
                 */
                #if (0u != CapSense_POS_MEDIAN_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_MEDIAN_FILTER_MASK))
                    {
                        /* Get the filter history for further zero-cross correction */
                        z1 = ptrHistory->posMedianZ1;
                        z2 = ptrHistory->posMedianZ2;

                        /* Preserve the filter history without zero-cross correction */
                        ptrHistory->posMedianZ2 = ptrHistory->posMedianZ1;
                        ptrHistory->posMedianZ1 = filteredPosition;

                        /* Perform zero-cross correction */
                        if (z1 > (resolutionHafl + filteredPosition))
                        {
                            filteredPosition += tempResolution;
                        }
                        if (filteredPosition > (resolutionHafl + z1))
                        {
                            z1 += tempResolution;
                            z2 += tempResolution;
                        }
                        if (z2 > (resolutionHafl + z1))
                        {
                            z1 += tempResolution;
                            filteredPosition += tempResolution;
                        }
                        if (z1 > (resolutionHafl + z2))
                        {
                            z2 += tempResolution;
                        }

                        /* Perform filtering */
                        filteredPosition = CapSense_FtMedian((uint32)z2, (uint32)z1, filteredPosition);

                        /* Perform zero-cross correction of filtered position */
                        if (filteredPosition >= tempResolution)
                        {
                            filteredPosition -= tempResolution;
                        }
                    }
                #endif /* #if (0u != CapSense_POS_MEDIAN_FILTER_EN) */

                #if (0u != CapSense_POS_IIR_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_IIR_FILTER_MASK))
                    {
                        /* Perform zero-cross correction */
                        if (ptrHistory->posIIR > (resolutionHafl + filteredPosition))
                        {
                            filteredPosition += tempResolution;
                        }
                        if (filteredPosition > (resolutionHafl + ptrHistory->posIIR))
                        {
                            ptrHistory->posIIR += tempResolution;
                        }
                        /*
                        * IIR filter can accumulate delay up to full circle and even more.
                        * This situation is not supported by the Component. If difference between
                        * between the new position and IIR filter history is bigher than
                        * CapSense_POS_IIR_RESET_RADIAL_SLIDER percent of circle
                        * then all enabled position filters are reset.
                        */
                        if (ptrHistory->posIIR > filteredPosition)
                        {
                            delta = ptrHistory->posIIR - filteredPosition;
                        }
                        else
                        {
                            delta = filteredPosition - ptrHistory->posIIR;
                        }
                        if (delta > ((tempResolution * CapSense_POS_IIR_RESET_RADIAL_SLIDER) / PERCENTAGE_100))
                        {
                            CapSense_InitPosFiltersSd(ptrHistory, filteredPosition);
                        }
                        else
                        {
                            /* Perform filtering */
                            filteredPosition = (uint16)CapSense_FtIIR1stOrder(filteredPosition, (uint32)ptrHistory->posIIR,
                                                ptrFlashWdgt->iirFilterCoeff, 0u);
                        }
                        /* Perform zero-cross correction of filtered position */
                        if (filteredPosition >= tempResolution)
                        {
                            filteredPosition -= tempResolution;
                        }
                        ptrHistory->posIIR = filteredPosition;
                    }
                #endif /* #if (0u != CapSense_POS_IIR_FILTER_EN) */

                #if (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN)
                    if (0u != (filterConfig & CapSense_AIIR_FILTER_MASK))
                    {
                        /* Perform zero-cross correction */
                        if (ptrHistory->posAIIR > (resolutionHafl + filteredPosition))
                        {
                            filteredPosition += tempResolution;
                        }
                        if (filteredPosition > (resolutionHafl + ptrHistory->posAIIR))
                        {
                            ptrHistory->posAIIR += tempResolution;
                        }
                        if (ptrHistory->posAIIR > filteredPosition)
                        {
                            delta = ptrHistory->posAIIR - filteredPosition;
                        }
                        else
                        {
                            delta = filteredPosition - ptrHistory->posAIIR;
                        }
                        if (delta > ((tempResolution * CapSense_POS_IIR_RESET_RADIAL_SLIDER) / PERCENTAGE_100))
                        {
                            CapSense_InitPosFiltersSd(ptrHistory, filteredPosition);
                        }
                        else
                        {
                            contextAIIR.previousX = ptrHistory->posAIIR;
                            contextAIIR.previousY = emptyPosition;
                            contextAIIR.coefficient = ptrHistory->posAIIRCoeff;
                            AdaptiveFilter_Run(&ptrRamWdgt->aiirConfig, &contextAIIR, &filteredPosition, &emptyPosition);
                            ptrHistory->posAIIRCoeff = contextAIIR.coefficient;
                        }
                        /* Perform zero-cross correction of filtered position */
                        if (filteredPosition >= tempResolution)
                        {
                            filteredPosition -= tempResolution;
                        }
                        ptrHistory->posAIIR = filteredPosition;
                    }
                #endif /* (0u != CapSense_POS_ADAPTIVE_IIR_FILTER_EN) */

                #if (0u != CapSense_POS_AVERAGE_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_AVERAGE_FILTER_MASK))
                    {
                        tempPosition = filteredPosition;
                        /* Perform zero-cross correction */
                        if (ptrHistory->posAverage > (resolutionHafl + filteredPosition))
                        {
                            filteredPosition += tempResolution;
                        }
                        if (filteredPosition > (resolutionHafl + ptrHistory->posAverage))
                        {
                            ptrHistory->posAverage += tempResolution;
                        }
                        /* Perform filtering */
                        filteredPosition = (uint16)((filteredPosition + ptrHistory->posAverage) >> 1u);
                        /* Perform zero-cross correction of filtered position */
                        if (filteredPosition >= tempResolution)
                        {
                            filteredPosition -= tempResolution;
                        }
                        ptrHistory->posAverage = tempPosition;
                    }
                #endif /* #if (0u != CapSense_POS_AVERAGE_FILTER_EN) */

                #if (0u != CapSense_POS_JITTER_FILTER_EN)
                    if (0u != (filterConfig & CapSense_POS_JITTER_FILTER_MASK))
                    {
                        /* Perform zero-cross correction */
                        if (ptrHistory->posJitter > (resolutionHafl + filteredPosition))
                        {
                            filteredPosition += tempResolution;
                        }
                        if (filteredPosition > (resolutionHafl + ptrHistory->posJitter))
                        {
                            ptrHistory->posJitter += tempResolution;
                        }
                        /* Perform filtering */
                        filteredPosition = (uint16)CapSense_FtJitter((uint32)filteredPosition, (uint32)ptrHistory->posJitter);
                        /* Perform zero-cross correction of filtered position */
                        if (filteredPosition >= tempResolution)
                        {
                            filteredPosition -= tempResolution;
                        }
                        ptrHistory->posJitter = filteredPosition;
                    }
                #endif /* #if (0u != CapSense_POS_JITTER_FILTER_EN) */
            }
            /* Update position data */
            ptrRamWdgt->position[touchIdx] = filteredPosition;
            ptrHistory = (NULL != ptrHistory) ? (ptrHistory + 1) : NULL;
        #endif /* (0u == CapSense_POSITION_FILTER_EN) */
    }
}
#endif /* #if (0u != CapSense_TOTAL_RADIAL_SLIDERS) */


#if (0u != CapSense_CSD_MATRIX_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpProcessCsdMatrix
****************************************************************************//**
*
* \brief
*   Processes the status of the CSD Matrix Button widget.
*
* \details
*   This API processes the status of the CSD Matrix Button widget.
*   It applies the hysteresis and debounce algorithm to each sensor value.
*
*   Then the API analyzes how many row and column sensors are active. If only
*   one per row and one per column, the API considers this as a valid touch and
*   updates the corresponding Data Structure registers with the button id and active
*   row and column sensors.
*
*   If multiple sensors are active in row or column sensors, this API sets
*   the corresponding Data Structure registers to the CapSense_MATRIX_POS_MULTI
*   value which indicates that it is not possible to detect the touched button id.
*
*   This API is expected to be called after each new widget scan. If it is
*   called multiple times for the same scan data, the debounce algorithm
*   will not work properly.
*
* \param currStatus   The current status of the widget sensors.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
* \return Returns the status of the widget's sensors. The row sensors occupy the
*         least significant bits and column sensors - the most significant bits.
*         Bit #0 - corresponds to Row Sensor #0.
*
*******************************************************************************/
uint32 CapSense_DpProcessCsdMatrix(uint32 currStatus, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 snsId;
    uint32 snsMask;
    uint32 newStatus;
    uint32 touchTh;
    uint8 * ptrDebounceCnt;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp;
    CapSense_RAM_WD_CSD_MATRIX_STRUCT *ptrRamWdgt;

    uint32 numActiveRows = 0u;
    uint32 numActiveCols = 0u;

    uint32 activeRow = CapSense_MATRIX_POS_NONE;
    uint32 activeCol = CapSense_MATRIX_POS_NONE;

    ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
    ptrRamWdgt = (CapSense_RAM_WD_CSD_MATRIX_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
    ptrDebounceCnt = ptrFlashWdgt->ptr2DebounceArr;

    /* Go through all widget's sensors */
    for (snsId = 0u; snsId < ptrFlashWdgt->totalNumSns; snsId++)
    {
        /* calculate current sensor mask */
        snsMask = 1Lu << snsId;

        /* Calculate touch threshold taking into account hysteresis and current sensor state. */
        touchTh = (0u == (currStatus & snsMask)) ?
                  ((uint32)ptrRamWdgt->fingerTh + ptrRamWdgt->hysteresis) :
                  ((uint32)ptrRamWdgt->fingerTh - ptrRamWdgt->hysteresis);

        /* Determine new potential sensor state */
        newStatus = (ptrSnsTmp->diff > touchTh) ? snsMask : 0u;

        /* Sensor state changed ? */
        if (newStatus != (currStatus & snsMask))
        {
            /* Decrement debounce counter */
            (*ptrDebounceCnt)--;
        }

        /* This piece of code has two objectives:
         * 1. Reset debounce counter if new sensor state has not changed.
         *    (It is possible that during previous processing cycles sensor state
         *     was changing which caused decrement of debounce counter.)
         * 2. Set new sensor state after successful debounce.
         */
        if ((newStatus == (currStatus & snsMask)) || (0u == (*ptrDebounceCnt)))
        {
            /* Set new sensor status */
            currStatus &= ~snsMask;
            currStatus |= newStatus;

            if (0u == newStatus)
            {
                /* Reset OFF->ON debounce counter */
                *ptrDebounceCnt = ptrRamWdgt->onDebounce;
            }
            else
            {
                /* Reset ON->OFF debounce counter */
                #if (0u != CapSense_OFF_DEBOUNCE_EN)
                    *ptrDebounceCnt = ptrRamWdgt->offDebounce;
                #else
                    *ptrDebounceCnt = DEFAULT_OFF_DEBOUNCE_VALUE;
                #endif /* #if (0u != CapSense_OFF_DEBOUNCE_EN) */
            }
        }

        /* Update information about active row/col sensors */
        if (0u != (currStatus & snsMask))
        {
            if (snsId < ptrFlashWdgt->numCols)
            {
                numActiveCols++;
                activeCol = snsId;
            }
            else
            {
                numActiveRows++;
                activeRow = snsId - ptrFlashWdgt->numCols;
            }
        }

        ptrSnsTmp++;
        ptrDebounceCnt++;
    }

    /* Update row and col registers in data structure */
    ptrRamWdgt->posCol = (numActiveCols > 1u) ? CapSense_MATRIX_POS_MULTI : (uint8)activeCol;
    ptrRamWdgt->posRow = (numActiveRows > 1u) ? CapSense_MATRIX_POS_MULTI : (uint8)activeRow;

    /* If only one row and one column sensors are active update the sensor ID register */
    if ((1u == numActiveRows) && (1u == numActiveCols))
    {
        ptrRamWdgt->posSnsId = CY_LO8((activeRow * ptrFlashWdgt->numCols) + activeCol);
    }
    else
    {
        ptrRamWdgt->posSnsId = CapSense_MATRIX_POS_NONE;
    }

    return currStatus;
}
#endif /* #if (0u != CapSense_CSD_MATRIX_WIDGET_EN) */


#if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpProcessCsdTouchpad
****************************************************************************//**
*
* \brief
*   Processes status of the CSD Touchpad widget. This includes running
*   Centroid algorithm and updating status in the Data Structure registers.
*
* \details
*   Processes status of the CSD Touchpad widget. This includes running
*   Centroid algorithm and updating status in the Data Structure registers.
*   The status is updated in the RAM data structure.
*
* \param currStatus   The current status of the widget sensors.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
* \return Returns the status of the widget's sensors. The row sensors occupy the
*         least significant bits and column sensors - the most significant bits.
*         Bit #0 - corresponds to Row Sensor #0.
*
*******************************************************************************/
uint32 CapSense_DpProcessCsdTouchpad(uint32 currStatus, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 snsCnt;
    uint32 snsMask;
    uint32 touchTh;
    uint32 rowSnsMask;
    uint32 colSnsMask;
    uint32 newStatus = 0Lu;
    uint8 * ptrDebounceCnt;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp;
    CapSense_RAM_WD_CSD_TOUCHPAD_STRUCT *ptrRamWdgt;

    #if (CapSense_ENABLE == CapSense_CENTROID_3X3_CSD_EN)
        uint16 posXTmp;
        uint16 posYTmp;
        uint32 numLocalMax;
    #endif

    #if (CapSense_ENABLE == CapSense_CENTROID_5X5_CSD_EN)
        uint32 i;
        uint16 snsDiff[CapSense_MAX_SENSORS_PER_5X5_TOUCHPAD];
        ADVANCED_CENTROID_CONFIG_STRUCT config5X5;
        ADVANCED_CENTROID_CONTEXT_STRUCT context5X5;
        #if (0u != CapSense_POSITION_FILTER_EN)
            uint16 filteredX;
            uint16 filteredY;
            uint16 curPosX[ADVANCED_CENTROID_MAX_TOUCHES];
        #endif /* (0u != CapSense_POSITION_FILTER_EN) */
    #endif

    #if (0u != CapSense_POSITION_FILTER_EN)
        CapSense_TOUCHPAD_POS_HISTORY_STRUCT *ptrHistory;
    #endif /* (0u != CapSense_POSITION_FILTER_EN) */

    snsCnt = ptrFlashWdgt->totalNumSns;
    ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
    ptrRamWdgt = (CapSense_RAM_WD_CSD_TOUCHPAD_STRUCT *)ptrFlashWdgt->ptr2WdgtRam;
    ptrDebounceCnt = ptrFlashWdgt->ptr2DebounceArr;

    #if (0u != CapSense_POSITION_FILTER_EN)
        ptrHistory = ptrFlashWdgt->ptr2PosHistory;
    #endif /* (0u != CapSense_POSITION_FILTER_EN) */

    /* Build masks for status word of row and column sensors */
    colSnsMask = (1Lu << ptrFlashWdgt->numCols) - 1Lu;
    rowSnsMask = (1Lu << ptrFlashWdgt->numRows) - 1Lu;
    rowSnsMask <<= ptrFlashWdgt->numCols;

    /* Calculate touch threshold based on current touchpad state */
    touchTh = (0Lu == currStatus) ?
              ((uint32)ptrRamWdgt->fingerTh + ptrRamWdgt->hysteresis) :
              ((uint32)ptrRamWdgt->fingerTh - ptrRamWdgt->hysteresis);

    /* Go through all widget's sensors */
    snsMask = 1Lu;
    for (; snsCnt-- > 0Lu;)
    {
        /* Determine new potential sensor state */
        newStatus |= (ptrSnsTmp->diff > touchTh) ? snsMask : 0Lu;
        ptrSnsTmp++;

        snsMask <<= 1u;
    }

    /* If widget is active perform the debounce.
     * Widget is considered as active if at least one sensor is active on both axes.
     */
    if (((newStatus & rowSnsMask) > 0u) && ((newStatus & colSnsMask) > 0u))
    {
        if (0u < (*ptrDebounceCnt))
        {
            /* Decrement debounce counter */
            (*ptrDebounceCnt)--;
        }

        if (0u == (*ptrDebounceCnt))
        {
            /* Update slider sensor state when debounce completes */
            currStatus = newStatus;
        }
    }
    else
    {
        /* If widget is not active: */

        /* Reset debounce counter */
        *ptrDebounceCnt = ptrRamWdgt->onDebounce;

        /* Reset current sensor state */
        currStatus = 0Lu;

        /* Clear centroid position data for 5x5 CSD touchpad */
        #if (CapSense_ENABLE == CapSense_CENTROID_5X5_CSD_EN)
            if (0u != (ptrFlashWdgt->staticConfig & CapSense_CENTROID_5X5_MASK))
            {
                ptrRamWdgt->position.touchNum = 0u;
                for (i = 0u; i < ADVANCED_CENTROID_MAX_TOUCHES; i++)
                {
                    ptrRamWdgt->position.pos[i].x = ADVANCED_CENTROID_POSITION_NONE;
                    ptrRamWdgt->position.pos[i].y = ADVANCED_CENTROID_POSITION_NONE;
                    ptrRamWdgt->position.pos[i].zX = ADVANCED_CENTROID_POSITION_NONE;
                    ptrRamWdgt->position.pos[i].zY = ADVANCED_CENTROID_POSITION_NONE;

                    /* Clear position filter history data if available */
                    #if (0u != CapSense_POSITION_FILTER_EN)
                        if (NULL != ptrHistory)
                        {
                            CapSense_InitPosFiltersDd(&ptrHistory[i], CapSense_TOUCHPAD_POS_NONE,
                                                            CapSense_TOUCHPAD_POS_NONE);
                        }
                    #endif /* (0u != CapSense_POSITION_FILTER_EN) */
                }
            }
        #endif

        /* Clear centroid position data for 3x3 CSD touchpad */
        #if (CapSense_ENABLE == CapSense_CENTROID_3X3_CSD_EN)
            if (0u != (ptrFlashWdgt->staticConfig & CapSense_CENTROID_3X3_MASK))
            {
                ptrRamWdgt->posX = CapSense_TOUCHPAD_POS_NONE;
                ptrRamWdgt->posY = CapSense_TOUCHPAD_POS_NONE;

                /* Clear position filter history data if available */
                #if (0u != CapSense_POSITION_FILTER_EN)
                    if (NULL != ptrHistory)
                    {
                        CapSense_InitPosFiltersDd(ptrHistory, CapSense_TOUCHPAD_POS_NONE,
                                                            CapSense_TOUCHPAD_POS_NONE);
                    }
                #endif /* (0u != CapSense_POSITION_FILTER_EN) */
            }
        #endif
    }

    /* If widget is still active after debounce run the centroid algorithm */
    if (0Lu != currStatus)
    {
        /* 5x5 CSD Touchpad processing */
        #if (CapSense_ENABLE == CapSense_CENTROID_5X5_CSD_EN)
            if (0u != (ptrFlashWdgt->staticConfig & CapSense_CENTROID_5X5_MASK))
            {
                /* Init advanced centroid configuration structure */
                config5X5.fingerTh = (uint16)ptrRamWdgt->fingerTh;
                config5X5.penultimateTh = ptrRamWdgt->edgePenultimateTh;
                config5X5.virtualSnsTh = ptrRamWdgt->edgeVirtualSensorTh;
                config5X5.resolutionX = ptrFlashWdgt->xResolution;
                config5X5.resolutionY = ptrFlashWdgt->yResolution;
                config5X5.crossCouplingTh = ptrRamWdgt->crossCouplingPosTh;
                config5X5.snsCountX = ptrFlashWdgt->numCols;
                config5X5.snsCountY = ptrFlashWdgt->numRows;
                config5X5.edgeCorrectionEn = (uint8)((ptrFlashWdgt->staticConfig & CapSense_EDGE_CORRECTION_MASK) >>
                                            CapSense_EDGE_CORRECTION_SHIFT);
                config5X5.twoFingersEn = (uint8)((ptrFlashWdgt->staticConfig & CapSense_TWO_FINGER_DETECTION_MASK) >>
                                            CapSense_TWO_FINGER_DETECTION_SHIFT);

                /* Collect sensor differences into an array */
                ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
                for (i = 0u; i < ptrFlashWdgt->totalNumSns; i++)
                {
                    snsDiff[i] = ptrSnsTmp->diff;
                    ptrSnsTmp++;
                }

                context5X5.ptrSns = snsDiff;
                context5X5.touch = &ptrRamWdgt->position;

                /* Save current position */
                #if (0u != CapSense_POSITION_FILTER_EN)
                    /* Do it for each detected position */
                    for (i = 0u; i < ADVANCED_CENTROID_MAX_TOUCHES; i++)
                    {
                        curPosX[i] = ptrRamWdgt->position.pos[i].x;
                    }
                #endif /* (0u != CapSense_POSITION_FILTER_EN) */

                /*
                * Calculate Advanced centroid.
                * One of the function arguments is pointer to touch position structure. It is supposed that
                * the function stores into the structure detected touches. If touch is not detected, then
                * corresponding structure fields are filled in by ADVANCED_CENTROID_POSITION_NONE.
                */
                (void)AdvancedCentroid_GetTouchCoordinates(&config5X5, &context5X5);

                /* Now filter the X and Y position data if configured */
                #if (0u != CapSense_POSITION_FILTER_EN)
                    /* Clear filter history for empty slots */
                    for (i = ptrRamWdgt->position.touchNum; i < ADVANCED_CENTROID_MAX_TOUCHES; i++)
                    {
                        if (NULL != ptrHistory)
                        {
                            CapSense_InitPosFiltersDd(&ptrHistory[i], CapSense_TOUCHPAD_POS_NONE,
                                                                    CapSense_TOUCHPAD_POS_NONE);
                        }
                    }
                    /* Filter position for detected touches */
                    for (i = 0u; i < ptrRamWdgt->position.touchNum; i++)
                    {
                        filteredX = ptrRamWdgt->position.pos[i].x;
                        filteredY = ptrRamWdgt->position.pos[i].y;

                        /* If there were no touch initialize the current position with new position data */
                        if (curPosX[i] == CapSense_TOUCHPAD_POS_NONE)
                        {
                            if (NULL != ptrHistory)
                            {
                                CapSense_InitPosFiltersDd(&ptrHistory[i], filteredX, filteredY);
                            }
                        }
                        else
                        {
                            CapSense_RunPosFiltersDd(ptrFlashWdgt, i, i, &filteredX, &filteredY);
                            ptrRamWdgt->position.pos[i].x = filteredX;
                            ptrRamWdgt->position.pos[i].y = filteredY;
                        }
                    }
                #endif /* (0u != CapSense_POSITION_FILTER_EN) */
            }
        #endif

        /* 3x3 CSD Touchpad processing */
        #if (CapSense_ENABLE == CapSense_CENTROID_3X3_CSD_EN)
            if (0u != (ptrFlashWdgt->staticConfig & CapSense_CENTROID_3X3_MASK))
            {
                /* Centroid processing */
                ptrSnsTmp = ptrFlashWdgt->ptr2SnsRam;
                snsCnt = ptrFlashWdgt->totalNumSns;
                touchTh = (uint32)ptrRamWdgt->fingerTh - ptrRamWdgt->hysteresis;

                /* Run local maximum search for Column sensors of touchpad */
                #if (CapSense_ENABLE == CapSense_4PTS_LOCAL_MAX_EN)
                    numLocalMax = CapSense_DpFindLocalMaxSd4pts(ptrFlashWdgt, ptrSnsTmp,
                                                                            (uint32)ptrFlashWdgt->numCols, touchTh);
                #else
                    numLocalMax = CapSense_DpFindLocalMaxSd(ptrSnsTmp, (uint32)ptrFlashWdgt->numCols, touchTh);
                #endif

                CY_ASSERT(0u != numLocalMax); /* There must be at least one local maximum found */

                /* Verify how many local maximums are found on X axis.
                 * The CSD Touchpads support only one finger touch so we should have only one local maximum per axis.
                 * If more are found then there are multiple finger touches and it is not possible to recover their position.
                 */
                if (1u == numLocalMax)
                {
                    /* Run centroid algorithm for the X axis */
                    #if (CapSense_ENABLE == CapSense_TOUCHPAD_MULT_METHOD)
                        (void)CapSense_DpCalcLinearCentroid(&posXTmp, (uint32)ptrFlashWdgt->xCentroidMultiplier,
                                    (uint32)ptrFlashWdgt->numCols, (uint32)(ptrFlashWdgt->xCentroidMultiplier >> 1));
                    #else
                        (void)CapSense_DpCalcLinearCentroid(&posXTmp, (uint32)ptrFlashWdgt->xCentroidMultiplier,
                                    (uint32)ptrFlashWdgt->numCols, 0u);
                    #endif

                    /* Get pointer to Row RAM sensor structure array */
                    ptrSnsTmp += ptrFlashWdgt->numCols;

                    /* Run local maximum search for Row sensors of touchpad */
                    #if (CapSense_ENABLE == CapSense_4PTS_LOCAL_MAX_EN)
                        numLocalMax = CapSense_DpFindLocalMaxSd4pts(ptrFlashWdgt, ptrSnsTmp,
                                                                                (uint32)ptrFlashWdgt->numRows, touchTh);
                    #else
                        numLocalMax = CapSense_DpFindLocalMaxSd(ptrSnsTmp, (uint32)ptrFlashWdgt->numRows, touchTh);
                    #endif

                    CY_ASSERT(0u != numLocalMax); /* There must be at least one local maximum found */

                    /* Verify how many local maximums are found on Y axis.
                     * The CSD Touchpads support only one finger touch so we should have only one local maximum per axis.
                     * If more are found then it is not possible to recover finger position.
                     */
                    if (1u == numLocalMax)
                    {
                        /* Run centroid algorithm for the Y axis */
                        #if (CapSense_ENABLE == CapSense_TOUCHPAD_MULT_METHOD)
                            (void)CapSense_DpCalcLinearCentroid(&posYTmp, (uint32)ptrFlashWdgt->yCentroidMultiplier,
                                        (uint32)ptrFlashWdgt->numRows, (uint32)(ptrFlashWdgt->yCentroidMultiplier >> 1u));
                        #else
                            (void)CapSense_DpCalcLinearCentroid(&posYTmp, (uint32)ptrFlashWdgt->yCentroidMultiplier,
                                        (uint32)ptrFlashWdgt->numRows, 0u);
                        #endif

                        /* Now filter the X and Y position data if needed */
                        #if (0u != CapSense_POSITION_FILTER_EN)
                            if (NULL != ptrHistory)
                            {
                                /* If there were no touch initialize the current position with new position data */
                                if (ptrRamWdgt->posX == CapSense_TOUCHPAD_POS_NONE)
                                {
                                    CapSense_InitPosFiltersDd(ptrHistory, posXTmp, posYTmp);
                                }
                                else
                                {
                                    CapSense_RunPosFiltersDd(ptrFlashWdgt, 0u, 0u, &posXTmp, &posYTmp);
                                }
                            }
                        #endif /* (0u != CapSense_POSITION_FILTER_EN) */

                        ptrRamWdgt->posX = posXTmp;
                        ptrRamWdgt->posY = posYTmp;
                    }
                }
                if (numLocalMax > 1u)
                {
                    /* Clear centroid position data for 3x3 CSD touchpad */
                    ptrRamWdgt->posX = CapSense_TOUCHPAD_POS_NONE;
                    ptrRamWdgt->posY = CapSense_TOUCHPAD_POS_NONE;

                    /* Clear position filter history data if available */
                    #if (0u != CapSense_POSITION_FILTER_EN)
                        if (NULL != ptrHistory)
                        {
                            CapSense_InitPosFiltersDd(ptrHistory, CapSense_TOUCHPAD_POS_NONE, CapSense_TOUCHPAD_POS_NONE);
                        }
                    #endif /* (0u != CapSense_POSITION_FILTER_EN) */
                }
            }
        #endif
    }

    return currStatus;
}
#endif /* #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN) */


#if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_DpProcessCsxTouchpad
****************************************************************************//**
*
* \brief
*   Processes the status of the CSX Touchpad widget.
*   The process involves running the 3x3 centroid algorithm with the finger id tracker.
*
* \details
*   Processes the status of the CSX Touchpad widget.
*   The process involves running the 3x3 centroid algorithm with the finger id tracker.
*   The X/Y coordinates are updated in the RAM data structure.
*
* \param currStatus The current status of the widget sensors.
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
* \return The number of detected touches.
*
*******************************************************************************/
uint32 CapSense_DpProcessCsxTouchpad(uint32 currStatus, CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 touchCount;

    (void)currStatus; /* Unused parameter */

    /* Check if sensors are active and located all local maxima */
    CapSense_DpFindLocalMaxDd(ptrFlashWdgt);

    /* Calculate centroid position for all found local maxima */
    CapSense_DpCalcTouchPadCentroid(ptrFlashWdgt);

    /* Identify all touches and assign them correct ID based on historical data */
    CapSense_DpTouchTracking(ptrFlashWdgt);

    /* Filter the position data and write it into data structure.
     * The returned value tells how many touch positions were written to the Data Structure */
    touchCount = CapSense_DpFilterTouchRecord(ptrFlashWdgt);

    return touchCount;
}
#endif /* #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN) */

#if (0u != CapSense_CSX_MULTIPHASE_TX_EN)
/*******************************************************************************
* Function Name: CapSense_DpDeconvolution
****************************************************************************//**
*
* \brief
*   Runs deconvolution algorithm for the CSX Touchpad widgets with the enabled
*   TX Multiphase scanning.
*
* \details
*   Runs deconvolution algorithm for the CSX Touchpad widgets with the enabled
*   TX Multiphase scanning.
*
*   The deconvolution coefficients are taken from the Flash Widget Object.
*   The deconvolution results are written over the scan data stored in the
*   RAM Sensor objects.
*
* \param ptrFlashWdgt The pointer to the Flash Widget Object.
*
*******************************************************************************/
void CapSense_DpDeconvolution(CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt)
{
    uint32 stNum;
    uint32 stInd;
    uint32 txNum;
    uint32 rxNum;
    uint32 freqNum;
    int32 accRaw;

    uint32 txCount = ptrFlashWdgt->numRows;
    uint32 rxCount = ptrFlashWdgt->numCols;
    const int16 *ptrSolveTable = ptrFlashWdgt->ptr2TxMultiphaseCoeff;
    CapSense_RAM_SNS_STRUCT *ptrSnsTmp = (CapSense_RAM_SNS_STRUCT *)ptrFlashWdgt->ptr2SnsRam;

    /* Run deconvolution algorithm for each touchpad column */
    for (rxNum = 0u; rxNum < rxCount; rxNum++)
    {
        /* for each scan frequency */
        for (freqNum = 0u; freqNum < CapSense_NUM_SCAN_FREQS; freqNum++)
        {
            /* for each RX/TX intersection */
            for (txNum = 0u; txNum < txCount; txNum++)
            {
                accRaw = 0u;

                for (stNum = 0u; stNum < txCount; stNum++)
                {
                    stInd = txCount - txNum + stNum;

                    if (stInd > (txCount - 1u))
                    {
                        stInd = stNum - txNum;
                    }

                    accRaw += (int32)ptrSnsTmp[rxNum * txCount + stNum].raw[freqNum] * (int32)ptrSolveTable[stInd];
                }

                /* Save calculated raw value to the temporary buffer. It is not possible at
                 * this stage to save it into the sensor object by overwriting the scan data
                 * because the scan data is used to calculate raw values of other sensor objects
                 */
                deconvRowData[txNum] = (accRaw >> TEMP_BUFFER_MSB_SHIFT);
            }

            for (txNum = 0u; txNum < txCount; txNum++)
            {
                ptrSnsTmp[rxNum * txCount + txNum].raw[freqNum] = deconvRowData[txNum];
            }
        }
    }
}

#endif /* #if (0u != CapSense_CSX_MULTIPHASE_TX_EN) */


/* [] END OF FILE */
