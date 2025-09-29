#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define ROBOT_SIZE 50
#define TREE_SIZE 40
#define MAX_TREES 10

typedef struct {
    int x, y;
    int alive;
} Tree;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Beach Lumberjack",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("Erreur creation fenetre: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur creation renderer: %s\n", SDL_GetError());
        return 1;
    }

    srand(time(NULL));

    // Robot position
    int robotX = SCREEN_WIDTH/2;
    int robotY = SCREEN_HEIGHT/2;
    int score = 0;

    // Créer des arbres
    Tree trees[MAX_TREES];
    for (int i = 0; i < MAX_TREES; i++) {
        trees[i].x = rand() % (SCREEN_WIDTH - TREE_SIZE);
        trees[i].y = rand() % (SCREEN_HEIGHT - TREE_SIZE);
        trees[i].alive = 1;
    }

    int running = 1;
    SDL_Event e;

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    robotY -= 10; break;
                    case SDLK_DOWN:  robotY += 10; break;
                    case SDLK_LEFT:  robotX -= 10; break;
                    case SDLK_RIGHT: robotX += 10; break;
                }
            }
            if (e.type == SDL_MOUSEMOTION) {
                robotX = e.motion.x;
                robotY = e.motion.y;
            }
        }

        // Collision robot-arbres
        for (int i = 0; i < MAX_TREES; i++) {
            if (trees[i].alive &&
                robotX < trees[i].x + TREE_SIZE &&
                robotX + ROBOT_SIZE > trees[i].x &&
                robotY < trees[i].y + TREE_SIZE &&
                robotY + ROBOT_SIZE > trees[i].y) {
                trees[i].alive = 0;
                score++;
                printf("Score: %d\n", score);
            }
        }

        // Effacer écran (plage = bleu ciel)
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        // Dessiner robot (carré rouge)
        SDL_Rect robot = {robotX, robotY, ROBOT_SIZE, ROBOT_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &robot);

        // Dessiner arbres (carrés verts)
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        for (int i = 0; i < MAX_TREES; i++) {
            if (trees[i].alive) {
                SDL_Rect tree = {trees[i].x, trees[i].y, TREE_SIZE, TREE_SIZE};
                SDL_RenderFillRect(renderer, &tree);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

