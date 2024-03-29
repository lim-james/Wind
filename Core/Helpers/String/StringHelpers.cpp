#include "StringHelpers.h"

#include <iterator>

std::vector<std::string> Helpers::Split(const std::string& content, const char& delimiter) {
	std::vector<std::string> parts;
	
	std::string buffer;
	std::istringstream iss(content);

	while (std::getline(iss, buffer, delimiter))
		parts.push_back(buffer);

	return parts;
}

std::vector<std::string> Helpers::Group(const std::string & content, const unsigned& size) {
	const unsigned count = content.length() / size;
	std::vector<std::string> result(count + 1, "");

	for (unsigned i = 0; i < count; ++i) {
		result[i] = content.substr(size * i, size);
	}

	result[count] = content.substr(size * count);
	
	return result;
}

std::string Helpers::Join(const std::vector<std::string>& list) {
	std::string result;

	for (const auto& item : list)
		result += item;

	return result;
}

std::string Helpers::Join(const std::vector<std::string>& list, const char& separator) {
	std::string result;

	for (const auto& item : list)
		result += item + separator;

	return result;
}

std::string Helpers::Join(const std::vector<std::string>& list, const std::string& separator) {
	std::string result;

	for (const auto& item : list)
		result += item + separator;

	return result;
}

//std::pair<std::string, std::string> Helpers::Pair(const std::string& content, const char& separator) {
//	const auto position = content.find(separator);
//	return std::make_pair(content.substr(0, position), content.substr(position + 1));
//}
//
//std::pair<std::string, std::string> Helpers::Pair(const std::string& content, const std::string& separator) {
//	const auto position = content.find(separator);
//	return std::make_pair(content.substr(0, position), content.substr(position + 1));
//}
//
//std::pair<std::string, std::string> Helpers::Pair(const std::string& content, const char* separator) {
//	const auto position = content.find(separator);
//	return std::make_pair(content.substr(0, position), content.substr(position + 1));
//}

std::string Helpers::Trim(std::string content) {
	while (!content.empty() && content.front() == ' ') content.erase(content.begin());
	while (!content.empty() && content.back() == ' ') content.pop_back();

	return content;
}