#!/bin/bash

# Check if the number of times to run ./lemipc 1 is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <number_of_times>"
  exit 1
fi

n=$1

# Run ./lemipc 1 n times in new terminals
for ((i=0; i<n/2; i++)); do
  xterm -hold -e "./lemipc 1; exec bash" &
done

for ((i=0; i<n/2; i++)); do
  xterm -hold -e "./lemipc 2; exec bash" &
done
# Run ./lemipc 0 once in a new terminal
xterm -hold -e "./lemipc 0; exec bash" &