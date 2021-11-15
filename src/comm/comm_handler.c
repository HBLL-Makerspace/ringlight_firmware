#include<drivers/usart.h>
#include<stdio.h>

#include<comm/frame.h>
#include<comm/comm_parser.h>
#include<comm/comm_handler.h>
#include<commands/commands.h>
#include<comm/protocol.h>

#define SIZE_OF_TRANSMISSION_BUFFER 10
//start byte, ringlight id, command id, end byte
#define BASE_TRANSMISSION_SIZE_OFFSET 4
#define END_BYTE_OFFSET 1

comm_frame frameBuffer[SIZE_OF_TRANSMISSION_BUFFER];
uint8_t frameBuffferCount;
uint8_t frameIndex;
uint8_t bufferIndexRead;
uint8_t bufferIndexWrite;
uint8_t currTransmissionLength;

bool thisHasFrame;
bool frameTransmissionOngoing;
comm_frame thisFrame;
comm_frame currTransmissionFrame;

void comm_handler_init() {
    thisHasFrame = 0;
    frameTransmissionOngoing = 0;
    frameIndex = 0;
    frameBuffferCount = 0;
    bufferIndexRead = 0;
    bufferIndexWrite = 0;
    currTransmissionLength = 0;
}

//if we do not have a current frame we are working through, grab a new frame
//go through each byte in the frame and send one per function call using USART_write()
//once we have sent the full frame, 
uint8_t comm_handler_send_frame() {

    //if we are not currently processing a frame, grab a new one
    if(!frameTransmissionOngoing){
        //if frame index is already at 0 (meaning we circled all the way around), make it the max val
        if(frameIndex == 0) {
            currTransmissionFrame = frameBuffer[SIZE_OF_TRANSMISSION_BUFFER - 1];
        }

        //we need to find the next frame to select which is index - 1
        else {
            currTransmissionFrame = frameBuffer[frameIndex - 1];
        }

        //get length of transmission frame for byte sender
        currTransmissionLength = BASE_TRANSMISSION_SIZE_OFFSET + command_get_from_id(currTransmissionFrame.cmd)->len;
    }

    //now that we for sure have a frame, we need to start sending it in bytes
    frameTransmissionOngoing = comm_handler_byte_sender();
    
    return 0;
}


bool comm_handler_byte_sender(){
    //keeps track of current frame and transmits one byte at a time
    //TODO

    //if length is greater than 0
    if(currTransmissionLength > 0) {
        currTransmissionLength--;

        //if we are in the first three bytes (before data)
        if((currTransmissionLength - END_BYTE_OFFSET - command_get_from_id(currTransmissionFrame.cmd)->len) >= 0){
            //allows me to see what part of the header we are on
            uint8_t headerIndex = currTransmissionLength - END_BYTE_OFFSET - command_get_from_id(currTransmissionFrame.cmd)->len;

            //send start byte
            if(headerIndex == 2){
                ;
                return true;
            }

            //send ringlight ID byte
            else if(headerIndex == 1){
                ;
                return true;
            }

            //send command ID
            else if(headerIndex == 0){
                ;
                return true;
            }
        }

        //else if we are currently sending data
        else if(currTransmissionLength - BASE_TRANSMISSION_SIZE_OFFSET > 0){
            //TODO add in data sending capabilities
        }

        //if we are at the end. send the end byte and return false that the transmission is over
        else if (currTransmissionLength == 0) {
            //send empty byte
            return false;
        }
    }

    //if we get here, we've exhausted our frame and need a new one
    return false;
}

void comm_handler_store_frame(comm_frame frame) {
    //if we still have room store the next frame, else we just dont store it
    if(frameBuffferCount < SIZE_OF_TRANSMISSION_BUFFER){
        frameBuffer[frameIndex] = frame;
        frameIndex++;
        //if index is beyond buffer size, reset it to 0
        if(frameIndex == SIZE_OF_TRANSMISSION_BUFFER){
            frameIndex = 0;
        }
    }
    //if we lose the frame, let em know
    else {
        printf("wasted frame: beyond transmission buffer size\n");
    }
}

uint8_t comm_handler_recieve_frame() {
    return 0;
}

comm_frame comm_handler_get_frame() {
    return comm_parser_get_frame();
}

comm_frame comm_handler_get_my_frame() {
    return thisFrame;
}

bool comm_handler_this_has_frame() {
    //if we have a frame to send, send it and then reset the flag. This is set up to only store one read frame at a time
    if(thisHasFrame){
        thisHasFrame = 0;
        return 1;
    }
    return 0;
}

uint8_t comm_handler_send_ack() {
    return 0;
}

uint8_t comm_handler_did_receive_frame() {
    return comm_parser_is_frame_available();
}


uint8_t comm_handler_tick() {
    static uint16_t timeout = 1;

    //if you are ready to read, please do
    if (USART_is_rx_ready()) {
        comm_parser_parse(USART_read());
        //if comm_parser_is_frame_available() is true, get the frame.
        if(comm_parser_is_frame_available()){
            comm_frame currFrame = comm_handler_get_frame();
            //printf("currFrame ID: %d\n", currFrame.id);
            //if frame is for you, set flag high so that main can transmit that frame and save that frame here.
            if(currFrame.id == 0){
                //printf("we got a frame!\n");
                thisHasFrame = 1;
                thisFrame = currFrame;
                //TODO if frame command is the identify command, if id, then also push it into store frame
            }
            //else frame is for someone else, decrement the ringlight ID, put it into 
            //the transmit buffer for sending and increment frameBufferCount to tell us that something needs to be sent out
            else {
                currFrame.id--;
                frameBuffferCount++;
                comm_handler_store_frame(currFrame);
                //printf("passed it on! new currFrame ID: %d\n", currFrame.id);

            }
        }
        timeout = 1;
    }
    //if ready to transmit and you have something to transmit, do it. Note: may need to be an else if
    if(USART_is_tx_ready() && (frameBuffferCount > 0)){
        //transmit using the WriteAByte() function

        ;
    }

    else
        timeout++;

    if (timeout == 0) {
        comm_parser_timeout();
    }
    return 0;
}
