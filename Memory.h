#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <assert.h>

#define KILO (1024LL)
#define MEGA (KILO * 1024LL)
#define GIGA (MEGA * 1024LL)

typedef struct Memory {
    size_t capacity;
    size_t size;
    uint8_t *buffer;
} Memory;

Memory memory;

static
void *memory_alloc(Memory *memory, size_t size) {
    assert(memory);
    assert(memory->size + size <= memory->capacity);

    void *result = memory->buffer + memory->size;
    memory->size = memory->size + size;

    return result;
}

static
void memory_free(Memory *memory, size_t size) {
    assert(memory);
    assert(memory->size >= size);
    
    memory->size   -= size;
    memory->buffer -= size;
}

static
void memory_copy(void *dest, const void *src, size_t bytes) {
    assert(dest && src && bytes);
    char *ndest = (char *) dest;
    char *nsrc  = (char *) src;
    for (size_t i = 0; i < bytes; i++) {
        *(ndest + i) = *(nsrc + i);
    }
}

#endif // MEMORY_H 
