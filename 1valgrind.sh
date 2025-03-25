#!/bin/bash
valgrind --tool=helgrind ./lemipc 1 & valgrind --tool=helgrind ./lemipc 2 & valgrind --tool=helgrind ./lemipc 1
