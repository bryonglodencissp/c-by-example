#define stringify(x) #x

#define HASH_INT_LEN(s)			((sizeof(s)/sizeof(s[0])) - 1)
#define HASH_INT_1(s,i,x)		(x*65599u+(unsigned char)s[(i)<HASH_INT_LEN(s)?HASH_INT_LEN(s)-1-(i):HASH_INT_LEN(s)])
#define HASH_INT_4(s,i,x)		HASH_INT_1(s,i,HASH_INT_1(s,i+1,HASH_INT_1(s,i+2,HASH_INT_1(s,i+3,x))))
#define HASH_INT_16(s,i,x)		HASH_INT_4(s,i,HASH_INT_4(s,i+4,HASH_INT_4(s,i+8,HASH_INT_4(s,i+12,x))))
#define HASH_INT_64(s,i,x)		HASH_INT_16(s,i,HASH_INT_16(s,i+16,HASH_INT_16(s,i+32,HASH_INT_16(s,i+48,x))))

#define HASH_S64(s)	((unsigned int)(HASH_INT_64(s,0,0)^(HASH_INT_64(s,0,0)>>16)))	///< Hash value from string with a max length of 64 bytes.
#define HASH_S16(s)	((unsigned int)(HASH_INT_16(s,0,0)^(HASH_INT_16(s,0,0)>>16)))	///< Hash value from string with a max length of 16 bytes.

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
	static const uint32_t sname##__id = HASH_S64(stringify(struct sname##_t)); \
	static const char *sname##__name = stringify(sname); \
	static const char *sname##__body = stringify(body); \
	struct sname { \
		DECL_STRUCT_FINGERPRINT \
		body \
	}; \
	typedef struct sname sname##_t;

#define decl_var(sname, x) struct sname x; x.__meta.id = sname##__id; x.__meta.name = sname##__name; x.__meta.body = sname##__body;
#define decl_get_id(x) x.__meta.id
#define decl_get_name(x) x.__meta.name
#define decl_get_body(x) x.__meta.body

struct str {
	char *str;
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
#define MEMBER_TYPE_STR(type) __member_type_str[(type)]

struct __member_meta {
	size_t id;
	size_t offset;
	size_t len;
	enum __member_type type;
};

void decl_get_member_meta(const char *member, const char *__meta_body, struct __member_meta *meta)
{
	char *cur   = __meta_body;
	char *token = __meta_body;
	size_t offset = 0;
	meta->id = 0;
	meta->offset = 0;
	meta->len = 0;
	meta->type = MEMBER_TYPE_UNKNOWN;
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
						meta->type = MEMBER_TYPE_SIZE_T;
						meta->len = sizeof(size_t);
					} else if (memcmp(mtype.str, "char*", mtype.len) == 0) {
						meta->type = MEMBER_TYPE_CHAR_PTR;
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
					if (strlen(member) == mname.len && memcmp(member, mname.str, mname.len) == 0)
						break;
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
