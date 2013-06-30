#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32_CL
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "yuv.h"

static void _read(enum yuv_format format, 
				char *buffer, 
				unsigned int w, 
				unsigned int h, 
				char *filepath) {
	FILE *fd;

	if (!filepath)
		return;
	
	fd = fopen(filepath, "rb");
	if (fd) {
		unsigned int count; 

		if (format == yuv_i420)
			count = fread(buffer, i420_size(w, h), 1, fd);
		else
			count = fread(buffer, uyvy_size(w, h), 1, fd);
		if (count != 1)
			printf("fread(%s)=%d failed\n", filepath, count);
		fclose(fd);
	} else {
		printf("fopen(%s) failed\n", filepath);
		exit(0);
	}
}

static void _write(enum yuv_format format,
					char *buffer, 
					unsigned int w, 
					unsigned int h, 
					char *filepath) {
	FILE *fd;

	if (!filepath)
		return;

	fd = fopen(filepath, "wb");
	if (fd) {
		unsigned int count;
		
		if (format == yuv_i420)
			count = fwrite(buffer, i420_size(w, h), 1, fd);
		else
			count = fwrite(buffer, uyvy_size(w, h), 1, fd);
		if (count != 1)
			printf("fwrite(%s)=%d failed\n", filepath, count);
		fclose(fd);
	} else {
		printf("fopen(%s) failed\n", filepath);
		exit(0);
	}
}

static double _test(int count, 
				enum yuv_format src_format,
				char *src, 
				enum yuv_format dst_format,
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

	_read(src_format, src, w, h, inpath);

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

	_write(dst_format, dst, w, h, outpath);
#ifdef WIN32_CL
	return (end - start) * 1.0f;
#else
	return (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000;
#endif
}

static int _get_yuv_format(enum yuv_format *format, char *argv, unsigned int w, unsigned int h, char **buffer) {
	if (strcmp("i420", argv) == 0) {
		*format = yuv_i420;
		*buffer = malloc(i420_size(w, h));
	} else if (strcmp("uyvy", argv) == 0) {
		*format = yuv_uyvy;
		*buffer = malloc(uyvy_size(w, h));
	} else if (strcmp("yuyv", argv) == 0) {
		*format = yuv_yuyv;
		*buffer = malloc(uyvy_size(w, h));
	} else
		return -1;
	return 0;		
}

static void _dp_i420(char *src, unsigned int w, unsigned int h) {
	char *src_y = (char *) src;
	char *src_u = src_y + w * h;
	char *src_v = src_u + w * h / 4;
	unsigned int i;

	printf("\nY\n");
	for (i = 0; i < w * h; i++) {
		printf("%02x ", src_y[i]);
	}
	printf("\nU\n");
	for (i = 0; i < w * h / 4; i++) {
		printf("%02x ", src_u[i]);
	}
	printf("\nV\n");
	for (i = 0; i < w * h / 4; i++) {
		printf("%02x ", src_v[i]);
	}
	printf("\n");

}

static void _dp_uyvy(char *src, unsigned int w, unsigned int h) {
	unsigned int i, j;
	
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			printf(" (%02x,  ", *src++);
			printf(" %02x) ", *src++);
		}
		printf("\n");
	}
}

int main(int argc, char **argv) {
	unsigned int w = (unsigned int) atoi(argv[1]);
	unsigned int h = (unsigned int) atoi(argv[2]);
	int count = atoi(argv[3]);
	char *filepath = argv[4];
	enum yuv_format src_format, dst_format;
	char *src = NULL;
	char *dst = NULL;
	double elapsed;

	if (_get_yuv_format(&src_format, argv[5], w, h, &src))
		return -1;
	if (_get_yuv_format(&dst_format, argv[6], w, h, &dst))
		return -1;

	if (src_format == yuv_uyvy) {
		elapsed = _test(count, src_format, src, dst_format, dst, w, h, filepath, "i420_4b.yuv", uyvy422_to_i420_4byte);
		printf("uyvy_to_i420_4b> elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

		elapsed = _test(count, src_format, src, dst_format, dst, w, h, filepath, "i420_wh.yuv", uyvy422_to_i420_wh);
		printf("uyvy_to_i420_wh> elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);
	} else if (src_format == yuv_i420) {
		elapsed = _test(count, src_format, src, dst_format, dst, w, h, filepath, "uyvy422.yuv", i420_to_uyvy422);
		printf("i420_to_uyvy> elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

		// _dp_i420(src, w, h);
		// _dp_uyvy(dst, w, h);

		elapsed = _test(count, src_format, src, dst_format, dst, w, h, filepath, "uyvy422_bottom.yuv", i420_to_uyvy422_bottom);
		printf("i420_to_uyvy_bottom> elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);

		// _dp_uyvy(dst, w, h);

		elapsed = _test(count, src_format, src, dst_format, dst, w, h, filepath, "uyvy422_bottom_err.yuv", i420_to_uyvy422_err);
		printf("i420_to_uyvy_err> elapsed=%fms, avg=%fms\n", elapsed, elapsed / count);
	} else {
		printf("fopen(%s) failed\n", filepath);
		exit(0);
	}

	return 0;
}
