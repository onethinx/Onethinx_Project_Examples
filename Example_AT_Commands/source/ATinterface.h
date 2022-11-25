#define ATcommVersion   0x00000012

extern void ATcomm(void);

extern coreConfiguration_t coreConfig;
extern RadioParams_t RadioParams;
extern LoRaWAN_keys_t LoRaWAN_keys;

typedef union
{
	arr8b_t		byteArray;
	struct __attribute__ ((__packed__))
	{
		uint8_t		syncWord;
		uint8_t		data[6];
		uint8_t		mess : 6;
		uint8_t		stop : 2;
	} normOLD;
	struct  __attribute__((packed, scalar_storage_order("big-endian"))) 
	{
		uint32_t	syncWord : 8;
		uint32_t	data0 : 8;
		uint32_t	data1 : 8;
		uint32_t	data2 : 8;
		uint32_t	data3 : 8;
		uint32_t	data4 : 8;
		uint32_t	data5 : 8;
		uint8_t		stop : 2;
	} norm;
	struct  __attribute__((packed, scalar_storage_order("big-endian"))) 
	{
		uint32_t	pbits :2;
		uint32_t	syncWord : 8;
		uint32_t	data0 : 8;
		uint32_t	data1 : 8;
		uint32_t	data2 : 8;
		uint32_t	data3 : 8;
		uint32_t	data4 : 8;
		uint32_t	data5 : 8;
		uint8_t		stop : 2;
	} shift;
} RXBuffer_t;

extern RXBuffer_t RXBuffer;