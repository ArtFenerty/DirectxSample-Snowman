#pragma once
/* 
 * D3D�豸�࣬���𱣴�D3D�豸ָ�룬�������󱸻����С�Լ�D3D�ĳ�ʼ��
 */

#include "stdafx.h"
#include "D3DUtils.h"

class CD3DDevice
{
public:
	CD3DDevice(HWND hWnd);
	~CD3DDevice(void);

	bool InitD3D(/*HWND hwnd, UINT backWidth, UINT backHeight, bool windowed*/);
	void SetDeviceType(D3DDEVTYPE deviceType);
	void SetD3DWindow(UINT backWidth, UINT backHeight, bool windowed);
	void GetD3DRect(RECT* rect);

	IDirect3DDevice9* GetDevicePoint();

private:
	IDirect3D9*				_d3d9;
	IDirect3DDevice9*		_d3dDevice9;
	D3DPRESENT_PARAMETERS	_d3dpp;
	D3DDEVTYPE				_d3dDeviceType;

	HWND _hWnd;
	UINT _backWidth;
	UINT _backHeight;
	bool _windowed;
};

