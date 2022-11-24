#pragma include once

#include "cy_sysclk.h"
#include "cy_ble_clk.h"

cy_en_ble_eco_status_t BLE_HAL_EcoSetFrequency(cy_en_ble_bless_eco_freq_t ecoFreq, cy_en_ble_bless_sys_eco_clk_div_t ecoSysDiv);
cy_en_ble_eco_status_t BLE_HAL_EcoSetTrim(uint8_t trim, uint8_t startUpTime);