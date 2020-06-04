#include"header.h"
#include<cstdlib>
#include<time.h>
#include<cmath>

Map::Map(int X,int Y){
	sizeX = X;	sizeY = Y;
	cells = new int[sizeX*sizeY];
	shuffle();
}
Map::~Map(){
}
void Map::clear(int c){
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			cells[y*sizeX + x] = c;
}
void Map::shuffle(){
	srand(time(0));
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			cells[y*sizeX + x] = rand() % maxTileIndex+1;
}
void Map::shuffle(int seed){
	srand(seed);
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			cells[y*sizeX + x] = rand() % 8;
}
void Map::generate(int seed) {
	if (seed == -1) seed = time(0);
	srand(seed);
	clear(4);
	int tmp = 0;
	while(tmp<2)
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			if (chance(1)) {
				tmp++;
				chg_cell(x, y, 0);
			}
	for (int n = 32; n > 0; n--) {
		for (int x = 0; x < sizeX; x++)
			for (int y = 0; y < sizeY; y++)
				if (cells[y*sizeX + x] != 0)
					if (chance(n/16* pow(2, multiplier(x, y, 0)))) {
						cells[y*sizeX + x] = 0;
					}
	}
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			if (cells[y*sizeX + x] == 4)
				if (multiplier(x,y,4)+multiplier(x,y,5)>7)
					cells[y*sizeX + x] = 5;
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			if (cells[y*sizeX + x] == 0)
				if (multiplier(x, y, 0) + multiplier(x, y, 3) > 7)
					cells[y*sizeX + x] = 3;
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			if (cells[y*sizeX + x] == 0)
				cells[y*sizeX + x] = 1;
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			if (cells[y*sizeX + x] == 3)
				cells[y*sizeX + x] = 0;
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			if (cells[y*sizeX + x] == 0)
			if(chance(75))	cells[y*sizeX + x] = 3;
	for (int x = 0; x < sizeX; x++)
		for (int y = 0; y < sizeY; y++)
			if (cells[y*sizeX + x] != 4 && cells[y*sizeX + x] != 5)
				if (multiplier(x,y,4)==0)
					if (chance(10)) 
						cells[y*sizeX + x] = 7;
	printf("SEED - %i\n", seed);
}
void Map::chg_cell(int x,int y,int c){
	cells[y*sizeX + x] = c;
}
int Map::multiplier(int X, int Y, int C) {
	int tmp = 0;
	for (int x = X - 1; x <= X + 1; x++)
		for (int y = Y - 1; y <= Y + 1; y++)
			if (x >= 0 && x <= sizeX && y >= 0 && y <= sizeY) {
				if (cells[y*sizeX + x] == C) tmp++;
			}
	return tmp;
}