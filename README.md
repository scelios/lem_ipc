# LemIPC

## Overview
LemIPC is a multiplayer game simulation that uses shared memory to enable communication between multiple processes. The game supports up to 4 teams and includes a graphical display to visualize the gameplay. The project also includes unit tests to ensure the reliability of its core functionalities.

## Features
- **Shared Memory Communication**: Processes communicate and share game state using shared memory.
- **Team-Based Gameplay**: Supports between 1 and 4 teams.
- **Graphical Display**: A graphical interface to visualize the game in real-time.
- **Unit Tests**: Comprehensive unit tests to validate the functionality of the game.

## How to Run
1. Compile the project using the provided `Makefile`:
   make
2. Run the game with the desired number of teams (between 1 and 4):
    ./lemipc <team> & ./lemipc <teams> & ...
(you should used ./lemipc <teams> & wait 0.001 if you used a large number of threads simultanously)

## Unit Tests
To run the unit tests:
    ./tester.sh