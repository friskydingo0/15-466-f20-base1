#pragma once

#include "PPU466.hpp"

// Adding these here assuming anything that includes this header wants to read/write data files
#include <fstream>
#include "read_write_chunk.hpp"

struct DataUtils {

	DataUtils();

	struct SpriteData
	{
		PPU466::Palette color_palette;
		PPU466::Tile sprite_tile;
	};
};
