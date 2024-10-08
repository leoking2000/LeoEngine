#pragma once
#include <string>
#include "Types.h"

namespace LEO
{
	std::string ReadFile(const std::string& filepath);

	std::string DirNameOf(const std::string& filepath);

	struct ImageData
	{
		i32 width;
		i32 height;
		i32 bpp;
		u8* data;
	};

	ImageData ReadImageData(const std::string& filepath);

	void FreeImageData(ImageData data);
}