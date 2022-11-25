/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <project.h>
//#include <stddef.h>
//#include <stdint.h>
//#include <stdbool.h>
#include "OnethinxCore01.h"
#include "OnethinxExt01.h"
#include "ATinterface.h"

extern volatile coreArguments_t coreArguments;
coreStatus_t coreStatus;
coreInfo_t coreInfo;
errorStatus_t coreErrorStatus;
RadioStatus_t RadioStatus;

coreConfiguration_t	coreConfig = {
	.Join.KeysPtr = 			&LoRaWAN_keys,
	.Join.DataRate =			DR_AUTO,
	.Join.Power =				PWR_MAX,
	.Join.MAXTries = 			100,
    .Join.SubBand_1st =     	EU_SUB_BANDS_DEFAULT,
	.Join.SubBand_2nd =     	EU_SUB_BANDS_DEFAULT,
	.TX.Confirmed = 			false,
	.TX.DataRate = 				DR_0,
	.TX.Power = 			    PWR_MAX,
	.TX.FPort = 				1,
	.System.Idle.Mode = 		M0_DeepSleep,
	.System.Idle.BleEcoON =		false,
	.System.Idle.DebugON =		true,
};

sleepConfig_t sleepConfig =
{
	.BleEcoON = false,
	.DebugON = true,
    .sleepMode = modeDeepSleep,
	.sleepCores = coresBoth,
	.wakeUpPin = wakeUpPinLow(false),
	.wakeUpTime = wakeUpTimeOff
};

#define PAYLOADSIZE 8
#define BITRATE_1K3  791222
RadioParams_t RadioParams =
{
	.Frequency = 8682850,		// Frequency in 100Hz steps
	.TXpower = PWR_MAX,
    .PacketType = PACKET_TYPE_GFSK,
    .FSK = 
        {
            .Modulation.BitRate = BITRATE_1K3, 
            .Modulation.Fdev = FDEV_DSB_60K,
            .Modulation.PulseShape = MOD_SHAPING_OFF,
            .Modulation.RxBandwidth = RX_BW_78200,

            .Packet.PreambleLength = 1,
            .Packet.PreambleDetectorLength = RADIO_PREAMBLE_DETECTOR_08_BITS,

        // .FSK.Packet.SyncWord = {0xAC,0x00,0,0,0,0,0,0},                       //!< The SX126x FSK syncword setting
        //  .FSK.Packet.SyncWordLength = 12,                //!< Size of the SyncWord in bits.
        
            .Packet.SyncWord = {{ 0xAA,0xC0,0,0,0,0,0,0 }},                       //!< The SX126x FSK syncword setting
            .Packet.SyncWordLength = 12,                //!< Size of the SyncWord in bits.
            .Packet.AddrComp = RADIO_ADDRESSCOMP_FILT_OFF,
            .Packet.PacketType = RADIO_PACKET_FIXED_LENGTH,
            .Packet.PayloadLength = PAYLOADSIZE,                  //!< Size of the payload (in bytes) to transmit or maximum size of the payload that the receiver can accept.
            .Packet.CRCType = RADIO_CRC_OFF,
            .Packet.CrcSeed = 0,                       //!< LFSR initial value to compute the FSK CRC
            .Packet.CrcPolynomial = 0,                  //!< Polynomial used to compute the FSK CRC
            .Packet.Whitening = RADIO_DC_FREE_OFF,
            .Packet.WhiteningSeed = 0
        }
};

LoRaWAN_keys_t LoRaWAN_keys = 
{
	.KeyType	= PreStored_key,
	.StoredKeys.KeyIndex = 0
};

uint8_t	cmdIDX = 0;
char	cmdBUF[255];
char	tmpBUF[255];
char	rxBUF[255];
uint8_t	tmpBUFIDX = 0;

uint16_t RXtimeout;

//     RadioFSKRBitrate_e			        BitRate                 : 32;
// 	RadioFSKRDeviation_e			    Fdev                    : 32;
// 	RadioFSKModShapings_e				PulseShape              : 8;
// 	RadioFSKRxBandwidth_e				RxBandwidth             : 8;
//  }  FSKModulationParams_t;

