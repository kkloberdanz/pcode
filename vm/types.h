#ifndef VM_TYPES_H
#define VM_TYPES_H

/*
 * Primitive types
 */

#include <inttypes.h>
#include <stdint.h>

#if defined(__GNUC__)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"

#elif defined(__clang__)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wlong-long"

#endif

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

enum {
    VM_WORD_SIZE = 8 /* bytes */
};

#if defined(__GNUC__)

#pragma GCC diagnostic pop

#elif defined(__clang__)

#pragma clang diagnostic pop

#endif

#endif /* VM_TYPES_H */
