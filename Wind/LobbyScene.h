#ifndef LOBBY_SCENE_H
#define LOBBY_SCENE_H

#include "Profile.h"
#include "ChatManager.h"

#include "Scene.h"

#include "UITextField.h"
#include "UIButton.h"
#include "Sprite.h"

#include <Events/Event.h>

class LobbyScene : public Scene {

	TCP* client;
	ChatManager* manager;

	UIButton* confirm;

	UITextField* ipField;
	UITextField* portField;

public:

	LobbyScene();
	~LobbyScene() override;

	void Awake() override;
	void Start() override;

	void PrepareForSegue(Scene* destination) override;

	void SetChatManager(ChatManager* const _manager);

private:

	void DidChangeHandler(UITextField* const target);
	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);
	void Close(Entity* target);

	UITextField* CreateTextField(const std::string& _prompt = "");

	void Connect();

};

#endif
