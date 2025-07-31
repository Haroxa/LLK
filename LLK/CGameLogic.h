#pragma once
#include "iostream"
#include "fstream"
#include "queue"
#include "vector"

#define BLANK -1
#define NR 10
#define NC 16
#define PATH_LEN (NR*NC+1)
struct Vertex 
{
	int row, col, val, num;
};

struct Link {
	enum ENDOPT { XY_END, ELE_START };
	int** tempMap;
	int x1, y1, x2, y2;
	bool IsEndFind(int x, int y,ENDOPT opt=XY_END) { 
		switch (opt)
		{
		case Link::ELE_START:
			if (x1 == x && y1 == y)
				return false;
			return tempMap[x1][y1] == tempMap[x][y];
		case Link::XY_END:
		default:
			return x2 == x && y2 == y;
		}
	}
};

class CGameLogic
{
public:
	Vertex m_avPath[PATH_LEN];			// 保存在进行连接判断时所经过的顶点
	int m_nVexNum;
	
	CGameLogic();
	int** InitMap(bool is_reset = true);
	void ResetMap(int** pGameMap);
	void ReleaseMap(int**& pGameMap);

	bool IsLink(int** pGameMap, Vertex v1, Vertex v2);
	int GetDetailPath(Vertex path[][NC], int x, int y);
	bool IsLinkBFS(int** pGameMap, Vertex v1, Vertex v2,Link::ENDOPT opt = Link::ENDOPT::XY_END);
	void GetTip(int** pGameMap);
	void UpdateScore(int mode, int& score);

	void Clear(int** pGameMap, Vertex v1, Vertex v2);
	int GetVexPath(Vertex* avPath);
	void ClearStack();

//protected:
	bool LinkInRow(int** pGameMap, Vertex v1, Vertex v2);
	bool LinkInCol(int** pGameMap, Vertex v1, Vertex v2);
	bool OneCornerLink(int** pGameMap, Vertex v1, Vertex v2);
	bool LineY(int** pGameMap, int nRow1, int nRow2, int nCol);
	bool LineX(int** pGameMap, int nRow, int nCol1, int nCol2);
	void PushVertex(Vertex v);
	void PopVertex();
	bool TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2);

	bool IsBlank(int** pGameMap);

};

struct CGameException
{
	wchar_t* msg;
	CGameException(const wchar_t* msg) {
		this->msg = new wchar_t{ *msg };
	}
	~CGameException() {
		delete msg;
	}
};

