#ifndef ONETHINXEXT01_H
#define ONETHINXEXT01_H	
 
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	coreFunction_L_RX 					= 0x80,
	coreFunction_L_TX 					= 0x81,
    coreFunction_Debug                  = 0xA3,
    coreFunction_Unlock                 = 0xA4,
} coreFunctionsExt_e;

/*!
 * ============================================================================
 * Public Radio Settings
 * ============================================================================
 */

/*!
 * \brief Represents the possible packet type (i.e. modem) used
 */
typedef enum
{
    PACKET_TYPE_GFSK                        = 0x00,
    PACKET_TYPE_LORA                        = 0x01,
    PACKET_TYPE_NONE                        = 0x0F,
} RadioPacketTypes_e;


/* ==== The LoRa Radio Settings  ====  */
 
/*!
 * \brief Represents the possible spreading factor values in LoRa packet types
 */
typedef enum
{
    LORA_SF5                                = 0x05,
    LORA_SF6                                = 0x06,
    LORA_SF7                                = 0x07,
    LORA_SF8                                = 0x08,
    LORA_SF9                                = 0x09,
    LORA_SF10                               = 0x0A,
    LORA_SF11                               = 0x0B,
    LORA_SF12                               = 0x0C,
} RadioLoRaSpreadingFactors_e;

/*!
 * \brief Represents the bandwidth values for LoRa packet type
 */
typedef enum
{
    LORA_BW_500                             = 6,
    LORA_BW_250                             = 5,
    LORA_BW_125                             = 4,
    LORA_BW_062                             = 3,
    LORA_BW_041                             = 10,
    LORA_BW_031                             = 2,
    LORA_BW_020                             = 9,
    LORA_BW_015                             = 1,
    LORA_BW_010                             = 8,
    LORA_BW_007                             = 0,
} RadioLoRaBandwidths_e;

/*!
 * \brief Represents the coding rate values for LoRa packet type
 */
typedef enum
{
    LORA_CR_4_5                             = 0x01,
    LORA_CR_4_6                             = 0x02,
    LORA_CR_4_7                             = 0x03,
    LORA_CR_4_8                             = 0x04,
} RadioLoRaCodingRates_e;

typedef enum
{
    LORA_LOWDATARATEOPTIMIZE_OFF            = 0x00,
    LORA_LOWDATARATEOPTIMIZE_ON             = 0x01
} RadioLoraLowDataRateOptimize_e;

/*!
 * \brief Holds the Radio lengths mode for the LoRa packet type
 */
typedef enum
{
    LORA_PACKET_VARIABLE_LENGTH             = 0x00,         //!< The packet is on variable size, header included
    LORA_PACKET_FIXED_LENGTH                = 0x01,         //!< The packet is known on both sides, no header included in the packet
    LORA_PACKET_EXPLICIT                    = LORA_PACKET_VARIABLE_LENGTH,
    LORA_PACKET_IMPLICIT                    = LORA_PACKET_FIXED_LENGTH,
} RadioLoRaHeaderType_e;

/*!
 * \brief Represents the CRC mode for LoRa packet type
 */
typedef enum
{
    LORA_CRC_ON                             = 0x01,         //!< CRC activated
    LORA_CRC_OFF                            = 0x00,         //!< CRC not used
} RadioLoRaCrcMode_e;

/*!
 * \brief Represents the IQ mode for LoRa packet type
 */
typedef enum
{
    LORA_IQ_NORMAL                          = 0x00,
    LORA_IQ_INVERTED                        = 0x01,
} RadioLoRaIQMode_e;

typedef enum {
	LORA_MAC_PRIVATE_SYNCWORD               = 0x1424,
	LORA_MAC_PUBLIC_SYNCWORD                = 0x3444
} LoRaSyncWord_e;

typedef struct {
    RadioLoRaSpreadingFactors_e			SF                      : 8;
	RadioLoRaBandwidths_e				BW                      : 8;
	RadioLoRaCodingRates_e				CR                      : 8;
    RadioLoraLowDataRateOptimize_e      LowDataRateOptimize     : 8;
 }  LoRaModulationParams_t;

typedef struct {
    uint16_t                            PreambleLength;
    RadioLoRaHeaderType_e               HeaderType              : 8;
    uint8_t                             PayloadSize;                    //!< Size of the payload (in bytes) to transmit or maximum size of the payload that the receiver can accept.
    RadioLoRaCrcMode_e                  CRCmode                 : 8;
    RadioLoRaIQMode_e                   IQmode                  : 8;
    uint16_t                            SyncWord;                       //!< The SX126x LoRa syncword setting
}  LoRaPacketParams_t;

