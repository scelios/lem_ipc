#include "lemipc.h"

int main(int argc, char *argv[])
{
	bool isFirst = false;
	int shmid;
	sharedMemory *shmaddr;
	unsigned short int myOrder = 0;

	if (checkArgs(argc, argv) == -1 \
		|| getSharedRessources(&isFirst, &shmid, &shmaddr) == -1)
		return EXIT_FAILURE;
	if (isFirst == true && initSharedRessources(shmid,shmaddr) == -1)
		return EXIT_FAILURE;
	if (isFirst == false)
	{
		myOrder = getMyRessources(shmaddr);
	}
	waitForPlayers(shmaddr);
	if (isFirst == true)
	{
		initGame(shmaddr);
		// launchGraphics();
	}
	launchGame(shmaddr, myOrder);
	if (isLast(shmaddr) == true)
	{
		cleanSharedRessources(shmid, shmaddr);
	}



	// detachSharedRessources(shmaddr); // ? should detach at the end

	return 0;
}