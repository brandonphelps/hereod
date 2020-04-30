

#include "sprite_sheet.h"

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

	// p.sprites.clear();
	//	WriteLine("Chunking up sprite sheet");

	int sprite_count = 0;
	for(int row = 0;
	    row < p.sprite_count_y; row++)
	{
		for(int col = 0; col < p.sprite_count_x; col++)
		{
			// WriteLine("Reading from sprite at: 0, " + std::to_string(row * p.sprite_height));
			ScreenData new_sprite;
			resize_buffer(new_sprite, p.sprite_width, p.sprite_height);
			BlitScreenData(image_data, new_sprite, 0, 0,
			               col * p.sprite_width, row * p.sprite_height);
			// WriteLine("Push sprite into vector");
			p.sprites[sprite_count] = new_sprite;
			sprite_count++;
		}
	}
}
