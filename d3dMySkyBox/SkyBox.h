#pragma once
/*
 * ��պ��࣬������պеĴ���
 */

#include "stdafx.h"
#include "D3DUtils.h"

class CSkyBox
{
public:
    CSkyBox(IDirect3DDevice9* pDevice);
    ~CSkyBox(void);

    bool InitSkyBox(int scale);

    void Render();
    bool SetTexture(const TCHAR* textureFile, int flag);

private:
    IDirect3DDevice9*       _d3dDevice;
    IDirect3DVertexBuffer9* _vb;
    IDirect3DIndexBuffer9*  _ib;
    IDirect3DTexture9*      _texture[5];	// ����������
};


