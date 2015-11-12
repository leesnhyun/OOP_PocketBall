#ifndef _COBJECT_
#define _COBJECT_
#include "d3dUtility.h"

class CObject
{
protected:
	float center_x, center_y, center_z;
	D3DXMATRIX mLocal;

public:
	virtual bool create(IDirect3DDevice9* pDevice) = 0;
	virtual void destroy() = 0;
	virtual void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld) = 0;

	void setPosition(float x, float y, float z);
	void setLocalTransform(const D3DXMATRIX& mLocal);

	D3DXVECTOR3 getPosition() const;
	D3DXMATRIX getLocalTransform() const;

	virtual ~CObject();
};

#endif

