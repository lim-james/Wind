#include "Profile.h"

void Profile::Decode(const std::string & _content) {
	Codable::Split(_content, 3, &name, &picture, &status);
}

std::string Profile::Encode() const {
	return Codable::Join(3, name, picture, status);
}
