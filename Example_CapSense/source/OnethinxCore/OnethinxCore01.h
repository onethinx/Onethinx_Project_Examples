/********************************************************************************
 *    ___             _   _     _			
 *   / _ \ _ __   ___| |_| |__ (_)_ __ __  __
 *  | | | | '_ \ / _ \ __| '_ \| | '_ \\ \/ /
 *  | |_| | | | |  __/ |_| | | | | | | |>  < 
 *   \___/|_| |_|\___|\__|_| |_|_|_| |_/_/\_\
 *
 ********************************************************************************
 *
 * Copyright (c) 2019 Onethinx BV <info@onethinx.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ********************************************************************************
 *
 * Created by: Rolf Nooteboom
 *
 * Library to use with the Onethinx Core LoRaWAN module.
 * 
 * For a description please see:
 * https://github.com/onethinx/OnethinxCoreAPI
 *
 ********************************************************************************
 *
 * Core revisions:
 * 0x000000AA	Fix DevAddr in OnethinxCore01.h and reserved byte amount fixed
 * 0x000000AB	Implemented M0+ reset: failed debugging
 * 0x000000AC	Implemented breakCurrentFunction
 * 0x000000AD	Added configurations pointer validity check
 * 0x000000AE	Transmit power issue solved (device was always sending at TX_MAX)
 * 0x000000B0	Added US sub-bands, enhanced RX1 + RX2 window accuracy, fixed CM0p SRAM memory mapping, 
 * 0x000000B1	Added LP functionality, added FlashWrites, added possibility to read Dev Address
 * 0x000000B2	Cleared RX-timeout flag if no downlink response is expected, added DevEUI & build info, fixed high power consumption glitches at wakeup, Added LowPowerDebug, Improved active-power consumption
 * 0x000000B3	Fix Join accept on SF12 in RX1 window
 * 0x000000B4	Fix WakeUp pin in Sleepmode, FlashRead fix for row !=0
 * 0x000000B5	Fix Flashwrites after sleep
 * 0x000000B7	Restructured stack core, Capsense confguration fix, Add LoRa<>LoRa functionality, Add MAC Cmd LinkADR, Fixed RX window timing, Fix confirmed downlink reply, Fix US join implementation, Add Low Power Join, Stability fixes
 * 0x000000B8	Restructured stack core, added low-power idle/join
 * 0x000000B9	Fix ADR
 * 0x000000BA	Unlock functions to use Port 6 & 7 for Capsense and SDW IOs
 * 0x000000BB	Fix TX timeout setting for EU SF12/125 (payload > 27 bytes), Fix Flashwrites
 * 0x000000BC	Fix LoRa to LoRa Communication
 * 0x000000BD	Added FSK modulation
 * 0x000000BE	Fixed small timing issues, MAC commands, restructured stack etc.
 * 0x000000BF   Changed RX timing window to SysTick timer (32MHz instead of 32KHz)
 * 0x000000C0   Fix SX126x Wakeup settings from coldstart (used when BleEcoON = true), added MAC save functionality to resume LoRaWAN operations after hibernate, added RX boost functionality, added Set & Get timestamp RTC function
 * 0x000000C1   Fix RX window timing after deepsleep
 * 
 ********************************************************************************/

#ifndef ONETHINXCORE01_H
#define ONETHINXCORE01_H	
 
#include <stdint.h>
#include <stdbool.h>

/* Do not modify. If stack version does not match, implement correct OnethinxCore.h & .c API drivers from the Onethinx Github page */
#define minimumVersion 	0x000000C0
#define maximumVersion 	0x000000C1

typedef struct arr8b_t  { uint8_t bytes[8];  } arr8b_t;
typedef struct arr16b_t { uint8_t bytes[16]; } arr16b_t;

/* Do not modify. If code { 0, 0, 0, 0, 0, 0, 0, 0 } is recognized, it is replaced with manufacturing DevEUI. This manufacturing DevEUI can be requested using LoRaWAN_GetInfo */
#define thisDevEUI	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

