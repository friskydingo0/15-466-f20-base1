#include <iostream>

#include "load_save_png.hpp"
#include "PPU466.hpp"

#include <glm/glm.hpp>

#include <vector>

int main(int argc, char **argv)
{
	std::vector< glm::u8vec4 > data = std::vector< glm::u8vec4 >();
	glm::uvec2 size = glm::uvec2(8,8);

	// Create a new vector of colors for the PNG. [0] is transparent black.
	std::vector< glm::u8vec4 > colorPalette = std::vector< glm::u8vec4 >();
	data.emplace_back(glm::u8vec4(0x00,0x00,0x00,0x00));

	auto get_bit = [](const int &num){
		return (num > 0 ? 1 : 0);
	};

	try
	{
		// Load the file for player sprite using provided load_save_png utility
		load_png("assets/car-8x8.png", &size, &data, LowerLeftOrigin);

		// Load its associated palette
		load_png("assets/car-8x8-palette.png", &size, &colorPalette, LowerLeftOrigin);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	std::cout << "PNG data: " << data.size() << std::endl;

	for (size_t i = 0; i < data.size(); i++)
	{
		// Find the color in the palette and split the bit indeces
		for (size_t colorIndex = 0; colorIndex < colorPalette.size(); colorIndex++)
		{
			if (data.at(i) == colorPalette[colorIndex])
			{
				uint8_t bit1 = (colorIndex >> 1) & 1;
				uint8_t bit0 = (colorIndex >> 0) & 1;

				PPU466::tile_table[32].bit0[i] << 1;
				PPU466::tile_table[32].bit1[i] << 1;
			}
		}
	}

	return 0;
}