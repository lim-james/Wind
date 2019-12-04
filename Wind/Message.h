#ifndef MESSAGE_H
#define MESSAGE_H

#include "Profile.h"
#include "Content.h"

struct Message : Codable {

	Profile profile;
	Content content;

	void Decode(const std::string& _content) override;
	std::string Encode() const override;

};

#endif

