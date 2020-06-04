#pragma once
#include"SDL.h"
#include"SDL_image.h"
#include<string>
#include"defines.h"

#define maxTileIndex 7	//	Maximum index of tile in tiles.png

struct OBJ {
	SDL_Rect* rt = nullptr;
	SDL_Texture* tx = nullptr;
	int tx_nbr = 0;
	bool hidden = false;
	int id;
};

/*enum Cell
{
	DIRT = 0,	SAND = 1,	SNOW = 2,	GRASS = 3,
	WATER = 4,	DWATER = 5,	SWAMP = 6,	LAVA = 7,
	UNDF8=8,	UNDF9=9,	UNDF10=10,	UNDF11=11,
	UNDF12=12,	UNDF13=13,	UNDF14=14,	UNDF15=15
};*/

class Map
{
public:
	Map(int X = 16,int Y = 8);
	~Map();
	void clear(int c = 0);
	void shuffle();
	void shuffle(int seed);
	void generate(int seed = -1);
	void chg_cell(int x, int y, int c);
	bool chance(int n) {
		if (rand() % 100 >= (100 - n)) return true;
		else return false;
	};
	int multiplier(int X, int Y, int C);
	int* cells = nullptr;
	int sizeX = 0;
	int sizeY = 0;
};

class Game
{
public:
	Game(int W = 1600, int H = 800,const char* title = "Title", int window_mode = SDL_WINDOW_FOREIGN);
	~Game();
	void input();
	void update();
	void render() const;
	bool isAlive() { return _isAlive; };
	//
	void loadTextures();
	void loadMap(Map map);
	void loadChar(int X = 0, int Y = 0, SDL_Texture* tex = nullptr);
	void killChar(int index);
	//
	void addObj(int W, int H, int X, int Y, SDL_Texture* tx, int tx_nbr);
	void delObj(int index);
private:
	SDL_Window* _win = nullptr;
	SDL_Renderer* _ren = nullptr;
	SDL_Event* _ev = nullptr;
	OBJ* _objects = nullptr;	int _count = 0;
	SDL_Texture* _textures[5];
	//
	Map _map; int _map_id = -1;
	int* _char_id = nullptr; int _char_count = 0;
	//
	//
	int _winW;
	int _winH;
	bool _isAlive;	bool _night = false;
	bool _shading = true;
	int _highlight = 1;
	//
	SDL_Color _bckgrd_color;
};