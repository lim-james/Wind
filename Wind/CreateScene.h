#ifndef CREATE_SCENE_H
#define CREATE_SCENE_H

#include "ChatManager.h"
#include "UIViewController.h"

#include "UIButton.h"
#include "UITextField.h"

class CreateScene : public UIViewController {

	TCP* client;
	ChatManager* manager;

	UIButton* confirm;

	//UITextField* titleField;
	UITextField* portField;

public:

	CreateScene();
	~CreateScene() override;

	void Start() override;

	void PrepareForSegue(Scene* destination) override;

	void SetChatManager(ChatManager* const _manager);

private:

	//void DidChangeHandler(UITextField* const target);
	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);
	void Close(Entity* target);

	void Join();

};

#endif
