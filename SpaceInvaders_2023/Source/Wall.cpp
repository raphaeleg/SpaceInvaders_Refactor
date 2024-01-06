#include "Wall.hpp"

void Wall::Render(Texture2D texture) noexcept {
    DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)}, DT_dst, DT_o, 0, WHITE);
    DrawText(TextFormat("%i", health), DT_posX, DT_posY, DT_fontSize, RED);
}