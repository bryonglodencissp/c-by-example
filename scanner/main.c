#include <stdio.h>
#include <stdlib.h>

const char *keywords[] = {
	"if", "for"
};

char text[] = "int main(void) { \
	const char text[] = \"Hello world\";\
	if (true) {\
		for (int i = 0; i < 10; i++) {\n\
			printf(\"Hello w%crld\", \'o\');\
		}\
	}\
}";

#define TOKEN_DECL(enumerator, token, string)
#define TOKEN_DECL_LIST                                             \
	TOKEN_DECL(TT_UNKNOWN,           '\0', "unknown")           \
	TOKEN_DECL(TT_TEXT,              '\0', "text")              \
	TOKEN_DECL(TT_TAB,               '\t', "tab")               \
	TOKEN_DECL(TT_NEWLINE,           '\n', "newline")           \
	TOKEN_DECL(TT_LITERAL,           '"',  "literal")           \
	TOKEN_DECL(TT_BRACKET_OPEN,      '{',  "bracket open")      \
	TOKEN_DECL(TT_BRACKET_CLOSE,     '}',  "bracket close")     \
	TOKEN_DECL(TT_PARENTHESES_OPEN,  '(',  "parenthesis open")  \
	TOKEN_DECL(TT_PARENTHESES_CLOSE, ')',  "parenthesis close")
#undef TOKEN_DECL

#define TOKEN_DECL(enumerator, token, string) enumerator,
enum token_types {
TOKEN_DECL_LIST
};
#undef TOKEN_DECL

#define TOKEN_DECL(enumerator, token, string) string,
static const char *token_names[] = {
TOKEN_DECL_LIST
};
#undef TOKEN_DECL

struct token {
	enum token_types type;
	char *value;
	size_t len;
	size_t cap;
	struct token *next;
};

struct token *token_new(struct token **prev)
{
	struct token *new;
	struct token *last = NULL;

	if (*prev) {
		struct token *cur = *prev;
		last = cur;
		while (cur) {
			if (cur->next)
				last = cur->next;
			cur = cur->next;
		}
	}

	new = calloc(sizeof(struct token), 1);
	if (new) {
		new->type  = TT_UNKNOWN;
		new->value = NULL;

		if (last)
			last->next = new;
		else
			*prev = new;
	}

	return new;
}

void token_free(struct token **head)
{
	struct token *cur = *head;
	struct token *next;

	while (cur) {
		next = cur->next;
		free(cur->value);
		free(cur);
		cur  = next;
	}

	*head = NULL;
}

void token_dump(struct token *head)
{
	struct token *cur = head;
	while (cur) {
		printf("%p, type(%u): %s\n", cur, cur->type, token_names[cur->type]);
		if (cur->type == TT_TEXT)
			printf("\t%s\n", cur->value);
		cur = cur->next;
	}
}

void token_append(struct token *t, char c)
{
	if (t->cap == 0) {
		t->value = calloc(1024, 1);
		t->cap   = 1024;
	} else if (t->cap == t->len) {
		t->value = realloc(t->value, t->cap + 1024);
		t->cap  += 1024;
	}

	// TODO realloc

	if (t->value) {
		t->value[t->len] = c;
		t->len++;
	}
}

int main(void)
{
	unsigned int indent = 0;
	char *t = strtok(text, " ");
	char *c = t;
	const char *text    = NULL;
	struct token *tlist = NULL;
	struct token *tcur  = NULL;

	while (t) {
		while (*c) {
			switch (*c) {
			case '{':
				tcur = token_new(&tlist);
				tcur->type = TT_BRACKET_OPEN;
			break;
			case '}':
				tcur = token_new(&tlist);
				tcur->type = TT_BRACKET_CLOSE;
			break;
			case '(':
				tcur = token_new(&tlist);
				tcur->type = TT_PARENTHESES_OPEN;
			break;
			case ')':
				tcur = token_new(&tlist);
				tcur->type = TT_PARENTHESES_CLOSE;
			break;
			case '[':
			break;
			case ']':
			break;
			case '\t':
				tcur = token_new(&tlist);
				tcur->type = TT_TAB;
				indent++;
			break;
			case '\n':
				indent = 0;
				tcur = token_new(&tlist);
				tcur->type = TT_NEWLINE;
			break;
			case '"':
				tcur = token_new(&tlist);
				tcur->type = TT_LITERAL;
			break;
			case '\'':
				tcur = token_new(&tlist);
				tcur->type = TT_LITERAL;
			break;
			default:
				if (!tcur) {
					tcur = token_new(&tlist);
					tcur->type = TT_TEXT;
				}

				token_append(tcur, *c);
			break;
			}

			c++;
		}

		tcur = NULL;

		indent = 0;
		t = strtok(NULL, " ");
		c = t;
	}

	token_dump(tlist);
	token_free(&tlist);

	return 0;
}
