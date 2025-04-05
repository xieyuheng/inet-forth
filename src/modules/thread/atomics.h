#pragma once

#define relaxed_load(pointer) \
    atomic_load_explicit(pointer, memory_order_relaxed)
#define acquire_load(pointer) \
    atomic_load_explicit(pointer, memory_order_acquire)

#define relaxed_store(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_relaxed)
#define release_store(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_release)
