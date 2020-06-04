#include"header.h"
#include<iostream>
Game::Game(int W, int H, const char* title, int window_mode) {
	_winW = W;	_winH = H;	_isAlive = true;
	_bckgrd_color.r = 0xff; _bckgrd_color.g = 0xff; _bckgrd_color.b = 0xff; _bckgrd_color.a = 0xff;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	_win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _winW, _winH, window_mode);
	_ren = SDL_CreateRenderer(_win, 0, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(_ren, SDL_BLENDMODE_BLEND);
	_ev = new SDL_Event;
	loadTextures();
}
Game::~Game(){
	delete _ev;
	delete[] _objects;
	IMG_Quit();
	SDL_Quit();
}
void Game::input(){
	static bool kdown = false;	static bool mdown = false;
	static bool kup = false;	static bool mup = false;
	SDL_PollEvent(_ev);
	switch (_ev->type) {
	case SDL_QUIT:
		_isAlive = false;
		break;
	case SDL_KEYDOWN:
		if(!kdown)	
			switch (_ev->key.keysym.sym) {
			case SDLK_ESCAPE:
				_isAlive = false;
				break;
			case SDLK_F1:
				_night = !_night;
				break;
			case SDLK_F2:
				for (int x = 0; x < _map.sizeX; x++)
					for (int y = 0; y < _map.sizeY; y++)
						_map.cells[y*_map.sizeX + x] = 0;
				loadMap(_map);
				break;
			case SDLK_F3:
				for (int x = 0; x < _map.sizeX; x++)
					for (int y = 0; y < _map.sizeY; y++) {
						_map.cells[y*_map.sizeX + x]++;
						if (_map.cells[y*_map.sizeX + x] == maxTileIndex + 1) _map.cells[y*_map.sizeX + x] = 0;
					}
				loadMap(_map);
				break;
			case SDLK_F4:
				_shading = !_shading;
				break;
			case SDLK_F5:
				_highlight++;
				if (_highlight == 3) _highlight = 0;
				break;
			case SDLK_F7:
				_map.shuffle();
				loadMap(_map);
				break;
			case SDLK_F8:
				_map.generate();
				loadMap(_map);
				break;
			case SDLK_F9:
				printf("Object_Count - %i\n", _count);
				printf("Map_ID - %i\n", _map_id);
				break;
		}
		kdown = true;	kup = false;
		break;
	case SDL_KEYUP:
		kdown = false;	kup = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (!mdown) {
			printf("123\n");
			int X = _ev->motion.x / (_winW / _map.sizeX); int Y = _ev->motion.y / (_winH / _map.sizeY);
			_map.cells[Y*_map.sizeX + X]++;
			if (_map.cells[Y*_map.sizeX + X] == maxTileIndex + 1) _map.cells[Y*_map.sizeX + X] = 0;
			loadMap(_map);
		}
		mdown = true;	mup = false;
		break;
	case SDL_MOUSEBUTTONUP:
		mdown = false;	mup = true;
		break;
	}
}
void Game::update(){}
void Game::render() const{
	// Background
	SDL_SetRenderDrawColor(_ren,_bckgrd_color.r, _bckgrd_color.g, _bckgrd_color.b, _bckgrd_color.a);
	SDL_RenderClear(_ren);
	// Objects
	SDL_Rect src;
	SDL_Rect dst;
	for (int i = 0; i < _count; i++) {
		dst.h = _objects[i].rt->h;	src.h = 100;
		dst.w = _objects[i].rt->w;	src.w = 100;
		dst.x = _objects[i].rt->x;	src.x = (_objects[i].tx_nbr % 4)*src.w;
		dst.y = _objects[i].rt->y;	src.y = (_objects[i].tx_nbr / 4)*src.h;
		SDL_RenderCopy(_ren, _objects->tx, &src, &dst);
		if(_shading) SDL_RenderCopy(_ren, _textures[1], &src, &dst);
	}
	// Cellshading(old)
	/*if (_shading) {
		dst.h = _winH;	src.h = _winH;
		dst.w = _winW;	src.w = _winW;
		dst.x = 0;		src.x = 0;
		dst.y = 0;		src.y = 0;
		SDL_RenderCopy(_ren, _textures[1], &src, &dst);
	}*/
	// Highlight(Old)
	if (_highlight == 2) {
		src.w = _winW / _map.sizeX; src.x = (_ev->motion.x / src.w) * src.w;
		src.h = _winH / _map.sizeY; src.y = (_ev->motion.y / src.h) * src.h;
		SDL_SetRenderDrawColor(_ren, 0x50, 0xf0, 0x70, 0xc0);
		SDL_RenderFillRect(_ren, &src);
	}
	//Night??
	if (_night) {
		src.w = _winW;	src.h = _winH;
		src.x = 0;		src.y = 0;
		SDL_SetRenderDrawColor(_ren, 0x00, 0x00, 0x50, 0x90);
		SDL_RenderFillRect(_ren, &src);
	}
	// Highlight(New)
	if (_highlight == 1) {
		src.w = 100; dst.x = (_ev->motion.x / (_winW / _map.sizeX))* (_winW / _map.sizeX);
		src.h = 100; dst.y = (_ev->motion.y / (_winH / _map.sizeY))* (_winH / _map.sizeY);
		dst.w = _winW / _map.sizeX;	src.x = 0;
		dst.h = _winH / _map.sizeY; src.x = 0;
		SDL_RenderCopy(_ren, _textures[2], &src, &dst);
	}
	//
	SDL_RenderPresent(_ren);
}
void Game::loadTextures() {
	_textures[0] = IMG_LoadTexture(_ren, "Assets/Tiles.png");
	_textures[1] = IMG_LoadTexture(_ren, "Assets/CellShading.png");
	_textures[2] = IMG_LoadTexture(_ren, "Assets/Highlights.png");
	_textures[3] = IMG_LoadTexture(_ren, "Assets/CharacterAll.png");
	_textures[4] = IMG_LoadTexture(_ren, "Assets/CharacterAll_Red.png");
}
void Game::loadMap(Map map){
	_map = map;
	if (_map_id != -1) {
		for (int i = _map.sizeX*_map.sizeY - 1; i >= 0; i--)
			delObj(_map_id + i);
	}
	_map_id = _count;
	int X = _map.sizeX;	int Y = _map.sizeY;
	int W = _winW / X;	int H = _winH / Y;
	for (int x = 0; x < X; x++)
		for (int y = 0; y < Y; y++)
			addObj(W, H, W*x, H*y, _textures[0], _map.cells[y*X + x]);
	/*for (int y = 0; y < Y; y++) {
		for (int x = 0; x < X; x++)
			printf("%i",_map.cells[y*X+x]);
		printf("\n");
	}*/
}
void Game::loadChar(int X, int Y, SDL_Texture* tex) {
	if (tex == nullptr)
		tex = _charTex;
}
void killChar(int index){
}
void Game::addObj(int W, int H, int X, int Y, SDL_Texture* tx, int tx_nbr){
	OBJ* tmp = new OBJ[_count];
	for (int i = 0; i < _count; i++)
		tmp[i] = _objects[i];
	delete[] _objects;
	_objects = new OBJ[++_count];
	for (int i = 0; i < _count - 1; i++)
		_objects[i] = tmp[i];
	delete[] tmp;
		_objects[_count - 1].rt = new SDL_Rect;
		_objects[_count - 1].rt->w = W;
		_objects[_count - 1].rt->h = H;
		_objects[_count - 1].rt->x = X;
		_objects[_count - 1].rt->y = Y;
		_objects[_count - 1].tx = tx;
		_objects[_count - 1].tx_nbr = tx_nbr;
		_objects[_count - 1].id = _count - 1;
}
void Game::delObj(int index){
	OBJ* tmp = new OBJ[_count - 1];
	for (int i = 0, j = 0; i < _count && i != index; i++, j++) {
		tmp[j] = _objects[i];
		tmp[j].id = j;
	}
	delete[] _objects;
	_objects = new OBJ[--_count];
	for (int i = 0; i < _count; i++)
		_objects[i] = tmp[i];
	delete[] tmp;
}