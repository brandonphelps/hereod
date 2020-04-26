
#ifndef HEROD_CONSOLE_H
#define HEROD_CONSOLE_H


#include "video.h"
#include "sprite_sheet.h"

#include <stdint.h>
#include <string>
#include <vector>

class Console
{
public:
	// render_dest is the location of where the console should be rendered onto. 
	void render(ScreenData& render_dest);

	void renderString(ScreenData& window, const std::string& msg, int start_x, int start_y);

	// note this does not allow for multiple  key pressed at the same time.
	// update to comput a keybord or game controller input. 
	void update(uint32_t keyCode);
	// contains all messages that have been typed into the console,
	// does not include the currently active typing message.
	std::vector<std::string> buffer_history;
	std::string current_message;
	SpriteSheet font_sheet;
	ScreenData render_window;
	uint32_t x_position;
	uint32_t y_position;
};




#endif
