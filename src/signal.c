#include "lemipc.h"

bool sigintReceived = false;

void handleSigint(int sig)
{
    (void)sig;
    sigintReceived = true;
    exit(EXIT_FAILURE); // Exit the program gracefully
}