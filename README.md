# TicTac Game

This project is a simple TicTac game built using SDL2. Below are the instructions on how to compile, run, and clean the project.

## Requirements


Make sure you have the following dependencies installed:
- [SDL2](https://github.com/libsdl-org/SDL)
- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
- MinGW or any compatible C++ compiler
- `make` utility


## Installation

1. Clone the repository:

   ```bash
   mkdir tictac
   cd tictac
   git clone https://github.com/Kshitijbudholiya/TicTacToe-Using-SDL2.git
   ```

2. Make sure the required libraries (`SDL2` and `SDL2_ttf`) are properly installed and set up.

## Compilation

To compile the project, run:

```bash
make
```

This command will compile the `main.c` file and generate an executable named `main.exe`.

## Running the Game

After compiling, you can run the game with the following command:

```bash
make run
```

This command will execute the generated `main.exe` file and start the game.

## Cleaning the Project

To clean up the generated `main.exe` file, you can run:

```bash
make clean
```

This command will delete the `main.exe` file from your project directory.
