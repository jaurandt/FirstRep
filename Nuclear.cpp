#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <limits>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

// ---------------------------------------------------------------------------
// ANSI color codes (cross-platform; work on Windows 10+ with virtual terminal)
// ---------------------------------------------------------------------------
namespace Color {
    const char* RESET  = "\033[0m";
    const char* RED    = "\033[31;1m";
    const char* YELLOW = "\033[33;1m";
    const char* CYAN   = "\033[36;1m";
}

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
const int BOARD_SIZE  = 9;
const int NUM_RES     = 30;
const int NUM_MIL     = 4;
const int INIT_REVEAL = 15;
const int INIT_NUKES  = 4;
const int INIT_SPIES  = 15;

const char SYM_MIL  = 'M';
const char SYM_RES  = 'R';
const char SYM_NONE = ' ';
const char SYM_NUKE = 'N';
const char SYM_OPEN = 'O';

// ---------------------------------------------------------------------------
// Data types
// ---------------------------------------------------------------------------
enum TileType { EMPTY, RESIDENTIAL, MILITARY };

struct Tile {
    TileType type       = EMPTY;
    bool occupied       = false;
    bool revealed       = false;
    bool contaminated   = false;
    bool nuked          = false;
};

struct Point { int x, y; };

struct GameState {
    Tile  board[BOARD_SIZE][BOARD_SIZE];
    char  display[BOARD_SIZE][BOARD_SIZE];
    Point resTiles[NUM_RES];
    Point milTiles[NUM_MIL];
    int   nukesLeft = INIT_NUKES;
    int   spiesLeft = INIT_SPIES;
};

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Read a validated integer in [0, max-1], reprompting on bad input.
int getCoord(const char* label, int max) {
    int val;
    while (true) {
        std::cout << label << " [0-" << (max - 1) << "]: ";
        if (std::cin >> val && val >= 0 && val < max)
            return val;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid — enter a number between 0 and " << (max - 1) << ".\n";
    }
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void flushInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ---------------------------------------------------------------------------
// Board initialisation
// ---------------------------------------------------------------------------
void initBoard(GameState& gs) {
    // Reset every tile and the display grid in one pass.
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++) {
            gs.board[r][c]   = Tile{};
            gs.display[r][c] = SYM_NONE;
        }

    // Place residential tiles at random unoccupied positions.
    for (int placed = 0; placed < NUM_RES; ) {
        int x = rand() % BOARD_SIZE, y = rand() % BOARD_SIZE;
        if (!gs.board[y][x].occupied) {
            gs.board[y][x] = { RESIDENTIAL, true };
            gs.resTiles[placed++] = { x, y };
        }
    }

    // Place military tiles at random unoccupied positions.
    for (int placed = 0; placed < NUM_MIL; ) {
        int x = rand() % BOARD_SIZE, y = rand() % BOARD_SIZE;
        if (!gs.board[y][x].occupied) {
            gs.board[y][x] = { MILITARY, true };
            gs.milTiles[placed++] = { x, y };
        }
    }

    // Pre-reveal the first INIT_REVEAL residential tiles.
    for (int i = 0; i < INIT_REVEAL; i++) {
        int x = gs.resTiles[i].x, y = gs.resTiles[i].y;
        gs.board[y][x].revealed = true;
        gs.display[y][x]        = SYM_RES;
    }

    gs.nukesLeft = INIT_NUKES;
    gs.spiesLeft = INIT_SPIES;
}

// ---------------------------------------------------------------------------
// Display
// ---------------------------------------------------------------------------
void drawBoard(const GameState& gs, bool endgame) {
    // Column header row
    std::cout << "[ ]";
    for (int c = 0; c < BOARD_SIZE; c++)
        std::cout << "[" << c << "]";
    std::cout << "\n";

    for (int r = 0; r < BOARD_SIZE; r++) {
        std::cout << "[" << r << "]";
        for (int c = 0; c < BOARD_SIZE; c++) {
            char sym        = gs.display[r][c];
            bool contaminated = gs.board[r][c].contaminated;

            const char* color = Color::RESET;
            if (!endgame) {
                if      (sym == SYM_NUKE) color = Color::RED;
                else if (sym == SYM_MIL)  color = Color::YELLOW;
                else if (sym == SYM_RES)  color = Color::CYAN;
            } else {
                if (contaminated && (sym == SYM_MIL || sym == SYM_RES))
                    color = Color::RED;
            }

            std::cout << color << "[" << sym << "]" << Color::RESET;
        }
        std::cout << "\n";
    }
}

void revealFullBoard(GameState& gs) {
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            switch (gs.board[r][c].type) {
                case RESIDENTIAL: gs.display[r][c] = SYM_RES;  break;
                case MILITARY:    gs.display[r][c] = SYM_MIL;  break;
                case EMPTY:       gs.display[r][c] = SYM_NONE; break;
            }
}

// ---------------------------------------------------------------------------
// Actions
// ---------------------------------------------------------------------------

// Contaminate a 3x3 area centred on (cx, cy), clamped to the board edges.
// This fixes the original bug where edge tiles could never be targeted.
void applyNuke(GameState& gs, int cx, int cy) {
    int r0 = std::max(0, cy - 1), r1 = std::min(BOARD_SIZE - 1, cy + 1);
    int c0 = std::max(0, cx - 1), c1 = std::min(BOARD_SIZE - 1, cx + 1);
    for (int r = r0; r <= r1; r++)
        for (int c = c0; c <= c1; c++)
            gs.board[r][c].contaminated = true;

    gs.board[cy][cx].nuked = true;
    gs.display[cy][cx]     = SYM_NUKE;
    gs.nukesLeft--;
}