//     uint16_t                            PreambleLength;
//     RadioFSKPreambleDetection_e         PreambleDetectorLength  : 8;
//     arr8b_t                             SyncWord;                       //!< The SX126x FSK syncword setting
//     uint8_t                             SyncWordLength          : 8;                 //!< Size of the SyncWord in bits.
//     RadioFSKAddressComp_e               AddrComp                : 8;
//     RadioFSKPacketLengthModes_e         PacketType              : 8;
//     uint8_t                             PayloadLength;                  //!< Size of the payload (in bytes) to transmit or maximum size of the payload that the receiver can accept.
//     RadioFSKCrcTypes_e                  CRCType                 : 8;
//     RadioFSKCrcSeed_e                   CrcSeed                 : 16;                        //!< LFSR initial value to compute the FSK CRC
//     RadioFSKCrcPolynomial_e             CrcPolynomial           : 16;                  //!< Polynomial used to compute the FSK CRC
//     RadioFSKDcFree_e                    Whitening               : 8;
//     RadioFSKWhiteningSeed_e             WhiteningSeed           : 16;
// }  FSKPacketParams_t;

//     uint32_t                            Frequency;
//     Radio_TXpower_e                     TXpower;                         //!< TXpower, not used in receive mode

//     RadioLoRaSpreadingFactors_e			SF                      : 8;
// 	RadioLoRaBandwidths_e				BW                      : 8;
// 	RadioLoRaCodingRates_e				CR                      : 8;
//     RadioLoraLowDataRateOptimize_e      LowDataRateOptimize     : 8;
//  }  LoRaModulationParams_t;

//     uint16_t                            PreambleLength;
//     RadioLoRaHeaderType_e               HeaderType              : 8;
//     uint8_t                             PayloadSize;                    //!< Size of the payload (in bytes) to transmit or maximum size of the payload that the receiver can accept.
//     RadioLoRaCrcMode_e                  CRCmode                 : 8;
//     RadioLoRaIQMode_e                   IQmode                  : 8;
//     uint16_t                            SyncWord;                       //!< The SX126x LoRa syncword setting
// }  LoRaPacketParams_t;

// Build AT command list, occurrences of 'partial duplicate commands' should come after the 'full command' eg: "RX" should come after "RX_LENGTH"
const char* const ATcmdList[] = {
	"PING",
	"RESET",
	"INFO",
	"STATUS",
	"SET_OTAA",
	"JOIN",
	"TX",
	"RX_LENGTH",
	"RX",
	"SLEEPMODE",
    "UNLOCK",
    "RADIO_SET_FSK_MODULATION",              //AAAAAAAABBBBBBBBCCDD A=BitRate B=Fdev C=PulseShape D=RXBandwidth
	"RADIO_SET_FSK_PACKET",                  //AAAABBCCCCCCCCCCCCCCCCDDEEFFGGHHIIIIJJJJKKLLLL A=PreambleLength B=PreambleDetectorLength C=SyncWord D=SyncWordSize  E=AddrComp F=PacketType G=PayloadLength H=CRCtype I=CRCseed J=CRCpoly K=Whitening L=WhiteningSeed
	"RADIO_SET_LORA_MODULATION",             //AABBCCDD A=SF B=WB C=CR D=LowDataRateOptimize
	"RADIO_SET_LORA_PACKET",                 //AAAABBCCDDEEFFFF A=PreambleLength B=HeaderType C=PayloadLength D=CRCmode E=IQmode F=CSyncWord
	"RADIO_SET_FREQUENCY",                  //AAAA (in steps of 100Hz)
	"RADIO_SET_TXPOWER",                    //BB
	"RADIO_RX",
	"RADIO_TX",
	"HELP",
	0
};

// Enum commands in the same order as the list above
typedef enum
{
    cmd_ping,
    cmd_reset,
    cmd_info,
    cmd_status,
    cmd_set_otaa,
    cmd_join,
    cmd_tx,
    cmd_rxlength,
    cmd_rx,
    cmd_sleepmode,
    cmd_unlock,
    cmd_radio_set_fsk_modulation,
    cmd_radio_set_fsk_packet,
    cmd_radio_set_lora_modulation,
    cmd_radio_set_lora_packet,
    cmd_radio_set_frequency,
    cmd_radio_set_txpower,
    cmd_radio_rx,
    cmd_radio_tx,
    cmd_help
} command_t;

typedef enum
{
    lora_start,
    lora_idle,
    lora_joining,
    lora_sending,
    lora_receive,
    lora_receiving,
    lora_done
} state_lora_t;

state_lora_t loraState = lora_start;

