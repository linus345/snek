#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "circular_buffer.h"

Circular_Buffer *init_buffer(int capacity, int size)
{
    // allocate memory for entire struct
    Circular_Buffer *buf = malloc(sizeof(Circular_Buffer));
    // allocate memory for actual buffer data
    void *data_start = malloc(size * capacity);

    // initialize struct
    buf->data_start = data_start;
    buf->data_end = (char *) data_start + (capacity+1) * size;
    buf->read_ptr = data_start;
    buf->write_ptr = data_start;
    buf->capacity = capacity+1;
    buf->count = 0;
    buf->size = size;

    // create mutex for reading/writing from/to shared buffer
    buf->mutex = SDL_CreateMutex();
    if(!buf->mutex) {
        fprintf(stderr, "Error: SDL_CreateMutex: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return buf;
}

void free_buffer(Circular_Buffer *buf)
{
    SDL_DestroyMutex(buf->mutex);
    free(buf);
}

int write_to_buffer(Circular_Buffer *buf, void *data)
{
    SDL_LockMutex(buf->mutex);
    void *next = ((char *) buf->write_ptr) + buf->size;
    if(next == buf->data_end) {
        next = buf->data_start;
    }
    // check if buffer is full
    if(buf_is_full(buf, next)) {
        SDL_UnlockMutex(buf->mutex);
        // return code to indicate that buffer is full
        return -1;
    }
    // store data in buffer
    memcpy(buf->write_ptr, data, buf->size);
    // increment write pointer
    buf->write_ptr = next;
    SDL_UnlockMutex(buf->mutex);
    // return code to indicate that write was a success
    return 0;
}

int read_from_buffer(Circular_Buffer *buf, void *data)
{
    printf("1\n");
    SDL_LockMutex(buf->mutex);
    printf("2\n");
    // check if buffer is empty
    if(buf_is_empty(buf)) {
        SDL_UnlockMutex(buf->mutex);
        // return code to indicate that buffer is empty
        return -1;
    }
    printf("3\n");
    void *next = ((char *) buf->read_ptr) + buf->size;
    if(next == buf->data_end) {
        next = buf->data_start;
    }
    printf("4\n");
    // get data from buffer
    printf("read_ptr: %p\n", buf->read_ptr);
    printf("write_ptr: %p\n", buf->write_ptr);
    printf("data ptr: %p\n", data);
    memcpy(data, buf->read_ptr, buf->size);
    printf("5\n");
    // increment read pointer
    buf->read_ptr = next;
    SDL_UnlockMutex(buf->mutex);
    // return code to indicate that read was successful
    return 0;
}

bool buf_is_empty(Circular_Buffer *buf)
{
    return buf->read_ptr == buf->write_ptr;
}

bool buf_is_full(Circular_Buffer *buf, void *next)
{
    return next == buf->read_ptr;
}
