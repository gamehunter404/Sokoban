#pragma once
#include"Common.h"


struct Chunck
{
public:
	Position pos;
	ChunckType backFace;
	ChunckType frontFace;
};



class Map
{
public:
	Map() {}
	void InitMap(std::vector<std::string> mapStrings)
	{
		if (mapStrings.size() > 0) {

			mapHeight = mapStrings.size();

			if (mapStrings[0].size() > 0)
				mapWidth = mapStrings[0].size();
		}

		map.resize(mapHeight);

		for (int i = 0; i < mapHeight; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				Chunck chunck;
				switch (mapStrings[i][j])
				{
				case MARK_TAEGET:
					chunck.backFace = CHUNCK_TARGET;
					chunck.frontFace = CHUNCK_NONE;
					this->targetPositions.push_back({ i,j });

					break;
				case MARK_EMPTY:
					chunck.backFace = CHUNCK_EMPTY;
					chunck.frontFace = CHUNCK_NONE;
					break;
				case MARK_PLAYER:
					this->playerPos = { i,j };
					chunck.backFace = CHUNCK_EMPTY;
					chunck.frontFace = CHUNCK_PLAYER;
					break;
				case MARK_WALL:
					chunck.backFace = CHUNCK_WALL;
					chunck.frontFace = CHUNCK_WALL;
					break;
				case MARK_BOX:
					chunck.frontFace = CHUNCK_BOX;
					chunck.backFace = CHUNCK_EMPTY;
					break;
				}
				chunck.pos = {i,j};
				map[i].push_back(chunck);
			}
		}
	}

	bool IsWall( Position pos)
	{
		
		return map[pos.i][pos.j].frontFace == CHUNCK_WALL;
	}

	bool IsBox( Position pos)
	{

		return map[pos.i][pos.j].frontFace == CHUNCK_BOX;
	}

	bool IsOutsideMap( Position pos)
	{
		if (pos.i > mapHeight || pos.i < 0)
		{
			return true;
		}

		if (pos.j > mapWidth || pos.j < 0)
		{
			return true;
		}

		return false;
	}

	bool IsTarget( Position pos)
	{
		if (IsOutsideMap(pos)) throw "position out of map";

		return map[pos.i][pos.j].frontFace == CHUNCK_NONE && map[pos.i][pos.j].backFace == CHUNCK_TARGET;

	}

	bool IsEmpty( Position pos)
	{
		if (IsOutsideMap(pos)) throw "position out of map";

		return map[pos.i][pos.j].frontFace == CHUNCK_NONE && map[pos.i][pos.j].backFace == CHUNCK_EMPTY;

	}

	//判断是否是可行走区域
	bool IsWalkArea( Position  pos)
	{
		if (IsOutsideMap(pos)) throw "position out of map";

		return IsEmpty(pos) || IsTarget(pos);

	}

	void MoveInMap( Position  source,  Position  target)
	{
		
		

		if (source == target) {
			
			return;
		}

		if (IsWalkArea(target))
		{
			if (map[target.i][target.j].frontFace == CHUNCK_NONE)
			{
				map[target.i][target.j].frontFace = map[source.i][source.j].frontFace;
				map[source.i][source.j].frontFace = CHUNCK_NONE;
			}
		}
	}

	int Width()
	{
		return mapWidth;
	}
	int Height()
	{
		return mapHeight;
	}

	void Show()
	{
		for (int i = 0; i < mapHeight; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				if (j == 0) std::cout<< std::endl;

				if (ChunckTypeMap.find(map[i][j].frontFace) != ChunckTypeMap.end())
				{
					std::cout << ChunckTypeMap[map[i][j].frontFace];
				}
				else if (ChunckTypeMap.find(map[i][j].backFace) != ChunckTypeMap.end())
				{
					std::cout << ChunckTypeMap[map[i][j].backFace];
				}

			}
		}

	}

	Position PlayerPos()
	{
		return playerPos;
	}
	void SetPlayerPos( Position pos)
	{
		this->playerPos = pos;
	}
	std::vector<Position> GetTargetPosition()
	{
		return targetPositions;
	}
	ChunckType GetChunckType(Position pos)
	{
		return map[pos.i][pos.j].frontFace != CHUNCK_NONE? map[pos.i][pos.j].frontFace: map[pos.i][pos.j].backFace;
	}

private:
	int mapWidth;
	int mapHeight;
	std::vector<std::vector<Chunck>> map;
	std::vector<Position> targetPositions;
	Position playerPos;
};

