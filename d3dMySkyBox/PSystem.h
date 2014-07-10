#pragma once

/*
 * ����ϵͳ���ṩ��ѩ����
 */

#include "d3dUtils.h"
#include "camera.h"
#include <list>

namespace psys
{
	// ���ӵĶ����ʽ
	struct Particle
	{
		D3DXVECTOR3 _position;
		D3DCOLOR    _color;
		static const DWORD FVF;
	};
	
	// ���ӵ�����
	struct Attribute
	{
		Attribute()
		{
			_lifeTime = 0.0f;
			_age      = 0.0f;
			_isAlive  = true;
		}

		D3DXVECTOR3 _position;     // λ��
		D3DXVECTOR3 _velocity;     // �ٶ�
		D3DXVECTOR3 _acceleration; // ���ٶ�
		float       _lifeTime;     // ���ӵ�����ʱ��  
		float       _age;          // ��ǰ������  
		D3DXCOLOR   _color;        // ��ɫ  
		D3DXCOLOR   _colorFade;    // ��ɫ��ʱ��ı仯
		bool        _isAlive;    
	};


	class CPSystem
	{
	public:
		CPSystem();
		virtual ~CPSystem();

		virtual bool init(IDirect3DDevice9* device, TCHAR* texFileName);

		virtual void reset();	// ��������ϵͳ���÷����������������ӵ�����
		virtual void resetParticle(Attribute* attribute) = 0;	// �����������ԣ�ͨ������������ʱ������
		virtual void addParticle();	// ����һ������

		virtual void update(float timeDelta) = 0;

		virtual void preRender();
		virtual void Render();
		virtual void postRender();

		bool isEmpty();
		bool isDead();

	protected:
		virtual void removeDeadParticles();

	protected:
		IDirect3DDevice9*       _device;
		D3DXVECTOR3             _origin;
		d3d::BoundingBox        _boundingBox;// ���ӵĻ��Χ��������Χ��ɱ��
		float                   _emitRate;   // rate new particles are added to system
		float                   _size;       // size of particles
		IDirect3DTexture9*      _tex;
		IDirect3DVertexBuffer9* _vb;
		std::list<Attribute>    _particles;
		int                     _maxParticles; // max allowed particles system can have


		// Following three data elements used for rendering the p-system efficiently
		DWORD _vbSize;      // size of vb
		DWORD _vbOffset;    // offset in vb to lock   
		DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset
	};

	class CSnow : public CPSystem
	{
	public:
		CSnow(d3d::BoundingBox* boundingBox, int numParticles);
		void resetParticle(Attribute* attribute);
		void update(float timeDelta);
	};
}