#include"header.h"
#include<iostream>

int main(int argc, char** argv) {
	Game game(1600,800);
	Map map(64,32);
	map.generate();
	game.loadMap(map);
	while (game.isAlive()) {
		game.input();
		game.render();
	}
	return 0;
}