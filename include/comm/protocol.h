/**
 * \file
 * 
 * \brief Descriptions and functions that relate to the communication protocol for the 
 * daisy chain.
 * 
 * Contains all the descriptions, defines, varialbes and functions that are directly related
 * to the communication prototol that is used in the daisy chain. This prototol is specific
 * to the ring lights and how they communicate, it has features that allow for indiviually 
 * addressable commands and generic broadcast commands.
 */

#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PROTOCOL_START_BYTE 0xFF
#define PROTOCOL_END_BYTE 0x00

#ifdef __cplusplus
}
#endif 

#endif