typedef enum
{
    resp_ok,
    resp_info,
    resp_joining,
    resp_sending,
    resp_receiving,
    resp_invalidcmd,
    resp_invalidparam,
    resp_bufferoverflow,
    resp_invalidhex,
    resp_core_radioerror,
    resp_core_macerror,
    resp_core_paramerror,
    resp_core_systemerror,
    resp_core_isbusy,
    resp_core_rxerror
} response_t;

char * uint16toDecimalBuilder(uint16_t val, uint8_t idx)
{
    uint8_t incr = 0;
    if (idx != (uint8_t) -1) tmpBUFIDX = idx;
    if (tmpBUFIDX > 246) return NULL;
    tmpBUF[tmpBUFIDX] = '0';
    while (val >= 10000) val -= 10000, tmpBUF[tmpBUFIDX]++;
    if (tmpBUF[tmpBUFIDX] != '0') incr = 1;
    tmpBUFIDX += incr;
    tmpBUF[tmpBUFIDX] = '0';
    while (val >= 1000) val -= 1000, tmpBUF[tmpBUFIDX]++;
    if (tmpBUF[tmpBUFIDX] != '0') incr = 1;
    tmpBUFIDX += incr;
    tmpBUF[tmpBUFIDX] = '0';
    while (val >= 100) val -= 100, tmpBUF[tmpBUFIDX]++;
    if (tmpBUF[tmpBUFIDX] != '0') incr = 1;
    tmpBUFIDX += incr;
    tmpBUF[tmpBUFIDX] = '0';
    while (val >= 10) val -= 10, tmpBUF[tmpBUFIDX]++;
    if (tmpBUF[tmpBUFIDX] != '0') incr = 1;
    tmpBUFIDX += incr;
    tmpBUF[tmpBUFIDX] = '0' + val;
    tmpBUF[++tmpBUFIDX] = 0;
    return (char *) tmpBUF;
}

char * uint32toHexBuilder(uint32_t val, uint8_t idx)
{
    if (idx != (uint8_t) -1) tmpBUFIDX = idx;
    if (tmpBUFIDX > 246) return NULL;
    tmpBUF[tmpBUFIDX] = (val >> 28) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = (val >> 24) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = (val >> 20) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = (val >> 16) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = (val >> 12) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = (val >> 8) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = (val >> 4) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = (val >> 0) & 0x0F;
    tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    tmpBUF[++tmpBUFIDX] = 0;
    return (char *) tmpBUF;
}

char * bytestoHexBuilder(uint8_t * val, uint8_t length, uint8_t idx)
{
    if (idx != (uint8_t) -1) tmpBUFIDX = idx;
    if (((length << 1) + (uint16_t) tmpBUFIDX) > 254) return NULL;
    for (int8_t cnt = 0; cnt < length; cnt++, tmpBUFIDX++)
    {
        tmpBUF[tmpBUFIDX] = (val[cnt] >> 4) & 0x0F;
        tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
        tmpBUF[++tmpBUFIDX] = (val[cnt] >> 0) & 0x0F;
        tmpBUF[tmpBUFIDX] += (tmpBUF[tmpBUFIDX] > 9)? 55 : '0';
    }
    tmpBUF[tmpBUFIDX] = 0;
    return (char *) tmpBUF;
}

char * stringBuilder(char* string, uint8_t idx)
{
    if (idx != (uint8_t) -1) tmpBUFIDX = idx;
    for (; tmpBUFIDX < 255;)
    {
        if (* string == 0) break;
        tmpBUF[tmpBUFIDX++] = * string++;
    }
    tmpBUF[tmpBUFIDX] = 0;
    return (char *) tmpBUF;
}

