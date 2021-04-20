#ifndef SORT_H
#define SORT_H

#include <stdlib.h>

// adapted from https://stackoverflow.com/questions/4300896/how-portable-is-the-re-entrant-qsort-r-function-compared-to-qsort

struct _sort_data {
    void *arg;
    int (*cmp)(const void *a, const void *b, void *arg);
};

static int _sort_r_arg_swap(void *s, const void *aa, const void *bb) {
    struct _sort_data *ss = (struct _sort_data*) s;
    return (ss->cmp)(aa, bb, ss->arg);
}

#ifdef EMSCRIPTEN
/**
 * QSort implemention found at https://github.com/git/git/blob/master/compat/qsort_s.c .
 */

static void msort_with_tmp(void *b, size_t n, size_t s,
			   int (*cmp)(const void *, const void *, void *),
			   char *t, void *ctx)
{
	char *tmp;
	char *b1, *b2;
	size_t n1, n2;

	if (n <= 1) return;

	n1 = n / 2;
	n2 = n - n1;
	b1 = b;
	b2 = (char *)b + (n1 * s);

	msort_with_tmp(b1, n1, s, cmp, t, ctx);
	msort_with_tmp(b2, n2, s, cmp, t, ctx);

	tmp = t;

	while (n1 > 0 && n2 > 0) {
		if (cmp(b1, b2, ctx) <= 0) {
			memcpy(tmp, b1, s);
			tmp += s;
			b1 += s;
			--n1;
		} else {
			memcpy(tmp, b2, s);
			tmp += s;
			b2 += s;
			--n2;
		}
	}

	if (n1 > 0) memcpy(tmp, b1, n1 * s);

	memcpy(b, t, (n - n2) * s);
}

int qsort_r(void *b, size_t n, size_t s,
		int (*cmp)(const void *, const void *, void *), void *ctx)
{
	const size_t size = n * s;
	char buf[1024];

	if (!n) return 0;

	if (!b || !cmp) return -1;

	if (size < sizeof(buf)) {
		/* The temporary array fits on the small on-stack buffer. */
		msort_with_tmp(b, n, s, cmp, buf, ctx);
	} else {
		/* It's somewhat large, so malloc it.  */
		char *tmp = malloc(size);

        if(tmp == NULL) return -1;

		msort_with_tmp(b, n, s, cmp, tmp, ctx);
		free(tmp);
	}

	return 0;
}
#endif

static inline void sort_r(
    void *base,
    size_t nel,
    size_t width,
    int (*cmp)(const void *a, const void *b, void *arg),
    void *arg
) {
#if (defined _GNU_SOURCE || defined __GNU__ || defined __linux__ || defined EMSCRIPTEN)
    qsort_r(base, nel, width, cmp, arg);
#elif (defined __APPLE__ || defined __MACH__ || defined __DARWIN__ || \
        defined __FREEBSD__ || defined __BSD__ || \
        defined OpenBSD3_1 || defined OpenBSD3_9)
    struct _sort_data tmp = {arg, cmp};
    qsort_r(base, nel, width, &tmp, &_sort_r_arg_swap);
#elif (defined _WIN32 || defined _WIN64 || defined __WINDOWS__)
    struct _sort_r_data tmp = {arg, cmp};
    qsort_s(*base, nel, width, &sort_r_arg_swap, &tmp);
#else
    #error cannnot detect platform for sort_r
#endif
}

#endif