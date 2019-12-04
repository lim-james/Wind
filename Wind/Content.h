#ifndef CONTENT_H
#define CONTENT_H

#include "Codable.h"

struct Content : Codable {

	std::string body;

	Content();
	Content(const std::string& body);

	void Decode(const std::string& _content) override;
	std::string Encode() const override;

};

#endif
