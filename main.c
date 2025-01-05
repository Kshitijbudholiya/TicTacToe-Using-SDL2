#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CELL_SIZE = 200;

enum Cell { EMPTY, X, O };
Cell board[3][3];
Cell currentPlayer = X;
bool running = true;
bool gameOver = false;
bool gameDraw = false;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

enum GameState { MENU, GAME };
GameState currentState = MENU;

void init();
void handleEvents();
void update();
void render();
void cleanup();
bool checkWinner(Cell player);
bool checkDraw();
void drawCell(int x, int y, Cell cell);
void resetGame();
void renderText(const std::string& text, int x, int y, SDL_Color color);

int main(int argc, char* argv[]) {
    init();

    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); // ~60 FPS
    }

    cleanup();
    return 0;
}

void init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf Init Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("src/font/LiberationSans-Regular.ttf", 24);  // Load font (ensure you have a valid font)
    if (!font) {
        std::cerr << "Font Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    resetGame();
}

void resetGame() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = EMPTY;
    currentPlayer = X;
    gameOver = false;
    gameDraw = false;
}

void handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            if (currentState == MENU) {
                // Check for Yes/No button clicks
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                if (mouseX > SCREEN_WIDTH / 4 && mouseX < 3 * SCREEN_WIDTH / 4) {
                    if (mouseY > SCREEN_HEIGHT / 3 && mouseY < SCREEN_HEIGHT / 2) {
                        // Yes clicked
                        currentState = GAME; // Start the game
                    } else if (mouseY > SCREEN_HEIGHT / 2 && mouseY < 2 * SCREEN_HEIGHT / 3) {
                        // No clicked
                        running = false; // Exit the game
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && !gameOver) {
                int x = e.button.x / CELL_SIZE;
                int y = e.button.y / CELL_SIZE;

                if (board[y][x] == EMPTY) {
                    board[y][x] = currentPlayer;
                    if (checkWinner(currentPlayer)) {
                        gameOver = true;
                    } else if (checkDraw()) {
                        gameDraw = true;
                        gameOver = true;
                    }
                    currentPlayer = (currentPlayer == X) ? O : X;
                }
            }
        }
    }
}

void update() {
    // Game logic (already handled in handleEvents)
}

void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (currentState == MENU) {
        // Render menu
        SDL_Color color = {255, 0, 0}; // Red color for text
        renderText("Do you want to play Tic-Tac-Toe?", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, color);
        renderText("Yes", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3, color);
        renderText("No", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, color);
    } else if (currentState == GAME) {
        // Render game grid
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 1; i < 3; ++i) {
            SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT);
            SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
        }

        // Draw X and O
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                drawCell(j, i, board[i][j]);
            }
        }

        // Display winner or draw message
        if (gameOver) {
            SDL_Color color = {255, 0, 0}; // Red color for winner
            if (gameDraw) {
                renderText("Draw!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 25, color);
            } else {
                std::string winner = (currentPlayer == X) ? "O wins!" : "X wins!";
                renderText(winner, SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 - 25, color);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();
}

bool checkWinner(Cell player) {
    // Check rows, columns, and diagonals
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;
    return false;
}

bool checkDraw() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == EMPTY)
                return false;
    return true;
}

void drawCell(int x, int y, Cell cell) {
    if (cell == X) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // Improved X: Two crossing lines
        SDL_RenderDrawLine(renderer, x * CELL_SIZE, y * CELL_SIZE,
                          (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
        SDL_RenderDrawLine(renderer, (x + 1) * CELL_SIZE, y * CELL_SIZE,
                          x * CELL_SIZE, (y + 1) * CELL_SIZE);
    } else if (cell == O) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        // Draw O as a circle
        int centerX = x * CELL_SIZE + CELL_SIZE / 2;
        int centerY = y * CELL_SIZE + CELL_SIZE / 2;
        int radius = CELL_SIZE / 2 - 10; // Slightly smaller than the cell to leave some space
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // Horizontal offset
                int dy = radius - h; // Vertical offset
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }
}

void renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Text Render Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
