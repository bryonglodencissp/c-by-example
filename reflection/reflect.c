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

#define HASH_INT_LEN(s)			((sizeof(s)/sizeof(s[0])) - 1)
#define HASH_INT_1(s,i,x)		(x*65599u+(unsigned char)s[(i)<HASH_INT_LEN(s)?HASH_INT_LEN(s)-1-(i):HASH_INT_LEN(s)])
#define HASH_INT_4(s,i,x)		HASH_INT_1(s,i,HASH_INT_1(s,i+1,HASH_INT_1(s,i+2,HASH_INT_1(s,i+3,x))))
#define HASH_INT_16(s,i,x)		HASH_INT_4(s,i,HASH_INT_4(s,i+4,HASH_INT_4(s,i+8,HASH_INT_4(s,i+12,x))))
#define HASH_INT_64(s,i,x)		HASH_INT_16(s,i,HASH_INT_16(s,i+16,HASH_INT_16(s,i+32,HASH_INT_16(s,i+48,x))))
#define HASH_INT_256(s,i,x)		HASH_INT_64(s,i,HASH_INT_64(s,i+64,HASH_INT_64(s,i+128,HASH_INT_64(s,i+192,x))))

#define HASH_S256(s)	((unsigned int)(HASH_INT_256(s,0,0)^(HASH_INT_256(s,0,0)>>16)))	///< Hash value from string with a max length of 256 bytes.
#define HASH_S64(s)	((unsigned int)(HASH_INT_64(s,0,0)^(HASH_INT_64(s,0,0)>>16)))	///< Hash value from string with a max length of 64 bytes.
#define HASH_S16(s)	((unsigned int)(HASH_INT_16(s,0,0)^(HASH_INT_16(s,0,0)>>16)))	///< Hash value from string with a max length of 16 bytes.

#define DECL_MEMBERS \
	uint32_t __id; \
	const char *__name;

#define stringify(x) #x

#define DEF_STRUCT(sname, body) \
	struct sname { \
		DECL_MEMBERS \
		body \
	}; \
	static const char *sname##__name = stringify(sname);

DEF_STRUCT(coord,
	size_t x;
	size_t y;
)

DEF_STRUCT(star,
	float x;
	float y;
)

#define decl(sname, x) struct sname x; x.__id = HASH_S64(stringify(sname)); x.__name = sname##__name;

void serialize(void *s)
{
	uint32_t *__id = s;
	const char **__name = (void *)((const char *)s + sizeof(__id));
	printf("{id:%" PRIu32 ",\"name\":\"%s\"}\n", *__id, *__name);
}

int main(void)
{
	decl(coord, c);
	decl(star, s)

	serialize(&c);
	serialize(&s);
	return 0;
}
