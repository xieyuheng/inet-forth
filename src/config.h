#pragma once

#define INET_FORTH_VERSION "0.1.0"

#define NODE_MAX_ARITY 16
#define NODE_ALLOCATOR_BATCH_SIZE 1024
#define NODE_ALLOCATOR_CACHE_SIZE (64 * 1024)
#define WORKER_TASK_QUEUE_SIZE (1024 * 1024)

// In a normal build, all debug flag should be off.

// To detect `step_task` in two threads
// accessing the same task.
#define DEBUG_TASK_LOCK 0

// To detect `worker_reconnect_node`
// and `worker_disconnect_node` in two threads
// accessing the same node.
#define DEBUG_NODE_LOCK 0

// To exclude problem with `node_allocator_t`.
// - To exclude ABA problem
//   we do not even free the memory of node.
// - When `node_allocator_t` is not used,
//   we can not print the net, so printing is
//   also disabled when this flag is on.
#define DEBUG_NODE_ALLOCATOR_DISABLED 0

#define DEBUG_WORK_STEALING_DISABLED 0

#define DEBUG_TASK_LOG 0
#define DEBUG_STEP_LOG 0

extern bool single_threaded_flag;
