//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#define nullptr NULL
#include <iostream>
//test
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BALL_SIZE = 20;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int PADDLE_SPEED = 5;

int ballX, ballY;
int ballSpeedX = 5;
int ballSpeedY = 5;

int player1X, player2X;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* pikachuTexture;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("1D Pikachu Volleyball Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void handleEvents(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        exit(0);
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                player1X -= PADDLE_SPEED;
                break;
            case SDLK_RIGHT:
                player1X += PADDLE_SPEED;
                break;
            case SDLK_a:
                player2X -= PADDLE_SPEED;
                break;
            case SDLK_d:
                player2X += PADDLE_SPEED;
                break;
        }
    }
}
void resetGame() {
    ballX = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ballY = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;

    player1X = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    player2X = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
}
void update() {
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    if (ballX < 0 || ballX + BALL_SIZE > SCREEN_WIDTH) {
        ballSpeedX = -ballSpeedX;
    }

    if (ballY + BALL_SIZE > SCREEN_HEIGHT || ballY < 0) {
        ballSpeedY = -ballSpeedY;
    }

    // Ball collision with paddles
    if (ballY + BALL_SIZE > SCREEN_HEIGHT - PADDLE_HEIGHT && ballX > player1X && ballX < player1X + PADDLE_WIDTH) {
        ballSpeedY = -ballSpeedY;
    }

    if (ballY < PADDLE_HEIGHT && ballX > player2X && ballX < player2X + PADDLE_WIDTH) {
        ballSpeedY = -ballSpeedY;
    }

    // Check if the ball goes out of bounds
    if (ballY + BALL_SIZE > SCREEN_HEIGHT) {
        std::cout << "Player 1 scores!" << std::endl;
        resetGame();
    } else if (ballY < 0) {
        std::cout << "Player 2 scores!" << std::endl;
        resetGame();
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect ballRect = { ballX, ballY, BALL_SIZE, BALL_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ballRect);

    SDL_Rect player1Rect = { player1X, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_Rect player2Rect = { player2X, 0, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &player1Rect);
    SDL_RenderFillRect(renderer, &player2Rect);

    SDL_Rect pikachuRect1 = { player1X, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_Rect pikachuRect2 = { player2X, 0, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_RenderCopy(renderer, pikachuTexture, nullptr, &pikachuRect1);
    SDL_RenderCopy(renderer, pikachuTexture, nullptr, &pikachuRect2);

    SDL_RenderPresent(renderer);
}



void closeSDL() {
    SDL_DestroyTexture(pikachuTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initSDL()) {
        return -1;
    }

    pikachuTexture = loadTexture("pikachu.png"); // Replace "pikachu.png" with the path to your Pikachu image file

    if (pikachuTexture == nullptr) {
        closeSDL();
        return -1;
    }

    resetGame();

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            handleEvents(e);
        }

        update();
        render();
    }

    closeSDL();

    return 0;
}
