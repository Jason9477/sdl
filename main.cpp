#include <SDL.h>
using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BALL_SIZE = 20;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int PADDLE_SPEED = 5;

int ballX, ballY;
int ballSpeedX = 5;
int ballSpeedY = 5;

int player1X, player1Y;
int player2X, player2Y;

int scorePlayer1 = 0;
int scorePlayer2 = 0;

SDL_Window* window;
SDL_Renderer* renderer;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("2P Volleyball Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void handleEvents(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        exit(0);
    }
    // Player 1 controls
    else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                player1X -= PADDLE_SPEED;
                break;
            case SDLK_RIGHT:
                player1X += PADDLE_SPEED;
                break;
        }
    }
    // Player 2 controls
    else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
                player2X -= PADDLE_SPEED;
                break;
            case SDLK_d:
                player2X += PADDLE_SPEED;
                break;
        }
    }
}

void update() {
    // Update ball position
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Bounce the ball off the walls
    if (ballX < 0 || ballX + BALL_SIZE > SCREEN_WIDTH) {
        ballSpeedX = -ballSpeedX;
    }

    if (ballY < 0) {
        ballSpeedY = -ballSpeedY;
    }

    // Ball collision with paddles
    if (ballY + BALL_SIZE > player1Y && ballX > player1X && ballX < player1X + PADDLE_WIDTH) {
        ballSpeedY = -ballSpeedY;
    }

    if (ballY < player2Y + PADDLE_HEIGHT && ballX > player2X && ballX < player2X + PADDLE_WIDTH) {
        ballSpeedY = -ballSpeedY;
    }

    // Check if the ball goes out of bounds
    if (ballY + BALL_SIZE > SCREEN_HEIGHT) {
        // Player 1 scores
        scorePlayer1++;
        resetGame();
    } else if (ballY < 0) {
        // Player 2 scores
        scorePlayer2++;
        resetGame();
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render ball
    SDL_Rect ballRect = { ballX, ballY, BALL_SIZE, BALL_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &ballRect);

    // Render paddles
    SDL_Rect player1Rect = { player1X, player1Y, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_Rect player2Rect = { player2X, player2Y, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &player1Rect);
    SDL_RenderFillRect(renderer, &player2Rect);

    // Render scores
    renderText("Player 1: " + std::to_string(scorePlayer1), SCREEN_WIDTH / 4, 20);
    renderText("Player 2: " + std::to_string(scorePlayer2), 3 * SCREEN_WIDTH / 4, 20);

    SDL_RenderPresent(renderer);
}

void resetGame() {
    // Reset ball position
    ballX = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ballY = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;

    // Reset paddle positions
    player1X = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    player1Y = SCREEN_HEIGHT - PADDLE_HEIGHT;

    player2X = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    player2Y = 0;
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initSDL()) {
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
