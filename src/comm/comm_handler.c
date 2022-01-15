#include<drivers/usart.h>
#include<stdio.h>

#include<comm/frame.h>
#include<comm/comm_parser.h>
#include<comm/comm_handler.h>
#include<commands/commands.h>
#include<comm/protocol.h>
#include<util/delay.h>


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

    //printf("frameTransmissionOngoing: %d", frameTransmissionOngoing);
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
    // _delay_ms(100);
    // printf("currTransmissionLength: %d and corresponding data: ", currTransmissionLength);
    // _delay_ms(100);
    //now that we for sure have a frame, we need to start sending it in bytes
    frameTransmissionOngoing = comm_handler_byte_sender();
    
    return 0;
}

bool comm_handler_byte_sender(){
    //keeps track of current frame and transmits one byte at a time
    //TODO
    //_delay_ms(100);
    //printf("currTransmissionLength before subtraction: %d\n", currTransmissionLength);
    //_delay_ms(100);

    //if length is greater than 0
    if(currTransmissionLength > 0) {
        currTransmissionLength--;
        //printf("currTransmissionLength after subtraction: %d\n", currTransmissionLength);
        // printf("* \n");
        // _delay_ms(100);
        //IMPORTANT: if debugging with serial, change start byte from 255 to 0 for utf-8 to decode correctly.
        const uint8_t start_byte = 255;
        const uint8_t end_byte = 0;

        

        //if we are in the first three bytes (before data)
        if((currTransmissionLength - END_BYTE_OFFSET - command_get_from_id(currTransmissionFrame.cmd)->len) < 255){
            //allows me to see what part of the header we are on
            // _delay_ms(1000);
            uint8_t headerIndex = currTransmissionLength - END_BYTE_OFFSET - command_get_from_id(currTransmissionFrame.cmd)->len;
            //printf("headerIndex: %d\n", headerIndex);
            //_delay_ms(1000);
            

            //send start byte
            if(headerIndex == 2){
                // printf("start \n");
                // _delay_ms(1000);
                USART_write(start_byte);
                return true;
            }

            //send ringlight ID byte
            else if(headerIndex == 1){
                // printf("ringlight ID: %d\n", currTransmissionFrame.id);
                // _delay_ms(1000);
                USART_write((const uint8_t) currTransmissionFrame.id);
                return true;
            }

            //send command ID
            else if(headerIndex == 0){
                // printf("command ID: %d\n", currTransmissionFrame.cmd);
                // _delay_ms(1000);
                USART_write((const uint8_t) currTransmissionFrame.cmd);
                return true;
            }
        }

        //else if we are currently sending data bytes
        else if(currTransmissionLength > 0){

            //returns the index for the data we need to access
            uint8_t currDataIndex = command_get_from_id(currTransmissionFrame.cmd)->len - currTransmissionLength;
            //gets pointer to data
            uint8_t* currData = currTransmissionFrame.data;
            currData = currData + currDataIndex;

            //access data at current index and send it
            // printf("data: %d\n", *currData);
            // _delay_ms(1000);
            USART_write((const uint8_t) *currData);
            return true;

        }

        //if we are at the end. send the end byte and return false that the transmission is over
        else if (currTransmissionLength == 0) {
            USART_write(end_byte);//send empty byte
            // printf("end byte: %d\n", PROTOCOL_END_BYTE);
            // _delay_ms(1000);
            frameBuffferCount--;
            // printf("finished transmitting frame!\n");
            // _delay_ms(100);
            return false;
        }
        // printf("Your if else block is messed up :(\n");
        // _delay_ms(100);
    }

    //if we get here, we've exhausted our frame and need a new one
    //printf("No frame to transmit!\n");
    // _delay_ms(100);
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
        //printf("wasted frame: beyond transmission buffer size\n");
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
                // printf("passed it on! new frameBufferCount: %d\n", frameBuffferCount);
                // _delay_ms(100);

            }
        }
        timeout = 1;
    }
    //if ready to transmit and you have something to transmit, do it. Note: may need to be an else if
    else if(USART_is_tx_ready() && (frameBuffferCount > 0)){
        //transmit using the WriteAByte() function
        // printf("sending frames\n");
        // _delay_ms(100);
        comm_handler_send_frame();
    }

    else
        timeout++;

    if (timeout == 0) {
        comm_parser_timeout();
    }
    return 0;
}
