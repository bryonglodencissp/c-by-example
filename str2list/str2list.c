#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct item_list {
	size_t count;
	unsigned int *val;
};
struct list {
	size_t count;
	struct item_list **item;
};

void list_dump(struct list *l)
{
	size_t x = 0;
	size_t y = 0;

	for (x = 0; x < l->count; x++) {
		printf("item_list[%zu] = %p, count: %zu, ",
			x, l->item[x], l->item[x]->count);
		for (y = 0; y < l->item[x]->count; y++) {
			printf("%u", l->item[x]->val[y]);
			if (y < l->item[x]->count - 1)
				printf(",");
		}
		printf("\n");
	}
}

int str2list_item(const char * const str, struct item_list **items)
{
	size_t i;
	size_t len;
	char *tmp, *token, *token_saveptr;
	struct item_list *l;

	tmp = strdup(str);
	for (len = 0 ;; len++, tmp = NULL) {
		token = strtok_r(tmp, ",", &token_saveptr);
		if (token == NULL)
			break;
	}
	free(tmp);

	if (len > 0) {
		l = malloc(sizeof(*l));
		if (l == NULL)
			return -ENOMEM;
		l->count = len;
		l->val = malloc(sizeof(*(l->val) * len));
		l->count = len;

		tmp = strdup(str);
		for (i = 0 ;; i++, tmp = NULL) {
			token = strtok_r(tmp, ",", &token_saveptr);
			if (token == NULL)
				break;
			l->val[i] = strtoul(token, NULL, 0);
		}
		free(tmp);

		*items = l;
	} else {
		return -ENOENT;
	}

	return 0;
}

int str2list(const char * const str)
{
	size_t i;
	size_t j;
	size_t len;
	size_t count = 0;
	char *tmp;
	char *l;
	struct item_list *item;
	struct list *list;

	tmp = strdup(str);
	len = strlen(tmp);

	/* Get count */
	for (i = 0; i < len; i++) {
		if (tmp[i] == '(')
			l = &tmp[i + 1];
		if (tmp[i] == ')') {
			tmp[i] = '\0';
			count++;
		}
	}

	if (count > 0) {
		list = malloc(sizeof(*list));
		if (list == NULL)
			return -ENOMEM;
		list->item = malloc(sizeof(list->item) * count);
		list->count = count;
		printf("count: %zu\n", count);

		strncpy(tmp, str, strlen(str) + 1);

		for (i = 0, j = 0; i < len; i++) {
			if (tmp[i] == '(')
				l = &tmp[i + 1];
			if (tmp[i] == ')') {
				tmp[i] = '\0';
				str2list_item(l, &item);
				list->item[j] = item;
				j++;
			}
		}
	}

	free(tmp);

	list_dump(list);

	return 0;
}

int
main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "%s <string (e.g (0),(0,4))>", argv[0]);
		return -1;
	}

	str2list(argv[1]);

	return 0;
}
