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
	OwnTexture(OwnTexture&& rhs) noexcept {
		std::swap(rhs._tex, _tex);
	}
	OwnTexture& operator= (OwnTexture&& rhs) noexcept {
		if (this != &rhs) { std::swap(rhs._tex, _tex); }
		return *this;
	}
	OwnTexture(OwnTexture const&) = default;
	OwnTexture& operator=(OwnTexture const&) = default;
	~OwnTexture() {
		UnloadTexture(_tex);
	}
	Texture2D Get() const noexcept { return _tex; }
};

struct Resources {
	std::vector<OwnTexture> shipTextures;
	const OwnTexture alienTexture{ "Assets/Alien.png" };
	const OwnTexture barrierTexture{ "Assets/Barrier.png" };
	const OwnTexture laserTexture{ "Assets/Laser.png" };
	const OwnTexture shipTexture1{ "Assets/Ship1.png" };
	const OwnTexture shipTexture2{ "Assets/Ship2.png" };
	const OwnTexture shipTexture3{ "Assets/Ship3.png" };
	[[nodiscard]] Texture2D GetAlien() const noexcept { return alienTexture.Get(); }
	[[nodiscard]] Texture2D GetWall() const noexcept { return barrierTexture.Get(); }
	[[nodiscard]] Texture2D GetProjectile() const noexcept { return laserTexture.Get(); }
	[[nodiscard]] Texture2D GetShip(std::size_t i) const noexcept { 
		switch (i) {
		case 1: return shipTexture2._tex;
		case 2: return shipTexture3._tex;
		default: return shipTexture1._tex;
		}
	}
};