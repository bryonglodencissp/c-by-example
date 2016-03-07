#include "io.h"

xg_ssize_t xg_io_copy(struct xg_io_writer *dst, struct xg_io_reader *src)
{
	uint8_t b[256];

	if (!dst->write) {
		while (src->read(b, sizeof(b)) >= 1)
			;
	}

	while (src->read(b, sizeof(b)) >= sizeof(b)) {
		if (dst->write(b, sizeof(b)) < sizeof(b))
			return 0;
	}

	return 0;
}
