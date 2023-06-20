#include "GameEngine.h"

int main(int argc, char *argv[])
{
	GameEngine game(argc, argv);
	game.GameLoop();
	return 0;
}