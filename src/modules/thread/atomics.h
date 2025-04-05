#pragma once

// This prevents the compiler from performing certain optimizations,
// ensuring that the program always reads
// the most up-to-date value of the variable.

#define relaxed_load(pointer) \
    atomic_load_explicit(pointer, memory_order_relaxed)
#define acquire_load(pointer) \
    atomic_load_explicit(pointer, memory_order_acquire)

#define relaxed_store(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_relaxed)
#define release_store(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_release)
