#include "menus/mapmenu.h"
#include "menus/menu_battle.h"
#include "graphics/ui.h"
#include "gamestate.h"
#include "map.h"

#include <stdlib.h>
#include <time.h>

String getCommand() {
    String result = StringCreate("");

    char k = (char) TerminalWaitKey();
    if (k == 0x1b) {
        char x = (char) TerminalWaitKey();
        char y = (char) TerminalWaitKey();
        if (x == 91)
            switch (y) {
                case 65 : StringAppendString(&result, StringCreate("GU")); break;
                case 68 : StringAppendString(&result, StringCreate("GL")); break;
                case 67 : StringAppendString(&result, StringCreate("GR")); break;
                case 66 : StringAppendString(&result, StringCreate("GD")); break;
            }
    } else {
        if (k == '\n')
            return StringCreate("");
        wprintf(L"%c", k);
        StringAppendChar(&result, k);
        String new = StringCreate("");
        StringReadln(&new);
        StringAppendString(&result, new);
    }

    return result;
}

MapNode* getRandomMap() {
    static uint id = 0;
    String mapFile[5];
    mapFile[0] = StringCreate("res/map1.map");
    mapFile[1] = StringCreate("res/map2.map");
    mapFile[2] = StringCreate("res/map3.map");
    mapFile[3] = StringCreate("res/map4.map");
    mapFile[4] = StringCreate("res/map5.map");

    srand(time(0));
    uint i = rand() % 5;

    Map* map = (Map*) malloc (sizeof (Map));
    MapMakeEmpty(map);
    MapFillFromFile(map, fopen(mapFile[i], "r"));

    MapNode* mn = (MapNode*) malloc(sizeof(MapNode));
    mn->map = *map;
    mn->id = ++id;
    mn->left = mn->right = mn->top = mn->bottom = NULL;

    return mn;
}

Enemy* getRandomEnemy() {
    String enemyFile[3];
    enemyFile[0] = StringCreate("res/enemy1.enemy");
    enemyFile[1] = StringCreate("res/enemy2.enemy");
    enemyFile[2] = StringCreate("res/enemy3.enemy");

    return LoadPlayerFromFile(enemyFile[rand()%3]);
}

