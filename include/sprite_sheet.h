


#ifndef H_SPRITE_SHEET_H
#define H_SPRITE_SHEET_H

#include <array>

#include "video.h"

#include "console_another.h"

class SpriteSheet
{
public:
	uint32_t sprite_width;
	uint32_t sprite_height;

	uint32_t sprite_count_x;
	uint32_t sprite_count_y;

	std::array<ScreenData, 100> sprites;

	// todo make const.
	ScreenData& GetSprite(int i)
	{
		return sprites[i];
	}

private:

};


void ChunkUpSprites(ScreenData& image_data,
                    SpriteSheet& p,
                    uint32_t sprite_width,
                    uint32_t sprite_height,
                    uint32_t sprite_count_x,
                    uint32_t sprite_count_y);

#endif
