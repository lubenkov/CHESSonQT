# chess-on-qt

A local chess game for two players, built with C++ and Qt 5.12.12.

![C++](https://img.shields.io/badge/language-C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-5.12.12-41CD52?style=flat-square&logo=qt&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6?style=flat-square&logo=windows&logoColor=white)

## About

Two players share the same computer and move pieces with the mouse. The game has a five-minute clock for each side, castling, en-passant capture, promotion to a queen, sound effects, and looping background music.

This is a small Qt Widgets project. The board and game logic are implemented in `main.cpp`.

> **Note:** The current code does not check whether a move leaves the king in check and does not detect checkmate or stalemate. Move validation also needs review before claiming complete chess-rule support.

## How the code is organised

| File / directory | Responsibility |
| :-- | :-- |
| `main.cpp` | Application entry point, board, move logic, timers, and audio |
| `resources/` | Piece images, window icon, font, sound effects, and music |
| `CHESSonQT.sln` / `CHESSonQT.vcxproj` | Visual Studio solution and project settings |

## Controls

| Key / input | Action |
| :-- | :-- |
| Left mouse button | Select a piece, then click a destination square to attempt a move |

## Build and run

1. Install Visual Studio 2022 with the **Desktop development with C++** workload and the **v143** build tools.
2. Set up **Qt 5.12.12 for MSVC 2017, x64** using Qt VS Tools. The project uses the Core, Gui, Multimedia, MultimediaWidgets, and Widgets modules.
3. Open `CHESSonQT.sln`, select **Debug | x64** or **Release | x64**, then build and run with `F5`.

> The project settings target Windows SDK 10.0.22621.0. Images, audio, and the font are loaded from relative paths under `resources/`; keep that directory intact and run the game with the repository root as its working directory.

## Dependency

- Qt 5.12.12 for MSVC 2017, x64 — Core, Gui, Multimedia, MultimediaWidgets, and Widgets.

<details>
<summary><b>🇷🇺 По-русски</b></summary>

<br>

Шахматы для двух игроков за одним компьютером, написанные на C++ и Qt 5.12.12. Управление мышью, таймер на пять минут для каждой стороны, звуковые эффекты и фоновая музыка.

Для сборки проект настроен на Visual Studio 2022 (`v143`) и Qt 5.12.12 для MSVC 2017, x64. Папка `resources/` должна быть доступна из рабочей папки программы. Проверка шаха, мата, пата и некоторых ходов пока не реализована.

</details>
