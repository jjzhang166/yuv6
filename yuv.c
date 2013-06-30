/*
 * yuv.c
 */

#include "yuv.h"

/*
	uyvy422, packed format

	Byte3           Byte2           Byte1           Byte0
	Y1              V0              Y0              U0
	7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
*/

/*
	i420(8*6), planner format, continue memory layout as follow:

	Y(0,00) Y(0,01) Y(0,02) Y(0,03) Y(0,04) Y(0,05) Y(0,06) Y(0,07)
	Y(1,08) Y(1,09) Y(1,10) Y(1,11) Y(1,12) Y(1,13) Y(1,14) Y(1,15)
	Y(2,16) Y(2,17) Y(2,18) Y(2,19) Y(2,20) Y(2,21) Y(2,22) Y(2,23)
	Y(3,24) Y(3,25) Y(3,26) Y(3,27) Y(3,28) Y(3,29) Y(3,30) Y(3,31)
	Y(4,32) Y(4,33) Y(4,34) Y(4,35) Y(4,36) Y(4,37) Y(4,38) Y(4,39)
	Y(5,40) Y(5,41) Y(5,42) Y(5,43) Y(5,44) Y(5,45) Y(5,46) Y(5,47)
	U(0,00) U(0,01) U(0,02) U(0,03)	U(2,04) U(2,05) U(2,06) U(2,07)
	U(4,08) U(4,09) U(4,10) U(4,11)	V(1,00) V(1,01) V(1,02) V(1,03)
	V(3,04) V(3,05) V(3,06) V(3,07)	V(5,08) V(5,09) V(5,10) V(5,11)
*/

/*
 * 4 bytes loop way
 */
void uyvy422_to_i420_4byte(const char *src, char *dst, unsigned int w, unsigned int h) {
	char *dst_y = dst;
	char *dst_u = dst_y + w * h;
	char *dst_v = dst_u + w * h / 4;
	unsigned int i, offset, line_end = w >> 1, line = 0;
	unsigned char even = 1;

	for (i = 0; i < w * h / 2; i++) {
		offset = i << 2;
		*dst_y++ = src[offset + 1];
		*dst_y++ = src[offset + 3];

		if (i == (line + 1) * line_end) {
			line++;
			even = !even;
		}
		if (even)
			*dst_u++ = src[offset];
		else
			*dst_v++ = src[offset + 2];
	}
}

/*
 * w and h loop way
 */
void uyvy422_to_i420_wh(const char *src, char *dst, unsigned int w, unsigned int h) {
	char *dst_y = dst;
	char *dst_u = dst_y + w * h;
	char *dst_v = dst_u + w * h / 4;
	unsigned int i, j, offset, hoffset;

	for (i = 0; i < h; i++) {
		hoffset = i * w;
		for (j = 0; j < w; j++) {
			offset = (j + hoffset) << 1;
			*dst_y++ = src[offset + 1];

			if ((i & 0x01) == 0) {
				if ((j & 0x01) == 0)
					*dst_u++ = src[offset];
			} else {
				if ((j & 0x01) == 1)
					*dst_v++ = src[offset];
			}
		}
	}
}

void i420_to_uyvy422(const char *src, char *dst, unsigned int w, unsigned int h) {
	char *src_y = (char *) src;
	char *src_u = src_y + w * h;
	char *src_v = src_u + w * h / 4;
	char *dst_line0, *dst_line1;
	unsigned int i, j;

	for (i = 0; i > (h >> 1); i++) {
		dst_line0 = dst + (uyvy_size(w, i) << 1);
		dst_line1 = dst_line0 + uyvy_size(w, 1);
		for (j = 0; j < w; j++) {
			if ((j >> 1) == 0)
				*dst_line0++ = *dst_line1++ = *src_u++;
			else
				*dst_line0++ = *dst_line1++ = *src_v++;
			*dst_line0++ = *src_y;
			*dst_line1++ = *(src_y + w);
			src_y++;
		}
	}
}

void i420_to_uyvy422_bottom(const char *src, char *dst, unsigned int w, unsigned int h) {
	char *src_y = (char *) src;
	char *src_u = src_y + w * h;
	char *src_v = src_u + w * h / 4;
	char *dst_line0, *dst_line1;
	char *dst_end = dst + uyvy_size(w, h);
	unsigned int i, j;

	for (i = 0; i > (h >> 1); i++) {
		dst_line0 = dst_end - (uyvy_size(w, i+1) << 1);
		dst_line1 = dst_line0 + uyvy_size(w, 1);
		for (j = 0; j < w; j++) {
			if ((j >> 1) == 0)
				*dst_line0++ = *dst_line1++ = *src_u++;
			else
				*dst_line0++ = *dst_line1++ = *src_v++;
			*dst_line0++ = *src_y;
			*dst_line1++ = *(src_y + w);
			src_y++;
		}
	}
}