void outputResponse(response_t response, uint32_t errorValue)
{
    const char* message = "Error FF: Unknown error";
    switch (response)
    {
        case resp_ok:
            message = "OK";
            break;
        case resp_info:
            coreStatus = LoRaWAN_GetInfo(&coreInfo);
            stringBuilder("--= Onethinx LoRaWAN Core OT-X18 =--\r\n: Type-Option-Stage-Region: ", 0);
            stringBuilder((char[]) { coreInfo.buildType, '-', coreInfo.stackOption, '-', coreInfo.stackStage, '-', 0 }, -1);
            uint16toDecimalBuilder(coreInfo.stackRegion, -1);
            stringBuilder("\r\n: Build: ", -1);
            uint16toDecimalBuilder(coreInfo.buildNumber, -1);
            stringBuilder(" (20", -1);
            uint16toDecimalBuilder(coreInfo.buildYear, -1);
            stringBuilder("-", -1);
            uint16toDecimalBuilder(coreInfo.buildMonth, -1);
            stringBuilder("-", -1);
            uint16toDecimalBuilder(coreInfo.buildDayOfMonth, -1);
            stringBuilder(" ", -1);
            uint16toDecimalBuilder(coreInfo.buildHour, -1);
            stringBuilder(":", -1);
            uint16toDecimalBuilder(coreInfo.buildMinute, -1);
            stringBuilder(":", -1);
            uint16toDecimalBuilder(coreInfo.buildSecond, -1);
            stringBuilder(")\r\n: DevEUI: ", -1);
            bytestoHexBuilder((uint8_t *) &coreInfo.devEUI, 8, -1);
            stringBuilder("\r\n: Stack Version: ", -1);
            uint32toHexBuilder(coreStatus.system.version, -1);
            stringBuilder("\r\n: CMD Version: ", -1);
            uint32toHexBuilder(ATcommVersion, -1);
            message = stringBuilder("\r\nOK", -1);
            break;
        case resp_joining:
            message = "joining...";
            break;
        case resp_sending:
            message = "sending...";
            break;
        case resp_receiving:
            message = "receiving...";
            break;case resp_invalidcmd:
            message = "Error 01: AT command not found";
            break;
        case resp_invalidparam:
            message = "Error 02: Invalid parameters";
            break;
        case resp_bufferoverflow:
            message = "Error 03: AT buffer overflow";
            break;
        case resp_invalidhex:
            message = "Error 04: Invalid Hexadecimal value";
            break;
        case resp_core_radioerror:
            message = "Error 05: LoRaWAN radio error 0x";
            break;
        case resp_core_macerror:
            message = "Error 06: LoRaWAN MAC error 0x";
            break;
        case resp_core_paramerror:
            message = "Error 07: LoRaWAN parameter error 0x";
            break;
        case resp_core_systemerror:
            message = "Error 08: LoRaWAN system error 0x";
            break;
        case resp_core_isbusy:
            message = "Error 09: LoRaWAN is busy";
            break;
        case resp_core_rxerror:
            message = "Error 0A: RX error";
            break;
    }
    UART_PutString( message);
    if (errorValue != 0) UART_PutString(uint32toHexBuilder(errorValue, 0));
    UART_PutString("\r\n");
}

response_t HEXtoBytes(char * source, char * dest, uint8_t byteSize)
{
    uint8_t cnt;
    for(cnt = 0; cnt < byteSize; cnt++)
    {
        if (*source > 0x60) *source -= 0x20;        // Convert to uppercase
        if (*source >= '0' && *source <= '9') *dest = *source - '0';
        else if (*source >= 'A' && *source <= 'F') *dest = *source - 55;
        else return resp_invalidhex;
        *dest <<= 4; source++;
        if (*source > 0x60) *source -= 0x20;        // Convert to uppercase
        if (*source >= '0' && *source <= '9') *dest += *source - '0';
        else if (*source >= 'A' && *source <= 'F') *dest += *source - 55;
        else return resp_invalidhex;
        source++; dest++;
    }
    return resp_ok;
}

void coreResponse()
{
    coreErrorStatus = LoRaWAN_GetError();
    if (coreErrorStatus.macErrors != mac_OK) return outputResponse(resp_core_macerror, coreErrorStatus.errorValue);
    if (coreErrorStatus.radioErrors != radio_OK) return outputResponse(resp_core_radioerror, coreErrorStatus.errorValue);
    if (coreErrorStatus.systemErrors != system_OK) return outputResponse(resp_core_systemerror, coreErrorStatus.errorValue);
    if (coreErrorStatus.paramErrors != param_OK) return outputResponse(resp_core_paramerror, coreErrorStatus.errorValue);
    outputResponse(resp_ok, 0);
}