void showMapMenuInformation(GameState *gameState, String status) {
    TerminalClear(*(gameState->terminal));

    String tmp = StringCreate("You are on the map ");
    StringAppendString(&tmp, StringFromUint(gameState->currentMap->id));
    UIDrawText(*(gameState->terminal), TerminalGetCenterX(*gameState->terminal,StringLength(tmp)), 5, PixelStyleCreateDefault(), tmp);

    MapPutOnTerminal(gameState->currentMap->map, *(gameState->terminal),
        TerminalGetCenterX(*(gameState->terminal), 2*gameState->currentMap->map.width) , TerminalGetCenterY(*gameState->terminal,gameState->currentMap->map.width));
    UIDrawBoxLine(*gameState->terminal,
        TerminalGetCenterX(*gameState->terminal, 2*gameState->currentMap->map.width) - 2, TerminalGetCenterY(*gameState->terminal,gameState->currentMap->map.height)-1,
        2 * (gameState->currentMap->map.width) +4, gameState->currentMap->map.height+2,
        PixelStyleCreateDefault(), THICKLINE);

    if (status)
        UIDrawText(*(gameState->terminal), TerminalGetCenterX(*gameState->terminal,StringLength(status)), TerminalGetHeight(*gameState->terminal) - 5, PixelStyleCreateDefault(), status);

    uint left = TerminalGetCenterX(*gameState->terminal,MapWidth(gameState->currentMap->map)) - 55;
    uint top = TerminalGetCenterY(*gameState->terminal,MapHeight(gameState->currentMap->map));

    UIDrawVLine(*(gameState->terminal), left-2,top - 1, 13, PixelCreateDefault(0x250f),PixelCreateDefault(0x2517),PixelCreateDefault(0x2503));
    UIDrawVLine(*(gameState->terminal), left+32,top - 1, 13, PixelCreateDefault(0x2513),PixelCreateDefault(0x251b),PixelCreateDefault(0x2503));

    UIDrawHLine(*(gameState->terminal), left-2,top - 1, 35, PixelCreateDefault(0x250f),PixelCreateDefault(0x2513),PixelCreateDefault(0x2501));
    UIDrawText(*(gameState->terminal), left + 5, top + 1, PixelStyleCreateDefault(), StringCreate("Player Information"));
    UIDrawHLine(*(gameState->terminal), left-2,top + 3, 35, PixelCreateDefault(0x2523),PixelCreateDefault(0x252b),PixelCreateDefault(0x2501));

    UIDrawText(*(gameState->terminal), left, top + 4, PixelStyleCreateDefault(), StringCreate("Player Name"));
    UIDrawText(*(gameState->terminal), left+13, top + 4, PixelStyleCreateDefault(), StringCreate(gameState->player->name));
    UIDrawHLine(*(gameState->terminal), left-2,top + 5, 35, PixelCreateDefault(0x2523),PixelCreateDefault(0x252b),PixelCreateDefault(0x2501));

    UIDrawText(*(gameState->terminal), left, top + 6, PixelStyleCreateDefault(), StringCreate("Player HP"));
    UIDrawText(*(gameState->terminal), left+13, top + 6, PixelStyleCreateDefault(), StringFromUint(gameState->player->HP));
    UIDrawHLine(*(gameState->terminal), left-2,top + 7, 35, PixelCreateDefault(0x2523),PixelCreateDefault(0x252b),PixelCreateDefault(0x2501));

    UIDrawText(*(gameState->terminal), left, top + 8, PixelStyleCreateDefault(), StringCreate("Player STR"));
    UIDrawText(*(gameState->terminal), left+13, top + 8, PixelStyleCreateDefault(), StringFromUint(gameState->player->STR));
    UIDrawHLine(*(gameState->terminal), left-2,top + 9, 35, PixelCreateDefault(0x2523),PixelCreateDefault(0x252b),PixelCreateDefault(0x2501));

    UIDrawText(*(gameState->terminal), left, top + 10, PixelStyleCreateDefault(), StringCreate("Player DEF"));
    UIDrawText(*(gameState->terminal), left+13, top + 10, PixelStyleCreateDefault(), StringFromUint(gameState->player->DEF));
    UIDrawHLine(*(gameState->terminal), left-2,top + 11, 35, PixelCreateDefault(0x2517),PixelCreateDefault(0x251b),PixelCreateDefault(0x2501));

    TerminalRender(*(gameState->terminal));
}

