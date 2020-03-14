
#ifndef VIDEO_H
#define VIDEO_H

typedef struct ScreenData
{
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint8_t bytesPerPixel;
  uint8_t *buffer;
} ScreenData;

void drawBuf(uint8_t*, uint32_t, uint32_t, uint32_t);

#endif
