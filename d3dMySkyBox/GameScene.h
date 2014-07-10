#pragma once
/*
 * ��Ϸ�����࣬�������������Ĵ����ͻ��ƣ���ɫ������
 */

#include "stdafx.h"
#include "D3DUtils.h"
#include "D3DTransform.h"
#include "Camera.h"
#include "SkyBox.h"
#include "PSystem.h"
#include "Terrain.h"
#include "MeshX.h"

#include "ShadowVolume.h"


class CGameScene
{
public:
	CGameScene(void);
	~CGameScene(void);

	bool InitGameScene(HWND hwnd);
	bool InitFont();
	bool CreateSkyBox();
	bool CreateCamera();
	bool CreateTerrain();
	bool CreateParticle();
	bool CreateMesh();
	void CreateLight();

	void SetWindow(UINT width, UINT height, bool windowed);

	void Display(float timeDelta);

private:
	// functions
	void FrameUpdate();
	void FrameRender();
	bool RenderScene();
	void RenderParticle();

	void GetMouseInput();

	void SetFPVPosition(D3DXVECTOR3* pos);
	void GetFPVPosition(D3DXVECTOR3* pos);
	void SetFPVFaceDir(D3DXVECTOR3* dir);

	void UpdateCameraView();
	void AdjustCameraPos(float h);

	void UpdatePartical(float deltaTime);

	bool CheckBound(D3DXVECTOR3* pos, D3DXVECTOR3* dir);	// �߽���

	void CountFPS(float deltaTime);

	void ShowMode();	// ��ʾģʽ
	void ShowHelp();	// ��ʾ����

	// member values
	// ���ڳߴ�
	UINT _windowWidth;
	UINT _windowHeight;
	bool _windowed;
	
	// ��Чѡ��
	bool _hasShadow;
	bool _hasParticle;

	// ����Ԫ��
	CD3DDevice*				_d3dDevice;
	CD3DTransform*			_d3dTransform;
	CSkyBox*				_skybox;
	CTerrain*		        _terrain;
	psys::CPSystem*			_particle;
	CCamera*				_camera;
	CCamera*				_lastModeCamera;

	// �������
	std::vector<CMeshX*>	_mesh;

	// ����Ԫ��(�����н���һ��ƽ�й��ambient light)
	D3DLIGHT9   _dirLight;
	D3DXCOLOR   _ambientLight[5];
	UINT		_indexALight;

	// �����ӽǣ���������ѩ�ˣ�
	CMeshX*		_FPVman;
	D3DXMATRIX  _FPVMatrix; // �任����
	D3DXVECTOR3 _FVPpos;
	D3DXVECTOR3 _faceDir; //�沿����
	float		_speed;	  //�����ٶ�
	float	    _angle;	  //��ת�Ƕ�
	float       _ahead;	  //ѩ����������ľ���
	
	bool  _isMouse;		  //�Ƿ��������
	POINT _lastMousePos;
	POINT _currentMousePos;

	bool _isMoving;

	// �����ģʽ(��һ�ӽ�����or�����ӽ�)
	bool _mode;

	// �߽�
	float _maxy;
	float _miny;
	float _maxx;
	float _minx;
	float _maxz;
	float _minz;

	// ����
	ID3DXFont*		_Font;
	D3DXFONT_DESC	_lf;
	TCHAR			_outputStr[256];

	// FPS
	DWORD _FPS;

	// CShadowVolume��
	CShadowVolume*  _shadowVolume;
};

