/**
 * A try to have golang style io interface in C
 * for use in memory constrained deep embedded systems
 */
#ifndef IO_H_
#define IO_H_

#include <stdint.h>
#include <stddef.h>

#if INTPTR_MAX == INT32_MAX
typedef int32_t xg_ssize_t;
#elif INTPTR_MAX == INT64_MAX
typedef int64_t xg_ssize_t;
#else
#error "Environment not 32 or 64-bit."
#endif

struct xg_io_reader {
	xg_ssize_t (*read)(uint8_t *dst, size_t n); 
};

struct xg_io_writer {
	xg_ssize_t (*write)(uint8_t *src, size_t n);
};

xg_ssize_t xg_io_copy(struct xg_io_writer *dst, struct xg_io_reader *src);

#endif /* IO_H_ */
