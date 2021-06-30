#ifndef FRAME_H__
#define FRAME_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct comm_frame {
  uint8_t id;
  uint8_t cmd;
  uint8_t* data;
  uint8_t checksum;
};

#ifdef __cplusplus
}
#endif

#endif