typedef enum {
	coreFunction_Init					= 0x01,
	coreFunction_Reset					= 0x02,
	coreFunction_GetInfo				= 0x03,
	coreFunction_LW_join				= 0x20,
	coreFunction_LW_send				= 0x30,
	coreFunction_LW_sendMac				= 0x31,
	coreFunction_LW_getRXdata			= 0x40,
	coreFunction_LW_sleep				= 0x50,
	coreFunction_LW_setDateTime			= 0x60,
	coreFunction_LW_getDateTime			= 0x61,
	coreFunction_LW_flashRead			= 0x70,
	coreFunction_LW_flashWrite			= 0x71,
} coreFunctions_e;

typedef struct __attribute__((packed, aligned(4)))
{
	uint8_t							clientId;
	uint8_t							userCode;
	uint16_t						intrMask;
	volatile void *					coreArgumentsPtr;
} ipc_msg_t;

typedef struct __attribute__((packed, aligned(4)))
{
	ipc_msg_t	 forCM0;
	ipc_msg_t	 fromCM0;
} ipc_msgs_t;

 typedef enum  {
	ABP_10x_key		 			= 0x01,
	OTAA_10x_key	 			= 0x02,
	OTAA_11x_key	 			= 0x03,
	PreStored_key				= 0xF0,
	UserStored_key				= 0xF1,
} keyType_e;

typedef struct __attribute__ ((__packed__)) {
	keyType_e					KeyType;
	uint8_t						KeyIndex;
} StoredKeys_t;

typedef struct __attribute__ ((__packed__))  {
	arr8b_t						DevEui;
	arr8b_t						AppEui;
	arr16b_t					AppKey;
} OTAA_10x_t;
	
typedef struct __attribute__ ((__packed__))  {
	arr8b_t						DevEui;
	arr8b_t						JoinEui;
	arr16b_t					AppKey;
	arr16b_t					NwkKey;
} OTAA_11x_t;

typedef struct __attribute__ ((__packed__)) {
	arr8b_t						DevEui;
	uint32_t			   		DevAddr;
	arr16b_t					NwkSkey;
	arr16b_t					AppSkey;
} ABP_10x_t;
		
typedef struct  __attribute__ ((__packed__))  {
	keyType_e					KeyType 		: 8;
	uint8_t						reserved		: 7;
	bool						PublicNetwork	: 1;
	union {
		uint8_t					totalbytes[64];
		StoredKeys_t			StoredKeys;
		OTAA_10x_t				OTAA_10x;
		OTAA_11x_t				OTAA_11x;
		ABP_10x_t				ABP_10x;
	};
} LoRaWAN_keys_t;

typedef enum {
    DR_0						= 0x00,		//!< EU: SF12 125KHz, US: SF10 125KHz
    DR_1						= 0x01,		//!< EU: SF11 125KHz, US: SF9 125KHz
    DR_2						= 0x02,		//!< EU: SF10 125KHz, US: SF8 125KHz
    DR_3						= 0x03,		//!< EU: SF9 125KHz, US: SF7 125KHz
    DR_4						= 0x04,		//!< EU: SF8 125KHz, US: SF8 500KHz
    DR_5						= 0x05,		//!< EU: SF7 125KHz
    DR_6						= 0x06,		//!< EU: SF7 250KHz
    DR_7						= 0x07,		//!< EU: FSK 50kbps
    DR_8						= 0x08,		//!< US: SF12 500KHz (downlinks only)
    DR_9						= 0x09,		//!< US: SF11 500KHz (downlinks only)
    DR_10						= 0x0A,		//!< US: SF10 500KHz (downlinks only)
    DR_11						= 0x0B,		//!< US: SF9 500KHz (downlinks only)
    DR_12						= 0x0C,		//!< US: SF8 500KHz (downlinks only)
    DR_13						= 0x0D,		//!< US: SF7 500KHz (downlinks only)
    DR_ADR						= 0xF0,		//!< ADR (Adaptive DataRate and Power Setting by LoRaWAN network)
	DR_AUTO						= 0xF1,		//!< Automatic DataRate (during joining)
} Radio_DataRate_e;

