#include "UITableView.h"

#include "Transform.h"
#include "Render.h"
#include "Button.h"
#include "Text.h"
#include "EntityEvents.h"
#include "InputEvents.h"
#include "LoadFNT.h"

#include <Events/EventsManager.h>

UITableView::UITableView() {
	Events::EventsManager::GetInstance()->Subscribe("SCROLL_INPUT", &UITableView::ScrollHandler, this);
}

void UITableView::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
}

void UITableView::Initialize() {
	Entity::Initialize();
	cells.clear();
	numberOfRows = nullptr;
	cellForRow = nullptr;
	didSelectRow = nullptr;
}

void UITableView::ReloadData() {
	const unsigned numRows = numberOfRows(this);

	auto transform = GetComponent<Transform>();
	const float width = transform->scale.x - 2.5f;

	while (cells.size() < numRows) {
		auto event = new Events::CreateAnyEntity<UITableViewCell>();
		auto cell = static_cast<UITableViewCell*>(event->entity);
		Events::EventsManager::GetInstance()->Trigger("CREATE_ENTITY", event);

		cell->SetParent(this);
		cell->SetTag("Cell");
		cell->row = cells.size();
		cell->GetComponent<Button>()->BindHandler(&UITableView::ClickHandler, this, MOUSE_CLICK);
		cells.push_back(cell);

		{
			auto event = new Events::CreateAnyEntity<UILabel>();
			auto label = static_cast<UILabel*>(event->entity);
			Events::EventsManager::GetInstance()->Trigger("CREATE_ENTITY", event);
				
			label->GetComponent<Transform>()->translation.y = -0.75f;
			label->GetComponent<Transform>()->scale.x = width;
			label->GetComponent<Render>()->tint.a = 0.f;
			label->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
			label->GetComponent<Text>()->color.Set(1.f);
			label->GetComponent<Text>()->text = "Title";
			label->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;

			cell->title = label;
			label->SetParent(cell);
		}

		{
			auto event = new Events::CreateAnyEntity<UILabel>();
			auto label = static_cast<UILabel*>(event->entity);
			Events::EventsManager::GetInstance()->Trigger("CREATE_ENTITY", event);
				
			label->GetComponent<Transform>()->translation.y = 0.5f;
			label->GetComponent<Transform>()->scale.x = width;
			label->GetComponent<Render>()->tint.a = 0.f;
			label->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
			label->GetComponent<Text>()->color.Set(0.f, 1.f, 1.f, 1.f);
			label->GetComponent<Text>()->scale = 0.5f;
			label->GetComponent<Text>()->text = "Subtitle";
			label->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;

			cell->subtitle = label;
			label->SetParent(cell);
		}

		{
			auto event = new Events::CreateAnyEntity<Sprite>();
			auto sprite = static_cast<UILabel*>(event->entity);
			Events::EventsManager::GetInstance()->Trigger("CREATE_ENTITY", event);
				
			sprite->GetComponent<Transform>()->translation.y = -0.125f;
			sprite->GetComponent<Transform>()->translation.x = -(width * 0.5f) - 1.25f;
			sprite->GetComponent<Transform>()->scale.Set(1.25f);

			cell->image = sprite;
			sprite->SetParent(cell);
		}
	}

	for (unsigned i = 0; i < numRows; ++i) {
		auto cell = cells[i];
		if (!cell->IsUsed())
			cell->Use();

		cell->GetComponent<Transform>()->scale.x = width;
		cell->GetComponent<Transform>()->scale.y = 3.5f;
		cell->GetComponent<Transform>()->translation.y = static_cast<float>(i) * 4.f;
		cell->GetComponent<Transform>()->translation.z = -10.f;

		cellForRow(this, cell, i);
	}

	//for (unsigned i = numRows; i < cells.size(); ++i) {
	//	cells[i]->Destroy();
	//}
}

void UITableView::ScrollHandler(Events::Event * event) {
	auto offset = static_cast<Events::ScrollInput*>(event)->data.y;

	for (auto& row : cells) {
		row->GetComponent<Transform>()->translation.y -= offset;
	}
}

void UITableView::ClickHandler(Entity * target) {
	if (didSelectRow) {
		auto cell = static_cast<UITableViewCell*>(target);
		didSelectRow(this, cell, cell->row);
	}
}
