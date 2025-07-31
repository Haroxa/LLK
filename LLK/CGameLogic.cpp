#include "pch.h"
#include "CGameLogic.h"
#include "CGameControl.h"

CGameLogic::CGameLogic() {
	m_nVexNum = 0;
}

int** CGameLogic::InitMap(bool is_reset) {
	// 获取地图大小和花色
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	int nPicNum = CGameControl::s_nPicNum;
	// 开辟内存区域
	int** pGameMap = new int* [nRows];
	if (NULL == pGameMap)
		throw new CGameException(_T("内存操作异常！\n"));
	else
		for (int i = 0; i < nRows; i++)
		{
			pGameMap[i] = new int[nCols] {0};
			if (NULL == pGameMap)
			{
				throw new CGameException(_T("内存操作异常！\n"));
			}
			//memset(pGameMap[i], NULL, sizeof(int) * nCols);
		}
	if (!is_reset)
		return pGameMap;
	// 多少花色
	if ((nRows * nCols) % (nPicNum * 2) != 0)
	{
		ReleaseMap(pGameMap);
		throw new CGameException(_T("游戏花色与游戏地图大小不匹配！"));
	}
	int nRepeatNum = nRows * nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++)
		// 重复数
		for (int j = 0; j < nRepeatNum; j++)
		{
			pGameMap[nCount / nCols][nCount % nCols] = i;
			nCount++;
		}
	ResetMap(pGameMap);
	return pGameMap;
}

void CGameLogic::ResetMap(int** pGameMap) {
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	// 设置种子
	srand((int)time(NULL));
	// 随机任意交换两个数字
	int nVertexNum = nRows * nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		// 随机得到两个坐标
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;
		// 交换两个数值
		int nTmp = pGameMap[nIndex1 / nCols][nIndex1 % nCols];
		pGameMap[nIndex1 / nCols][nIndex1 % nCols] = pGameMap[nIndex2 /
			nCols][nIndex2 % nCols];
		pGameMap[nIndex2 / nCols][nIndex2 % nCols] = nTmp;
	}
}

void CGameLogic::ReleaseMap(int**& pGameMap)
{
	for (int i = 0; i < CGameControl::s_nRows; ++i)
		delete[] pGameMap[i];
	delete[] pGameMap;
	pGameMap = NULL;
}

bool CGameLogic::IsLink(int** pGameMap, Vertex v1, Vertex v2) {
	PushVertex(v1);
	return LinkInRow(pGameMap, v1, v2) || LinkInCol(pGameMap, v1, v2) || 
		OneCornerLink(pGameMap, v1, v2) || TwoCornerLink(pGameMap, v1, v2);
}

int CGameLogic::GetDetailPath(Vertex path[][NC], int x, int y) {
	auto k = path[x][y];
	if (k.num == 0)
		return 0;
	int id = GetDetailPath(path, k.row, k.col);
	auto k1 = path[k.row][k.col];
	// 判断路径拐角点
	if (k.num != k1.num)
		m_avPath[id++] = k;
	return id;
}

const int dx[4] = {  0,-1, 1, 0 };
const int dy[4] = { -1, 0, 0, 1 };

Vertex path[NR][NC];
Vertex visit[PATH_LEN];		// 记录 path 修改位置，防止每次全部初始化浪费时间

bool CGameLogic::IsLinkBFS(int** pGameMap, Vertex v1, Vertex v2,Link::ENDOPT opt) 
{
	Link link{ pGameMap, v1.row, v1.col, v2.row, v2.col };
	int nRows = CGameControl::s_nRows, nCols = CGameControl::s_nCols;
	int vInd = 0;
	int t = 0 , x, y;
	bool isEnd = false;
	// 由于每次入队的位置都是按距离顺序入队，无需考虑选择最短
	std::queue<Vertex> queue;
	queue.push({ v1.row, v1.col, 1, 0 });	
	// path 存的是先前的坐标 与 之间的 距离、方向
	path[v1.row][v1.col] = { -1, -1, 1, 0 };
	visit[vInd++] = v1;
	while (!queue.empty() && !isEnd) {
		auto k = queue.front();
		queue.pop();
		// 四个方向进行判断
		for (int i = 0; i < 4; i++) {
			x = k.row + dx[i]; y = k.col + dy[i];
			// 越界
			if (x < 0 || x >= nRows || y < 0 || y >= nCols)
				continue;
			// 结束
			if (link.IsEndFind(x,y,opt)) {
				path[x][y] = { k.row, k.col, path[k.row][k.col].val + 1, i + 1 };
				visit[vInd++] = { x, y };
				isEnd = true;
				break;
			}
			// 不为空
			if (pGameMap[x][y] != BLANK)
				continue;
			// 未访问过 且 可抵达的位置 或者 可以更新最短距离的位置
			// 此处 val 记录距离，num 记录方位
			if (path[x][y].val == 0 ) {
				path[x][y].val = path[k.row][k.col].val + 1;
				queue.push({ x,y,path[x][y].val,i + 1 });
				path[x][y] = { k.row, k.col, path[x][y].val, i + 1 };
				visit[vInd++] = { x, y };
			}
		}
	}
	// 只有满足结束条件，才查找路径
	if (isEnd) {
		m_nVexNum = GetDetailPath(path, x, y);
		PushVertex({ x,y,path[x][y].val });
	}
	// 清空
	while (vInd--) {
		auto k = visit[vInd];
		path[k.row][k.col] = {};
	}
	return isEnd;
}

