#pragma once

#define new(type) allocate(sizeof(type))
#define new_shared(type) allocate_shared(sizeof(type))
