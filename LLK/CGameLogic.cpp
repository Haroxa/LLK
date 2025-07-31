#include "pch.h"
#include "CGameLogic.h"
#include "CGameControl.h"

CGameLogic::CGameLogic() {
	m_nVexNum = 0;
}

int** CGameLogic::InitMap(bool is_reset) {
	// ��ȡ��ͼ��С�ͻ�ɫ
	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	int nPicNum = CGameControl::s_nPicNum;
	// �����ڴ�����
	int** pGameMap = new int* [nRows];
	if (NULL == pGameMap)
		throw new CGameException(_T("�ڴ�����쳣��\n"));
	else
		for (int i = 0; i < nRows; i++)
		{
			pGameMap[i] = new int[nCols] {0};
			if (NULL == pGameMap)
			{
				throw new CGameException(_T("�ڴ�����쳣��\n"));
			}
			//memset(pGameMap[i], NULL, sizeof(int) * nCols);
		}
	if (!is_reset)
		return pGameMap;
	// ���ٻ�ɫ
	if ((nRows * nCols) % (nPicNum * 2) != 0)
	{
		ReleaseMap(pGameMap);
		throw new CGameException(_T("��Ϸ��ɫ����Ϸ��ͼ��С��ƥ�䣡"));
	}
	int nRepeatNum = nRows * nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++)
		// �ظ���
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
	// ��������
	srand((int)time(NULL));
	// ������⽻����������
	int nVertexNum = nRows * nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		// ����õ���������
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;
		// ����������ֵ
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
	// �ж�·���սǵ�
	if (k.num != k1.num)
		m_avPath[id++] = k;
	return id;
}

const int dx[4] = {  0,-1, 1, 0 };
const int dy[4] = { -1, 0, 0, 1 };

Vertex path[NR][NC];
Vertex visit[PATH_LEN];		// ��¼ path �޸�λ�ã���ֹÿ��ȫ����ʼ���˷�ʱ��

bool CGameLogic::IsLinkBFS(int** pGameMap, Vertex v1, Vertex v2,Link::ENDOPT opt) 
{
	Link link{ pGameMap, v1.row, v1.col, v2.row, v2.col };
	int nRows = CGameControl::s_nRows, nCols = CGameControl::s_nCols;
	int vInd = 0;
	int t = 0 , x, y;
	bool isEnd = false;
	// ����ÿ����ӵ�λ�ö��ǰ�����˳����ӣ����迼��ѡ�����
	std::queue<Vertex> queue;
	queue.push({ v1.row, v1.col, 1, 0 });	
	// path �������ǰ������ �� ֮��� ���롢����
	path[v1.row][v1.col] = { -1, -1, 1, 0 };
	visit[vInd++] = v1;
	while (!queue.empty() && !isEnd) {
		auto k = queue.front();
		queue.pop();
		// �ĸ���������ж�
		for (int i = 0; i < 4; i++) {
			x = k.row + dx[i]; y = k.col + dy[i];
			// Խ��
			if (x < 0 || x >= nRows || y < 0 || y >= nCols)
				continue;
			// ����
			if (link.IsEndFind(x,y,opt)) {
				path[x][y] = { k.row, k.col, path[k.row][k.col].val + 1, i + 1 };
				visit[vInd++] = { x, y };
				isEnd = true;
				break;
			}
			// ��Ϊ��
			if (pGameMap[x][y] != BLANK)
				continue;
			// δ���ʹ� �� �ɵִ��λ�� ���� ���Ը�����̾����λ��
			// �˴� val ��¼���룬num ��¼��λ
			if (path[x][y].val == 0 ) {
				path[x][y].val = path[k.row][k.col].val + 1;
				queue.push({ x,y,path[x][y].val,i + 1 });
				path[x][y] = { k.row, k.col, path[x][y].val, i + 1 };
				visit[vInd++] = { x, y };
			}
		}
	}
	// ֻ����������������Ų���·��
	if (isEnd) {
		m_nVexNum = GetDetailPath(path, x, y);
		PushVertex({ x,y,path[x][y].val });
	}
	// ���
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
	//��֤ nCol1 ��ֵС�� nCol2
	if (nCol1 > nCol2) 
		std::swap(nCol1,nCol2);
	//ֱͨ
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
	//ֱͨ
	for (int i = nRow1 + 1; i < nRow2; i++)
		if (pGameMap[i][nCol] != BLANK)
			return false;
	return true;
}

bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2) {
	// ֱ���ܹ����ӣ���ô����һ��������������к����ཻ�ĵ㣬ֻ����������Ϊ�գ�
	// ���п���ʵ�ֶ���ֱ������
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
		// �ҵ�һ���� Y ��ƽ�е���ͨ�߶�
		if (pGameMap[v1.row][nCol] == BLANK && pGameMap[v2.row][nCol] == BLANK)
			if (LineY(pGameMap, v1.row, v2.row, nCol))
				if (LineX(pGameMap, v1.row, v1.col, nCol) && 
					LineX(pGameMap, v2.row, v2.col, nCol))
				{
					// ����ڵ�
					Vertex vx1 = { v1.row, nCol, BLANK };
					Vertex vx2 = { v2.row, nCol, BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
	for (int nRow = 0; nRow < CGameControl::s_nRows; nRow++)
		// �ҵ�һ���� X ��ƽ�е���ͨ�߶�
		if (pGameMap[nRow][v1.col] == BLANK && pGameMap[nRow][v2.col] == BLANK)
			if (LineX(pGameMap, nRow, v1.col, v2.col))
				if (LineY(pGameMap, nRow, v1.row, v1.col) && 
					LineY(pGameMap, nRow, v2.row, v2.col))
				{
					// ����ڵ�
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