typedef enum {
	PWR_ADR						= 0xF0,		//!< ADR (ADR MAX PWR, same as PWR_MAX)
	PWR_MAX						= 0x00,		//!<  15dBm for SX1261,  22dBm for SX1262
	PWR_ATT_2dB					= 0x01,		//!<  14dBm for SX1261,  20dBm for SX1262
	PWR_ATT_4dB					= 0x02,		//!<  12dBm for SX1261,  18dBm for SX1262
	PWR_ATT_6dB					= 0x03,		//!<  10dBm for SX1261,  16dBm for SX1262
	PWR_ATT_8dB					= 0x04,		//!<   8dBm for SX1261,  14dBm for SX1262
	PWR_ATT_10dB				= 0x05,		//!<   6dBm for SX1261,  12dBm for SX1262
	PWR_ATT_12dB				= 0x06,		//!<   4dBm for SX1261,  10dBm for SX1262
	PWR_ATT_14dB				= 0x07,		//!<   2dBm for SX1261,   8dBm for SX1262
	PWR_ATT_16dB				= 0x08,		//!<   0dBm for SX1261,   6dBm for SX1262
	PWR_ATT_18dB				= 0x09,		//!<  -2dBm for SX1261,   4dBm for SX1262
	PWR_ATT_20dB				= 0x0A,		//!<  -4dBm for SX1261,   2dBm for SX1262
	PWR_ATT_22dB				= 0x0B,		//!<  -6dBm for SX1261,   0dBm for SX1262
	PWR_ATT_24dB				= 0x0C,		//!<  -8dBm for SX1261,  -2dBm for SX1262
	PWR_ATT_26dB				= 0x0D,		//!< -10dBm for SX1261,  -4dBm for SX1262
	PWR_ATT_28dB				= 0x0E,		//!< -12dBm for SX1261,  -6dBm for SX1262
	PWR_ATT_30dB				= 0x0F,		//!< -14dBm for SX1261,  -8dBm for SX1262
    PWR_ATT_32dB				= 0x10,		//!< -16dBm for SX1261,  -9dBm for SX1262
	PWR_MIN						= 0x11,		//!< -17dBm for SX1261,  -9dBm for SX1262
} Radio_TXpower_e;

typedef enum {
	US_SUB_BAND_1			    = 0,
	US_SUB_BAND_2			    = 1,
	US_SUB_BAND_3			    = 2,
	US_SUB_BAND_4			    = 3,
	US_SUB_BAND_5			    = 4,
	US_SUB_BAND_6			    = 5,
	US_SUB_BAND_7			    = 6,
	US_SUB_BAND_8			    = 7,
    US_SUB_BAND_NONE		    = 14,
    US_SUB_BANDS_ALL   		    = 15,
    EU_SUB_BANDS_DEFAULT        = 0,
	EU_SUB_BAND1_ON		        = 1,
	EU_SUB_BAND1_OFF		    = 2,
	EU_SUB_BAND2_ON			    = 3,
	EU_SUB_BAND2_OFF		    = 4
} Radio_SubBands_e;

typedef enum  {
	M0_Active		 			= 0x0,			//!< Keep M0+ active during system idle
	M0_Sleep		 			= 0x1,			//!< Put M0+ in Sleep mode during system idle
	M0_DeepSleep	 			= 0x2,			//!< Put M0+ in DeepSleep mode during system idle
} IdleMode_e;

