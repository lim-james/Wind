#ifndef PROFILE_H
#define PROFILE_H

#include "Codable.h"

struct Profile : Codable {

	std::string name;
	std::string picture;
	std::string status;

	void Decode(const std::string& _content) override;
	std::string Encode() const override;

};

#endif
