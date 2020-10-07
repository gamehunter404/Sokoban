
#include"Common.h"
#include"Map.h"
#include"GameState.h"
#include <windows.h>
#include<fstream>
#include<io.h>
#include"GameSetting.h"
using namespace std;

GameSetting setting;


void InitGame();//初始化游戏
void GetInput();//获取输入
void UpdateLogic();//更新游戏逻辑
void Draw();//绘制
void DestoryGame();//销毁，释放游戏资源

void ReadMapInfos(string path, vector<MapData>& mapInfos);//读取游戏的关卡信息
void ReadMapStr(const char* path,MapData&data);
bool IsRightMap(MapData&data);//判断一个地图数据是否正确
bool IsTxtFile(const char* fileName,int len,string&res);


//待封装的函数

int main()
{
	InitGame();
	while (!setting.isGameOver)
	{
		GetInput();
		UpdateLogic();
		Draw();
	}
	DestoryGame();
}


void InitGame()
{
	ReadMapInfos(LEVEL_PATH, setting.mapDatas);//读取地图数据

	ChunckTypeMap[CHUNCK_BOX] = MARK_BOX;
	ChunckTypeMap[CHUNCK_EMPTY] = MARK_EMPTY;
	ChunckTypeMap[CHUNCK_PLAYER] = MARK_PLAYER;
	ChunckTypeMap[CHUNCK_WALL] = MARK_WALL;
	ChunckTypeMap[CHUNCK_TARGET] = MARK_TAEGET;

	setting.states[GAMESTATE_NONE] = new GameState();
	setting.states[GAMESTATE_START] = new GameStartState();
	setting.states[GAMESTATE_PLAYING] = new GamePlayState();
	setting.states[GAMESTATE_END] = new GameEndState();

	setting.prevState = setting.currentState = GAMESTATE_START;
}

void GetInput()
{
	char input = '@';
	setting.gInput = INPUT_NONE;


	if (_kbhit())
	{
		input = _getch();
	}
	
	switch (input)
	{
	case 'a':
		//left
		setting.gInput = INPUT_LEFT;
		break;
	case 'd':
		//right
		setting.gInput = INPUT_RIGHT;
		break;
	case 'w':
		//up
		setting.gInput = INPUT_UP;
		break;
	case 's':
		//down
		setting.gInput = INPUT_DOWN;
		break;
	case '\r':
		setting.gInput = INPUT_ENTER;
		break;
	case 'r':
		setting.gInput = INPUT_RESET;
		break;
	case 27:
		setting.gInput = INPUT_ESC;
		break;
	}
}

void UpdateLogic()
{
	auto state = setting.states[setting.currentState];
	if (setting.prevState != setting.currentState)
	{
		//退出旧状态,进入新的状态
		setting.states[setting.prevState]->Exit(&setting);
		state->Enter(&setting);
		setting.prevState = setting.currentState;
	}
	else
	{
		setting.prevState = setting.currentState;
		state->Update(&setting);
		EGameState targetState = state->ChangeState(&setting);

		if (targetState != GAMESTATE_NONE)
		{
			setting.currentState = targetState;
		}
	}
}

void Draw()
{
	auto state = setting.states[setting.currentState];
	state->Draw(&setting);
}

void DestoryGame()
{
}

void ReadMapInfos(string src,vector<MapData>& mapInfos)
{
	intptr_t hFile = 0;
	string fileName;
	string folderPath = src+ "\\*";
	string filepath;
	struct _finddata_t fileinfo;

	if ((hFile = _findfirst(folderPath.c_str(),&fileinfo)) != -1)
	{
		//成功找到
		do
		{
			fileName = "";
			if (!(fileinfo.attrib&_A_NORMAL)&&!(fileinfo.attrib&_A_HIDDEN)&&IsTxtFile(fileinfo.name,strlen(fileinfo.name),fileName))
			{
				MapData data;
				data.mapName = fileName;
				filepath = src+"\\"+fileinfo.name;
				ReadMapStr(filepath.c_str(),data);

				if (IsRightMap(data))
				{

					setting.mapDatas.push_back(data);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		//关闭句柄
		_findclose(hFile);
	}
}

void ReadMapStr(const char * path, MapData &data)
{
	ifstream inputFile(path,ifstream::binary);
	inputFile.seekg(0,ifstream::end);
	//一般情况下地图的大小都不会超过int类型的范围
	int fileSize = static_cast<int>(inputFile.tellg());
	inputFile.seekg(0,ifstream::beg);

	if (fileSize > 0)
	{
		char *strBuffer = new char[fileSize];
		inputFile.read(strBuffer,fileSize);
		for (int i = 0,j = 0; i < fileSize; i++)
		{
			if (strBuffer[i] != '\r')
			{
				if (data.map.size() <= j) data.map.push_back("");
				data.map[j] += strBuffer[i];
			}
			else {
				i++;
				j++;
			}
		}

		delete[]strBuffer;
	}
	else
	{
		//TODO 数据量太大了
	}

}

bool IsRightMap(MapData &data)
{
	//检测四周是否有边界，以及箱子的数量和目标的数量是否一致
	if (data.map.size() == 0)
		return false;


	int r = data.map.size();//地图有几行
	int c = data.map[0].size();//地图有几列
	int boxNum = 0, targetNum = 0;

	for (int j = 0; j < c; j++)
	{
		if (data.map[0][j] != MARK_WALL || data.map[r - 1][j] != MARK_WALL) return false;
	}

	for (int i = 0; i < r; i++)
	{
		if (data.map[i][0] != MARK_WALL || data.map[i][c - 1] != MARK_WALL) return false;
	}


	//检查箱子的数量和目标的数量是否一致

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			if (data.map[i][j] == MARK_BOX) boxNum++;
			if (data.map[i][j] == MARK_TAEGET) targetNum++;
		}
	}

	return boxNum == targetNum;


}

bool IsTxtFile(const char * fileName, int len, string & res)
{
	if (len == 0) return false;

	vector<string> tmp(1,"");

	for (int i = 0,j = 0; i < len; i++)
	{
		if (fileName[i] != '.')
			tmp[j] += fileName[i];
		else {
			j++;
			tmp.push_back("");
		}
	}

	//组合文件名
	for (int i = 0; i < tmp.size() - 1; i++)
	{
		res += tmp[i];
		if(i!= tmp.size()-2)
			res += '.';
	}

	return tmp[tmp.size() - 1] == MAPFILETYPE;
}