typedef enum  {
	M4_NoWait			 		= 0x0,			//!< Do not wait till stack finished
	M4_WaitActive		 		= 0x1,			//!< Wait while stack busy, M4 stays in Active mode
	M4_WaitSleep		 		= 0x2,			//!< M4 goes into Sleep while is stack busy
	M4_WaitDeepSleep	 		= 0x3,			//!< M4 goes into DeepSleep while is stack busy
} WaitMode_e;

typedef union {
	struct  __attribute__ ((__packed__)) {
		struct  __attribute__ ((__packed__)) {
			bool					Confirmed			: 1;
			uint8_t										: 7;
			Radio_DataRate_e		DataRate			: 8;
			Radio_TXpower_e			Power				: 8;
			uint8_t					FPort				: 8;
		} TX;
		struct  __attribute__ ((__packed__)) {
			LoRaWAN_keys_t *		KeysPtr;
			Radio_DataRate_e		DataRate			: 8;			//!< Not used for US version (defined by LoRaWAN spec)
			Radio_TXpower_e			Power				: 8;
			uint8_t					MAXTries			: 8;
            uint8_t					SubBand_1st    		: 4;
            uint8_t					SubBand_2nd    		: 4;
		} Join;
		struct  __attribute__ ((__packed__)) {
			struct {
				IdleMode_e				Mode			: 2;			/**< Set M0 Idle Mode to M0_Active, M0_Sleep or M0_DeepSleep */
				bool 					BleEcoON	  	: 1;			/**< Leaves BLE ECO ON during idle. Consumes additional power, enable only when ECO/BLE functionality is needed */
				bool 					DebugON			: 1;			/**< Leaves Debug Port active during idle. Consumes additional power, enable only for debugging purposes */
				uint8_t									: 4;			/**< Reserved */
			} Idle;
		} System;
		struct  __attribute__ ((__packed__)) {
			bool					Boost    			: 1;			/**< Increases RX sensitivity at the cost of 0.6mA in receive mode */
			uint8_t										: 7;
		} RX;
	};
	uint8_t reserved[32];
} coreConfiguration_t;

typedef enum __attribute__ ((__packed__)) {
	param_OK					= 0x00,
	param_Invalid				= 0x01,
	param_OutOfRange			= 0x02,
	param_UndefinedError		= 0xFA,
} paramErrors_e;

typedef enum __attribute__ ((__packed__)) {
	radio_OK					= 0x00,
	radio_BusyError				= 0x01,		//!< Radio Busy Error
	radio_BuckStartError		= 0x02,		//!< Buck converter failed to start
	radio_XoscStartError		= 0x03,		//!< XOSC failed to start
	radio_Rc13mCalibError		= 0x04,		//!< RC 13MHz oscillator calibration failed
	radio_Rc64kCalibError		= 0x05,		//!< RC 64kHz oscillator calibration failed
	radio_PllCalibError			= 0x06,		//!< PLL calibration failed
	radio_PllLockError			= 0x07,		//!< PLL lock failed
	radio_ImgCalibError			= 0x08,		//!< Image calibration failed
	radio_AdcCalibError			= 0x09,		//!< ADC calibration failed
	radio_PaRampError			= 0x0A,		//!< PA ramp failed
	radio_InvalidFrequency		= 0x0B,		//!< Invalid frequency set
	radio_UndefinedError		= 0xFA
} radioErrors_e;

typedef enum __attribute__ ((__packed__)) {
	mac_OK						= 0x00,
	mac_BusyError				= 0x01,
	mac_NotJoinedError			= 0x02,
	mac_ChannelsOccupiedError	= 0x03,
	mac_UnrecognizedKeyType		= 0x04,
    mac_EmptyPayload_Error 		= 0x05,
	mac_RXheader_Error 			= 0x06,
	mac_RX_MIC_Error 			= 0x07,
	mac_RX_InvalidDevAddr 		= 0x08,
	mac_InvalidPacket_Error 	= 0x09,
	mac_RXtimeout_Error 		= 0x0A,
	mac_CRC_Error       		= 0x0B,
	mac_FCNT_Error       		= 0x0C,
	mac_Confirmation_Error 		= 0x0D,
	mac_PayloadSize_Error 		= 0x0E,
	mac_UndefinedError			= 0xFA,
} macErrors_e;

