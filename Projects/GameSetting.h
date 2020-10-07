#pragma once
#include"Common.h"
#include<vector>

class GameState;

struct GameSetting
{
	std::map< EGameState, GameState* > states;
	std::vector<MapData> mapDatas;
	EInputType gInput;
	EGameState prevState;
	EGameState currentState;
	Map gameMap;
	int mapIndex;
	bool isGameWin = false;
	bool isGameOver = false;
	bool isGameStart = false;


	//重新设置游戏状态
	void ResetGameSetting()
	{
		mapIndex = 0;
		gInput = INPUT_NONE;
		isGameStart = false;
		isGameWin = false;
		isGameOver = false;
		gInput = INPUT_NONE;
		prevState = GAMESTATE_START;
		currentState = GAMESTATE_START;
	}

	int MapDataNum()
	{
		return mapDatas.size();
	}

	void SetGameMap()
	{
		if (mapIndex >= 0 && mapIndex < mapDatas.size())
		{
			gameMap.InitMap(mapDatas[mapIndex]);
		}
	}
};


