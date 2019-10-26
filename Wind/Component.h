#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

struct Component {

	Component();
	virtual ~Component();

	virtual void Initialize();

	virtual void SetActive(const bool& state);
	const bool& IsActive() const;

	void SetParent(Entity* const entity);
	Entity* const GetParent();

protected:

	Entity* parent;
	bool active;

};

#endif
