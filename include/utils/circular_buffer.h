/**
 * \file
 * \brief Circular Buffer implemented in cpp
 * 
 * A circualr buffer that is implemented in cpp, careful of the size since
 * most microcontrollers do not have a ton of ram. So the smaller the buffer
 * the better. 
 * 
 * The maximum size of the this particular circular buffer is
 * limitted to the max size of the type size_t, since this is platform
 * independent care must be taken to be certian that the size that is specified
 * will fit into the size_t type. For AVR's the typical size of the size_t type
 * is 16 bits or a uint16_t, but this is not guarenteed. The size of size_t is
 * the same as the largest possible return value of sizeof.
 * 
 * \author Ben Brenkman
 * \date 30 June 2021
 */

#include<stdlib.h>

/**
 * \class
 * \brief Circular Buffer implemented in cpp
 * 
 * A templated circular buffer that can handle most data types. The template T
 * is the type of the buffer. There is no specific type that it is restricted too
 * 
 * \warning Has not been tested with all common data types. Only with uint8_t 
 */
template<typename T> 
class CircularBuffer {
    private:
        /** Where the buffer data is stored. Memory is self managed. */
        T* buffer;

        /**
         * Where is start of the buffer is in the ciruclar buffer.
         * This is defining the first element in the buffer. 
         */
        size_t head;

        /**
         * Where the end of the buffere is or the last element. 
         */ 
        size_t tail;

        /**
         * Maximum size of the circular buffer.
        */
        size_t max_size;

        T empty_item;
    public:
        CircularBuffer(size_t max_size);
        ~CircularBuffer();

        /**
         * Add an item into the buffer. If the buffer is full it will not
         * add the item.
         * 
         * \param[in] item The item to add to the buffer.
         */
        void enqueue(T item);

        /**
         * Remove an item form the back of the buffer and return it.
         *
         * \return The item from the front of the buffer.
         */
        T dequeue();

        /**
         * Get the item currently at the begingin of the buffer. Does not remove it.
         * 
         * \return The item from the front of the buffer.
         */
        T front();

        /**
         * Checks if the buffer is empty.
         * 
         * \return Buffer empty state
         * \retval true The buffer is empty.
         * \retval false The buffer is not empty.
         */
        bool isEmpty();

        /**
         * Checks if the buffer is full.
         * 
         * \return Buffer fill state
         * \retval true The buffer is full
         * \retval false The buffer is not full
         */
        bool isFull();

        /**
         * Gets the size of the buffer, or how many items are in the buffer.
         * 
         * \return The number of items in the buffer.
         */
        size_t size();
};