typedef enum __attribute__ ((__packed__)) {
	system_OK					= 0x00,
	system_BusyError			= 0x01,
	system_NotStarted			= 0x02,
	system_IPCError				= 0x03,
	system_FlashWriteError		= 0x04,
	system_VersionMatchError	= 0x05,
	system_UndefinedError	   	= 0xFA
} systemErrors_e ;

typedef union {
	struct
	{
		paramErrors_e 			errorStatus 			: 8;	//!< Parameter Errors
		uint8_t											: 8;	//!< reserved
	};
	uint8_t reserved[16];
} parameterStatus_t;

typedef union {
	struct
	{
		radioErrors_e 			errorStatus 			: 8;	//!< Radio Errors
		uint8_t							 				: 6;	//!< reserved
		bool 					isConfigured			: 1;	//!< The Radio is configured? (implemented since Stack version 0x000000B5)
		bool 					isBusy					: 1;	//!< Radio is busy?
	};
	uint8_t reserved[16];
} radioStatus_t;

typedef union {
	struct
	{
		macErrors_e 			errorStatus  			: 8;	//!< MAC Errors
		uint8_t		 			bytesToRead  			: 8;	//!< Total bytes in Receive buffer
		uint8_t							 				: 1;	//!< reserved
		bool 					MACcmdReceived 			: 1;	//!< The MAC received a MAC command (implemented since Stack version 0x000000BC)
		bool 					confDown     			: 1;	//!< Is a confirmed downlink received? (implemented since Stack version 0x000000B5)
		bool 					isConfigured			: 1;	//!< The MAC is configured?
		bool 					messageReceived			: 1;	//!< The MAC received a message
		bool 					isJoined 				: 1;	//!< Is Device Joined?
		bool 					isBusy					: 1;	//!< Is the MAC busy?
		bool 					isPublicNetwork			: 1;	//!< True for LoRaWAN Public Network
		uint32_t				devAddr;						//!< The Device address received when joining the network
		uint8_t 				Margin;							//!< The Demodulation Margin (in dB above the demodulation floor) for last LinkCheckReq command (implemented since Stack version 0x000000BC)
		uint8_t 				GwCnt;							//!< The number of gateways that successfully received the last LinkCheckReq command (implemented since Stack version 0x000000BC)
		uint8_t 				fPort;							//!< The port number of the last received packet (implemented since Stack version 0x000000BC)
	};
	uint8_t reserved[16];
}  macStatus_t;

typedef union {
	struct
	{
		uint32_t				version;					  //!< The version of the stack
		systemErrors_e 			errorStatus  			: 8;  //!< System Errors
		uint8_t					batteryLevel;				  //!< The battery level for the DevStatusAns command (network check on battery level) to be set optionally by the user (0 = external, 1 = minimum battery, 254 = maximum battery, 255 = not implemented)
		uint8_t							 				: 5;  //!< reserved
		bool 					breakCurrentFunction	: 1;  //!< Break current execution
		bool 					isStarted  				: 1;  //!< System is started?
		bool 					isBusy  				: 1;  //!< System is busy?
		bool 					isSleeping 				: 1;  //!< System is sleeping?
	};
	uint8_t reserved[16];
}  systemStatus_t;

typedef struct {
	parameterStatus_t		parameters;
	radioStatus_t			radio;
	macStatus_t				mac;
	systemStatus_t			system;
} coreStatus_t;

typedef union {
	uint32_t					errorValue;
	struct  __attribute__ ((__packed__)) {
		paramErrors_e			paramErrors			: 8;
		radioErrors_e			radioErrors			: 8;
		macErrors_e				macErrors			: 8;
		systemErrors_e			systemErrors		: 8;
	};
} errorStatus_t;