void execCommand(command_t command, uint8_t length, uint8_t cmdLength)
{
    response_t retErr;
    if (command == cmd_reset){
        loraState = lora_start;
        return;
    }
    if(coreArguments.status.system.isBusy) return outputResponse(resp_core_isbusy, 0);
    switch (command)
    {
        case cmd_ping:
            outputResponse((length != cmdLength)? resp_invalidparam: resp_ok, 0);
            break;
        case cmd_info:
            outputResponse((length != cmdLength)? resp_invalidparam: resp_info, 0);
            break;
        case cmd_status:
            coreResponse();
            break;
        case cmd_set_otaa:
            if ((length - cmdLength) != 0x45) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 3], (char *) &LoRaWAN_keys.OTAA_10x.DevEui, 8)) != resp_ok) return outputResponse(retErr, 0);
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 20], (char *) &LoRaWAN_keys.OTAA_10x.AppEui, 8)) != resp_ok) return outputResponse(retErr, 0);
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 37], (char *) &LoRaWAN_keys.OTAA_10x.AppKey, 16)) != resp_ok) return outputResponse(retErr, 0);
            LoRaWAN_keys.KeyType = OTAA_10x_key;
            LoRaWAN_keys.PublicNetwork = (cmdBUF[cmdLength + 1] == '0')? false : true;
            outputResponse(resp_ok, 0);
            break;
        case cmd_join:
            LoRaWAN_Join(false);
            outputResponse(resp_joining, 0);
            loraState = lora_joining;
            break;
        case cmd_tx:
            length = (length - (cmdLength + 1)) >> 1;
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], tmpBUF, length)) != resp_ok) return outputResponse(retErr, 0);
            LoRaWAN_Send((uint8_t *) tmpBUF, length, false);
            outputResponse(resp_sending, 0);
            loraState = lora_sending;
            break;
        case cmd_rxlength:
            if (length != cmdLength) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            coreStatus = LoRaWAN_GetStatus();
            Cy_SCB_UART_PutString(UART_HW, uint16toDecimalBuilder((uint16_t) coreStatus.mac.bytesToRead, 0));
            Cy_SCB_UART_PutString(UART_HW, "\r\n");
            outputResponse(resp_ok, 0);
            break;
        case cmd_rx:
            if (length != cmdLength) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            coreStatus = LoRaWAN_GetStatus();
            LoRaWAN_GetRXdata((uint8_t *) rxBUF, coreStatus.mac.bytesToRead);
            Cy_SCB_UART_PutString(UART_HW, bytestoHexBuilder((uint8_t *) rxBUF, coreStatus.mac.bytesToRead, 0));
            Cy_SCB_UART_PutString(UART_HW, "\r\n");
            outputResponse(resp_ok, 0);
            break;

        case cmd_unlock:
            LoRaWAN_Unlock();
            outputResponse(resp_ok, 0);
            break;
                                                                 // AT+RADIO_SET_FSK_MODULATION B6120C00E17A0000001B0000
                                                                 // AT+RADIO_SET_FSK_MODULATION B6120C00E17A0000001B0000
        case cmd_radio_set_fsk_modulation:      // "AT+RADIO_SET_FSK_MODULATION"              //AAAAAAAABBBBBBBBCCDDEEEE A=BitRate B=Fdev C=PulseShape D=RXBandwidth E=Reserved
	        if ((length - cmdLength) != 1 + (2 * sizeof(RadioParams.FSK.Modulation))) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], (char *) &RadioParams.FSK.Modulation, sizeof(RadioParams.FSK.Modulation))) != resp_ok) return outputResponse(retErr, 0);
            RadioParams.PacketType = PACKET_TYPE_GFSK;
            outputResponse(resp_ok, 0);
            break;
                                                                     // AT+RADIO_SET_FSK_PACKET 010004AAC00000000000000C000008010000000000000000
        case cmd_radio_set_fsk_packet:          // "AT+RADIO_SET_FSK_PACKET"                  //AAAABBCCCCCCCCCCCCCCCCDDEEFFGGHHIIIIJJJJKKLLLLMM A=PreambleLength B=PreambleDetectorLength C=SyncWord D=SyncWordSize  E=AddrComp F=PacketType G=PayloadLength H=CRCtype I=CRCseed J=CRCpoly K=Whitening L=WhiteningSeed M=Reserved
            if ((length - cmdLength) != 1 + (2 * sizeof(RadioParams.FSK.Packet))) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], (char *) &RadioParams.FSK.Packet, sizeof(RadioParams.FSK.Packet))) != resp_ok) return outputResponse(retErr, 0);
            RadioParams.PacketType = PACKET_TYPE_GFSK;
            outputResponse(resp_ok, 0);
            break;
                                                                // AT+RADIO_SET_LORA_MODULATION B6120C00
        case cmd_radio_set_lora_modulation:     // "AT+RADIO_SET_LORA_MODULATION"             //AABBCCDD A=SF B=BW C=CR D=LowDataRateOptimize
            if ((length - cmdLength) != 1 + (2 * sizeof(RadioParams.LoRa.Modulation))) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], (char *) &RadioParams.LoRa.Modulation, sizeof(RadioParams.LoRa.Modulation))) != resp_ok) return outputResponse(retErr, 0);
            RadioParams.PacketType = PACKET_TYPE_LORA;
            outputResponse(resp_ok, 0);
            break;
                                                                    // AT+RADIO_SET_LORA_PACKET E17A0000001B0000
        case cmd_radio_set_lora_packet:         // "AT+RADIO_SET_LORA_PACKET"                 //AAAABBCCDDEEFFFF A=PreambleLength B=HeaderType C=PayloadLength D=CRCmode E=IQmode F=CSyncWord
	        if ((length - cmdLength) != 1 + (2 * sizeof(RadioParams.LoRa.Packet))) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], (char *) &RadioParams.LoRa.Packet, sizeof(RadioParams.LoRa.Packet))) != resp_ok) return outputResponse(retErr, 0);
            RadioParams.PacketType = PACKET_TYPE_LORA;
            outputResponse(resp_ok, 0);
            break;
                                                                      // AT+RADIO_SET_FREQUENCY 627D8400
        case cmd_radio_set_frequency:           // "AT+RADIO_SET_FREQUENCY",                  //AAAAAAAA (in steps of 100Hz)
            if ((length - cmdLength) != 1 + (2 * sizeof(RadioParams.Frequency))) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], (char *) &RadioParams.Frequency, sizeof(RadioParams.Frequency))) != resp_ok) return outputResponse(retErr, 0);
            outputResponse(resp_ok, 0);
            break;
                                                                           // AT+RADIO_SET_TXPOWER 00
        case cmd_radio_set_txpower:             // "AT+RADIO_SET_TXPOWER 20",                    //AA
            if ((length - cmdLength) != 1 + (2 * sizeof(RadioParams.TXpower))) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], (char*) &RadioParams.TXpower, sizeof(RadioParams.TXpower))) != resp_ok) return outputResponse(retErr, 0);
            outputResponse(resp_ok, 0);
            break;

        case cmd_radio_rx:
            if ((length - cmdLength) != 1 + (2 * sizeof(RXtimeout))) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], (char*) &RXtimeout, sizeof(RXtimeout))) != resp_ok) return outputResponse(retErr, 0);
            RXtimeout = __builtin_bswap16(RXtimeout);
            //LoRa_RX(&RadioParams, &RadioStatus, (uint8_t *) rxBUF, 255, 60000);
            outputResponse(resp_receiving, 0);
            loraState = lora_receive;
            break;
        case cmd_radio_tx:
            length = (length - (cmdLength + 1)) >> 1;
            if ((retErr = HEXtoBytes(&cmdBUF[cmdLength + 1], tmpBUF, length)) != resp_ok) return outputResponse(retErr, 0);
            RadioParams_t RadioParamsTX = RadioParams;
            switch (RadioParamsTX.PacketType)
            {
                case PACKET_TYPE_GFSK:
                    RadioParamsTX.FSK.Packet.PayloadLength = length;
                    break;
                case PACKET_TYPE_LORA:
                    RadioParamsTX.LoRa.Packet.PayloadSize = length;
                    break;
            }
            LoRa_TX(&RadioParamsTX, &RadioStatus, (uint8_t *) &tmpBUF, length, 0, M4_NoWait); 
            outputResponse(resp_sending, 0);
            loraState = lora_sending;
            break;
        case cmd_sleepmode:
            if ((length - cmdLength) != 2) {
                outputResponse(resp_invalidparam, 0);
                return;
            }
            switch (cmdBUF[cmdLength + 1])
            {
                case '0':
                    sleepConfig.sleepMode = modeHibernate;
                    break;
                case '1':
                    sleepConfig.sleepMode = modeDeepSleep;
                    break;
                case '2':
                    sleepConfig.sleepMode = modeSleep;
                    break;
                default:
                    outputResponse(resp_invalidparam, 0);
                    return;
                    
            }
            outputResponse(resp_ok, 0);
            while (!Cy_SCB_IsTxComplete(UART_HW));                          // Wait till finished sending OK
            Cy_SysClk_ClkPathSetSource(2U, CY_SYSCLK_CLKPATH_IN_IMO);       // Set Clock MUX to IMO because Sleep will disable BLE ECO clock (ALTHF)
            LoRaWAN_Sleep(&sleepConfig);
            Cy_SysClk_ClkPathSetSource(2U, CY_SYSCLK_CLKPATH_IN_ALTHF);     // Restore Clock MUX to BLE ECO clock (ALTHF)
            break;
        case cmd_help:
            outputResponse((length != cmdLength)? resp_invalidparam: resp_ok, 0);
            break;
    }

}

