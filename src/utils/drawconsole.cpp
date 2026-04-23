//#pragma once
//#include "../include/utils/drawconsole.h"
//#include "../include/game/player.h"
//
//using namespace std;
//
//
//HANDLE hConsole;
//HWND console;
//
//void InitConsole(int width, int height) {
//    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    console = GetConsoleWindow();
//
//
//    MoveWindow(console, 100, 100, width, height, TRUE);
//
//
//    SetWindowLong(console, GWL_STYLE,
//        GetWindowLong(console, GWL_STYLE) & ~WS_SIZEBOX);
//
//    CONSOLE_CURSOR_INFO cursorInfo;
//    GetConsoleCursorInfo(hConsole, &cursorInfo);
//    cursorInfo.bVisible = false;
//    SetConsoleCursorInfo(hConsole, &cursorInfo);
//
//    system("cls");
//}
//
//void clear_console() {
//    system("cls");
//}
//
//
//
//
//void print_grid(Grid& grid, int width, int height, int tile) {
//    string floor_space;
//    floor_space.assign(tile-3, ' ');
//    string floor_minus;
//    floor_minus.assign(tile-3, '-');
//    for (int i = 0; i < grid.h; ++i) {
//        for (int j = 0; j < grid.w; ++j) {
//            Cell& cell = grid.grid[i][j];
//
//            bool left_wall = cell.left;
//            if (j > 0) left_wall = left_wall || grid.grid[i][j - 1].right;
//
//            bool top_wall = cell.top;
//            if (i > 0) top_wall = top_wall || grid.grid[i - 1][j].bottom;
//
//            cout << "+";
//            cout << (top_wall ? floor_minus : floor_space);
//        }
//
//        Cell& last = grid.grid[i][grid.w - 1];
//        bool last_right_wall = last.right;
//        if (grid.w > 1) last_right_wall = last_right_wall || grid.grid[i][grid.w - 1].right;
//        cout << "+" << endl;
//
//        for (int j = 0; j < grid.w; ++j) {
//            Cell& cell = grid.grid[i][j];
//
//            bool left_wall = cell.left;
//            if (j > 0) left_wall = left_wall || grid.grid[i][j - 1].right;
//
//            cout << (left_wall ? "|" : " ");
//            cout << floor_space;
//        }
//
//        Cell& last_vert = grid.grid[i][grid.w - 1];
//        bool last_right_vert_wall = last_vert.right;
//        if (grid.w > 1) last_right_vert_wall = last_right_vert_wall || grid.grid[i][grid.w - 1].right;
//        cout << (last_right_vert_wall ? "|" : " ") << endl;
//    }
//
//    for (int j = 0; j < grid.w; ++j) {
//        Cell& cell = grid.grid[grid.h - 1][j];
//
//        bool bottom_wall = cell.bottom;
//        if (grid.h > 1) bottom_wall = bottom_wall || grid.grid[grid.h - 1][j].bottom;
//
//        bool left_wall = cell.left;
//        if (j > 0) left_wall = left_wall || grid.grid[grid.h - 1][j - 1].right;
//
//        cout << "+";
//        cout << (bottom_wall ? floor_minus : floor_space);
//    }
//
//    Cell& last_cell = grid.grid[grid.h - 1][grid.w - 1];
//    bool last_bottom_wall = last_cell.bottom;
//    if (grid.h > 1) last_bottom_wall = last_bottom_wall || grid.grid[grid.h - 1][grid.w - 1].bottom;
//    cout << "+" << endl;
//}
//
//void print_grid(Grid& grid, int width, int height, int tile, Player& player) {
//    string floor_space;
//    floor_space.assign(tile - 3, ' ');
//    string floor_minus;
//    floor_minus.assign(tile - 3, '-');
//    for (int i = 0; i < grid.h; ++i) {
//        for (int j = 0; j < grid.w; ++j) {
//            Cell& cell = grid.grid[i][j];
//
//            bool left_wall = cell.left;
//            if (j > 0) left_wall = left_wall || grid.grid[i][j - 1].right;
//
//            bool top_wall = cell.top;
//            if (i > 0) top_wall = top_wall || grid.grid[i - 1][j].bottom;
//
//            cout << "+";
//            cout << (top_wall ? floor_minus : floor_space);
//        }
//
//        Cell& last = grid.grid[i][grid.w - 1];
//        bool last_right_wall = last.right;
//        if (grid.w > 1) last_right_wall = last_right_wall || grid.grid[i][grid.w - 1].right;
//        cout << "+" << endl;
//
//        for (int j = 0; j < grid.w; ++j) {
//            Cell& cell = grid.grid[i][j];
//
//            bool left_wall = cell.left;
//            if (j > 0) left_wall = left_wall || grid.grid[i][j - 1].right;
//
//            cout << (left_wall ? "|" : " ");
//
//            if (i == player.get_y() && j == player.get_x()) {
//                int padding = (tile - 3) / 2;
//                cout << string(padding, ' ') << "O" << string(tile - 3 - padding - 1, ' ');
//            }
//            else {
//                cout << floor_space;
//            }
//        }
//
//        Cell& last_vert = grid.grid[i][grid.w - 1];
//        bool last_right_vert_wall = last_vert.right;
//        if (grid.w > 1) last_right_vert_wall = last_right_vert_wall || grid.grid[i][grid.w - 1].right;
//        cout << (last_right_vert_wall ? "|" : " ") << endl;
//    }
//
//    for (int j = 0; j < grid.w; ++j) {
//        Cell& cell = grid.grid[grid.h - 1][j];
//
//        bool bottom_wall = cell.bottom;
//        if (grid.h > 1) bottom_wall = bottom_wall || grid.grid[grid.h - 1][j].bottom;
//
//        bool left_wall = cell.left;
//        if (j > 0) left_wall = left_wall || grid.grid[grid.h - 1][j - 1].right;
//
//        cout << "+";
//        cout << (bottom_wall ? floor_minus : floor_space);
//    }
//
//    Cell& last_cell = grid.grid[grid.h - 1][grid.w - 1];
//    bool last_bottom_wall = last_cell.bottom;
//    if (grid.h > 1) last_bottom_wall = last_bottom_wall || grid.grid[grid.h - 1][grid.w - 1].bottom;
//    cout << "+" << endl;
//}