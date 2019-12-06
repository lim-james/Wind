#include "Content.h"

Content::Content() 
	: type(TEXT_CONTENT)
	, body("") { }

void Content::Decode(const std::string & _content) {
	type = static_cast<int>(_content.front());

	auto content = _content;
	content.erase(content.begin());

	switch (type) {
	case TEXT_CONTENT:
		title = "TEXT";
		content.erase(content.begin());
		body = content;
		break;
	case FILE_CONTENT:
	{
		int length = static_cast<int>(content.front());
		title = content.substr(1, length);
		body = content.substr(length + 1);
	}
		break;
	default:
		break;
	}

}

std::string Content::Encode() const {
	const char length = static_cast<char>(title.length());
	const std::string titleResult = length + title;
	return static_cast<char>(type) + titleResult + body;
}