bool initialized = false;

void ATcomm(void)
{
    if (loraState == lora_start)
    {
        //Cy_GPIO_Pin_FastInit(LED_RED_PORT, LED_RED_PIN, CY_GPIO_DM_STRONG_IN_OFF, 1, LED_RED_HSIOM);
        //Cy_GPIO_Pin_FastInit(LED_BLUE_PORT, LED_BLUE_PIN, CY_GPIO_DM_STRONG_IN_OFF, 0, LED_BLUE_HSIOM);
        if (initialized)
        {
            *((uint32_t *) 0x40210000) = 0x05FA0000;   // SW RESET M4
		    NVIC_SystemReset();
        }
        LoRaWAN_Init(&coreConfig);
        initialized = true;
        //LED_R_SET(0);
        //LED_B_SET(1);
        outputResponse(resp_info, 0);
        loraState = lora_idle;
    }

    if (loraState == lora_receive)
    {
        LoRa_RX(&RadioParams, &RadioStatus, (uint8_t *) &rxBUF, sizeof(rxBUF), RXtimeout, M4_NoWait);        // Continue receiving if RX timeout is set to zero
        loraState = lora_receiving;
    }
    if (loraState == lora_receiving)
    {
        if (LoRaWAN_GetStatus().system.isBusy == false)
        {
            if (RadioStatus.IRQstatus.IRQ_RX_DONE)
            {
                if (RadioStatus.IRQstatus.IRQ_CRC_ERROR)
                    outputResponse(resp_core_rxerror, 0);
                else {
                    uint8_t length = coreArguments.arg4;
                    if (length > 0)
                    {
                        Cy_SCB_UART_PutString(UART_HW, "RX: ");
                        Cy_SCB_UART_PutString(UART_HW, bytestoHexBuilder((uint8_t *) rxBUF, length, 0));   
                        Cy_SCB_UART_PutString(UART_HW, "\r\n");
                    }
                }
            }
            loraState = (RXtimeout == 0)? lora_receive : lora_done;        // Continue receiving if RX timeout is set to zero
        }
    }
    else if ((loraState != lora_idle) && (LoRaWAN_GetStatus().system.isBusy == false))
    {
        outputResponse(resp_ok, 0);
        loraState = lora_idle;
    }
    uint32_t cmd;
    if ((cmd = Cy_SCB_UART_Get(UART_HW)) == CY_SCB_UART_RX_NO_DATA) return;
    uint8_t RXbyte = (uint8_t) cmd;
    switch(cmdIDX)
    {
        case 0:
            if (RXbyte == 'A') cmdIDX++;
            break;
        case 1:
            if (RXbyte == 'T') cmdIDX++;
            else cmdIDX = 0;
            break;
        case 2:
            if (RXbyte == '+') cmdIDX++;
            else cmdIDX = 0;
            break;
        case 255:   // Error MAX command size exceeded
            outputResponse(resp_bufferoverflow, 0);
            cmdIDX = 0;
            break;
        default:
            if (RXbyte != '\r' && RXbyte != '\n') cmdBUF[++cmdIDX-4] = RXbyte;
            else 
            {
                uint8_t cnt, cmdPNT = 0; cmdIDX -= 3;
                while (ATcmdList[cmdPNT] != 0) {
                    for (cnt = 0; cnt <= cmdIDX; cnt++) {
                        if (ATcmdList[cmdPNT][cnt] == 0) {
                            execCommand(cmdPNT, cmdIDX, cnt);
                            cmdIDX = 0;
                            return;
                        }
                        if (cmdBUF[cnt] != ATcmdList[cmdPNT][cnt]) break;           // different character, try next cmd
                    }
                    cmdPNT++;
                }
                outputResponse(resp_invalidcmd, 0);
                cmdIDX = 0;
            }
            break;
    }
}
	
/* [] END OF FILE */