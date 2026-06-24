/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** renderer
*/

#include "renderer.hpp"

renderer::renderer()
{
    _ingotModel = LoadModel("src/gui/models/ingot.glb");
    _breadModel = LoadModel("src/gui/models/bread.glb");
    _beanModel = LoadModel("src/gui/models/bean.glb");
    _diamondModel = LoadModel("src/gui/models/diamond.glb");
    _villagerModel = LoadModel("src/gui/models/villager.glb");
    _eggModel = LoadModel("src/gui/models/egg.glb");
    _grassModel = LoadModel("src/gui/models/grass.glb");
    _islandModel = LoadModel("src/gui/models/island.glb");
    if (_islandModel.meshCount > 0) {
        BoundingBox bb = GetModelBoundingBox(_islandModel);
        float w = bb.max.x - bb.min.x;
        _islandScale = (w > 0.0f) ? 0.95f / w : 1.0f;
    }
}

renderer::~renderer()
{
    UnloadModel(_ingotModel);
    UnloadModel(_breadModel);
    UnloadModel(_beanModel);
    UnloadModel(_diamondModel);
    UnloadModel(_villagerModel);
    UnloadModel(_eggModel);
    UnloadModel(_grassModel);
    UnloadModel(_islandModel);
}

void renderer::drawSky(float light) const
{
    Color skyDay = {120, 190, 255, 255};
    Color skyNight = {12, 14, 40, 255};
    Color sky = {
        (unsigned char)(skyNight.r + (skyDay.r - skyNight.r) * light),
        (unsigned char)(skyNight.g + (skyDay.g - skyNight.g) * light),
        (unsigned char)(skyNight.b + (skyDay.b - skyNight.b) * light),
        255
    };
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), sky);
}

void renderer::drawGround(const game_state &state, float light,
    const Camera3D &cam) const
{
    (void)state;
    if (_grassModel.meshCount == 0)
        return;

    BoundingBox bb = GetModelBoundingBox(_grassModel);
    float modelW = bb.max.x - bb.min.x;
    if (modelW <= 0.0f)
        modelW = 1.0f;
    float s = 1.0f / modelW;
    float y = -0.05f - bb.max.y * s;

    auto shade = static_cast<unsigned char>(80 + 175 * light);
    Color tint = {shade, shade, shade, 255};

    int cx = (int)cam.target.x;
    int cz = (int)cam.target.z;
    int half = 50;

    for (int z = cz - half; z <= cz + half; z++)
        for (int x = cx - half; x <= cx + half; x++)
            DrawModel(_grassModel, {static_cast<float>(x), y,
                static_cast<float>(z)}, s, tint);
}

void renderer::drawCelestials(const game_state &state, float phase) const
{
    float angle = phase * 2.0f * PI;
    float cx = (state.width() - 1) / 2.0f;
    float cz = (state.height() - 1) / 2.0f;

    float radius = 30.0f;
    float heightFactor = 0.6f;
    float bodySize = 3.0f;

    float sx = std::cos(angle) * radius;
    float sy = std::sin(angle) * radius * heightFactor;

    if (sy > 0.0f)
        DrawCube({cx + sx, sy, cz}, bodySize, bodySize, bodySize,
            Color{255, 220, 90, 255});
    else
        DrawCube({cx - sx, -sy, cz}, bodySize, bodySize, bodySize,
            Color{230, 230, 240, 255});
}

void renderer::drawTile(int x, int y) const
{
    Vector3 pos = {static_cast<float>(x), 0.0f, static_cast<float>(y)};

    if (_islandModel.meshCount > 0) {
        for (int i = 0; i < _islandModel.meshCount; i++) {
            Color c = (i == 0)
            ? Color{38, 111, 4, 255}
            : Color{107, 58, 42, 255};
            _islandModel.materials[_islandModel.meshMaterial[i]]
                .maps[MATERIAL_MAP_DIFFUSE].color = c;
            DrawMesh(_islandModel.meshes[i],
                _islandModel.materials[_islandModel.meshMaterial[i]],
                MatrixMultiply(
                    MatrixScale(_islandScale, _islandScale, _islandScale),
                    MatrixTranslate(pos.x, pos.y, pos.z)));
        }
    } else {
        DrawCube(pos, 0.95f, 0.1f, 0.95f, Color{34, 139, 34, 255});
        DrawCubeWires(pos, 0.95f, 0.1f, 0.95f, Color{0, 80, 0, 255});
    }
}

void renderer::drawTiles(const game_state &state) const
{
    for (int y = 0; y < state.height(); y++)
        for (int x = 0; x < state.width(); x++)
            drawTile(x, y);
}

