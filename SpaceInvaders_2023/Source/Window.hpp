#pragma once
#include "raylib.h"
#include <string_view>
#include <print>
//TODO: a Window object could probably also configure the FTP
class Window {
public:
    Window(const int screenWidth, const int screenHeight, std::string_view title) {
        try {
            InitWindow(screenWidth, screenHeight, title.data());
        }
        catch (const std::runtime_error& e) {
            std::println("Error: Could not create window", e.what());
        }
    };
    ~Window() { CloseWindow(); };
    Window(const Window& other) = delete;
    Window operator= (const Window& other) = delete;
    Window(const Window&& other) = delete;
    Window operator= (const Window&& other) = delete;
};