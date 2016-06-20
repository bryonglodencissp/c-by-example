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

#define stringify(x) #x

uint32_t generateHash(const char *string, size_t len)
{
	uint32_t hash = 0;
	for(size_t i = 0; i < len; ++i)
		hash = 65599 * hash + string[i];
	return hash ^ (hash >> 16);
}

#define DECL_STRUCT_FINGERPRINT \
	struct { \
		uint32_t id; \
		const char *name; \
		const char *body; \
	} __meta;

	struct __meta {
		uint32_t id;
		const char *name;
		const char *body;
	};

#define DECL_STRUCT_MEMBER(type, name) type name;

#define DEF_STRUCT(sname, body) \
	static const char *sname##__name = stringify(sname); \
	static const char *sname##__body = stringify(body); \
	struct sname { \
		DECL_STRUCT_FINGERPRINT \
		body \
	}; \
	typedef struct sname sname##_t;

#define decl_var(sname, x) struct sname x; x.__meta.id = generateHash(stringify(sname), sizeof(stringify(s))); x.__meta.name = sname##__name; x.__meta.body = sname##__body;
#define decl_get_id(x) x.__meta.id
#define decl_get_name(x) x.__meta.name
#define decl_get_body(x) x.__meta.body

struct str {
	const char *str;
	size_t len;
};

enum __member_type {
	MEMBER_TYPE_UNKNOWN  = 0,
	MEMBER_TYPE_SIZE_T   = 1,
	MEMBER_TYPE_CHAR_PTR = 2
};

static const char *__member_type_str[] = {
	"unknown",
	"size_t",
	"char*"
};

struct __member_meta {
	size_t id;
	size_t offset;
	size_t len;
	enum __member_type type;
};

static inline const char *decl_get_type_str(enum __member_type type)
{
	return __member_type_str[type];
}

void decl_get_member_meta(const char *member, const char *__meta_body, struct __member_meta *meta)
{
	const char *cur   = __meta_body;
	const char *token = __meta_body;
	size_t offset = 0;
	meta->id = 0;
	meta->offset = 0;
	meta->len = 0;
	meta->type = MEMBER_TYPE_UNKNOWN;
	enum __member_type type = MEMBER_TYPE_UNKNOWN;
	struct str mtype = { .str = NULL };
	struct str mname = { .str = NULL };

	while (*cur) {
		if (*cur == ';' || *cur == ' ') {
			size_t len = cur - token;
			size_t pos;
			if (token != __meta_body)
				pos = cur - __meta_body - len;
			else
				pos = 0;

			if (len) {
				// Fetch member type
				if (!mtype.str) {
					mtype.str = token;
					mtype.len = len;

					mname.str = NULL;
					mname.len = 0;

					if (memcmp(mtype.str, "size_t", mtype.len) == 0) {
						type = MEMBER_TYPE_SIZE_T;
						meta->len = sizeof(size_t);
					} else if (memcmp(mtype.str, "char*", mtype.len) == 0) {
						type = MEMBER_TYPE_CHAR_PTR;
						meta->len += sizeof(char *);
					}

					if (meta->id && mtype.str != __meta_body)
						offset += meta->len;

					meta->id++;
				} else {
					mname.str = token;
					mname.len = len;
				}

				// Fetch member name
				if (mname.str) {
					if (strlen(member) == mname.len && memcmp(member, mname.str, mname.len) == 0) {
						meta->type = type;
						break;
					}
					mtype.str = NULL;
				}
			}
			token = cur + 1;
		}
		cur++;
	}

	if (meta->id)
		meta->offset = offset;
}

enum __member_type decl_get_type(void *s, const char *member)
{
	struct __meta *meta = (struct __meta *)s;
	struct __member_meta mmeta;
	decl_get_member_meta(member, meta->body, &mmeta);
	return mmeta.type;
}

void decl_print_member(void *s, const char *member)
{
	struct __meta *meta = (struct __meta *)s;
	struct __member_meta mmeta;
	decl_get_member_meta(member, meta->body, &mmeta);

	if (mmeta.type == MEMBER_TYPE_UNKNOWN) {
		printf("--- member \"%s\" -> unknown\n", member);
		return;
	}

	printf("--- member \"%s\"\n", member);
	printf("%s offset: %zu\n", member, mmeta.offset);
	printf("%s len: %zu\n", member, mmeta.len);
	printf("%s type(%u): \"%s\"\n", member, mmeta.type, decl_get_type_str(mmeta.type));
	if (mmeta.type == MEMBER_TYPE_SIZE_T)
		printf("%s: %zu\n", member, *(size_t *)((uintptr_t)s + sizeof(*meta) + mmeta.offset));
}


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