void renderer::drawResource(int i, Vector3 pos) const
{
    static const Color fallback[7] = {
        BEIGE, LIGHTGRAY, BROWN, SKYBLUE, PURPLE, ORANGE, GOLD
    };

    switch (i) {
    case 0:
        if (_breadModel.meshCount > 0)
            DrawModel(_breadModel, pos, 0.8f, WHITE);
        else
            DrawCube(pos, 0.2f, 0.2f, 0.2f, fallback[i]);
        break;
    case 2:
        if (_beanModel.meshCount > 0)
            DrawModelEx(_beanModel, pos, {1.0f, 0.0f, 0.0f}, 0.0f,
                {0.05f, 0.05f, 0.05f}, BROWN);
        else
            DrawCube(pos, 0.2f, 0.2f, 0.2f, fallback[i]);
        break;
    case 3:
        if (_diamondModel.meshCount > 0)
            DrawModel(_diamondModel, pos, 0.03f, WHITE);
        else
            DrawCube(pos, 0.2f, 0.2f, 0.2f, fallback[i]);
        break;
    case 1:
        if (_ingotModel.meshCount > 0)
            DrawModel(_ingotModel, pos, 0.05f, LIGHTGRAY);
        else
            DrawCube(pos, 0.2f, 0.2f, 0.2f, fallback[i]);
        break;
    case 4:
        if (_ingotModel.meshCount > 0)
            DrawModel(_ingotModel, pos, 0.05f, PURPLE);
        else
            DrawCube(pos, 0.2f, 0.2f, 0.2f, fallback[i]);
        break;
    case 5:
        if (_ingotModel.meshCount > 0)
            DrawModel(_ingotModel, pos, 0.05f, ORANGE);
        else
            DrawCube(pos, 0.2f, 0.2f, 0.2f, fallback[i]);
        break;
    case 6:
        if (_ingotModel.meshCount > 0)
            DrawModel(_ingotModel, pos, 0.05f, GOLD);
        else
            DrawCube(pos, 0.2f, 0.2f, 0.2f, fallback[i]);
        break;
    default:
        break;
    }
}

void renderer::drawTileResources(int x, int y, const game_state &state) const
{
    static const Vector2 offsets[7] = {
        {-0.3f, -0.3f}, {0.0f, -0.3f}, {0.3f, -0.3f},
        {-0.3f,  0.0f},                {0.3f,  0.0f},
        {-0.3f,  0.3f}, {0.0f,  0.3f}
    };
    const Tile &cell = state.tile(x, y);

    for (int i = 0; i < 7; i++) {
        if (cell.resources[i] <= 0)
            continue;
        Vector3 pos = {static_cast<float>(x) + offsets[i].x, 0.4f,
            static_cast<float>(y) + offsets[i].y};
        drawResource(i, pos);
    }
}

void renderer::drawResources(const game_state &state) const
{
    for (int y = 0; y < state.height(); y++)
        for (int x = 0; x < state.width(); x++)
            drawTileResources(x, y, state);
}

void renderer::drawEggs(const game_state &state) const
{
    for (const Egg &egg : state.eggs()) {
        Vector3 pos = {static_cast<float>(egg.x), 0.45f,
            static_cast<float>(egg.y)};
        if (_eggModel.meshCount > 0)
            DrawModel(_eggModel, pos, 0.05f, WHITE);
        else
            DrawSphere({pos.x, 0.25f, pos.z}, 0.2f, WHITE);
    }
}

void renderer::drawPlayers(const game_state &state) const
{
    for (const Player &player : state.players()) {
        Vector3 pos = {static_cast<float>(player.x), 0.3f,
            static_cast<float>(player.y)};
        if (_villagerModel.meshCount > 0)
            DrawModel(_villagerModel, pos, 0.05f, WHITE);
        else
            DrawCube({pos.x, 0.4f, pos.z}, 0.4f, 0.6f, 0.4f, RED);
    }
}

void renderer::drawPlayerLevels(const game_state &state, const Camera3D &cam) const
{
    const std::vector<Player> &players = state.players();

    for (size_t i = 0; i < players.size(); i++) {
        Vector3 pos = {static_cast<float>(players[i].x), 1.0f,
            static_cast<float>(players[i].y)};
        Vector2 screen = GetWorldToScreen(pos, cam);
        const char *text = TextFormat("Lv.%d", players[i].level);
        int width = MeasureText(text, 18);

        DrawText(text, static_cast<int>(screen.x) - width / 2,
            static_cast<int>(screen.y), 18, YELLOW);
    }
}

void renderer::drawElapsedTime(const game_state &state)
{
    const double baseFreq = 100.0;
    int elapsed;
    int minutes;
    int seconds;
    const char *text;
    int width;

    if (!state.ready())
        return;
    _gameElapsed += GetFrameTime() * (state.timeUnit() / baseFreq);
    elapsed = static_cast<int>(_gameElapsed);
    minutes = elapsed / 60;
    seconds = elapsed % 60;
    text = TextFormat("%02d:%02d", minutes, seconds);
    width = MeasureText(text, 24);
    DrawText(text, GetScreenWidth() - width - 20, 20, 24, RAYWHITE);
}

void renderer::draw(const game_state &state, const Camera3D &cam,
    const ParticleSystem &particles)
{
    float phase = std::fmod(GetTime(), 60.0f) / 60.0f;
    float light = (std::sin(phase * 2.0f * PI) + 1.0f) / 2.0f;

    drawSky(light);
    if (!state.ready()) {
        DrawText("Waiting for server data...", 20, 20, 20, RAYWHITE);
        return;
    }
    BeginMode3D(cam);
    drawGround(state, light, cam);
    drawCelestials(state, phase);
    drawTiles(state);
    drawResources(state);
    drawEggs(state);
    drawPlayers(state);
    particles.draw(cam);
    EndMode3D();
    if (IsKeyPressed(KEY_L))
        _showLevels = !_showLevels;
    if (_showLevels)
        drawPlayerLevels(state, cam);
    drawElapsedTime(state);
}