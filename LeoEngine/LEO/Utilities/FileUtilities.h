#pragma once
#include <string>

namespace LEO
{
	std::string ReadFile(const std::string& filepath);

	std::string DirNameOf(const std::string& filepath);
}