#!/bin/bash
valgrind --tool=helgrind bash -c "./lemipc 1 & ./lemipc 2 & ./lemipc 1 & wait"