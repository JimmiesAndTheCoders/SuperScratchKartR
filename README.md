# Super Scratch Kart R

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white) ![raylib](https://img.shields.io/badge/engine-raylib-white) ![WIP](https://img.shields.io/badge/status-work%20in%20progress-orange) ![MIT](https://img.shields.io/badge/license-MIT-green)

A free, open-source fan-made 3D kart racing game inspired by SuperTuxKart and the SSK/SSKDX series by [Lucasliu9595](https://scratch.mit.edu/users/Lucasliu9595/), built from the ground up in C++ using the [raylib](https://www.raylib.com) library!

> :warning: Fan Project Disclaimer: This is a non-commercial, fan-made project. All original concepts and characters are inspired by the "Super Scratch Kart" series and the Scratch Team. This project is not affiliated with or endorsed by the original creators.
>
> Note: This project is currently a work in progress (WIP). As it is in active development, you may encounter bugs, performance issues, or incomplete features. We appreciate your patience and feedback!

## :racing_car: Features

* Modern Engine: High-performance 3D rendering powered by raylib.

* Classic Gameplay: Drift-heavy kart physics and high-speed racing.

* Cross-Platform: Designed to run on Windows, Linux, and macOS.

* Open for Contribution: Built with clean C++ code for easy modding and updates.

## :video_game: Controls

| Action         | Key Binding       |
|----------------|-------------------|
| Accelerate     | W / Up Arrow      |
| Brake / Reverse| S / Down Arrow    |
| Steer Left     | A / Left Arrow    |
| Steer Right    | D / Right Arrow   |
| Pause          | Esc               |

## :hammer_and_wrench: Build Instructions

### Prerequisites

* A C++ 17 compatible compiler (GCC, Clang, MSVC, or MSYS2 for Windows).
* raylib 5.5+.
* CMake 4.2.3+ (Required for build configuration)
* Git
* Notepad++ or Dev-C++ (recommended for development)

### Building from source

Open a terminal and run these commands:
```bash
git clone https://github.com/JimmiesAndTheCoders/SuperScratchKartR.git
cd SuperScratchKartR
```

### How to compile

#### :window: Windows

Ensure you have **raylib** installed at `C:\raylib`.
1. Open the project folder.
2. Run `build.bat`

#### :penguin: Linux

Ensure you have `gcc` and `raylib` development headers installed (`libraylib-dev`).
1. Open your terminal in the project folder.
2. Make the script executable: `chmod +x build.sh`
3. Run it: `./build.sh`

#### :apple: MacOS

Ensure you have `brew install raylib` or the raylib framework installed.
1. Open your terminal in the project folder.
2. Make the script executable: `chmod +x build.sh`
3. Run it: `./build.sh`

## :world_map: Roadmap

I made this roadmap, just for development of things that are crucial for this game. When I complete all of these, I will add more features and content, but for now, this is the bare minimum for a playable game.

### Development

* [x] Implement basic kart physics and controls.
* [x] Add sound effects and music.
* [x] Add .glb files for track. (formerly .obj)
* [x] Add drifting, and a more robust physics system.
* [ ] Add character models and animations.
* [ ] Implement AI opponents.
* [ ] Optimize performance and fix bugs.

## :star2: Credits

* Programming: Jimmy Santoyo
* Original Concept and Characters: [Lucasliu9595](https://scratch.mit.edu/users/Lucasliu9595) and the Scratch Team
* Music: Kevin Macleod
* Engine: [raylib](https://www.raylib.com) by Ramon Santamaria ([raysan5](https://github.com/raysan5))

## :speech_balloon: Feedback and Contributions

We welcome feedback, bug reports, and contributions! If you'd like to contribute, please fork the repository and submit a pull request with your changes. For major changes, please open an issue first to discuss what you would like to change.

## :page_facing_up: Non-commercial Notice

This project is a fan-made creation for educational and entertainment purposes. It is distributed as-is without a formal commercial license. Use, study, and modification of the source code are encouraged for personal and community learning.
