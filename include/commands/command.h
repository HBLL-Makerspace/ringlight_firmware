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

#define CMD_SET_CHN_COLOR 0x00
#define CMD_SET_CHN_INTENSITY 0x00
#define CMD_GET_CHN_COLOR 0x00
#define CMD_GET_CHN_INTENSITY 0x00

/**
 * \class
 * \brief Command class
 */
class Command {
    public:
        /**
         * \brief Command identifier.
         * 
         * Each command has a unique ID that is used to identify the command.
         */
        const uint8_t cmd;

        /**
         * \brief Data for the command.
         */
        uint8_t* data;

        /**
         * \brief Give the extended class a chance to process the data.
         * 
         * Each command has information encoded in the data array. Running this function gives the 
         * extended classes a chance to look at the array and get the information needed to fill
         * up their member variables so that execution happens.
         * 
         * \return Successful processing.
         * \retval true Was able to get all information
         * \retval false Failed to get all information
         */
        virtual bool processCommand();
};

#endif
