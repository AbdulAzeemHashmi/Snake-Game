# Snake - GLUT Ultimate Edition   
*A single-file, dependency-light rainbow snake with binary high scores, speed-ups, and full on-screen UX.*    

---

## About

This is an **old school Snake** game written in **pure C style C++17** using **OpenGL/GLUT**.  
Everything like **name entry**, **live score**, **speed ups**, and **top 5 leaderboard** happens **inside one window**.  
The entire project is **one .cpp file**, zero external libraries beyond **FreeGLUT**, and zero globals/statics.

---

## Screenshots

### Name Entry Screen
![Name Entry](https://github.com/AbdulAzeemHashmi/Snake-Game/blob/main/Screenshot%20from%202026-06-11%2020-38-50.png)

### Gameplay (Paused)
![Gameplay Paused](https://github.com/AbdulAzeemHashmi/Snake-Game/blob/main/Screenshot%20from%202026-06-11%2020-39-52.png)

### Game Over Screen
![Game Over](https://github.com/AbdulAzeemHashmi/Snake-Game/blob/main/Screenshot%20from%202026-06-11%2020-40-04.png)

---

## Features

| Feature | Included |
|---------|----------|
| **Name input in-game** | Yes |
| **Binary high-score file** (`scores.bin`) | Yes |
| **Speed increases** every 5 fruits | Yes |
| **Rainbow gradient snake** + head/tail highlight | Yes |
| **Toggle-able grid** + instructions on screen | Yes |
| **Resizable window** (keeps 600x600 logic) | Yes |
| **Robust binary file** (corruption safe) | Yes |
| **Single source file** | Yes |
| **No globals / no OOP / only allowed headers** | Yes |

---

## Quick Start

### 1. Install FreeGLUT

| OS | Command |
|--|--|
| **Linux** | `sudo apt install freeglut3-dev build-essential` |
| **macOS** | `brew install freeglut` |
| **Windows** | MinGW-w64 + freeglut (copy `freeglut.dll` next to exe) |

### 2. Build
```bash
g++ game.cpp -lGL -lGLU -lglut -std=c++17 -Wall -Wextra -o snake
```

### 3. Run
```bash
./snake
```

---

## Controls

| Key | Action |
|--|--|
| **Arrow keys** | Move snake |
| **p / P** | Pause / resume |
| **r / R** | Restart after game-over |
| **q / Q** | Quit |
| **Backspace** | Delete character during name entry |
| **Enter** | Confirm name |

---

## File Tree

```
snake-glut-ultimate/
├── snake.cpp       <- single source file
├── scores.bin      <- binary high-score table (auto-created)
├── README.md       <- this file
└── LICENSE         <- MIT
```

---

## High-Score Format (`scores.bin`)

| Field        | Size | Description |
|--|--|--|
| `playerName` | 32 B | last player |
| `score`      | 4 B  | last score |
| `topCount`   | 4 B  | #entries <= 5 |
| `Entry[topCount]` | 36 B each | name + score pairs |

---

## Configuration Constants

Edit **top of `snake.cpp`** to tweak:

| Constant | Meaning |
|--|--|
| `BASE_DELAY_MS` | starting speed (ms/frame) |
| `MIN_DELAY_MS`  | fastest speed |
| `SPEED_INTERVAL`| fruits per speed increase |
| `ROWS / COLS`   | logical board size |
| `CELL`          | pixel size per grid cell |

---

## Extending the Game

| Idea | How |
|--|--|
| Sound effects | `system("aplay beep.wav &")` |
| Wrap-around walls | Change bounds check in `moveSnake()` |
| Power-ups | Add new `enum` in `GameState`, spawn logic in `placeFood()` |
| Colour themes | Toggle grid colours on a key press |

---

## Known Limitations

- Board completely full results in immediate game-over.
- Binary file corruption triggers auto-reset (safe).
- No sound (intentional for single-file simplicity).
