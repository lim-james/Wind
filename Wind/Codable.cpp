#include "Codable.h"

std::string Codable::Join(int count, Codable const * const...) {
	va_list ap;
	va_start(ap, count);

	std::string result = "";

	for (int i = 0; i < count; ++i) {
		auto arg = va_arg(ap, Codable*);
		auto content = arg->Encode();
		char prefix = static_cast<char>(content.length());
		result += prefix + content;
	}

	va_end(ap);

	return result;
}

std::string Codable::Join(int count, std::string ...) {
	va_list ap;
	va_start(ap, count);

	std::string result = "";

	for (int i = 0; i < count; ++i) {
		auto arg = va_arg(ap, std::string);
		char prefix = static_cast<char>(arg.length());
		result += prefix + arg;
	}

	va_end(ap);

	return result;
}

void Codable::Split(std::string content, int count, Codable * const...) {
	va_list ap;
	va_start(ap, count);

	auto index = 0;

	for (int i = 0; i < count; ++i) {
		auto arg = va_arg(ap, Codable*);
		int length = static_cast<int>(content[index]);
		arg->Decode(content.substr(index + 1, length));
		index += length + 1;
	}

	va_end(ap);
}

void Codable::Split(std::string content, int count, std::string* const ...) {
	va_list ap;
	va_start(ap, count);

	auto index = 0;

	for (int i = 0; i < count; ++i) {
		auto arg = va_arg(ap, std::string*);
		int length = static_cast<int>(content[index]);
		*arg = content.substr(index + 1, length);
		index += length + 1;
	}

	va_end(ap);}
