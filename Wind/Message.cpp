#include "Message.h"

void Message::Decode(const std::string & _content) {
	Codable::Split(_content, 2, &profile, &content);
}

std::string Message::Encode() const {
	return Codable::Join(2, &profile, &content);
}
