#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TREES 15
#define TREE_SIZE 64
#define ROBOT_SIZE 64
#define DOVE_SIZE 48

typedef struct {
    int x, y;
    int alive;
} Tree;

typedef struct {
    int x, y;
    int active;
} Dove;

int main() {
    srand(time(NULL));

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur SDL_image: %s\n", IMG_GetError());
        return 1;
    }

    // Récupérer la résolution écran
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int SCREEN_WIDTH = dm.w * 0.8;
    int SCREEN_HEIGHT = dm.h * 0.8;

    SDL_Window *window = SDL_CreateWindow("Beach Lumberjack",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger textures
    SDL_Texture *texBeach = IMG_LoadTexture(renderer, "assets/beach.png");
    SDL_Texture *texWaves = IMG_LoadTexture(renderer, "assets/waves.png");
    SDL_Texture *texRobot = IMG_LoadTexture(renderer, "assets/robot.png");
    SDL_Texture *texTree = IMG_LoadTexture(renderer, "assets/tree.png");
    SDL_Texture *texSun = IMG_LoadTexture(renderer, "assets/sun.png");
    SDL_Texture *texDove = IMG_LoadTexture(renderer, "assets/dove.png");

    if (!texBeach || !texWaves || !texRobot || !texTree || !texSun || !texDove) {
        printf("Erreur chargement image: %s\n", IMG_GetError());
        return 1;
    }

    // Joueur
    char pseudo[50];
    printf("Entrez votre pseudo: ");
    scanf("%49s", pseudo);

    int robotX = SCREEN_WIDTH/2, robotY = SCREEN_HEIGHT/2;
    int score = 0;

    // Arbres
    Tree trees[MAX_TREES];
    for (int i = 0; i < MAX_TREES; i++) {
        trees[i].x = rand() % (SCREEN_WIDTH - TREE_SIZE);
        trees[i].y = rand() % (SCREEN_HEIGHT - TREE_SIZE);
        trees[i].alive = 1;
    }

    // Doves (colombes)
    Dove dove = {0, 100, 0};

    int running = 1;
    SDL_Event e;
    int waveOffset = 0;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: running = 0; break;
                    case SDLK_UP:    robotY -= 10; break;
                    case SDLK_DOWN:  robotY += 10; break;
                    case SDLK_LEFT:  robotX -= 10; break;
                    case SDLK_RIGHT: robotX += 10; break;
                }
            }
        }

        // Collision avec arbres
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

        // Animation mer
        waveOffset = (waveOffset + 2) % SCREEN_WIDTH;

        // Apparition aléatoire des colombes
        if (!dove.active && rand() % 500 == 0) {
            dove.x = -DOVE_SIZE;
            dove.y = rand() % (SCREEN_HEIGHT/2);
            dove.active = 1;
        }
        if (dove.active) {
            dove.x += 5;
            if (dove.x > SCREEN_WIDTH) dove.active = 0;
        }

        // Rendu
        SDL_RenderClear(renderer);

        // Fond plage
        SDL_RenderCopy(renderer, texBeach, NULL, NULL);

        // Mer animée (deux vagues qui bougent en boucle)
        SDL_Rect waveRect1 = { -waveOffset, SCREEN_HEIGHT-150, SCREEN_WIDTH, 150 };
        SDL_Rect waveRect2 = { SCREEN_WIDTH-waveOffset, SCREEN_HEIGHT-150, SCREEN_WIDTH, 150 };
        SDL_RenderCopy(renderer, texWaves, NULL, &waveRect1);
        SDL_RenderCopy(renderer, texWaves, NULL, &waveRect2);

        // Soleil
        SDL_Rect sunRect = {50, 50, 100, 100};
        SDL_RenderCopy(renderer, texSun, NULL, &sunRect);

        // Arbres
        for (int i = 0; i < MAX_TREES; i++) {
            if (trees[i].alive) {
                SDL_Rect treeRect = {trees[i].x, trees[i].y, TREE_SIZE, TREE_SIZE};
                SDL_RenderCopy(renderer, texTree, NULL, &treeRect);
            }
        }

        // Robot
        SDL_Rect robotRect = {robotX, robotY, ROBOT_SIZE, ROBOT_SIZE};
        SDL_RenderCopy(renderer, texRobot, NULL, &robotRect);

        // Colombes
        if (dove.active) {
            SDL_Rect doveRect = {dove.x, dove.y, DOVE_SIZE, DOVE_SIZE};
            SDL_RenderCopy(renderer, texDove, NULL, &doveRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Sauvegarde score
    FILE *f = fopen("scores.txt", "a");
    if (f) {
        fprintf(f, "%s : %d\n", pseudo, score);
        fclose(f);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

