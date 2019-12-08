#include "Profile.h"

void Profile::Decode(const std::string & _content) {
	Codable::Split(_content, 4, &name, &ip, &picture, &status);
}

std::string Profile::Encode() const {
	return Codable::Join(4, name, ip, picture, status);
}
