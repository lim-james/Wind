#ifndef RENDER_H
#define RENDER_H

#include "Component.h"

#include <Math/Vectors.hpp>

struct Render : Component {

	unsigned texture;
	// size of UV in tilemap
	vec4f uvRect;
	vec4f tint;

	Render();

	void Initialize() override;
	void SetActive(const bool& state) override;

	void SetTilemapSize(const int& width, const int& height);
	void SetCellRect(const int& x, const int& y, const int& width, const int& height);

private:

	vec2f tilemapUnit;
	vec4f cellRect;

};

#endif