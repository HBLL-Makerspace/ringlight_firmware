#ifndef COMM_CONVERTERS_H__
#define COMM_CONVERTERS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union floattobytes {
    float value;
    uint8_t data[4];
};

typedef union doubletobytes {
    double value;
    uint8_t data[8];
};

typedef union uint16tobytes {
    uint16_t value;
    uint8_t data[2];
};

typedef union uint32tobytes {
    uint16_t value;
    uint8_t data[4];
};

typedef union uint64tobytes {
    uint16_t value;
    uint8_t data[8];
};

#ifdef __cplusplus
}
#endif

#endif
