#pragma once

bool pointer_is_8_bytes_aligned(void *pointer);

// clear memory to zero
void *allocate(size_t size);
void *allocate_many(size_t size, size_t unit_size);
void *allocate_pointers(size_t size);

// clear new memory to zero
void *reallocate(void *pointer, size_t old_size, size_t new_size);
void *reallocate_many(void *pointer, size_t old_size, size_t new_size, size_t unit_size);
void *reallocate_pointers(void *pointer, size_t old_size, size_t new_size);

bool pointer_is_page_aligned(void *pointer);

// aligned to cache line to avoid false sharing
void *allocate_shared(size_t size);