typedef volatile struct {
	coreFunctions_e					function;
	coreConfiguration_t *			configurationPtr;
	coreStatus_t					status;
	uint32_t  						arg1;
	uint32_t	 					arg2;
	uint32_t 						arg3;
	uint32_t	 					arg4;
} coreArguments_t;

#define errorStatus_NoError					0

typedef union
{
    uint32_t value;
    struct {
        uint32_t		Second				: 6;
        uint32_t		Minute				: 6;
        uint32_t		Hour				: 5;			/**< Hour in 24h mode */
        uint32_t		DayOfMonth			: 5;			/**< First day of month = 0 */
        uint32_t		Month				: 4;			/**< First month = 0 */
        uint32_t		Year				: 6;			/**< Year 2000 = 0 */
    };
} dateTime_t;

typedef struct {
	bool enabled					: 1;
	bool risingEdge					: 1;
	bool internalPullUpDown			: 1;
	uint32_t						: 21;
} wakeUpPin_t;

typedef struct {
	bool enabled				: 1;
	bool isDateTime				: 1;
	uint8_t						: 6;
	union {
		dateTime_t   		dateTime;
		struct {
			uint8_t			days;					/**< Delay in days */
			uint8_t			hours;					/**< Delay in hours */
			uint8_t			minutes;				/**< Delay in minutes */
			uint8_t			seconds;				/**< Delay in seconds */
		};
	};
} wakeUpTime_t;

typedef enum  {
	modeSleep		 			= 0x1,
	modeDeepSleep	 			= 0x2,
	modeHibernate	 			= 0x3,			/**< Hibernates, WCO & RTC are shut down */
	modeHibernate_RtcOn			= 0x4,			/**< Hibernates, RTC will keep running (consumes approx. an additional .7uA) */
	modeHibernate_MACsave		= 0x5,			/**< Hibernates, RTC on, LoRaWAN MAC is saved to EEPROM and restored at wakeup. EEPROM USAGE = 0x1400.7800 - 0x1400.7FFF*/
} sleepMode_e;

typedef enum  {
	coresM0p					= 1,
	coresM4	 					= 2,
	coresBoth	 				= 3
} sleepCores_e;

typedef enum  {
	MACcmd_None						= 0,
	LinkCheckReq					= 1,
	DeviceTimeReq	 				= 2,
	LinkCheck_DeviceTimeReq	 		= 3
} MACcmd_e;

#define 	wakeUpPinHigh(pullDown)							((wakeUpPin_t) { .enabled = true, .risingEdge = true, .internalPullUpDown =  pullDown})
#define 	wakeUpPinLow(pullUp) 							((wakeUpPin_t) { .enabled = true, .risingEdge = false, .internalPullUpDown =  pullUp})
#define 	wakeUpPinOff      								((wakeUpPin_t) { .enabled = false })
#define 	wakeUpTimestamp(_dateTime)     					((wakeUpTime_t) { .enabled = true, .isDateTime = true, .dateTime = _dateTime })
#define 	wakeUpDelay(_days, _hours, _minutes, _seconds)  ((wakeUpTime_t) { .enabled = true, .isDateTime = false, .days = _days, .hours = _hours, .minutes = _minutes, .seconds = _seconds })
#define 	wakeUpTimeOff				     				((wakeUpTime_t) { .enabled = false })

