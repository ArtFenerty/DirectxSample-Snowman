#pragma once

/*
 * d3d�����࣬������εĳ�ʼ������Ⱦ�������м����˵��εķ���
 */

#include "d3dUtils.h"
#include <string>
#include <vector>

class CTerrain
{
public:
	CTerrain(
		IDirect3DDevice9* device,
		std::string heightmapFileName,	// �߶�ͼ�ļ�
		int numVertsPerRow,				// �ж������
		int numVertsPerCol,				// �ж������
		int cellSpacing,				// ��λ����Ĵ�С
		float heightScale				// �߶ȱ���ϵ��
		);   

	~CTerrain();

	int  getHeightmapEntry(int row, int col);
	void setHeightmapEntry(int row, int col, int value);

	float getHeight(float x, float z);

	bool  loadTexture(TCHAR* fileName);
	//bool  genTexture(D3DXVECTOR3* directionToLight);
	bool  Render();

private:
	IDirect3DDevice9*       _device;
	IDirect3DTexture9*      _tex;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9*  _ib;

	int _numVertsPerRow;
	int _numVertsPerCol;
	int _cellSpacing;

	int _numCellsPerRow;		// �е�Ԫ�����
	int _numCellsPerCol;		// �е�Ԫ�����
	int _width;				
	int _depth;
	int _numVertices;
	int _numTriangles;

	float _heightScale;

	std::vector<int> _heightmap;

	// helper methods
	bool readRawFile(std::string fileName);
	bool computeVertices();
	bool computeIndices();
	bool computeNormals();
	//bool  lightTerrain(D3DXVECTOR3* directionToLight);
	//float computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);
};