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

void uyvy422_to_i420(const char *src, char *dst, unsigned int w, unsigned int h) {
	unsigned int i, point_y;
	char *dst_y = dst;
	char *dst_u = dst_y + w * h;
	char *dst_v = dst_u + w * h / 4;

	for (i = 0; i < w * h / 2; i++) {
		*dst_y++ = src[i*4 + 1];
		*dst_y++ = src[i*4 + 3];

		point_y = (unsigned int) (dst_y - dst);
		if ((point_y / w) % 2 == 0)
			*dst_u++ = src[i*4];
		else
			*dst_v++ = src[i*4 + 2];
	}
}