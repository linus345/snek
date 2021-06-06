#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <SDL2/SDL.h>

typedef struct {
    void *data_start;
    void *data_end;
    void *write_ptr;
    void *read_ptr;
    int capacity;
    int count;
    size_t size;
    SDL_mutex *mutex;
} Circular_Buffer;

Circular_Buffer *init_buffer(int capacity, int size);
void free_buffer(Circular_Buffer *buf);
int write_to_buffer(Circular_Buffer *buf, void *data);
int read_from_buffer(Circular_Buffer *buf, void *data);
bool buf_is_empty(Circular_Buffer *buf);
bool buf_is_full(Circular_Buffer *buf, void *next);

#endif
