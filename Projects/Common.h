#pragma once
#include<iostream>
#include <conio.h>
#include<string>
#include<vector>
#include<map>


const char MARK_WALL = '#';
const char MARK_PLAYER = 'P';
const char MARK_BOX = 'O';
const char MARK_EMPTY = ' ';
const char MARK_TAEGET = '.';
const char* LEVEL_PATH = "Levels";
const char* MAPFILETYPE = "txt";



enum EGameState
{
	GAMESTATE_NONE = 0,
	GAMESTATE_START,
	GAMESTATE_PLAYING,
	GAMESTATE_END
};

enum EInputType
{
	INPUT_NONE = 0,
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_ENTER,//回车确认
	INPUT_RESET,//重新恢复游戏
	INPUT_ESC,//返回主菜单
};

enum ChunckType
{
	CHUNCK_NONE =0,
	CHUNCK_EMPTY,
	CHUNCK_PLAYER,
	CHUNCK_WALL,
	CHUNCK_TARGET,
	CHUNCK_BOX
};

struct Position
{
	int i;
	int j;

	Position operator+(const Position& value) const
	{
		return { this->i + value.i,this->j + value.j };
	}

	Position operator-(const Position& value) const
	{
		return { this->i - value.i,this->j - value.j };
	}

	bool operator==(const Position&value) const
	{
		return this->i == value.i&&this->j == value.j;
	}

	bool operator!=(const Position&value) const
	{
		return this->i != value.i||this->j != value.j;
	}

	Position& operator-()
	{
		this->i *= -1;
		this->j *= -1;
		return *this;
	}



	void Print() const
	{
		std::cout << i << " " << j << std::endl;

	}


};

struct MapData
{
	std::vector<std::string> map;
	std::string mapName;

};

//游戏设置结构体，保存着游戏的设置

Position moveDirs[5] = {

	{0,0},{-1,0},{1,0}, {0,-1},{0,1}
};

std::map<ChunckType, char> ChunckTypeMap;