/** Sleep configuration  */
/** Make sure to initialize all members, best practise is to use examples */
typedef struct  __attribute__ ((__packed__))
{
	wakeUpPin_t		wakeUpPin;							/**< S24 bits  */
	wakeUpTime_t	wakeUpTime;							/**< S40 bits  */
	sleepMode_e		sleepMode			: 3;			/**< Set sleepmode to Sleep, DeepSleep or Hibernate */
	bool 			BleEcoON	 	 	: 1;			/**< Leaves BLE ECO ON during sleep. Consumes additional power, enable only when ECO/BLE functionality is needed during sleep */
	bool 			DebugON				: 1;			/**< Leaves Debug Port active during idle. Consumes additional power, enable only for debugging purposes */
	sleepCores_e	sleepCores			: 3;
	uint32_t							: 32;			/**< Reserved */
} sleepConfig_t;

typedef enum  {
	stack_AS								= 1,			/**< Region Australia 923 MHz*/
	stack_AU 								= 2,			/**< Region Japan 915-928 MHz */
	stack_CN_L	 							= 3,			/**< Region China 470-510 MHz */
	stack_CN_H	 							= 4,			/**< Region China 779-787 MHz */
	stack_EU_L								= 5,			/**< Region Europe 433 MHz */
	stack_EU_H								= 6,			/**< Region Europe 863-870 MHz*/
	stack_IN								= 7,			/**< Region India 865-867 MHz */
	stack_KR								= 8,			/**< Region Korea 920-923 MHz*/
	stack_US								= 9,			/**< Region North America 902-928 MHz */
	stack_RU								= 10			/**< Region Russia 864-870 MHz*/
} stackRegion_e;

typedef struct  __attribute__ ((__packed__))
{
	uint32_t		buildYear				: 6;			/**< core firmware Year of build */
	uint32_t		buildMonth				: 4;			/**< core firmware Month of build */
	uint32_t		buildDayOfMonth			: 5;			/**< core firmware Day of build */
	uint32_t		buildHour				: 5;			/**< core firmware Hour of build (24h mode) */
	uint32_t		buildMinute				: 6;			/**< core firmware Minute of build */
	uint32_t		buildSecond				: 6;			/**< core firmware Second of build */
	uint32_t 		buildNumber;							/**< core firmware incremental build number */
	uint8_t			devEUI[8];								/**< core devEUI */
	char			buildType;								/**< core firmware build type */
	stackRegion_e 	stackRegion				:8;				/**< core firmware stack region */
	char		 	stackOption;							/**< core firmware stack option: 'S'ecure, 'P'SA, 'C'onfigurable */
	char			stackStage;								/**< core firmware lifecycle stage: 'a' pre-alpha, 'A' Alpha, 'b' perpetual beta, 'B' Beta, 'r' release candidate, 'R' Release */
	char			codeName[16];							/**< core firmware code name */
} coreInfo_t;
	
coreStatus_t				LoRaWAN_Reset(void);
coreStatus_t		 		LoRaWAN_Init(coreConfiguration_t * coreConfigurationPtr);
coreStatus_t				LoRaWAN_GetInfo(coreInfo_t * coreInfo);
coreStatus_t				LoRaWAN_Join(WaitMode_e waitMode);
coreStatus_t				LoRaWAN_FlashRead(uint8_t* buffer, uint8_t block, uint8_t length);
coreStatus_t				LoRaWAN_FlashWrite(uint8_t* buffer, uint8_t block, uint8_t length);
coreStatus_t 				LoRaWAN_SetDateTime(dateTime_t* dt);
coreStatus_t 				LoRaWAN_GetDateTime(dateTime_t* dt);
coreStatus_t 				LoRaWAN_GetRXdata(uint8_t * RXdata, uint8_t length);
coreStatus_t 				LoRaWAN_Send(uint8_t* buffer, uint8_t length, WaitMode_e waitMode);
coreStatus_t 				LoRaWAN_SendMac(uint8_t* buffer, uint8_t length, WaitMode_e waitMode, MACcmd_e MACcmd);
coreStatus_t 				LoRaWAN_Sleep(sleepConfig_t * sleepConfig);
coreStatus_t 				LoRaWAN_GetStatus();
errorStatus_t 				LoRaWAN_GetError();

#endif /* ONETHINXCORE01_H */