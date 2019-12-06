#include "Profile.h"

void Profile::Decode(const std::string & _content) {
	Codable::Split(_content, 2, &name, &status);
}

std::string Profile::Encode() const {
	return Codable::Join(2, name, status);
}
