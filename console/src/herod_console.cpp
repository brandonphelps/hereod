
#include "herod_console.h"

#include <sstream>

// remove platform depedant code
#include <windows.h>
#include "console_another.h"
#include "game_controller.h"
#include <string>

void Console::add_enter_callback(std::function<void(const std::string&)> hook)
{
	enter_hook = hook;
}

std::string toHex(uint8_t* start, size_t length);


void Console::add_message(const std::string& msg)
{
	std::stringstream oss;
	oss << "\t Calling: " << this << std::endl;
	WriteLine("\t Calling : " + oss.str());
	WriteLine("Console adding message: " + msg);
	buffer_history.push_back(msg);
	WriteLine("New buffer length: " + std::to_string(buffer_history.size()));
}

// this function must only be called with a key transition occurs, rather than
// on every update loop. 
void Console::update(const KeyboardInputController* keyboard, uint32_t keycode)
{
	// this is for when shift is not pressed due to the lower alpha be higher value then upper
	WriteLine("Obtained key code value: " + std::to_string(keycode));
	if(keycode >= 'A' && keycode <= 'Z')
	{
		char value = static_cast<char>(keycode);
		if(!keyboard->shift.EndedDown)
		{
			value = static_cast<char>(keycode + 97 - 65);
		}
		WriteLine("Updating messages with keyboard is down: " + std::to_string(value) + "\n\r");
		current_message += value;
	}
	else if(keycode >= '0' && keycode <= '9')
	{
		char value;
		if(keyboard->shift.EndedDown)
		{
			if(keycode == '0')
			{
				value = ')';
			}
			else if(keycode == '9')
			{
				value = '(';
			}
			else if(keycode == '8')
			{
				value = '*';
			}
			else if(keycode == '7')
			{
				value = '&';
			}
			else if(keycode == '6')
			{
				value = '^';
			}
			else if(keycode == '5')
			{
				value = '%';
			}
			else if(keycode == '4')
			{
				value = '$';
			}
			else if(keycode == '3')
			{
				value = '#';
			}
			else if(keycode == '2')
			{
				value = '@';
			}
			else if(keycode == '1')
			{
				value = '!';
			}
		}
		else
		{
			value = static_cast<char>(keycode);
		}
		current_message += value;
		WriteLine("@@@@@@@@@@@@@@@@@@@@ Adding in: " + std::to_string(value));
	}
	else
	{
		if(keycode == ' ')
		{
			current_message += ' ';
		}
		else if(keycode == VK_OEM_PLUS)
		{
			if(keyboard->shift.EndedDown)
			{
				current_message += '+';
			}
			else
			{
				current_message += '=';
			}
		}
		else if(keycode == VK_OEM_MINUS)
		{
			if(keyboard->shift.EndedDown)
			{
				current_message += '_';
			}
			else
			{
				current_message += '-';
			}
		}
		else if(keycode == VK_OEM_2)
		{
			if(keyboard->shift.EndedDown)
			{
				current_message += '?';
			}
			else
			{
				current_message += '/';
			}
		}
		else if(keycode == VK_OEM_4)
		{
			if(keyboard->shift.EndedDown)
			{
				current_message += '{';
			}
			else
			{
				current_message += '[';
			}
		}
		else if(keycode == VK_OEM_6)
		{
			if(keyboard->shift.EndedDown)
			{
				current_message += '}';
			}
			else
			{
				current_message += ']';
			}
		}

		if(keycode == VK_BACK && !current_message.empty())
		{
			current_message.pop_back();
		}
	}
	if(keyboard->enter.EndedDown)
	{
		WriteLine("Enter was pressed down");
		// buffer_history.push_back(current_message);
		add_message(current_message);
		if(enter_hook)
		{
			enter_hook(current_message);
		}
		current_message = "";
	}

	// else
	// {
	// 	if(keycode == VK_OEM_PLUS)
	// 	{
	// 		current_message += '+';
	// 	}
	// 	else
	// 	{
	// 		current_message += static_cast<char>(keycode);
	// 	}
	// }
		
	// WriteLine("Current message: " + current_message);
}

void Console::renderString(ScreenData& dest, const std::string& str, int start_x, int start_y)
{
	// update the render window as needed.
	int index = 0;
	static int odd_ball_offset = 26 + 10 + 26;
	for(int i = 0; i < str.size(); i++)
	{
		// 0 - 9
		if(str[i] >= '0' && str[i] <= '9')
		{
			index = (str[i] - 48) + 26;
		}
		// A-Z
		else if(str[i] >= 'A' && str[i] <= 'Z')
		{
			index = str[i] - 'A';
		}
		// a-z
		else if(str[i] >= 'a' && str[i] <= 'z')
		{
			index = (str[i] - 97) + 10 + 26;
		}
		else if(str[i] == '@')
		{
			index = odd_ball_offset + 1;
		}
		else if(str[i] >= '!' && str[i] <= '%')
		{
			index = odd_ball_offset + (str[i] - '!');
		}
		else if(str[i] == '^')
		{
			index = odd_ball_offset + 5;
		}
		else if(str[i] == '&')
		{
			index = odd_ball_offset + 6;
		}
		else if(str[i] == '*')
		{
			index = odd_ball_offset + 7;
		}
		else if(str[i] == '(')
		{
			index = odd_ball_offset + 8;
		}
		else if(str[i] == ')')
		{
			index = odd_ball_offset + 9;
		}
		else if(str[i] == '{')
		{
			index = odd_ball_offset + 10;
		}
		else if(str[i] == '}')
		{
			index = odd_ball_offset + 11;
		}
		else if(str[i] == '[')
		{
			index = odd_ball_offset + 12;
		}
		else if(str[i] == ']')
		{
			index = odd_ball_offset + 13;
		}
		else if(str[i] == '+')
		{
			index = odd_ball_offset + 14;
		}
		else if(str[i] == '-')
		{
			index = odd_ball_offset + 15;
		}
		else if(str[i] == '/')
		{
			index = odd_ball_offset + 16;
		}
		else if(str[i] == '=')
		{
			index = odd_ball_offset + 17;
		}
		else if(str[i] == '_')
		{
			index = odd_ball_offset + 18;
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
