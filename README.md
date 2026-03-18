# 🐍 Snake – GLUT Ultimate Edition  
*A single-file, dependency-light rainbow snake with binary high scores, speed-ups, and full on-screen UX.*

---

## 🎮 About

This is an **old school Snake** game written in **pure C style C++17** using **OpenGL/GLUT**.  
Everything—**name entry**, **live score**, **speed ups**, **top 5 leaderboard**—happens **inside one window**.  
The entire project is **one `.cpp` file**, zero external libraries beyond **FreeGLUT**, and zero globals/statics.

---

## ✨  Features

| Feature | Included |
|---------|----------|
| 🖥️ **Name input in-game** | ✅ |
| 📊 **Binary high-score file** (`scores.bin`) | ✅ |
| ⚡ **Speed increases** every 5 fruits | ✅ |
| 🌈 **Rainbow gradient snake** + head/tail highlight | ✅ |
| 🔲 **Toggle-able grid** + instructions on screen | ✅ |
| 🔄 **Resizable window** (keeps 600×600 logic) | ✅ |
| 🛡️ **Robust binary file** (corruption safe) | ✅ |
| 📄 **Single source file** | ✅ |
| 🧼 **No globals / no OOP / only allowed headers** | ✅ |

---

## 🚀  Quick Start

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

## 🕹️  Controls

| Key | Action |
|--|--|
| **Arrow keys** | Move snake |
| **p / P** | Pause / resume |
| **r / R** | Restart after game-over |
| **q / Q** | Quit |
| **Backspace** | Delete character during name entry |
| **Enter** | Confirm name |

---

## 📁  File Tree

```
snake-glut-ultimate/
├── snake.cpp       ← single source file
├── scores.bin      ← binary high-score table (auto-created)
├── README.md       ← this file
└── LICENSE         ← MIT
```

---

## 📊  High-Score Format (`scores.bin`)

| Field        | Size | Description |
|--|--|--|
| `playerName` | 32 B | last player |
| `score`      | 4 B  | last score |
| `topCount`   | 4 B  | #entries ≤ 5 |
| `Entry[topCount]` | 36 B each | name + score pairs |

---

## 🛠️  Configuration Constants

Edit **top of `snake.cpp`** to tweak:

| Constant | Meaning |
|--|--|
| `BASE_DELAY_MS` | starting speed (ms/frame) |
| `MIN_DELAY_MS`  | fastest speed |
| `SPEED_INTERVAL`| fruits per speed increase |
| `ROWS / COLS`   | logical board size |
| `CELL`          | pixel size per grid cell |

---

## 🧪  Extending the Game

| Idea | How |
|--|--|
| Sound effects | `system("aplay beep.wav &")` |
| Wrap-around walls | Change bounds check in `moveSnake()` |
| Power-ups | Add new `enum` in `GameState`, spawn logic in `placeFood()` |
| Colour themes | Toggle grid colours on a key press |

---

## 🐛  Known Limitations

- Board completely full → immediate game-over.  
- Binary file corruption → auto-reset (safe).  
- No sound (intentional for single-file simplicity).

