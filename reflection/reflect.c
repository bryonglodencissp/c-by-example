/**
 * Hacking reflection type system with macros (ANSI C99)
 * Outputs:
 * ```
 * decl_get_id(coord, c): 2818093677
 * decl_get_name(coord, c): coord
 * decl_get_body(coord, c): size_t x; size_t y;
 * x offset: 0
 * decl_get_id(star, c): 537904285
 * decl_get_name(star, s): star
 * decl_get_body(coord, c): float x; float y;
 * {id:2818093677,"name":"coord"}
 * {id:537904285,"name":"star"}
 * ```
 * Preprocessor hash calculator from: http://www.heeden.nl/statichashc.htm
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "reflect.h"

DEF_STRUCT(coord,
	DECL_STRUCT_MEMBER(size_t, monkey)
	DECL_STRUCT_MEMBER(size_t, x)
	DECL_STRUCT_MEMBER(size_t, y)
)

DEF_STRUCT(star,
	DECL_STRUCT_MEMBER(char*, name)
	DECL_STRUCT_MEMBER(coord_t*, coord)
)

DEF_STRUCT(my1337,
	DECL_STRUCT_MEMBER(char*, str)
)

int main(void)
{
	decl_var(coord, c);

	printf("decl_get_id(c): %u\n", decl_get_id(c));
	printf("decl_get_name(c): %s\n", decl_get_name(c));
	printf("decl_get_body(c): \"%s\"\n", decl_get_body(c));
	printf("decl_get_type(c, \"monkey\"): \"%s\"\n", decl_get_type_str(decl_get_type(&c, "monkey")));
	printf("decl_get_type(c, \"blaat\"): \"%s\"\n", decl_get_type_str(decl_get_type(&c, "blaat")));

	c.monkey = 1337;
	c.x      = 666;
	c.y      = 777;

	decl_print_member(&c, "monkey");
	decl_print_member(&c, "x");
	decl_print_member(&c, "y");
	decl_print_member(&c, "blaat");

	return 0;
}
