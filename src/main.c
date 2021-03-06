#include <stdio.h>
#include <stdlib.h>

#include "graphics/terminal.h"
#include "graphics/pixel.h"
#include "graphics/ui.h"
#include "xstring.h"
#include "gamestate.h"
#include "boolean.h"

#include "menus/splashscreen.h"
#include "menus/mainmenu.h"
#include "menus/mapmenu.h"
#include "menus/newgame.h"
#include "menus/endgame.h"

int main(int argc, char** argv){
	Terminal terminal = TerminalCreateDefault();
	TerminalClearScreen();
	GameState *gameState = GameStateNew(&terminal, NULL, NULL);

	/*char c = TerminalWaitKey();
	while (1) {
		wprintf(L">> %d %c", (int) c, c);
		c = TerminalWaitKey();
	}*/

	SplashScreenShow(gameState);

	while (1) {
		boolean stay = true;
		do {
			int menu = MainMenuShow(gameState);
			switch (menu) {
				case 1: NewGameShow(gameState); stay = true; break;
				case 2: if (gameState->player != NULL && gameState->player->name != NULL) stay = false; break;
				case 3:
					if (!GameStateLoad(gameState)) {
						TerminalClear(*gameState->terminal);
						String failedLoad = StringCreate("Loading savefile failed : maybe you do not have save file yet");
						UIDrawText(*gameState->terminal, TerminalGetCenterX(*gameState->terminal, StringLength(failedLoad)), TerminalGetCenterY(*gameState->terminal,1), PixelStyleCreateDefault(), failedLoad);
						TerminalRender(*gameState->terminal);
						TerminalWaitKey();
					}
					break;
				case 4: return 0;
				default: stay = true;
			}
		} while (stay);

		int res = MapMenuShow(gameState);
		if (res != 0) {
			EndGameShow(gameState, res == 1);
			gameState = GameStateNew(&terminal, NULL, NULL);
		}
	}

	//UIDrawBoxLine(terminal, 11, 11, 30, 30, PixelStyleCreateDefault(), THINLINE);
	//UIDrawBoxLine(terminal, 41, 11, 30, 30, PixelStyleCreateDefault(), THICKLINE);
	//UIDrawBoxLine(terminal, 71, 11, 30, 30, PixelStyleCreateDefault(), MULTILINE);

	//UIDrawBoxChar(terminal, 101, 11, 30, 30, PixelCreate(RESET,RED,WHITE,L'#'));

	//UIDrawText(terminal, TerminalGetCenterX(terminal, StringLength(StringCreate("jauhar"))), 10, PixelStyleCreateDefault(), StringCreate("jauhar"));

	GameStateDealoc(gameState);

	return 0;
}
