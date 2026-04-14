# Nuclear Wars

A console-based strategy game originally written around 2004 (age ~14) and later modernized to clean C++17.

Hunt down 4 hidden military sites on a 9×9 grid using nuclear weapons, but watch out for civilian casualties.

## Gameplay

- 15 residential tiles are revealed at the start as intel
- Use **spy runs** to peek at any hidden tile
- Use **nukes** to strike a target; each blast contaminates a 3×3 area
- Destroy all 4 military sites before you run out of nukes to win

**Scoring:** +8 reputation per military site destroyed, −2 per residential area hit

## Building

**Requirements:** A C++17 compiler (g++ via [MSYS2](https://www.msys2.org/) or equivalent)

**With CMake:**
```
cmake -B build -G "MinGW Makefiles"
cmake --build build
./build/NuclearWars.exe
```

**Direct compile:**
```
g++ -std=c++17 -o NuclearWars.exe Nuclear.cpp
./NuclearWars.exe
```

**From VS Code:** Open `Nuclear.cpp` and press `F5`.

## Controls

| Key | Action |
|-----|--------|
| `p` | Spy run reveals a hidden tile |
| `n` | Launches a nuke at chosen tile |
