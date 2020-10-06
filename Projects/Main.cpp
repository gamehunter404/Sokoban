
#include"Common.h"
#include"Map.h"
#include <windows.h>
using namespace std;




vector<string> mapStrData = {

	"########",
	"# .. P #",
	"#      #",
	"#      #",
	"# OO   #",
	"#      #",
	"#      #",
	"########"
};


EInputType gInput;
Map gameMap;

int MapHeight = 0;
int MapWidth = 0;
bool isGameWin = false;
bool isGameOver = false;

void InitGame();//初始化游戏
void GetInput();//获取输入
void UpdateLogic();//更新游戏逻辑
void Draw();//绘制
void IsGameWin();


//待封装的函数



int main()
{
	InitGame();

	while (true)
	{
		GetInput();
		UpdateLogic();
		Draw();
	}
}


void InitGame()
{
	gameMap.InitMap(mapStrData);
	ChunckTypeMap[CHUNCK_BOX] = MARK_BOX;
	ChunckTypeMap[CHUNCK_EMPTY] = MARK_EMPTY;
	ChunckTypeMap[CHUNCK_PLAYER] = MARK_PLAYER;
	ChunckTypeMap[CHUNCK_WALL] = MARK_WALL;
	ChunckTypeMap[CHUNCK_TARGET] = MARK_TAEGET;
}

void GetInput()
{
	char input = '@';
	gInput = INPUT_NONE;


	if (_kbhit())
	{
		input = _getch();
	}
	
	switch (input)
	{
	case 'a':
		//left
		gInput = INPUT_LEFT;
		break;
	case 'd':
		//right
		gInput = INPUT_RIGHT;
		break;
	case 'w':
		//up
		gInput = INPUT_UP;
		break;
	case 's':
		//down
		gInput = INPUT_DOWN;
		break;
	}
}

void UpdateLogic()
{
	Position playerPosition = gameMap.PlayerPos();
	Position moveDir = moveDirs[(int)gInput];
	Position desiredPlayerPos = playerPosition + moveDir;

	if (isGameWin) {
		Sleep(2000);
		isGameOver = true;
		return;
	}
	
	if (gameMap.IsOutsideMap(desiredPlayerPos)) return;

	if (gameMap.IsWall(desiredPlayerPos))
	{
		desiredPlayerPos = playerPosition;
	}
	else if(gameMap.IsBox(desiredPlayerPos))
	{
		Position desiredBoxPos = desiredPlayerPos + moveDir;

		//找到最后一个箱子，处理移动一排箱子的情况
		while (gameMap.IsBox(desiredBoxPos))
		{
			desiredBoxPos = desiredBoxPos + moveDir;
		}

		if (desiredBoxPos != desiredPlayerPos&&gameMap.IsWalkArea(desiredBoxPos))
		{
			while (desiredBoxPos != desiredPlayerPos)
			{
				gameMap.MoveInMap( desiredBoxPos - moveDir, desiredBoxPos);
				desiredBoxPos = desiredBoxPos - moveDir;
			}

		}else
			desiredPlayerPos = playerPosition;

		
	}

	

	gameMap.MoveInMap(playerPosition,desiredPlayerPos);
	gameMap.SetPlayerPos(desiredPlayerPos);
	IsGameWin();
}

void Draw()
{
	system("cls");
	gameMap.Show();

	if (isGameWin)
	{
		cout << endl<< "游戏胜利！！！！！！！！！！！"<<endl;
	}
}

void IsGameWin()
{
	auto targetPositions = gameMap.GetTargetPosition();
	int count = 0;

	for (int i = 0; i < targetPositions.size(); i++)
	{
		if (!gameMap.IsOutsideMap(targetPositions[i]))
		{
			if (gameMap.GetChunckType(targetPositions[i])==CHUNCK_BOX)
			{
				count++;
			}
		}
	}
	isGameWin = count == targetPositions.size();
}
