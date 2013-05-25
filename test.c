#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "yuv.h"

#define uyvy_size(w, h)	(w * h * 2)
#define i420_size(w, h)	(w * h * 3 / 2)

static double _test(int count, 
					char *src, 
					char *dst, 
					unsigned int w, 
					unsigned int h, 
					void (*convert)(const char *, char *, unsigned int, unsigned int)) {
	struct timeval start, end;

	gettimeofday(&start, NULL);

	while (count-- > 0)
		convert(src, dst, w, h);

	gettimeofday(&end, NULL);
	return (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000;
}

int main(int argc, char **argv) {
	unsigned int w = (unsigned int) atoi(argv[1]);
	unsigned int h = (unsigned int) atoi(argv[2]);
	int count = atoi(argv[3]);
	char *src = malloc(uyvy_size(w, h));
	char *dst = malloc(i420_size(w, h));
	double elapsed = _test(count, src, dst, w, h, uyvy422_to_i420);
	
	printf("elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

	free(src);
	free(dst);
	return 0;
}
