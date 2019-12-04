#include "Content.h"

#include <Logger/Logger.h>

Content::Content() 
	: type(TEXT_CONTENT)
	, body("") { }

void Content::Decode(const std::string & _content) {

	Console::Warn << "Decoding\n";
	Console::Warn << _content << '\n';

	type = static_cast<int>(_content.front());

	Console::Warn << "Type : " << type << '\n';

	auto content = _content;
	content.erase(content.begin());

	switch (type) {
	case TEXT_CONTENT:
		title = "TEXT";
		body = _content;
		break;
	case FILE_CONTENT:
	{
		const int length = static_cast<int>(content.front());
		title = content.substr(1, length);
		body = content.substr(length + 1);
		Console::Warn << "file content.\n";
		Console::Warn << "Length : " << length << '\n';
		Console::Warn << "Title : " << title << '\n';
		Console::Warn << "Body : " << body << '\n';
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
