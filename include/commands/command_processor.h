#ifndef COMMAND_PROCESSOR_H__
#define COMMAND_PROCESSOR_H__

#include<stdint.h>
#include<comm/frame.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t cmd_process_frame(comm_frame data);

#ifdef __cplusplus
}
#endif

#endif