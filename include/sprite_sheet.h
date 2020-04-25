


#ifndef H_SPRITE_SHEET_H
#define H_SPRITE_SHEET_H

#include "video.h"


class SpriteSheet
{
public:
	uint32_t sprite_width;
	uint32_t sprite_height;

	uint32_t sprite_count_x;
	uint32_t sprite_count_y;

	std::vector<ScreenData> sprites;

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
                    uint32_t sprite_count_y)
{
	p.sprite_width = sprite_width;
	p.sprite_height = sprite_height;
	p.sprite_count_x = sprite_count_x;
	p.sprite_count_y = sprite_count_y;

	p.sprites.clear();

	for(int row = 0;
	    row < p.sprite_count_y; row++)
	{
		ScreenData new_sprite;
		new_sprite.bytesPerPixel = 4;
		new_sprite.buffer = NULL;
		resize_buffer(new_sprite, p.sprite_width, p.sprite_height);
		BlitScreenData(image_data, new_sprite, 0, 0, row * p.sprite_width, 0);
		p.sprites.push_back(new_sprite);
	}
}


#endif
