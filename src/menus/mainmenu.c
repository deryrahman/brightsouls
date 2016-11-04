#include "graphics/terminal.h"
#include "graphics/ui.h"
#include "menus/mainmenu.h"

void MainMenuShow(Terminal terminal) {
    TerminalClear(terminal);

    UIDrawBoxLine(terminal, 1, 1, TerminalGetWidth(terminal) - 2, TerminalGetHeight(terminal) - 2, PixelStyleCreateDefault(), MULTILINE);

    FILE *file = fopen("/home/jauhararifin/Projects/brightsouls/res/mainmenu.img","r");
    Image mainmenuImage;
    if(file) {
        mainmenuImage = ImageCreateFromFile(file, PixelStyleCreateDefault());
        UIDrawImage(terminal, TerminalGetCenterX(terminal, ImageWidth(mainmenuImage)), 5, mainmenuImage);
        fclose(file);
    }

    String str = StringStr("New Game");
    UIDrawText(terminal,TerminalGetCenterX(terminal, StringLength(str)), ImageHeight(mainmenuImage) + 10, PixelStyleCreateDefault(), str);

    str = StringStr("Start Game");
    UIDrawText(terminal,TerminalGetCenterX(terminal, StringLength(str)), ImageHeight(mainmenuImage) + 12, PixelStyleCreateDefault(), str);

    str = StringStr("Load");
    UIDrawText(terminal,TerminalGetCenterX(terminal, StringLength(str)), ImageHeight(mainmenuImage) + 14, PixelStyleCreateDefault(), str);

    str = StringStr("Exit");
    UIDrawText(terminal,TerminalGetCenterX(terminal, StringLength(str)), ImageHeight(mainmenuImage) + 16, PixelStyleCreateDefault(), str);

    TerminalRender(terminal);
}