
#include "herod_console.h"

// remove platform depedant code
#include <windows.h>
#include "console_another.h"
#include <string>

void Console::add_enter_callback(std::function<void(const std::string&)> hook)
{
	enter_hook = hook;
}

void Console::add_message(const std::string& msg)
{
	buffer_history.push_back(msg);
}

void Console::update(uint32_t keycode)
{
	WriteLine("Updating message with keycode: " + std::to_string(keycode));

	if(keycode == VK_RETURN)
	{
		if(enter_hook)
		{
			enter_hook(current_message);
		}
		buffer_history.push_back(current_message);
		current_message = "";
	}
	else if(keycode == VK_BACK)
	{

		if(!current_message.empty())
		{
			int prev_size = current_message.size();
			WriteLine("BACK Space");
			current_message.pop_back();
			int after_size = current_message.size();
			if(prev_size != after_size)
			{
				WriteLine("pop back is a lie: " + std::to_string(prev_size) + ", " + std::to_string(after_size));
			}
		}
	}
	else
	{
		if(keycode == VK_OEM_PLUS)
		{
			current_message += '+';
		}
		else
		{
			current_message += static_cast<char>(keycode);
		}
	}
		
	WriteLine("Current message: " + current_message);
}

void Console::renderString(ScreenData& dest, const std::string& str, int start_x, int start_y)
{
	// update the render window as needed.
	int index = 0;
	for(int i = 0; i < str.size(); i++)
	{
		if(str[i] >= 48 && str[i] <= 57)
		{
			index = (str[i] - 48) + 26;
		}
		else if(str[i] >= 65 && str[i] <= 90)
		{
			index = toupper(str[i]) - 65;
		}
		else
		{
			continue;
		}

		BlitScreenData(font_sheet.GetSprite(index), dest,
		               start_x + (i * font_sheet.sprite_width), start_y);
	}
}

void Console::render(ScreenData& render_dest)
{

	DrawRectangle(render_window.buffer, render_window.width, render_window.height, 0, 0, render_window.width, render_window.height, 0x00, 0x00, 0x00);

	// 0, would be below the console since we draw coords from sprite upper left.
	// 1, is used for the actual typing. 
	int high_index = 2;
	// std::list<std::string>::iter it = buffer
	for(int i = buffer_history.size()-1; i >= 0; --i)
	{
		renderString(render_window, buffer_history[i],
		             0, render_window.height - (high_index++ * font_sheet.sprite_height));
	}

	renderString(render_window, current_message,
	             0, render_window.height - font_sheet.sprite_height);


	BlitScreenData(render_window, render_dest, 200, 200);
}
