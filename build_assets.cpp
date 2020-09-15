#include <iostream>

#include "load_save_png.hpp"
#include "PPU466.hpp"
#include "read_write_chunk.hpp"

#include <glm/glm.hpp>

#include <vector>

int main(int argc, char **argv)
{
	// ------------- Read PNG for pixel data -----------------
	std::vector< glm::u8vec4 > data = std::vector< glm::u8vec4 >();
	glm::uvec2 size = glm::uvec2(8,8);
	
	// Create a new vector of colors for the PNG. [0] is transparent black.
	std::vector< glm::u8vec4 > colorPalette = std::vector< glm::u8vec4 >();
	glm::uvec2 palette_size = glm::uvec2(1,4);

	try
	{
		// Load the file for player sprite using provided load_save_png utility
		load_png("assets/car-8x8.png", &size, &data, LowerLeftOrigin);

		// Load its associated palette
		load_png("assets/car-8x8-palette.png", &palette_size, &colorPalette, LowerLeftOrigin);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	PPU466::Palette temp_palette = PPU466::Palette();
	PPU466::Tile temp_tile = PPU466::Tile();
	
	std::cout << "PNG data: " << data.size() << std::endl;
	
	// Jim : Not enough paranoia
	for (int i = 0; i < data.size(); i++)
	{
		// Find the color in the palette and split the bit indeces
		for (size_t colorIndex = 0; colorIndex < colorPalette.size(); colorIndex++)
		{
			if (data[i] == colorPalette[colorIndex])
			{
				uint8_t bit1 = (colorIndex >> 1) & 1;
				uint8_t bit0 = (colorIndex >> 0) & 1;
				
				std::cout << colorIndex << "-" << int(bit1) << int(bit0) << "|";

				temp_tile.bit0[i/8] = (temp_tile.bit0[i/8] << 1) | bit0;
				temp_tile.bit1[i/8] = (temp_tile.bit1[i/8] << 1) | bit1;
			}
		}
		if ((i+1) % 8 == 0) {
			std::cout << std::endl;
		}
	}
	
	colorPalette[0] = glm::u8vec4(0x00,0x00,0x00,0x00);

	// Based on https://stackoverflow.com/a/21276944
	std::copy(colorPalette.begin(), colorPalette.end(), temp_palette.begin());

	// Write asserts
	std::cout << std::hex;
	for (auto &col : temp_palette)
	{
		std::cout << int(col.r) << "|" << int(col.g) << "|" << int(col.b) << "|" << int(col.a) << std::endl;
	}

	// ------------- End Read PNG for pixel data -----------------

	// Write the sprite data to a file
	struct SpriteData
	{
		PPU466::Palette color_palette;
		PPU466::Tile sprite_tile;
	} sprite_data;
	
	sprite_data.color_palette = temp_palette;
	sprite_data.sprite_tile = temp_tile;

	std::ofstream out("player_sprite" + ".dat", std::ios::binary);

	// Using read/write functions from read_write_chunk.hpp
	
	// write_chunk("poop", &sprite_data, <some file ptr>)

	return 0;
}