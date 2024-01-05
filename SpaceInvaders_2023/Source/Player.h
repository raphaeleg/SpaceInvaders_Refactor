#include "raylib.h"
#include "EntityType.h"

static constexpr float PLAYER_BASE_HEIGHT = 70.0f;
static constexpr float PLAYER_RADIUS = 50;

class Player
{
private:
    static constexpr float speed = 7;
    int lives = 3;
    float x_pos = GetScreenWidth() / 2.0f;
    float timer = 0;

    static constexpr float animation_speed = 0.4f;
    int activeTexture = 0;
    static constexpr Vector2 DT_o = { 50,50 };

    void UpdateMovement();
    void UpdateAnimation() noexcept;

public:
    EntityType type = EntityType::PLAYER;

    void Render(Texture2D texture) noexcept;
    void Update();
    bool IsDead() const noexcept { return lives <= 0; };
    float GetPosition() const noexcept { return x_pos; };
    int GetLives() const noexcept { return lives; };
    int GetActiveTexture() const noexcept { return activeTexture; };
    void DecreaseHealth() noexcept { lives--; };
};