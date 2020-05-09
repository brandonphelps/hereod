
#ifndef HEROD_CONSOLE_H
#define HEROD_CONSOLE_H

#include "video.h"
#include "sprite_sheet.h"
#include "game_input.h"
#include "game_controller.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <list>

#include <functional>

class Console
{
public:
	void add_enter_callback(std::function<void(const std::string&)> hook);
	
	// render_dest is the location of where the console should be rendered onto. 
	void render(ScreenData& render_dest);

	void renderString(ScreenData& window, const std::string& msg, int start_x, int start_y);
	void add_message(const std::string& msg);

	// keycode should correspond to the pressed key to add to the console input.
	// should not consist of modifier keys like shift, etc keyboard will contain those. 
	void update(const KeyboardInputController* keyboard, uint32_t keycode);
 	// contains all messages that have been typed into the console,
	// does not include the currently active typing message.
	std::vector<std::string> buffer_history;
	std::string current_message;
	SpriteSheet font_sheet;
	ScreenData render_window;
	uint32_t x_position;
	uint32_t y_position;



private:
	// is called if an enter update is called. 
	std::function<void(const std::string&)> enter_hook;
	float sum_dt;
};




#endif