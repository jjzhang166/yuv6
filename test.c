#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32_CL
#include <windows.h>
#else
#include <sys/time.h>
#endif

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

#ifdef WIN32_CL
	unsigned long long start, end;
#else
	struct timeval start, end;
#endif

	_read(src, w, h, inpath);

#ifdef WIN32_CL
	start = GetTickCount64();
#else
	gettimeofday(&start, NULL);
#endif

	while (count-- > 0)
		convert(src, dst, w, h);

#ifdef WIN32_CL
	end = GetTickCount64();
#else	
	gettimeofday(&end, NULL);
#endif

	_write(dst, w, h, outpath);
#ifdef WIN32_CL
	return (end - start) * 1.0f;
#else
	return (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000;
#endif
}

int main(int argc, char **argv) {
	unsigned int w = (unsigned int) atoi(argv[1]);
	unsigned int h = (unsigned int) atoi(argv[2]);
	int count = atoi(argv[3]);
	char *filepath = argv[4];
	char *src = malloc(uyvy_size(w, h));
	char *dst = malloc(i420_size(w, h));
	double elapsed;

	elapsed = _test(count, src, dst, w, h, filepath, "i420_4b.yuv", uyvy422_to_i420_4byte);
	printf("4b> elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

	elapsed = _test(count, src, dst, w, h, filepath, "i420_wh.yuv", uyvy422_to_i420_wh);
	printf("wh> elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

	free(src);
	free(dst);
	return 0;
}