/**
 * \file
 * 
 * \brief Command class for the ring light communication protocol.
 * 
 * Command class is a base class for the commnad system for the ring lights.
 * Commands are ways to control aspects of the ringlight like conrolling the
 * color, light intensity etc.
 * 
 * New commands are created by extending either this class (the base command) or
 * by extending another command.
 * 
 * \author Ben Brenkman
 * \date 30 June 2021
 */

#ifndef COMMAND_BASE_H__
#define COMMAND_BASE_H__

#include<stdint.h>
#include<stdlib.h>

#define CMD_SET_CHN_COLOR 0x01
#define CMD_SET_CHN_INTENSITY 0x00
#define CMD_GET_CHN_COLOR 0x00
#define CMD_GET_CHN_INTENSITY 0x00

/**
 * \class
 * \brief Command class
 */
typedef struct Command {
    /**
     * \brief Command identifier.
     * 
     * Each command has a unique ID that is used to identify the command.
     */
    const uint8_t cmd;

    /**
     * \brief How many bytes in the command.
     * 
     * The command parser needds to know how much data to expect from an incoming data
     * stream so that it can extract the command data.
     */
    const uint16_t len;

    /**
     * \brief Give the command a chance to process the data.
     * 
     * Each command has information encoded in the data array. Running this function gives the 
     * command a chance to look at the array and get the information needed to fill up their 
     * member variables so that execution happens.
     * 
     * \return Successful processing.
     * \retval true Was able to get all information
     * \retval false Failed to get all information
     */
    uint8_t (*processCommand) (uint8_t* data);
};

/**
 * \brief Gets the command struct based on the id
 * 
 * Commands are stored as static structs in memory, a switch statement is used to retrieve
 * the struct from memory and return the command.
 * 
 * \param [in] id Id of the command that you are trying to find.
 * 
 * \return Pointer to command
 * \retval Pointer Found command
 * \retval \c NULL Failed to find the command with specified id
 */
Command* command_get_from_id(uint8_t id) {
    switch (id)
    {
    case 0x00:
        return NULL;
    default:
        return NULL;
    }
}

#endif
