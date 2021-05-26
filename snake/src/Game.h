#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

const int X = 0;
const int Y = 1;
const int SNAKE_INIT_LEN = 5;
const int BODY_W = 10;
const int BODY_H = 10;
const int START_X = 300;
const int START_Y = 300;
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

class Game {
public:
    Game();
    ~Game();
    int Init();
    void Loop();
    void Shutdown();
private:
    void Input();
    void Update();
    void Output();

    void LoadData();


    bool isRunning;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicks;

    SDL_Texture* mTex;

    enum {
        up,
        down,
        left,
        right
    };

    /** body: 10 x 10 */
    /** game size: 60 x 60 */
    std::vector<std::vector<int>> snake_body;
    int snake_len;
    int snake_dir;

    int snake_head[2];
    int food[2];

    TTF_Font* score;

    void MoveSnake();
    int  InSnake(int x, int y);
    void Draw_Snake();
    int  Rand(int min, int max);
    // int x1, y1, x2, y2;
};

#endif // GAME_H