#ifndef CONTENT_H
#define CONTENT_H

#include "Codable.h"

#define TEXT_CONTENT 0
#define FILE_CONTENT 1
#define JOIN_CONTENT 2

struct Content : Codable {

	int type;
	std::string title;
	std::string body;

	Content();

	void Decode(const std::string& _content) override;
	std::string Encode() const override;

};

#endif
