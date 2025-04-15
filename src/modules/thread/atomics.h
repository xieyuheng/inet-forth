#pragma once

#define relaxed_load(pointer) \
    atomic_load_explicit(pointer, memory_order_relaxed)
#define relaxed_store(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_relaxed)

#define acquire_load(pointer) \
    atomic_load_explicit(pointer, memory_order_acquire)

#define release_store(pointer, value) \
    atomic_store_explicit(pointer, value, memory_order_release)

#define atomic_add1(pointer) atomic_fetch_add(pointer, 1)
#define atomic_sub1(pointer) atomic_fetch_sub(pointer, 1)
