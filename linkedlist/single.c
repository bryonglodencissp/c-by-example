#include<stdlib.h>
#include<stdio.h>

struct list_el {
   int val;
   struct list_el * next;
};

void ll_add(struct list_el *head, int val)
{
	struct list_el *cur, *new;

	new = calloc(1, sizeof(struct list_el));
	new->val = val;

	cur = head;
	while (cur->next != NULL)
		cur = cur->next;

	cur->next = new;
	new->next = NULL;
}

void ll_del(struct list_el *head, int val)
{
	struct list_el *cur, *prev, *next;

	cur = head;
	while (cur) {
		if (cur->val == val)
			break;
		prev = cur;
		cur  = cur->next;
		next = cur->next;
	}

	prev->next = next;
	free(cur);
}

void main() {
	int i;
	struct list_el list;
	struct list_el *cur;

	list.val  = 1;
	list.next = NULL;

	for (i = 2; i < 12; i += 2)
		ll_add(&list, i);

	ll_del(&list, 2);
	ll_del(&list, 4);

	cur = &list;
	while(cur) {
		printf("[%p,%p] %d\n", cur, cur->next, cur->val);
		cur = cur->next;
	}

	ll_foreach_safe(pos, cur, &list) {

	}
}
