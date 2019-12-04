#ifndef CODABLE_H
#define CODABLE_H

#include <string>
#include <vector>
#include <stdarg.h>

struct Codable {

	virtual void Decode(const std::string& _content) = 0;
	virtual std::string Encode() const = 0;

	static std::string Join(int count, Codable const * const ...);
	static std::string Join(int count, std::string ...);
	static void Split(std::string content, int count, Codable* const ...);
	static void Split(std::string content, int count, std::string* const ...);

};

#endif
