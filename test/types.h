#ifndef _TYPES_H_
#define _TYPES_H_

#include <cstdint>

using int32 = int32_t;
using uint32 = uint32_t;

#define DISALLOW_COPY_AND_ASSIGN(Class) \
	Class(const Class &) = delete; \
	Class & operator=(const Class &) = delete;

#endif // _TYPES_H_