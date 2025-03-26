#pragma once

// This prevents the compiler from performing certain optimizations,
// ensuring that the program always reads
// the most up-to-date value of the variable.

#define load_relaxed(pointer) \
    atomic_load_explicit(pointer, memory_order_relaxed)
#define load_acquire(pointer) \
    atomic_load_explicit(pointer, memory_order_acquire)

#define store_relaxed(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_relaxed)
#define store_release(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_release)
