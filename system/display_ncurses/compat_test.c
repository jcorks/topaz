#include <ncurses/ncurses.h>

int main(){
    initscr();
    printw("Test");
    refresh();
    getch();
    endwin();
    return 0;
}
