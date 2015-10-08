#include <cstdlib>
#include "pSystem.h"

using namespace psys;

const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

CPSystem::CPSystem()
{
    _device = 0;
    _vb = 0;
    _tex = 0;
}

CPSystem::~CPSystem()
{
    d3d::Release<IDirect3DVertexBuffer9*>(_vb);
    d3d::Release<IDirect3DTexture9*>(_tex);
}

bool CPSystem::init(IDirect3DDevice9* device, TCHAR* texFileName)
{
    _device = device;

    HRESULT hr = 0;
    hr = device->CreateVertexBuffer(
        _vbSize * sizeof(Particle),
        D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
        Particle::FVF,
        D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC 
        &_vb,
        0);

    if (FAILED(hr))
    {
        ::MessageBox(0, _T("CreateVertexBuffer() - FAILED"), _T("PSystem"), 0);
        return false;
    }

    hr = D3DXCreateTextureFromFile(
        device,
        texFileName,
        &_tex);

    if (FAILED(hr))
    {
        ::MessageBox(0, _T("D3DXCreateTextureFromFile() - FAILED"), _T("PSystem"), 0);
        return false;
    }

    return true;
}

void CPSystem::reset()
{
    std::list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        resetParticle(&(*i));
    }
}

void CPSystem::addParticle()
{
    Attribute attribute;

    resetParticle(&attribute);

    _particles.push_back(attribute);
}

void CPSystem::preRender()
{
    _device->SetRenderState(D3DRS_LIGHTING, false);
    _device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
    _device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
    _device->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw(_size));
    _device->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0.0f));

    // control the size of the particle relative to distance
    _device->SetRenderState(D3DRS_POINTSCALE_A, d3d::FtoDw(0.0f));
    _device->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
    _device->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));

    // use alpha from texture
    _device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    _device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    _device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CPSystem::postRender()
{
    _device->SetRenderState(D3DRS_LIGHTING, true);
    _device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    _device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
    _device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CPSystem::Render()
{
    // ���÷�Ƭ��Ⱦ�ķ�ʽ�����㻺���Ϊ���Ƭ��ÿ�ο�����Ҫ��Ⱦ�����ݵ�һ��Ƭ�У�
    // ��Ⱦ��Ƭ��ͬʱ��������һ��Ƭ�ε����ݣ����ַ�ʽ����ھ��˲����ԣ�ʹ��GPU��CPU���еع���

    if (!_particles.empty())
    {
        // ������ص���Ⱦģʽ
        preRender();

        _device->SetTexture(0, _tex);
        _device->SetFVF(Particle::FVF);
        _device->SetStreamSource(0, _vb, 0, sizeof(Particle));

        // render batches one by one
        // start at beginning if we're at the end of the vb
        if (_vbOffset >= _vbSize)
            _vbOffset = 0;

        Particle* v = 0;

        _vb->Lock(
            _vbOffset    * sizeof(Particle),
            _vbBatchSize * sizeof(Particle),
            (void**)&v,
            _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

        DWORD numParticlesInBatch = 0;

        // Until all particles have been rendered.
        std::list<Attribute>::iterator i;
        for (i = _particles.begin(); i != _particles.end(); i++)
        {
            // �������л�Ծ��������ݵ�����
            if (i->_isAlive)
            {
                // Copy a batch of the living particles to the
                // next vertex buffer segment
                v->_position = i->_position;
                v->_color = (D3DCOLOR)i->_color;
                v++; // next element;

                numParticlesInBatch++; //increase batch counter

                // if this batch full?�����һ��Ƭ�����˾ͻ��ƣ�
                if (numParticlesInBatch == _vbBatchSize)
                {
                    // Draw the last batch of particles that was
                    // copied to the vertex buffer. 
                    _vb->Unlock();

                    _device->DrawPrimitive(
                        D3DPT_POINTLIST,
                        _vbOffset,
                        _vbBatchSize);

                    // While that batch is drawing, start filling the
                    // next batch with particles.

                    // move the offset to the start of the next batch(��ʼ������һ��Ƭ�ε�����)
                    _vbOffset += _vbBatchSize;

                    // don't offset into memory thats outside the vb's range.
                    // If we're at the end, start at the beginning.
                    if (_vbOffset >= _vbSize)
                        _vbOffset = 0;

                    _vb->Lock(
                        _vbOffset    * sizeof(Particle),
                        _vbBatchSize * sizeof(Particle),
                        (void**)&v,
                        _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

                    numParticlesInBatch = 0; // reset for new batch
                }
            }
        }

        _vb->Unlock();

        // its possible that the LAST batch being filled never 
        // got rendered because the condition 
        // (numParticlesInBatch == _vbBatchSize) would not have
        // been satisfied.  We draw the last partially filled batch now.
        // �������һ��δ����Ƭ�Σ�����У�
        if (numParticlesInBatch)
        {
            _device->DrawPrimitive(
                D3DPT_POINTLIST,
                _vbOffset,
                numParticlesInBatch);
        }

        // next block
        _vbOffset += _vbBatchSize;

        // reset render states
        postRender();
    }
}

bool CPSystem::isEmpty()
{
    return _particles.empty();
}

bool CPSystem::isDead()
{
    std::list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        // is there at least one living particle?  If yes,
        // the system is not dead.
        if (i->_isAlive)
            return false;
    }
    // no living particles found, the system must be dead.
    return true;
}

void CPSystem::removeDeadParticles()
{
    std::list<Attribute>::iterator i;

    i = _particles.begin();

    while (i != _particles.end())
    {
        if (i->_isAlive == false)
        {
            // erase returns the next iterator, so no need to
            // incrememnt to the next one ourselves.
            i = _particles.erase(i);
        }
        else
        {
            i++; // next in list
        }
    }
}

//*****************************************************************************
// Snow System
//***************

CSnow::CSnow(d3d::BoundingBox* boundingBox, int numParticles)
{
    _boundingBox = *boundingBox;
    _size = 0.25f;
    _vbSize = 2048;			// vertex buffer�Ĵ�С
    _vbOffset = 0;
    _vbBatchSize = 512;			// ÿһ��Ƭ�εĴ�СΪ512

    for (int i = 0; i < numParticles; i++)
        addParticle();
}

void CSnow::resetParticle(Attribute* attribute)
{
    attribute->_isAlive = true;

    // ��boundingbox��������ɳ�ʼ����
    d3d::GetRandomVector(
        &attribute->_position,
        &_boundingBox._min,
        &_boundingBox._max);

    // ���ǴӶ�������
    attribute->_position.y = _boundingBox._max.y;

    // ���ģ��һ���ٶ�ƫ��
    attribute->_velocity.x = d3d::GetRandomFloat(0.0f, 1.0f) * -3.0f;
    attribute->_velocity.y = d3d::GetRandomFloat(0.0f, 1.0f) * -10.0f;
    attribute->_velocity.z = 0.0f;

    // white snow flake
    attribute->_color = d3d::WHITE;
}

void CSnow::update(float timeDelta)
{
    std::list<Attribute>::iterator i;
    for (i = _particles.begin(); i != _particles.end(); i++)
    {
        i->_position += i->_velocity * timeDelta;

        // �����Ƿ���ָ����Χ��
        if (_boundingBox.isPointInside(i->_position) == false)
        {
            // ������Щ����������
            resetParticle(&(*i));
        }
    }
}
