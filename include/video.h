
#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>


class ScreenData
{
public:
	// set the pixel at pos x, y 
	void set_pixel_color(uint32_t x, uint32_t y,
	                     uint32_t color_mask);

public:
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint8_t bytesPerPixel;
  uint8_t *buffer;
};

void drawBuf(uint8_t*, uint32_t, uint32_t);
void drawBuf(uint8_t*, uint32_t, uint32_t, uint32_t);

void DrawRectangle(uint8_t* buffer,
                   uint32_t buf_width, uint32_t buf_height,
                   uint32_t x, uint32_t y,
                   uint32_t width, uint32_t height, uint8_t red, uint8_t blue, uint8_t green);

void DrawRectangle(ScreenData* screenData,
                   uint32_t x, uint32_t y,
                   uint32_t width, uint32_t height,
                   uint32_t color_mask);
#endif
