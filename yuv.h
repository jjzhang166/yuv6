#ifndef __YUV_H__
#define __YUV_H__

#ifdef __cplusplus
extern "C" {
#endif

#define uyvy_size(w, h)	(w * h * 2))
#define i420_size(w, h)	((w * h * 3) / 2)

void uyvy422_to_i420_4byte(const char *src, char *dst, unsigned int w, unsigned int h);
void uyvy422_to_i420_wh(const char *src, char *dst, unsigned int w, unsigned int h);

#ifdef __cplusplus
}
#endif
#endif
