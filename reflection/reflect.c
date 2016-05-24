/**
 * Hacking reflection type system with macros
 * Outputs:
 * ```
 * {id:2818093677,"name":"coord"}
 * {id:537904285,"name":"star"}
 * ```
 * Preprocessor hash calculator from: http://www.heeden.nl/statichashc.htm
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "reflect.h"

DEF_STRUCT(coord,
	DECL_STRUCT_MEMBER(size_t, x)
	DECL_STRUCT_MEMBER(size_t, y)
)

DEF_STRUCT(star,
	DECL_STRUCT_MEMBER(float, x)
	DECL_STRUCT_MEMBER(float, y)
)

void serialize(void *s)
{
	uint32_t *__id = s;
	const char **__name = (void *)((const char *)s + sizeof(__id));
	printf("{id:%" PRIu32 ",\"name\":\"%s\"}\n", *__id, *__name);
}

int main(void)
{
	decl_var(coord, c);
	decl_var(star, s)

	printf("decl_get_id(coord, c): %u\n", decl_get_id(c));
	printf("decl_get_name(coord, c): %s\n", decl_get_name(c));

	printf("decl_get_id(star, c): %u\n", decl_get_id(s));
	printf("decl_get_name(star, s): %s\n", decl_get_name(s));

	serialize(&c);
	serialize(&s);

	return 0;
}
