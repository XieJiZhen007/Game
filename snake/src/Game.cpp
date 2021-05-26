#include "Game.h"
#include <math.h>
#include <time.h>

Game::Game():isRunning(true), mWindow(nullptr), mRenderer(nullptr) {
    // x1 = 400;
    // y1 = 300;
    // x2 = 400;
    // y2 = 100;
    // BODY_W = 10;
    mTex = nullptr;
    snake_dir = right;
    snake_len = SNAKE_INIT_LEN;
    score = nullptr;
}

Game::~Game() {

}

int Game::Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Failed to init sdl2\n");
        return 0;
    }

    mWindow = SDL_CreateWindow("title", 100, 100, 800, 600, 0);
    if (!mWindow) {
        SDL_Log("Failed to create window\n");
        return 0;   
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer\n: %s", SDL_GetError());
        return 0;
    }

    TTF_Init();
    score = TTF_OpenFont("../assets/himalaya.ttf", 24);
    if (!score) {
        SDL_Log("Failed to get ttf: %s", SDL_GetError());
        return 0;
    }

    /** init snake */
    for (int i = 0; i < SNAKE_INIT_LEN; i++) {
        std::vector<int> pos = {START_X - i * BODY_W, START_Y};
        snake_body.push_back(pos);
    }

    snake_head[X] = snake_body[0][X];
    snake_head[Y] = snake_body[0][Y];
    for (auto i : snake_body) {
        for (auto j : i) {
            // SDL_Log("%d\n", j);
        }
    }
    // LoadData();

    srand(time(nullptr));

    mTicks = SDL_GetTicks();

    do {
        food[X] = Rand(0, SCREEN_WIDTH - BODY_W);
        food[Y] = Rand(0, SCREEN_HEIGHT - BODY_H);
    } while (InSnake(food[X], food[Y]));
    // Snake
    return 1;
}

void Game::Loop() {
    while (isRunning) {
        Input();
        Update();
        Output();
        // SDL_Delay(500);
    }
    // Output();
}

void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    // SDL_Delay(2000);
}

void Game::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_W] && snake_dir != down) {
        snake_dir = up;
    } else if (state[SDL_SCANCODE_S] && snake_dir != up) {
        snake_dir = down;
    } else if (state[SDL_SCANCODE_A] && snake_dir != right) {
        snake_dir = left;
    } else if (state[SDL_SCANCODE_D] && snake_dir != left) {
        snake_dir = right;
    }
}

void Game::Update() {
    // SDL_Log("in update");
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicks + 16));
    float deltaTime = (SDL_GetTicks() - mTicks) / 1000.0f;
    if (deltaTime > 0.05)
        deltaTime = 0.05f;
    mTicks = SDL_GetTicks();

    switch(snake_dir) {
        case up:
            snake_head[Y] -= BODY_H;
            break;
        case down:
            snake_head[Y] += BODY_H;
            break;
        case left:
            snake_head[X] -= BODY_W;
            break;
        case right:
            snake_head[X] += BODY_W;
            break;
    }
    // SDL_Log("X: %d, Y: %d", snake_head[X], snake_head[Y]);

    for (int i = 1; i < snake_len; i++) {
        if (snake_head[X] == snake_body[i][X] && snake_head[Y] == snake_body[i][Y]) {
            isRunning = false;
            return;
        }
    }

    if (snake_head[X] < 0 || snake_head[X] >= SCREEN_WIDTH ||
        snake_head[Y] < 0 || snake_head[Y] >= SCREEN_HEIGHT) {
        
        if (snake_head[X] < 0) {
            snake_head[X] = SCREEN_WIDTH - BODY_W;
        } else if (snake_head[X] >= SCREEN_WIDTH) {
            snake_head[X] = 0;
        }

        if (snake_head[Y] < 0) {
            snake_head[Y] = SCREEN_HEIGHT - BODY_H;
        } else if (snake_head[Y] >= SCREEN_HEIGHT) {
            snake_head[Y] = 0;
        }
    }

    if (InSnake(food[X], food[Y])) {
        std::vector<int> pos = {0, 0};
        snake_body.push_back(pos);

        snake_len++;
        do {
            food[X] = Rand(0, SCREEN_WIDTH - BODY_W);
            food[Y] = Rand(0, SCREEN_HEIGHT - BODY_H);
        } while (InSnake(food[X], food[Y]));
    }
    MoveSnake();

    SDL_Delay(50);

}

void Game::Output() {
    // static int x = 0, y = 0;
    // SDL_Log("in output");
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    // SDL_Rect r = {100, 100, BODY_W, BODY_H};
    // SDL_RenderDrawRect(mRenderer, &r);
    // SDL_RenderDrawLine(mRenderer, 0, 0, x, y);
    SDL_RenderDrawLine(mRenderer, SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Color white = {255, 255, 255};
    SDL_Surface* sufMes = TTF_RenderText_Solid(score, "hello", white);
    SDL_Texture* Mes = SDL_CreateTextureFromSurface(mRenderer, sufMes);
    SDL_Rect MesRect = {600, 300, 100, 100};
    SDL_RenderCopy(mRenderer, Mes, nullptr, &MesRect);

    // x = x % 800 + 10;
    // y = y % 600 + 10;
    // mTex position
    // SDL_Rect r = {100, 100, 500, 230};
    // SDL_RenderCopy(mRenderer, mTex, nullptr, &r);
    Draw_Snake();

    SDL_Rect pos = {food[X], food[Y], BODY_W, BODY_H};
    SDL_RenderDrawRect(mRenderer, &pos);

    SDL_RenderPresent(mRenderer);

}

void Game::LoadData() {
    mTex = IMG_LoadTexture(mRenderer, "../assets/abc.png");

    // if (mTex) {
    //     SDL_Log("png load success!\n");
    // }
}

void Game::MoveSnake() {
    for (int i = snake_len - 1; i > 0; i--) {
        snake_body[i][X] = snake_body[i - 1][X];
        snake_body[i][Y] = snake_body[i - 1][Y];
    }

    snake_body[0][X] = snake_head[X];
    snake_body[0][Y] = snake_head[Y];

}

int Game::InSnake(int x, int y) {
    if (snake_head[X] == x && snake_head[Y] == y) {
        return 1;
    } 

    for (int i = 0; i < snake_len; i++) {
        if (snake_body[i][X] == x && snake_body[i][Y] == y) 
            return 1;
    }

    return 0;
}

void Game::Draw_Snake() {
    // SDL_Log("Draw Snake");
    for (int i = 0; i < snake_len; i++) {
        // SDL_Log("Draw Snake loop, i: %d", i);
        // SDL_Log("Draw Snake loop, X: %d, Y: %d", snake_body[i][X],snake_body[i][Y]);
        SDL_Rect body = {snake_body[i][X], snake_body[i][Y], BODY_W, BODY_H};
        SDL_RenderDrawRect(mRenderer, &body);
    }

    // SDL_Rect body = {snake_body[0][X], snake_body[0][Y], BODY_W, BODY_H};
    // SDL_RenderDrawRect(mRenderer, &body);
    // SDL_Log("body_x: %d, body_y: %d", snake_body[0][X], snake_body[0][Y]);

    // SDL_Log("out Draw snake");
}

int Game::Rand(int min, int max) {
    return (((rand() % (max - 1 + min)) + min) / 10) * 10;
}