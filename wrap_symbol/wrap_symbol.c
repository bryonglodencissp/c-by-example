/* See https://sourceware.org/binutils/docs-2.24/ld/Options.html under --wrap */
#include <stdio.h>
#include <stdlib.h>

int main()
{
	void *mydata = NULL;

	mydata = malloc(1024);
	free(mydata);

	mydata = calloc(4, 256);
	free(mydata);

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
	printf("calloc: nmemb %zu, size %zu (%zu)@%p\n", nmemb, size, nmemb * size, lptr);
	return lptr;
}

/* This function wraps the real realloc */
void *__wrap_realloc (void *ptr, size_t size)
{
	void *lptr = __real_realloc(ptr, size);
	printf("realloc: %p, ptr %p, size %zu\n", lptr, ptr, size);
	return lptr;
}
