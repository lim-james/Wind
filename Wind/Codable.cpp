#include "Codable.h"

std::pair<int, std::string> Codable::Decode(std::string content, const int & start) {
	int i = start;
	int power = static_cast<int>(content[i++]);

	std::pair<int, std::string> result;
	result.first = power;
	--power;

	int length = 0;
	for (; power >= 0; --power) {
		length += static_cast<int>(content[i++] * pow(CODABLE_BASE, power));
	}
 
	result.first += length + 1;
	result.second = content.substr(i, length);
	
	return result;
}

std::pair<int, std::string> Codable::Decode(std::string content, const int & start, int & length) {
	int i = start;
	int power = static_cast<int>(content[i++]);

	std::pair<int, std::string> result;
	result.first = power;
	--power;

	length = 0;
	for (; power >= 0; --power) {
		length += static_cast<int>(content[i++] * pow(CODABLE_BASE, power));
	}
 
	result.first += length + 1;
	result.second = content.substr(i, length);
	
	return result;}

void Codable::Split(std::string content, int count, Codable * const...) {
	va_list ap;
	va_start(ap, count);

	auto index = 0;

	for (int i = 0; i < count; ++i) {
		auto arg = va_arg(ap, Codable*);
		const auto result = Decode(content, index);
		arg->Decode(result.second);
		index += result.first;
	}
	
	va_end(ap);
}

void Codable::Split(std::string content, int count, std::string* const ...) {
	va_list ap;
	va_start(ap, count);

	auto index = 0;

	for (int i = 0; i < count; ++i) {
		auto arg = va_arg(ap, std::string*);
		const auto result = Decode(content, index);
		*arg = result.second;
		index += result.first;
	}

	va_end(ap);
}

std::string Codable::Encode(const std::string & result) {
	int length = result.size();
	std::string prefix = "";

	while (length > 0) {
		prefix.insert(prefix.begin(), static_cast<char>(length % CODABLE_BASE));
		length /= CODABLE_BASE;
	}

	prefix.insert(prefix.begin(), static_cast<char>(prefix.size() % CODABLE_BASE));

	return prefix + result;
}

std::string Codable::Join(int count, Codable const * const...) {
	va_list ap;
	va_start(ap, count);

	std::string result = "";

	for (int i = 0; i < count; ++i) {
		auto arg = va_arg(ap, Codable*);
		result += Encode(arg->Encode());
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
		result += Encode(arg);
	}

	va_end(ap);

	return result;
}


