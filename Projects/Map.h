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
	void InitMap(MapData& mapData)
	{
		std::vector<std::string> &mapStrings = mapData.map;

		if (mapStrings.size() > 0) {

			mMapHeight = mapStrings.size();

			if (mapStrings[0].size() > 0)
				mMapWidth = mapStrings[0].size();
		}

		mMap.resize(mMapHeight);

		for (int i = 0; i < mMapHeight; i++)
		{
			for (int j = 0; j < mMapWidth; j++)
			{
				Chunck chunck;
				switch (mapStrings[i][j])
				{
				case MARK_TAEGET:
					chunck.backFace = CHUNCK_TARGET;
					chunck.frontFace = CHUNCK_NONE;
					this->mTargetPositions.push_back({ i,j });

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
				mMap[i].push_back(chunck);
			}
		}
	}

	void DestoryMap()
	{
		this->mMapHeight = 0;
		this->mMapWidth = 0;
		this->mMap.clear();
		this->mTargetPositions.clear();
		this->playerPos = {0,0};
	}

	bool IsWall( Position pos)
	{
		
		return mMap[pos.i][pos.j].frontFace == CHUNCK_WALL;
	}

	bool IsBox( Position pos)
	{

		return mMap[pos.i][pos.j].frontFace == CHUNCK_BOX;
	}

	bool IsOutsideMap( Position pos)
	{
		if (pos.i > mMapHeight || pos.i < 0)
		{
			return true;
		}

		if (pos.j > mMapWidth || pos.j < 0)
		{
			return true;
		}

		return false;
	}

	bool IsTarget( Position pos)
	{
		if (IsOutsideMap(pos)) throw "position out of map";

		return mMap[pos.i][pos.j].frontFace == CHUNCK_NONE && mMap[pos.i][pos.j].backFace == CHUNCK_TARGET;

	}

	bool IsEmpty( Position pos)
	{
		if (IsOutsideMap(pos)) throw "position out of map";

		return mMap[pos.i][pos.j].frontFace == CHUNCK_NONE && mMap[pos.i][pos.j].backFace == CHUNCK_EMPTY;

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
			if (mMap[target.i][target.j].frontFace == CHUNCK_NONE)
			{
				mMap[target.i][target.j].frontFace = mMap[source.i][source.j].frontFace;
				mMap[source.i][source.j].frontFace = CHUNCK_NONE;
			}
		}
	}

	int Width()
	{
		return mMapWidth;
	}
	int Height()
	{
		return mMapHeight;
	}

	void Show()
	{
		for (int i = 0; i < mMapHeight; i++)
		{
			for (int j = 0; j < mMapWidth; j++)
			{
				if (j == 0) std::cout<< std::endl;

				if (ChunckTypeMap.find(mMap[i][j].frontFace) != ChunckTypeMap.end())
				{
					std::cout << ChunckTypeMap[mMap[i][j].frontFace];
				}
				else if (ChunckTypeMap.find(mMap[i][j].backFace) != ChunckTypeMap.end())
				{
					std::cout << ChunckTypeMap[mMap[i][j].backFace];
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
		return mTargetPositions;
	}
	ChunckType GetChunckType(Position pos)
	{
		return mMap[pos.i][pos.j].frontFace != CHUNCK_NONE? mMap[pos.i][pos.j].frontFace: mMap[pos.i][pos.j].backFace;
	}

private:
	int mMapWidth;
	int mMapHeight;
	std::vector<std::vector<Chunck>> mMap;
	std::vector<Position> mTargetPositions;
	Position playerPos;
};

