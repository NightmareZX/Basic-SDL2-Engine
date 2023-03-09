#include "GameEngine.h"

int main(int argc, char *argv[])
{
	GameEngine::GetInstance()->GameLoop();
	return 0;
}