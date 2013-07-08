#ifndef __YUV_H__
#define __YUV_H__

#ifdef __cplusplus
extern "C" {
#endif

enum yuv_format {
	yuv_i420,
	yuv_uyvy,
	yuv_yuyv,
};

#define uyvy_size(w, h)	(((w) * (h) << 1))
#define i420_size(w, h)	(((w) * (h) * 3) / 2)

void uyvy422_to_i420_4byte(const char *src, char *dst, unsigned int w, unsigned int h);
void uyvy422_to_i420_wh(const char *src, char *dst, unsigned int w, unsigned int h);

void i420_to_uyvy422(const char *src, char *dst, unsigned int w, unsigned int h);
void i420_to_uyvy422_bottom(const char *src, char *dst, unsigned int w, unsigned int h);

void i420_to_uyvy422_err(const char *src, char *dst, unsigned int w, unsigned int h);
void i420_scale(const char *src, char *dst, unsigned int w, unsigned int h, float scale);

void i420_3d_to_yuyv422_sbs(const char *src_left, const char *src_right, char *dst, unsigned int w, unsigned int h);

#ifdef __cplusplus
}
#endif
#endif
