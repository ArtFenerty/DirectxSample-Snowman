#pragma once
/*
 * ������࣬������������������ƽ�ƣ���ת�Ȳ���
 */
#include "stdafx.h"

class CCamera
{
public:
    enum CameraType { LANDOBJECT, AIRCRAFT };

    CCamera(void);
    CCamera(CameraType cameraType);
    ~CCamera(void);

    void strafe(float units); // right��ƽ��
    void fly(float units);    // up��ƽ��
    void walk(float units);   // look��ƽ��

    void pitch(float angle); // ��right����ת
    void yaw(float angle);   // ��up����ת
    void roll(float angle);  // ��look����ת

    void rotation(D3DXVECTOR3* axis, float angle); // ����������ת

    void getViewMatrix(D3DXMATRIX* V);
    void setCameraType(CameraType cameraType);
    //void getCameraType(CameraType* cameraType);
    void getPosition(D3DXVECTOR3* pos);
    void setPosition(D3DXVECTOR3* pos);
    void setPosition(float x, float y, float z);

    void getRight(D3DXVECTOR3* right);
    void getUp(D3DXVECTOR3* up);
    void getLook(D3DXVECTOR3* look);

private:
    CameraType	_cameraType;
    D3DXVECTOR3	_right;
    D3DXVECTOR3	_up;
    D3DXVECTOR3	_look;
    D3DXVECTOR3	_pos;
};