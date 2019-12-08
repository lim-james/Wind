#ifndef CODABLE_H
#define CODABLE_H

#include <string>
#include <vector>
#include <stdarg.h>

#define CODABLE_BASE 128

struct Codable {

	virtual void Decode(const std::string& _content) = 0;
	virtual std::string Encode() const = 0;

	static std::pair<int, std::string> Decode(std::string content, const int& start);
	static std::pair<int, std::string> Decode(std::string content, const int& start, int& length);
	static void Split(std::string content, int count, Codable* const ...);
	static void Split(std::string content, int count, std::string* const ...);

	static std::string Encode(const std::string& result);
	static std::string Join(int count, Codable const * const ...);
	static std::string Join(int count, std::string ...);

};

#endif
