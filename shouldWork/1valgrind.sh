#!/bin/bash
valgrind --tool=helgrind bash -c "./lemipc 1 & sleep 0.001
./lemipc 2 & sleep 0.001
./lemipc 1 & sleep 0.001
./lemipc 2 & wait"