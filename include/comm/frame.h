#ifndef FRAME_H__
#define FRAME_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Frame structure
 * 
 * Each frame from the communication protocol contains the information needed
 * to determine which ring light the frame is for and what the command that the
 * ringlight needs to execute.
 * 
 * \note If you take a close look at struct, there is an array for the data associated with a
 * command. You will also notice that there is no information here that tells us how big the 
 * array is, that is becaseu the array size is dependent on the command. Each command has a 
 * set known data size.
 */
typedef struct comm_frame {
  /**
   * \brief ID of the frame.
   * 
   * Each ring light has a unique id. The id is just a sequential counting of each ring light.
   * So the ring light id will be set based on the connected order. If a ringlight is 3 then it 
   * is the 3rd connected ring light.
   */
  uint8_t id;

  /**
   * \brief Command that must be executed.
   * 
   * Commands control the ring lights, sending data and telling the ring light what to do with that data.
   * Commands can control the color, intensity and some other camera triggering. Events are also a part of
   * the command system that allows events to be controlled based on timings.
   */
  uint8_t cmd;

  /**
   * \brief Data for the command
   * 
   * Each command can have different amounts of data that the command needs to execute.
   * For example a command that changes channel 0 to a specific color, needs the channel
   * number and color to successfuly execute.
   */
  uint8_t* data;

  /**
   * \brief Checksum of the data
   * 
   * Makes sure that everything was trasnfered correctly by calculating the checksum.
   */
  uint8_t checksum;
} comm_frame;

#ifdef __cplusplus
}
#endif

#endif
