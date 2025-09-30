// beach_lumberjack.c
// Version améliorée : tailles dynamiques, effet coupe arbre, réorganisation

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TREES 15
#define CUT_ANIM_FRAMES 10  // nombre de frames avant disparition arbre

typedef struct {
    int x, y;
    int alive;
    int cutting; // animation en cours
    int frame;   // frame d'animation
} Tree;

int main() {
    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
        dm.w = 800; dm.h = 600;
    }
    int SCREEN_WIDTH = (int)(dm.w * 0.8);
    int SCREEN_HEIGHT = (int)(dm.h * 0.8);

    SDL_Window *window = SDL_CreateWindow("Beach Lumberjack",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        fprintf(stderr, "Erreur création SDL : %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger textures
    SDL_Texture *texBeach = IMG_LoadTexture(renderer, "assets/beach.png");
    SDL_Texture *texRobot = IMG_LoadTexture(renderer, "assets/robot.png");
    SDL_Texture *texTree  = IMG_LoadTexture(renderer, "assets/tree.png");
    SDL_Texture *texSun   = IMG_LoadTexture(renderer, "assets/sun.png");

    if (!texBeach || !texRobot || !texTree || !texSun) {
        fprintf(stderr, "Erreur chargement texture : %s\n", IMG_GetError());
        return 1;
    }

    // Pseudo
    char pseudo[50] = "Joueur";
    printf("Entrez votre pseudo: ");
    if (scanf("%49s", pseudo) != 1) {
        strncpy(pseudo, "Joueur", sizeof(pseudo));
    }

    // Robot au centre
    int robotX = SCREEN_WIDTH / 2;
    int robotY = SCREEN_HEIGHT / 2;
    int score = 0;

    // Génération arbres
    Tree trees[MAX_TREES];
    for (int i = 0; i < MAX_TREES; i++) {
        trees[i].x = rand() % SCREEN_WIDTH;
        trees[i].y = rand() % SCREEN_HEIGHT;
        trees[i].alive = 1;
        trees[i].cutting = 0;
        trees[i].frame = 0;
    }

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                SCREEN_WIDTH = e.window.data1;
                SCREEN_HEIGHT = e.window.data2;

                // replacer arbres dans la fenêtre
                for (int i = 0; i < MAX_TREES; i++) {
                    if (trees[i].alive) {
                        if (trees[i].x > SCREEN_WIDTH - 50) trees[i].x = SCREEN_WIDTH - 50;
                        if (trees[i].y > SCREEN_HEIGHT - 50) trees[i].y = SCREEN_HEIGHT - 50;
                    }
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: running = 0; break;
                    case SDLK_UP:    robotY -= 15; break;
                    case SDLK_DOWN:  robotY += 15; break;
                    case SDLK_LEFT:  robotX -= 15; break;
                    case SDLK_RIGHT: robotX += 15; break;
                }
            }
        }

        // tailles dynamiques
        int ROBOT_SIZE = SCREEN_HEIGHT / 10;
        int TREE_SIZE  = SCREEN_HEIGHT / 12;

        // limiter robot
        if (robotX < 0) robotX = 0;
        if (robotY < 0) robotY = 0;
        if (robotX + ROBOT_SIZE > SCREEN_WIDTH) robotX = SCREEN_WIDTH - ROBOT_SIZE;
        if (robotY + ROBOT_SIZE > SCREEN_HEIGHT) robotY = SCREEN_HEIGHT - ROBOT_SIZE;

        // collisions
        for (int i = 0; i < MAX_TREES; i++) {
            if (trees[i].alive && !trees[i].cutting) {
                if (robotX < trees[i].x + TREE_SIZE &&
                    robotX + ROBOT_SIZE > trees[i].x &&
                    robotY < trees[i].y + TREE_SIZE &&
                    robotY + ROBOT_SIZE > trees[i].y) {
                    trees[i].cutting = 1; // déclenche animation
                    trees[i].frame = 0;
                    score++;
                    printf("Score: %d\n", score);
                }
            }
        }

        // Rendu
        SDL_RenderClear(renderer);

        SDL_Rect bg = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, texBeach, NULL, &bg);

        SDL_Rect sunRect = {20, 20, SCREEN_WIDTH/10, SCREEN_WIDTH/10};
        SDL_RenderCopy(renderer, texSun, NULL, &sunRect);

        // arbres
        for (int i = 0; i < MAX_TREES; i++) {
            if (trees[i].alive) {
                SDL_Rect treeRect = { trees[i].x, trees[i].y, TREE_SIZE, TREE_SIZE };

                if (trees[i].cutting) {
                    // réduire progressivement taille de l’arbre
                    int shrink = (trees[i].frame * TREE_SIZE) / CUT_ANIM_FRAMES;
                    treeRect.w -= shrink;
                    treeRect.h -= shrink;
                    treeRect.x += shrink/2;
                    treeRect.y += shrink/2;
                    trees[i].frame++;

                    if (trees[i].frame >= CUT_ANIM_FRAMES) {
                        trees[i].alive = 0; // arbre disparaît
                    }
                }
                SDL_RenderCopy(renderer, texTree, NULL, &treeRect);
            }
        }

        SDL_Rect robotRect = {robotX, robotY, ROBOT_SIZE, ROBOT_SIZE};
        SDL_RenderCopy(renderer, texRobot, NULL, &robotRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    FILE *f = fopen("scores.txt", "a");
    if (f) {
        fprintf(f, "%s : %d\n", pseudo, score);
        fclose(f);
    }

    SDL_DestroyTexture(texBeach);
    SDL_DestroyTexture(texRobot);
    SDL_DestroyTexture(texTree);
    SDL_DestroyTexture(texSun);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

