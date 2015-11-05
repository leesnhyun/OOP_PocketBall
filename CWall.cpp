#include "d3dUtility.h"
#include "CWall.h"

// ���� ������
CWall::CWall()
{
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_width = 0;
	m_depth = 0;
	m_pBoundMesh = NULL;
}

// ���� �Ҹ���
CWall::~CWall()
{

}

bool CWall::create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color)
//���� ȭ�鿡 ������
{
	if (NULL == pDevice)
		return false;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;

	m_width = iwidth;
	m_depth = idepth;

	if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
		return false;
	return true;
}

// ���� ȭ�鿡�� �Ҹ��Ŵ
void CWall::destroy()
{
	if (m_pBoundMesh != NULL) {
		m_pBoundMesh->Release();
		m_pBoundMesh = NULL;
	}
}

// ���� ȭ�鿡 �׷���
void CWall::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice)
		return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pBoundMesh->DrawSubset(0);
}

// ���� ���� �浹 �ߴ��� Ȯ��
bool CWall::hasIntersected(CSphere& ball)
{
	// Insert your code here.
	return false;
}

// ���� ���� �浹�� ��� ���� ����� �ӵ��� �ٲ�
void CWall::hitBy(CSphere& ball)
{
	if (this->hasIntersected(ball))
	{

	}
	// Insert your code here.
}

// ���� ��ġ�� �ٲ�
void CWall::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->m_x = x;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

// ���� ���̸� ��ȯ
float CWall::getHeight(void) const
{
	return M_HEIGHT;
}

void CWall::setLocalTransform(const D3DXMATRIX& mLocal)
{
	m_mLocal = mLocal;
}