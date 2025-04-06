#pragma once

#define new(type) allocate(sizeof(type))
#define new_page_aligned(type) allocate_page_aligned(sizeof(type))