/* ==== The FSK Radio Settings  ==== */

/*!
 * \brief Represents the modulation shaping parameter
 */
typedef enum
{
    MOD_SHAPING_OFF                         = 0x00,
    MOD_SHAPING_G_BT_03                     = 0x08,
    MOD_SHAPING_G_BT_05                     = 0x09,
    MOD_SHAPING_G_BT_07                     = 0x0A,
    MOD_SHAPING_G_BT_1                      = 0x0B,
} RadioFSKModShapings_e;

/*!
 * \brief Represents the modulation shaping parameter
 */
typedef enum
{
    RX_BW_4800                              = 0x1F,
    RX_BW_5800                              = 0x17,
    RX_BW_7300                              = 0x0F,
    RX_BW_9700                              = 0x1E,
    RX_BW_11700                             = 0x16,
    RX_BW_14600                             = 0x0E,
    RX_BW_19500                             = 0x1D,
    RX_BW_23400                             = 0x15,
    RX_BW_29300                             = 0x0D,
    RX_BW_39000                             = 0x1C,
    RX_BW_46900                             = 0x14,
    RX_BW_58600                             = 0x0C,
    RX_BW_78200                             = 0x1B,
    RX_BW_93800                             = 0x13,
    RX_BW_117300                            = 0x0B,
    RX_BW_156200                            = 0x1A,
    RX_BW_187200                            = 0x12,
    RX_BW_234300                            = 0x0A,
    RX_BW_312000                            = 0x19,
    RX_BW_373600                            = 0x11,
    RX_BW_467000                            = 0x09,
} RadioFSKRxBandwidth_e;

/*!
 * \brief Represents the modulation shaping parameter
 */
// Values can be added with the formula: BR = 1024000000 / BitRate 
typedef enum
{
    BITRATE_25K                             = 40960,
    BITRATE_50K                             = 20480,
    BITRATE_100K                            = 10240,
    INTERNAL_BR_MAX                         = 0xFFFFFF
} RadioFSKRBitrate_e;

/*!
 * \brief Represents the modulation shaping parameter
 */
// Values can be added with the formula: Fdev = (Frequency Deviation * 2^25) / Fxtal            Fdev = Frequency Deviation * 1.048576
typedef enum
{
    FDEV_SSB_25K                            = 26214,
    FDEV_DSB_50K                            = 26214,
    FDEV_SSB_30K                            = 31457,
    FDEV_DSB_60K                            = 31457,
    FDEV_SSB_50K                            = 52429,
    FDEV_DSB_100K                           = 52429,
    INTERNAL_DEV_MAX                        = 0xFFFFFF
} RadioFSKRDeviation_e;


/*!
 * \brief Represents the preamble length used to detect the packet on Rx side
 */
typedef enum
{
    RADIO_PREAMBLE_DETECTOR_OFF             = 0x00,         //!< Preamble detection length off
    RADIO_PREAMBLE_DETECTOR_08_BITS         = 0x04,         //!< Preamble detection length 8 bits
    RADIO_PREAMBLE_DETECTOR_16_BITS         = 0x05,         //!< Preamble detection length 16 bits
    RADIO_PREAMBLE_DETECTOR_24_BITS         = 0x06,         //!< Preamble detection length 24 bits
    RADIO_PREAMBLE_DETECTOR_32_BITS         = 0x07,         //!< Preamble detection length 32 bit
} RadioFSKPreambleDetection_e;

/*!
 * \brief Represents the possible combinations of SyncWord correlators activated
 */
typedef enum
{
    RADIO_ADDRESSCOMP_FILT_OFF              = 0x00,         //!< No correlator turned on, i.e. do not search for SyncWord
    RADIO_ADDRESSCOMP_FILT_NODE             = 0x01,
    RADIO_ADDRESSCOMP_FILT_NODE_BROAD       = 0x02,
} RadioFSKAddressComp_e;

/*!
 *  \brief Radio GFSK packet length mode
 */
typedef enum
{
    RADIO_PACKET_FIXED_LENGTH               = 0x00,         //!< The packet is known on both sides, no header included in the packet
    RADIO_PACKET_VARIABLE_LENGTH            = 0x01,         //!< The packet is on variable size, header included
} RadioFSKPacketLengthModes_e;

