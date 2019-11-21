#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include "TCP.h"

#include "Scene.h"

#include "UITextField.h"
#include "UIButton.h"

#include <Math/Vectors.hpp>

class ChatRoom : public Scene {

	TCP* client;

	UITextField* textField;
	UIButton* confirm;

public:

	ChatRoom();
	~ChatRoom() override;

	void Awake() override;

private:

	void DidChangeHandler(UITextField* const target);
	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);

};

#endif
