#ifndef FILE_HELPERS_H
#define FILE_HELPERS_H

#include <String>

namespace Helpers {

	std::string LoadFile(const std::string& filepath);
	void WriteFile(const std::string& filepath, const std::string& content, const int& mode = std::ios::binary);

}

#endif