/*!
 * \brief Radio whitening initial seed value
 */
typedef enum
{
    RADIO_WHITENINGSEED                     = 0x01FF,
} RadioFSKWhiteningSeed_e;

/*!
 * \brief Radio whitening mode activated or deactivated
 */
typedef enum
{
    RADIO_DC_FREE_OFF                       = 0x00,
    RADIO_DC_FREEWHITENING                  = 0x01,
} RadioFSKDcFree_e;

/*!
 * \brief Represents the CRC length
 */
typedef enum
{
    RADIO_CRC_OFF                           = 0x01,         //!< No CRC in use
    RADIO_CRC_1_BYTES                       = 0x00,
    RADIO_CRC_2_BYTES                       = 0x02,
    RADIO_CRC_1_BYTES_INV                   = 0x04,
    RADIO_CRC_2_BYTES_INV                   = 0x06,
    RADIO_CRC_2_BYTES_IBM                   = RADIO_CRC_2_BYTES,
    RADIO_CRC_2_BYTES_CCIT                  = RADIO_CRC_2_BYTES_INV,
} RadioFSKCrcTypes_e;

/*!
 * \brief LFSR initial value to compute the FSK CRC
 */
typedef enum
{
    RADIO_CRC_IBM_SEED                      = 0xFFFF,   //!< FSR initial value to compute IBM type CRC
    RADIO_CRC_CCITT_SEED                    = 0x1D0F,   //!< FSR initial value to compute CCIT type CRC
} RadioFSKCrcSeed_e;

/*!
 * \brief Polynomial used to compute the FSK CRC
 */
typedef enum
{
    RADIO_CRC_POLYNOMIAL_IBM                = 0x8005,   //!< Polynomial to compute IBM type CRC
    RADIO_CRC_POLYNOMIAL_CCITT              = 0x1021,   //!< Polynomial value to compute CCIT type CRC
} RadioFSKCrcPolynomial_e;

typedef struct {
    RadioFSKRBitrate_e			        BitRate                 : 32;
	RadioFSKRDeviation_e			    Fdev                    : 32;
	RadioFSKModShapings_e				PulseShape              : 8;
	RadioFSKRxBandwidth_e				RxBandwidth             : 8;
 }  FSKModulationParams_t;

typedef struct {
    uint16_t                            PreambleLength;
    RadioFSKPreambleDetection_e         PreambleDetectorLength  : 8;
    arr8b_t                             SyncWord;                                       //!< The SX126x FSK syncword setting
    uint8_t                             SyncWordLength          : 8;                    //!< Size of the SyncWord in bits.
    RadioFSKAddressComp_e               AddrComp                : 8;
    RadioFSKPacketLengthModes_e         PacketType              : 8;
    uint8_t                             PayloadLength;                                  //!< Size of the payload (in bytes) to transmit or maximum size of the payload that the receiver can accept.
    RadioFSKCrcTypes_e                  CRCType                 : 8;
    RadioFSKCrcSeed_e                   CrcSeed                 : 16;                   //!< LFSR initial value to compute the FSK CRC
    RadioFSKCrcPolynomial_e             CrcPolynomial           : 16;                   //!< Polynomial used to compute the FSK CRC
    RadioFSKDcFree_e                    Whitening               : 8;
    RadioFSKWhiteningSeed_e             WhiteningSeed           : 16;
}  FSKPacketParams_t;


/* ==== The Radio Settings  ==== */

typedef struct {
    uint32_t                            Frequency;
    Radio_TXpower_e                     TXpower;                                        //!< TXpower (PWR = MAX - (2 * value)), not used in receive mode
    union
    {
        struct
        {
            LoRaModulationParams_t              Modulation;
            LoRaPacketParams_t                  Packet;
        } LoRa;
        struct
        {
            FSKModulationParams_t              Modulation;
            FSKPacketParams_t                  Packet;
        } FSK;
    };
    RadioPacketTypes_e                  PacketType;
    uint8_t                             RXboost                 : 1;                    //!< RX Boost mode
    uint8_t                                                     : 7;                    //!< Reserved
}  RadioParams_t;

/*!
 * \brief Represents the possible device error states
 */
typedef union
{
    uint8_t Value;
    struct
    {   //bit order is lsb -> msb
        uint8_t				    : 1;  //!< Reserved
        uint8_t CmdStatus		: 3;  //!< Command status
        uint8_t ChipMode		: 3;  //!< Chip mode
        uint8_t CpuBusy		    : 1;  //!< Flag for CPU radio busy
    };
} chipStatus_t;

