/* See https://sourceware.org/binutils/docs-2.24/ld/Options.html under --wrap */
#include <stdio.h>
#include <stdlib.h>

static size_t mem_in_use;
static size_t mem_alloced;

int main()
{
	void *mydata = NULL;

	mydata = malloc(1024);
	free(mydata);

	mydata = calloc(4, 256);
	free(mydata);

	printf("Mem alloced: %zu bytes\n", mem_alloced);
	printf("Mem in use: %zu bytes\n", mem_in_use);

	return 0;
}

void *__real_malloc (size_t size);
void *__real_free (void *ptr);
void *__real_calloc (size_t nmemb, size_t size);
void *__real_realloc (void *ptr, size_t size);

/* This function wraps the real malloc */
void * __wrap_malloc (size_t size)
{
	void *lptr = __real_malloc(size);
	mem_alloced += size;
	printf("malloc: %lu bytes @%p\n", size, lptr);
	return lptr;
}

/* This function wraps the real free */
void * __wrap_free (void *data)
{
	void *lptr = __real_free(data);
	printf("free: %p\n", data);
	return lptr;
}

/* This function wraps the real calloc */
void *__wrap_calloc (size_t nmemb, size_t size)
{
	void *lptr = __real_calloc(nmemb, size);
	mem_alloced += nmemb * size;
	printf("calloc: nmemb %zu, size %zu (%zu)@%p\n", nmemb, size, nmemb * size, lptr);
	return lptr;
}

/* This function wraps the real realloc */
void *__wrap_realloc (void *ptr, size_t size)
{
	void *lptr = __real_realloc(ptr, size);
	mem_alloced += size;
	printf("realloc: %p, ptr %p, size %zu\n", lptr, ptr, size);
	return lptr;
}
