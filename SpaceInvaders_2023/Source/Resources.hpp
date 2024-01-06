#pragma once
#include "raylib.h"
#include "vector"
#include <stdexcept>
#include <format>

struct OwnTexture {
	Texture2D _tex;
	std::string_view _path;
	explicit OwnTexture(std::string_view path) {
		_path = path;
		_tex = LoadTexture(path.data());
		if (_tex.id <= 0) {
			throw std::runtime_error(std::format("Could not load texture: {}", path));
		}
	}
	OwnTexture(OwnTexture&&) = default;
	OwnTexture& operator= (OwnTexture&&) = default;
	OwnTexture(OwnTexture const&) = default;
	OwnTexture& operator=(OwnTexture const&) = default;
	~OwnTexture() {
		UnloadTexture(_tex);
	}
};