/*!
 * \brief Represents the possible IRQ states
 */
typedef union
{
    uint16_t Value;
    struct
    {
        uint8_t IRQ_TX_DONE					    : 1;                    //!< Radio received TX DONE IRQ
        uint8_t IRQ_RX_DONE					    : 1;                    //!< Radio received RX DONE IRQ
        uint8_t IRQ_PREAMBLE_DETECTED			: 1;                    //!< Radio received PREAMBLE DETECTED IRQ
        uint8_t IRQ_SYNCWORD_VALID			    : 1;                    //!< Radio received SYNCWORD VALID IRQ
        uint8_t IRQ_HEADER_VALID				: 1;                    //!< Radio received HEADER VALID IRQ
        uint8_t IRQ_HEADER_ERROR				: 1;                    //!< Radio received HEADER ERROR IRQ
        uint8_t IRQ_CRC_ERROR					: 1;                    //!< Radio received CRC ERROR IRQ
        uint8_t IRQ_CAD_DONE					: 1;                    //!< Radio received CAD DONE IRQ
        uint8_t IRQ_CAD_ACTIVITY_DETECTED		: 1;                    //!< Radio received CAD ACTIVITY DETECTED IRQ
        uint8_t IRQ_RX_TX_TIMEOUT				: 1;                    //!< Radio received RX TX TIMEOUT IRQ
        uint8_t								    : 6;                    //!< Reserved
    };
} IRQstatus_t;

/*!
 * \brief Represents the possible device error states
 */
typedef union
{
    uint16_t Value;
    struct
    {
        uint8_t Rc64kCalib              : 1;                    //!< RC 64kHz oscillator calibration failed
        uint8_t Rc13mCalib              : 1;                    //!< RC 13MHz oscillator calibration failed
        uint8_t PllCalib                : 1;                    //!< PLL calibration failed
        uint8_t AdcCalib                : 1;                    //!< ADC calibration failed
        uint8_t ImgCalib                : 1;                    //!< Image calibration failed
        uint8_t XoscStart               : 1;                    //!< XOSC oscillator failed to start
        uint8_t PllLock                 : 1;                    //!< PLL lock failed
        uint8_t BuckStart               : 1;                    //!< Buck converter failed to start
        uint8_t PaRamp                  : 1;                    //!< PA ramp failed
        uint8_t                         : 7;                    //!< Reserved
    };
} deviceErrors_t;

/*!
 * \brief Represents the packet status for every packet type
 */
typedef union
{
	uint32_t value;
	struct {
		int8_t RssiPkt;                                //!< The RSSI of the last packet
	    int8_t SnrPkt;                                 //!< The SNR of the last packet multiplied by 4
        int8_t SignalRssiPkt;                          //!< The RSSI of the LoRa signal of the last packet
	};
} LoRaPacketStatus_t;

/*!
 * \brief Represents the LoRa Packet statistics
 */
typedef struct  __attribute__((scalar_storage_order("big-endian"))) 
{
	uint16_t NbPktReceived;
    uint16_t NbPktCrcErr;
    uint16_t NbPktHeaderErr;
} LoRaStats_t;

typedef struct 
{
    chipStatus_t                    chipStatus;                 //!< The device error states
    IRQstatus_t                     IRQstatus;                  //!< The IRQ states
    deviceErrors_t                  deviceErrors;               //!< The device error states
    LoRaPacketStatus_t              LoRaPacketStatus;           //!< The packet status for every packet type
    LoRaStats_t                     LoRaStats;                  //!< The LoRa Packet statistics
} RadioStatus_t;

// Open stack implementation below

coreStatus_t        LoRa_RX(RadioParams_t * RadioParams, RadioStatus_t * RadioStatus, uint8_t * payload, uint8_t payloadSize, uint16_t timeOutMS, WaitMode_e waitMode);
coreStatus_t        LoRa_TX(RadioParams_t * RadioParams, RadioStatus_t * RadioStatus, uint8_t * payload, uint8_t payloadSize, uint16_t timeOutMS, WaitMode_e waitMode);
void                LoRaWAN_Debug(bool debugLedsOn, uint32_t * coreStatePNT); 

// Use the Unlock function before using any other extended functions. Unlocking may void LoRa Alliance Certification by Similarity.
void                LoRaWAN_Unlock();

#endif /* ONETHINXEXT01_H */