void handleSpyRun(GameState& gs) {
    if (gs.spiesLeft == 0) {
        std::cout << "Out of spy runs. Press Enter.\n";
        flushInput(); std::cin.get();
        return;
    }

    int x, y;
    while (true) {
        std::cout << "Select coordinates for spy run.\n";
        x = getCoord("  X", BOARD_SIZE);
        y = getCoord("  Y", BOARD_SIZE);
        if (!gs.board[y][x].revealed) break;
        std::cout << "That tile is already revealed... Pick another.\n";
    }

    gs.board[y][x].revealed = true;
    switch (gs.board[y][x].type) {
        case RESIDENTIAL: gs.display[y][x] = SYM_RES;  break;
        case MILITARY:    gs.display[y][x] = SYM_MIL;  break;
        case EMPTY:       gs.display[y][x] = SYM_OPEN; break;
    }
    gs.spiesLeft--;
}

void handleNuke(GameState& gs) {
    int x, y;
    while (true) {
        std::cout << "Select coordinates to nuke.\n";
        x = getCoord("  X", BOARD_SIZE);
        y = getCoord("  Y", BOARD_SIZE);
        if (!gs.board[y][x].nuked) break;
        std::cout << "That tile was already nuked... Pick another.\n";
    }
    applyNuke(gs, x, y);
}

// ---------------------------------------------------------------------------
// Scoring
// ---------------------------------------------------------------------------
int countContaminatedMil(const GameState& gs) {
    int count = 0;
    for (int i = 0; i < NUM_MIL; i++)
        if (gs.board[gs.milTiles[i].y][gs.milTiles[i].x].contaminated)
            count++;
    return count;
}

int calcReputation(const GameState& gs) {
    int rep = 30;
    for (int i = 0; i < NUM_RES; i++)
        if (gs.board[gs.resTiles[i].y][gs.resTiles[i].x].contaminated)
            rep -= 2;
    for (int i = 0; i < NUM_MIL; i++)
        if (gs.board[gs.milTiles[i].y][gs.milTiles[i].x].contaminated)
            rep += 8;
    return rep;
}

// ---------------------------------------------------------------------------
// Instructions
// ---------------------------------------------------------------------------
void showInstructions() {
    std::cout <<
        "OBJECTIVE\n"
        "  Destroy all " << NUM_MIL << " military sites using nuclear weapons.\n"
        "  Avoid civilian casualties to keep your reputation high.\n\n"
        "RESOURCES\n"
        "  " << INIT_NUKES << " nukes -> each wipes a 3x3 area around the target\n"
        "  " << INIT_SPIES << " spy runs -> reveal any one hidden tile\n\n"
        "SCORING  (+8 per military site destroyed, -2 per residential area hit)\n\n"
        "SYMBOLS\n"
        "  R -> residential area\n"
        "  M -> military site\n"
        "  N -> nuked tile\n"
        "  O -> empty tile (revealed by spy)\n\n"
        "COMMANDS\n"
        "  p / P -> use a spy run\n"
        "  n / N -> launch a nuke\n\n"
        "The board is a " << BOARD_SIZE << "x" << BOARD_SIZE << " grid.\n"
        "Coordinates start at 0 in the top-left corner.\n\n"
        "Press Enter to start.\n";
    std::cin.get();
    std::cin.get();
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // Enable ANSI escape codes on Windows 10+
#ifdef _WIN32
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hOut, &mode);
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleTitle("Nuclear Wars v3.0");
    }
#endif

    bool playAgain = true;
    while (playAgain) {
        GameState gs;
        initBoard(gs);

        std::cout << "=== Nuclear Wars ===\n\n";
        std::cout << "Instructions? (y/n): ";
        char ch;
        std::cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            clearScreen();
            showInstructions();
        }

        bool running = true;
        while (running) {
            clearScreen();
            drawBoard(gs, false);
            std::cout << "\nSpy runs: " << gs.spiesLeft
                      << "  |  Nukes: " << gs.nukesLeft << "\n";
            std::cout << "Command (p=spy / n=nuke): ";

            char move;
            std::cin >> move;
            switch (move) {
                case 'p': case 'P': handleSpyRun(gs); break;
                case 'n': case 'N': handleNuke(gs);   break;
                default:
                    std::cout << "Unknown command: use 'p' or 'n'.\n";
                    flushInput(); std::cin.get();
            }

            if (countContaminatedMil(gs) == NUM_MIL || gs.nukesLeft == 0)
                running = false;
        }

        // End-of-round summary
        clearScreen();
        drawBoard(gs, false);
        std::cout << "\nGame over!  Reputation: " << calcReputation(gs) << "\n";
        std::cout << "Press Enter to reveal the board.\n";
        flushInput(); std::cin.get();

        clearScreen();
        revealFullBoard(gs);
        drawBoard(gs, true);

        std::cout << "\nPlay again? (y/n): ";
        std::cin >> ch;
        playAgain = (ch == 'y' || ch == 'Y');
        clearScreen();
    }

    std::cout << "Thanks for playing.\n";
    return 0;
}
