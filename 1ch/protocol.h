#ifndef ___PROTOCOL_H___
#define ___PROTOCOL_H___


//---------------------------------------
// PC --> Master Protocol definition
//---------------------------------------
// BYTE0: STX
// BYTE1: target master address
// BYTE2: command
// BYTE3: target slave address
// BYTE4:
// BYTE5:
// BYTE6: checksum
//---------------------------------------
#define PC_PROTOCOL_LEN			7
#define PCMAIN_STX		        0xA5

#define CMD_WELCOM			0xAA
#define CMD_PORT_EVT			0x01
#define CMD_PORT_RESET			0x02
#define CMD_ID_RESET                    0x11
#define CMD_ID_REQ                      0x12
#define CMD_ID_SET                      0x13

#define CMD_PORT_ALL_RESET		0xAA

//---------------------------------------
// Master --> PC Protocol definition
//---------------------------------------
// BYTE0: STX
// BYTE1: target master address
// BYTE2: command
// BYTE3: target slave address
// BYTE4:
// BYTE5:
// BYTE6: checksum
//---------------------------------------
#define MAINPC_STX				0xA6

//---------------------------------------
// Master --> Slave Protocol definition
//---------------------------------------
// BYTE0: STX
// BYTE1: target slave address
// BYTE2: command
// BYTE3:
// BYTE4: checksum
//---------------------------------------
#define SUB_PROTOCOL_LEN		        5
#define MULTI_ID				0xFF
#define MAINSUB_STX				0xB5


//---------------------------------------
// Slave --> Master Protocol definition
//---------------------------------------
// BYTE0: STX
// BYTE1: target slave address
// BYTE2: command
// BYTE3: data
// BYTE4: checksum
//---------------------------------------
#define SUBMAIN_STX				0xB6


#endif	//___PROTOCOL_H___

