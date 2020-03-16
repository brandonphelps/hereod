
#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>


typedef struct ScreenData
{
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint8_t bytesPerPixel;
  uint8_t *buffer;
} ScreenData;

void drawBuf(uint8_t*, uint32_t, uint32_t, uint32_t);

void DrawRectangle(uint8_t* buffer, uint8_t buf_width, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t red, uint8_t blue, uint8_t green);

#endif
