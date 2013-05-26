#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "yuv.h"

#define uyvy_size(w, h)	(w * h * 2)
#define i420_size(w, h)	(w * h * 3 / 2)

static void _read(char *buffer, unsigned int w, unsigned int h, char *filepath) {
	FILE *fd;

	if (!filepath)
		return;
	
	fd = fopen(filepath, "rb");
	if (fd) {
		unsigned int count = fread(buffer, w * h * 2, 1, fd);
		if (count != 1)
			printf("fread(%s)=%d failed\n", filepath, count);
		fclose(fd);
	} else {
		printf("fopen(%s) failed\n", filepath);
		exit(0);
	}
}

static void _write(char *buffer, unsigned int w, unsigned int h, char *filepath) {
	FILE *fd;

	if (!filepath)
		return;

	fd = fopen(filepath, "wb");
	if (fd) {
		unsigned int count = fwrite(buffer, w * h * 3 / 2, 1, fd);
		if (count != 1)
			printf("fwrite(%s)=%d failed\n", filepath, count);
		fclose(fd);
	} else {
		printf("fopen(%s) failed\n", filepath);
		exit(0);
	}
}

static double _test(int count, 
					char *src, 
					char *dst, 
					unsigned int w, 
					unsigned int h,
					char *inpath,
					char *outpath,
					void (*convert)(const char *, char *, unsigned int, unsigned int)) {
	struct timeval start, end;

	_read(src, w, h, inpath);

	gettimeofday(&start, NULL);
	while (count-- > 0)
		convert(src, dst, w, h);
	gettimeofday(&end, NULL);

	_write(dst, w, h, outpath);

	return (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000;
}

int main(int argc, char **argv) {
	unsigned int w = (unsigned int) atoi(argv[1]);
	unsigned int h = (unsigned int) atoi(argv[2]);
	int count = atoi(argv[3]);
	char *filepath = argv[4];
	char *src = malloc(uyvy_size(w, h));
	char *dst = malloc(i420_size(w, h));
	double elapsed;
	
	elapsed = _test(count, src, dst, w, h, filepath, "i420.yuv", uyvy422_to_i420);
	printf("elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

	elapsed = _test(count, src, dst, w, h, filepath, "i420_p.yuv", uyvy422_to_i420_plus);
	printf("elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

	free(src);
	free(dst);
	return 0;
}