int MapMenuShow(GameState *gameState) {
    if (!gameState->currentMap) {
        gameState->currentMap = getRandomMap();
        gameState->nMap = 1;
        gameState->playerPosition = gameState->currentMap->map.startCenter;
    }
    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, MAP_PLAYER);

    String command = StringCreate("");
    boolean game_loop = true;
    uint beforeMove = MAP_FREE;
    String status = NULL;
    while (game_loop) {
        showMapMenuInformation(gameState, status);

        command = getCommand();

        boolean move = false;
        uint mx = 0;
        uint my = 0;
        if (StringEquals(command, StringCreate("GU")))
            move = true, mx = 0, my = -1;
        else if (StringEquals(command, StringCreate("GD")))
            move = true, mx = 0, my = 1;
        else if (StringEquals(command, StringCreate("GR")))
            move = true, mx = 1, my = 0;
        else if (StringEquals(command, StringCreate("GL")))
            move = true, mx = -1, my = 0;
        else if (StringEquals(command, StringCreate("SAVE"))) {
            GameStateSave(gameState);
            status = StringCreate("Game saved!");
        } else if (StringEquals(command, StringCreate("EXIT")))
            return 0;

        uint afterMove = MAP_FREE;
        if (move) {
            uint nx = gameState->playerPosition.x + mx;
            uint ny = gameState->playerPosition.y + my;
            if (nx > 0 && nx <= MapWidth(gameState->currentMap->map)
                && ny > 0 && ny <= MapHeight(gameState->currentMap->map)
                && MapGet(gameState->currentMap->map, nx, ny) != MAP_WALL) {
                afterMove = MapGet(gameState->currentMap->map, nx, ny);
                MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, beforeMove);
                MapSet(gameState->currentMap->map, nx, ny, MAP_PLAYER);
                gameState->playerPosition.x = nx;
                gameState->playerPosition.y = ny;
            }

            if (nx == 0) {
                if (gameState->currentMap->left == NULL && gameState->nMap < 25)
                    gameState->currentMap->left = getRandomMap(), gameState->nMap++;
                if (gameState->currentMap->left != NULL) {
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, beforeMove);
                    MapNode* tmp = gameState->currentMap;
                    gameState->currentMap = gameState->currentMap->left;
                    gameState->currentMap->right = tmp;
                    gameState->playerPosition = gameState->currentMap->map.startRight;
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, MAP_PLAYER);
                }
            } else if (ny == 0) {
                if (gameState->currentMap->top == NULL && gameState->nMap < 25)
                    gameState->currentMap->top = getRandomMap(), gameState->nMap++;
                if (gameState->currentMap->top != NULL) {
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, beforeMove);
                    MapNode* tmp = gameState->currentMap;
                    gameState->currentMap = gameState->currentMap->top;
                    gameState->currentMap->bottom = tmp;
                    gameState->playerPosition = gameState->currentMap->map.startBottom;
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, MAP_PLAYER);
                }
            } else if (ny > gameState->currentMap->map.height) {
                if (gameState->currentMap->bottom == NULL && gameState->nMap < 25)
                    gameState->currentMap->bottom = getRandomMap(), gameState->nMap++;
                if (gameState->currentMap->bottom != NULL) {
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, beforeMove);
                    MapNode* tmp = gameState->currentMap;
                    gameState->currentMap = gameState->currentMap->bottom;
                    gameState->currentMap->top = tmp;
                    gameState->playerPosition = gameState->currentMap->map.startTop;
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, MAP_PLAYER);
                }
            } else if (nx > gameState->currentMap->map.width) {
                if (gameState->currentMap->right == NULL && gameState->nMap < 25)
                    gameState->currentMap->right = getRandomMap(), gameState->nMap++;
                if (gameState->currentMap->right != NULL) {
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, beforeMove);
                    MapNode* tmp = gameState->currentMap;
                    gameState->currentMap = gameState->currentMap->right;
                    gameState->currentMap->left = tmp;
                    gameState->playerPosition = gameState->currentMap->map.startLeft;
                    MapSet(gameState->currentMap->map, gameState->playerPosition.x, gameState->playerPosition.y, MAP_PLAYER);
                }
            }

            beforeMove = MAP_FREE;
            status = NULL;
            if (afterMove == MAP_ENEMY) {
                Enemy* enemy = getRandomEnemy();

                status = StringCreate("You are about to fight an enemy, his name is ");
                StringAppendString(&status, enemy->name);
                showMapMenuInformation(gameState, status);
                TerminalWaitKey();

                int result = BattleMenuShow(gameState, enemy);
                if (result == 0) beforeMove = MAP_ENEMY;

                if (result == 0)
                    status = StringCreate("It's a draw");
                else if (result == 1)
                    status = StringCreate("You win!");
                else
                    game_loop = false;
            } else if (afterMove == MAP_HEAL) {
                PlayerGetPotion(gameState->player);
                status = StringCreate("You got a medicine, your HP now is ");
                StringAppendString(&status, StringFromUint(gameState->player->HP));
            }
        }
    }

    if (gameState->player->HP <= 0)
        return -1;
    else
        return 1;
}
