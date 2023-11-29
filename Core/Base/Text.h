#pragma once

#include<map>
#include"./Types.h"

namespace Abstract
{
	struct Character {
		uint TextureID; // ID handle of the glyph texture
		iVec2   Size;      // Size of glyph
		iVec2   Bearing;   // Offset from baseline to left/top of glyph
		uint Advance;   // Horizontal offset to advance to next glyph
	};

	using CharMap = std::map<char, Character>;	
}