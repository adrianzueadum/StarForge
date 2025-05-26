#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

// Constants
const int MAP_SIZE = 10;
const char PLAYER = 'P';
const char ORE = 'O';
const char ENERGY = 'E';
const char EMPTY = '.';
const char HAZARD = 'X';

// Game state
struct Player {
    int x, y;
    int ore, energy;
    int health;
    bool hasShield;
};

struct Game {
    vector<vector<char>> map;
    Player player;
    bool gameOver;
};

// Initialize game
void initGame(Game& game) {
    game.map = vector<vector<char>>(MAP_SIZE, vector<char>(MAP_SIZE, EMPTY));
    game.player = {0, 0, 0, 0, 100, false};
    game.gameOver = false;

    // Place resources randomly
    srand(time(0));
    for (int i = 0; i < 10; ++i) {
        int rx = rand() % MAP_SIZE;
        int ry = rand() % MAP_SIZE;
        if (game.map[rx][ry] == EMPTY && (rx != 0 || ry != 0)) {
            game.map[rx][ry] = (rand() % 2 == 0) ? ORE : ENERGY;
        }
    }
}

// Display map
void displayMap(const Game& game) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (i == game.player.x && j == game.player.y) {
                cout << PLAYER << " ";
            } else {
                cout << game.map[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << "Health: " << game.player.health << " | Ore: " << game.player.ore 
         << " | Energy: " << game.player.energy << " | Shield: " 
         << (game.player.hasShield ? "Yes" : "No") << endl;
}

// Move player
void movePlayer(Game& game, char direction) {
    int newX = game.player.x;
    int newY = game.player.y;

    if (direction == 'w' && newX > 0) newX--;
    else if (direction == 's' && newX < MAP_SIZE - 1) newX++;
    else if (direction == 'a' && newY > 0) newY--;
    else if (direction == 'd' && newY < MAP_SIZE - 1) newY++;

    // Check for resources or hazards
    if (game.map[newX][newY] == ORE) {
        game.player.ore++;
        game.map[newX][newY] = EMPTY;
    } else if (game.map[newX][newY] == ENERGY) {
        game.player.energy++;
        game.map[newX][newY] = EMPTY;
    } else if (game.map[newX][newY] == HAZARD) {
        if (!game.player.hasShield) {
            game.player.health -= 20;
            cout << "Hit by meteor! Health -20" << endl;
        } else {
            cout << "Shield protected you!" << endl;
            game.player.hasShield = false;
        }
        game.map[newX][newY] = EMPTY;
    }

    game.player.x = newX;
    game.player.y = newY;
}

// Craft items
void craftItem(Game& game) {
    cout << "Crafting menu:\n1. Shield (2 Ore, 1 Energy)\nChoose (1 or other to cancel): ";
    int choice;
    cin >> choice;
    if (choice == 1 && game.player.ore >= 2 && game.player.energy >= 1 && !game.player.hasShield) {
        game.player.ore -= 2;
        game.player.energy -= 1;
        game.player.hasShield = true;
        cout << "Crafted a Shield!" << endl;
    } else {
        cout << "Cannot craft: insufficient resources or already have a shield." << endl;
    }
}

// Random event: Meteor shower
void randomEvent(Game& game) {
    if (rand() % 100 < 20) { // 20% chance
        int hx = rand() % MAP_SIZE;
        int hy = rand() % MAP_SIZE;
        if (game.map[hx][hy] == EMPTY && (hx != game.player.x || hy != game.player.y)) {
            game.map[hx][hy] = HAZARD;
            cout << "Meteor shower! A hazard appeared at (" << hx << ", " << hy << ")." << endl;
        }
    }
}

// Main game loop
int main() {
    Game game;
    initGame(game);
    cout << "Welcome to StarForge! Use WASD to move, 'c' to craft, 'q' to quit.\n";

    while (!game.gameOver) {
        displayMap(game);
        cout << "Enter action: ";
        char action;
        cin >> action;

        if (action == 'q') {
            cout << "Game Over. Thanks for playing!" << endl;
            break;
        } else if (action == 'c') {
            craftItem(game);
        } else if (action == 'w' || action == 'a' || action == 's' || action == 'd') {
            movePlayer(game, action);
            randomEvent(game);
        }

        if (game.player.health <= 0) {
            cout << "You died! Game Over." << endl;
            game.gameOver = true;
        }
    }

    return 0;
}
