#include "Profile.h"

#include <Logger/Logger.h>

void Profile::Decode(const std::string & _content) {
	Console::Warn << "Decoding profile\n";
	Codable::Split(_content, 2, &name, &status);
	Console::Warn << "Decoded profile\n";
}

std::string Profile::Encode() const {
	return Codable::Join(2, name, status);
}
