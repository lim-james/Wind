#ifndef UI_TABLE_VIEW_H
#define UI_TABLE_VIEW_H

#include "UITableViewCell.h"
#include <functional>

class UITableView : public Entity {

	std::vector<UITableViewCell*> cells;

public:

	UITableView();

	virtual void Build();
	virtual void Initialize();

	template<typename Context>
	void BindNumberOfRowsHandler(unsigned(Context::*callback)(UITableView*), Context* context);

	template<typename Context>
	void BindCellForRowHandler(void(Context::*callback)(UITableView*, UITableViewCell*, unsigned), Context* context);

	template<typename Context>
	void BindSelectHandler(void(Context::*callback)(UITableView*, UITableViewCell*, unsigned), Context* context);

	void ReloadData();

private:

	std::function<unsigned(UITableView*)> numberOfRows;
	std::function<void(UITableView*, UITableViewCell*, unsigned)> cellForRow;
	std::function<void(UITableView*, UITableViewCell*, unsigned)> didSelectRow;

	void ScrollHandler(Events::Event* event);

	void ClickHandler(Entity* target);

};

template<typename Context>
void UITableView::BindNumberOfRowsHandler(unsigned(Context::*callback)(UITableView*), Context* context) {
	numberOfRows = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void UITableView::BindCellForRowHandler(void(Context::*callback)(UITableView*, UITableViewCell*, unsigned), Context* context) {
	cellForRow = std::bind(callback, context, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

template<typename Context>
inline void UITableView::BindSelectHandler(void(Context::* callback)(UITableView *, UITableViewCell *, unsigned), Context * context) {
	didSelectRow = std::bind(callback, context, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

#endif