void CGameLogic::GetTip(int** pGameMap) {
	int nRows = CGameControl::s_nRows, nCols = CGameControl::s_nCols;
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			if (pGameMap[i][j] == BLANK)
				continue;
			if (IsLinkBFS(pGameMap, { i,j }, {}, Link::ENDOPT::ELE_START))
				return;
		}
	}
}


void CGameLogic::UpdateScore(int mode, int& score) {
	switch (mode) {
	case CGameControl::MODE::BASIC:
		score += 2;
		break;
	case CGameControl::MODE::RELAX:
		score += m_avPath[m_nVexNum-1].val;
		break;
	default:
		break;
	}
}


void CGameLogic::Clear(int** pGameMap, Vertex v1, Vertex v2) {
	pGameMap[v1.row][v1.col] = BLANK;
	pGameMap[v2.row][v2.col] = BLANK;
}

int CGameLogic::GetVexPath(Vertex* avPath) {
	for (int i = 0; i < m_nVexNum; i++)
		avPath[i] = m_avPath[i];
	return m_nVexNum;
}


bool CGameLogic::LinkInRow(int** pGameMap, Vertex v1, Vertex v2) {
	if (v1.row != v2.row)
		return false;
	int nCol1 = v1.col, nCol2 = v2.col, nRow = v1.row;
	//保证 nCol1 的值小于 nCol2
	if (nCol1 > nCol2) 
		std::swap(nCol1,nCol2);
	//直通
	for (int i = nCol1 + 1; i < nCol2; i++)
		if (pGameMap[nRow][i] != BLANK) 
			return false;
	return true;
}

bool CGameLogic::LinkInCol(int** pGameMap, Vertex v1, Vertex v2) {
	if (v1.col != v2.col)
		return false;
	int nRow1 = v1.row, nRow2 = v2.row, nCol = v1.col;
	if (nRow1 > nRow2)
		std::swap(nRow1,nRow2);
	//直通
	for (int i = nRow1 + 1; i < nRow2; i++)
		if (pGameMap[i][nCol] != BLANK)
			return false;
	return true;
}

bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2) {
	// 直角能够消子，那么顶点一定在与两个点的行和列相交的点，只有这两个点为空，
	// 才有可能实现二条直线消子
	if (pGameMap[v1.row][v2.col] == BLANK)
		if (LineY(pGameMap, v1.row, v2.row, v2.col) && 
			LineX(pGameMap, v1.row, v1.col, v2.col))
		{
			Vertex v = { v1.row, v2.col, BLANK };
			PushVertex(v);
			return true;
		}
	if (pGameMap[v2.row][v1.col] == BLANK)
		if (LineY(pGameMap, v1.row, v2.row, v1.col) && 
			LineX(pGameMap, v2.row, v1.col, v2.col))
		{
			Vertex v = { v2.row, v1.col, BLANK };
			PushVertex(v);
			return true;
		}
	return false;
}

bool CGameLogic::LineY(int** pGameMap, int nRow1, int nRow2, int nCol) {
	Vertex v1 = { nRow1, nCol }, v2 = { nRow2, nCol };
	return LinkInCol(pGameMap, v1, v2);
}

bool CGameLogic::LineX(int** pGameMap, int nRow, int nCol1, int nCol2) {
	Vertex v1 = { nRow, nCol1, BLANK }, v2 = { nRow, nCol2, BLANK };
	return LinkInRow(pGameMap, v1, v2);
}

void CGameLogic::PushVertex(Vertex v) {
	m_avPath[m_nVexNum++] = v;
}

void CGameLogic::PopVertex() {
	if (m_nVexNum > 0)
		m_avPath[--m_nVexNum] = {};
}

void CGameLogic::ClearStack() {
	while (m_nVexNum)
		PopVertex();
}

bool CGameLogic::TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2) {
	for (int nCol = 0; nCol < CGameControl::s_nCols; nCol++)
		// 找到一条与 Y 轴平行的连通线段
		if (pGameMap[v1.row][nCol] == BLANK && pGameMap[v2.row][nCol] == BLANK)
			if (LineY(pGameMap, v1.row, v2.row, nCol))
				if (LineX(pGameMap, v1.row, v1.col, nCol) && 
					LineX(pGameMap, v2.row, v2.col, nCol))
				{
					// 保存节点
					Vertex vx1 = { v1.row, nCol, BLANK };
					Vertex vx2 = { v2.row, nCol, BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
	for (int nRow = 0; nRow < CGameControl::s_nRows; nRow++)
		// 找到一条与 X 轴平行的连通线段
		if (pGameMap[nRow][v1.col] == BLANK && pGameMap[nRow][v2.col] == BLANK)
			if (LineX(pGameMap, nRow, v1.col, v2.col))
				if (LineY(pGameMap, nRow, v1.row, v1.col) && 
					LineY(pGameMap, nRow, v2.row, v2.col))
				{
					// 保存节点
					Vertex vx1 = { nRow, v1.col, BLANK };
					Vertex vx2 = { nRow, v2.col, BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
	return false;
}

bool CGameLogic::IsBlank(int** pGameMap) {
	int nRow = CGameControl::s_nRows, nCol = CGameControl::s_nCols;
	for (int i = 0; i < nRow; i++) 
		for (int j = 0; j < nCol; j++) 
			if (pGameMap[i][j] != BLANK)
				return false;
	return true;
}