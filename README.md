<div align="center">

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:00b09b,100:96c93d&height=200&section=header&text=Snake%20GLUT%20Ultimate%20Edition&fontSize=32&fontColor=ffffff&animation=fadeIn&fontAlignY=35&desc=A%20Single%20File%20Rainbow%20Snake%20in%20C%2B%2B%20and%20OpenGL&descAlignY=55&descSize=16" width="100%"/>

<br/>

![C++](https://img.shields.io/badge/C%2B%2B17-Language-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-Graphics-5586A4?style=for-the-badge&logo=opengl&logoColor=white)
![GLUT](https://img.shields.io/badge/FreeGLUT-Windowing-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-purple?style=for-the-badge)

<img src="https://img.shields.io/github/stars/AbdulAzeemHashmi/Snake-Game?style=social" alt="stars"/>
<img src="https://img.shields.io/github/forks/AbdulAzeemHashmi/Snake-Game?style=social" alt="forks"/>
<img src="https://img.shields.io/github/last-commit/AbdulAzeemHashmi/Snake-Game?color=brightgreen" alt="last commit"/>

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=22&pause=1000&color=00B09B&center=true&vCenter=true&width=650&lines=Rainbow+Gradient+Snake+in+OpenGL;Binary+High+Score+Leaderboard;Speed+Ups+Every+5+Fruits;Zero+Globals%2C+One+Source+File" alt="Typing SVG" />

</div>

<br/>

> 🐍 *A single file, dependency light rainbow snake with binary high scores, speed ups, and full on screen UX.*

<br/>

## 📌 About

This is an **old school Snake** game written in **pure C style C++17** using **OpenGL/GLUT**. 🎮

Everything like 📝 **name entry**, 🏆 **live score**, ⚡ **speed ups**, and 🥇 **top 5 leaderboard** happens **inside one window**.

The entire project is 📄 **one .cpp file**, zero external libraries beyond **FreeGLUT**, and zero globals or statics.

<div align="center">
<img src="https://media.giphy.com/media/xUOxf48a2xUvpk2xM8/giphy.gif" width="380" alt="snake game animation"/>
</div>

---

## 📸 Screenshots

<div align="center">

### 📝 Name Entry Screen

![Name Entry](https://github.com/AbdulAzeemHashmi/Snake-Game/blob/main/Screenshot%20from%202026-06-11%2020-38-50.png)

### ⏸️ Gameplay (Paused)

![Gameplay Paused](https://github.com/AbdulAzeemHashmi/Snake-Game/blob/main/Screenshot%20from%202026-06-11%2020-39-52.png)

### 💀 Game Over Screen

![Game Over](https://github.com/AbdulAzeemHashmi/Snake-Game/blob/main/Screenshot%20from%202026-06-11%2020-40-04.png)

</div>

---

## ✨ Features

<div align="center">

| Feature | Included |
|---------|----------|
| 📝 **Name input in game** | ✅ Yes |
| 💾 **Binary high score file** (`scores.bin`) | ✅ Yes |
| ⚡ **Speed increases** every 5 fruits | ✅ Yes |
| 🌈 **Rainbow gradient snake** with head/tail highlight | ✅ Yes |
| 🔲 **Toggle able grid** with on screen instructions | ✅ Yes |
| 📐 **Resizable window** (keeps 600x600 logic) | ✅ Yes |
| 🛡️ **Robust binary file** (corruption safe) | ✅ Yes |
| 📄 **Single source file** | ✅ Yes |
| 🚫 **No globals, no OOP, only allowed headers** | ✅ Yes |

</div>

<br/>

## 🚀 Quick Start

### 1️⃣ Install FreeGLUT

<div align="center">

| OS | Command |
|--|--|
| 🐧 **Linux** | `sudo apt install freeglut3-dev build-essential` |
| 🍎 **macOS** | `brew install freeglut` |
| 🪟 **Windows** | MinGW-w64 with freeglut (copy `freeglut.dll` next to exe) |

</div>

### 2️⃣ Build

```bash
g++ game.cpp -lGL -lGLU -lglut -std=c++17 -Wall -Wextra -o snake
```

### 3️⃣ Run

```bash
./snake
```

<br/>

## 🎮 Controls

<div align="center">

| Key | Action |
|--|--|
| ⬆️⬇️⬅️➡️ **Arrow keys** | Move snake |
| ⏸️ **p / P** | Pause and resume |
| 🔄 **r / R** | Restart after game over |
| ❌ **q / Q** | Quit |
| ⌫ **Backspace** | Delete character during name entry |
| ↩️ **Enter** | Confirm name |

</div>

<br/>

## 📂 File Tree

```
snake-glut-ultimate/
├── 🐍 snake.cpp       <- single source file
├── 💾 scores.bin      <- binary high score table (auto created)
├── 📘 README.md       <- this file
└── 📜 LICENSE         <- MIT
```

<br/>

## 💾 High Score Format (`scores.bin`)

<div align="center">

| Field | Size | Description |
|--|--|--|
| `playerName` | 32 B | last player |
| `score` | 4 B | last score |
| `topCount` | 4 B | number of entries, 5 or fewer |
| `Entry[topCount]` | 36 B each | name and score pairs |

</div>

<br/>

## ⚙️ Configuration Constants

Edit the **top of `snake.cpp`** to tweak:

<div align="center">

| Constant | Meaning |
|--|--|
| `BASE_DELAY_MS` | 🐢 starting speed (ms per frame) |
| `MIN_DELAY_MS` | ⚡ fastest speed |
| `SPEED_INTERVAL` | 🍎 fruits per speed increase |
| `ROWS / COLS` | 🗺️ logical board size |
| `CELL` | 🔲 pixel size per grid cell |

</div>

<br/>

## 🧩 Extending the Game

<div align="center">

| Idea | How |
|--|--|
| 🔊 Sound effects | `system("aplay beep.wav &")` |
| 🌀 Wrap around walls | Change bounds check in `moveSnake()` |
| ⭐ Power ups | Add new `enum` in `GameState`, spawn logic in `placeFood()` |
| 🎨 Colour themes | Toggle grid colours on a key press |

</div>

<br/>

## ⚠️ Known Limitations

- 🧱 Board completely full results in immediate game over.
- 🛡️ Binary file corruption triggers auto reset (safe).
- 🔇 No sound (intentional for single file simplicity).

<br/>

<div align="center">

### ⭐ If you found this project helpful, consider giving it a star

<a href="https://github.com/AbdulAzeemHashmi/Snake-Game/stargazers">
<img src="https://img.shields.io/badge/Star%20this%20repo-⭐-yellow?style=for-the-badge" alt="star this repo"/>
</a>

<br/><br/>

Made with 💚 by Abdul Azeem, one file, zero dependencies, full nostalgia.

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:96c93d,100:00b09b&height=100&section=footer" width="100%"/>

</div>
