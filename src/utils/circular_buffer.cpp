#include<utils/circular_buffer.h>
#include<stdlib.h>

template<typename T>
CircularBuffer<T>::CircularBuffer(size_t max_size) {
    buffer = malloc((max_size + 1) * sizeof(T));
    head = 0;
    tail = 0;
    max_size = max_size;
}

template<typename T>
CircularBuffer<T>::~CircularBuffer() {
    free(buffer);
}

template<typename T>
void CircularBuffer<T>::enqueue(T item) {
    if (isEmpty())
        return;
    
    buffer[tail] = item;
    tail = (tail + 1) % max_size;
}

template<typename T>
T CircularBuffer<T>::dequeue() {
    if (isEmpty())
        return NULL;
    
    T item = buffer[head];
    T empty;
    buffer[head] = empty_item;
    head = (head + 1) % max_size;

    return item;
}

template<typename T>
T CircularBuffer<T>::front() {
    return buffer[head];
}

template<typename T>
bool CircularBuffer<T>::isEmpty() {
    return head == tail;
}

template<typename T>
bool CircularBuffer<T>::isFull() {
    return tail == (head - 1) % max_size;
}

template<typename T>
size_t CircularBuffer<T>::size() {
    if (tail >= head)
        return tail - head;
    return max_size - head - tail;
}