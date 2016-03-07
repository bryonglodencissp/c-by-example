#include "io.h"

xg_ssize_t xg_io_copy(struct xg_io_writer *dst, struct xg_io_reader *src)
{
	uint8_t b;

	while (src->read(&b, 1) >= 1) {
		if (dst->write(&b, 1) != 1)
			return 0;
	}

	return 0;
}
