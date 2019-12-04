#include "Content.h"

Content::Content() : body("") { }

Content::Content(const std::string & body) : body(body) {}

void Content::Decode(const std::string & _content) {
	body = _content;
}

std::string Content::Encode() const {
	return body;
}
