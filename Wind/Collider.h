#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#include <functional>

struct Collider : Component {

	Collider();
	
	void Initialize() override;
	void SetActive(const bool& state) override;

	template<typename Context>
	void BindCollisionEnter(void(Context::*callback)(Entity*), Context* context);
	template<typename Context>
	void BindCollisionEnter(void(Context::*callback)(Entity*) const, Context* context);

	template<typename Context>
	void BindCollisionStay(void(Context::*callback)(Entity*), Context* context);
	template<typename Context>
	void BindCollisionStay(void(Context::*callback)(Entity*) const, Context* context);

	template<typename Context>
	void BindCollisionExit(void(Context::*callback)(Entity*), Context* context);
	template<typename Context>
	void BindCollisionExit(void(Context::*callback)(Entity*) const, Context* context);

	void OnCollisionEnter(Entity* const object) const;
	void OnCollisionStay(Entity* const object) const;
	void OnCollisionExit(Entity* const object) const;

private:

	std::function<void(Entity* const)> enterCallback;
	std::function<void(Entity* const)> stayCallback;
	std::function<void(Entity* const)> exitCallback;
	
};

template<typename Context>
void Collider::BindCollisionEnter(void(Context::*callback)(Entity*), Context* context) {
	enterCallback = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Collider::BindCollisionEnter(void(Context::*callback)(Entity*) const, Context* context) {
	enterCallback = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Collider::BindCollisionStay(void(Context::*callback)(Entity*), Context* context) {
	stayCallback = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Collider::BindCollisionStay(void(Context::*callback)(Entity*) const, Context* context) {
	stayCallback = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Collider::BindCollisionExit(void(Context::*callback)(Entity*), Context* context) {
	exitCallback = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Collider::BindCollisionExit(void(Context::*callback)(Entity*) const, Context* context) {
	exitCallback = std::bind(callback, context, std::placeholders::_1);
}


#endif
