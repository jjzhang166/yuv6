#ifndef __YUV_H__
#define __YUV_H__

#ifdef __cplusplus
extern "C"
{
#endif

void uyvy422_to_i420(const char *src, char *dst, unsigned int w, unsigned int h);

#ifdef __cplusplus
}
#endif
#endif