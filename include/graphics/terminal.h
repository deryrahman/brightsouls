#ifndef _H_TERMINAL
#define _H_TERMINAL

#include "graphics/pixel.h"
#include "integer.h"
#include "boolean.h"
#include "xstring.h"

typedef Pixel* TerminalBuffer;

typedef struct {
    uint width;
    uint height;
    TerminalBuffer backBuffer;
    TerminalBuffer buffer;
} Terminal;

int TerminalWaitKey();
void TerminalClearScreen();
void TerminalGotoXY(uint x, uint y);
String TerminalReadTextbox(Terminal terminal, uint x, uint y, uint maxlength);

uint TerminalGetActualWidth();
uint TerminalGetActualHeight();

uint TerminalGetWidth(Terminal terminal);
uint TerminalGetHeight(Terminal terminal);
TerminalBuffer TerminalGetBuffer(Terminal terminal);

uint TerminalGetCenterX(Terminal terminal, uint width);
uint TerminalGetCenterY(Terminal terminal, uint height);

void TerminalMake(Terminal* terminal, uint width, uint height);
void TerminalMakeDefault(Terminal* terminal);

Terminal TerminalCreate(uint width, uint height);
Terminal TerminalCreateDefault();

Pixel TerminalGet(Terminal terminal, uint x, uint y);
void TerminalSet(Terminal terminal, uint x, uint y, Pixel pixel);

void TerminalClear(Terminal terminal);
void TerminalRender(Terminal terminal);

void TerminalDealoc(Terminal* terminal);

#endif
