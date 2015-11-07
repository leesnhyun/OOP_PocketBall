#include "d3dUtility.h"
#include "CBorder.h"

// �׵θ��� ������
CBorder::CBorder()
{
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	mesh = NULL;
}

// �׵θ��� �Ҹ���
CBorder::~CBorder()
{
	d3d::Release<ID3DXMesh*>(mesh);

	for (int i = 0; i < Textures.size(); i++)
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
}

bool CBorder::create(IDirect3DDevice9* pDevice, float ix, float iz, float idepth, D3DXCOLOR color)
//�׵θ��� ȭ�鿡 ������
{
	if (NULL == pDevice) return false;

	// �޽��� �����ϱ� ���� adj���ۿ� material���۸� ����.
	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	// t.x ������ �ҷ��� �޽��� ����ϴ�.
	hr = D3DXLoadMeshFromX(
		"t.x", D3DXMESH_MANAGED, pDevice,
		&adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);

	// ����ó��
	if (FAILED(hr)){
		::MessageBox(0, "X file load fail", 0, 0);
		return false;
	}

	// x���Ϸκ��� ���͸���(material)�� �ؽ���(texture)�� �����մϴ�.
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			// DX�� .X���Ϸκ��� �޽� ������ �����ö� ambient value�� ������ ���� �����Ƿ�, 
			// ���⼭ ������ ���ش�.
			// ������ OOP���ÿ����� �ؽ��� ��������� �ʿ䰡 ��� ���ڷ� �Ѿ�� color������ ����.
			mtrls[i].MatD3D.Ambient = color;
			mtrls[i].MatD3D.Diffuse = color;
			mtrls[i].MatD3D.Specular = color;
			mtrls[i].MatD3D.Emissive = d3d::BLACK;

			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// i��° material�� �����մϴ�.
			Mtrls.push_back(mtrls[i].MatD3D);

			// .x���Ϸκ��� �ؽ��������� �����ɴϴ�.
			/*if (mtrls[i].pTextureFilename != 0)
			{
				// i��° subset���� ����
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(pDevice, mtrls[i].pTextureFilename,&tex);

				// �ε��� �ؽ��� ����
				Textures.push_back(tex);
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back(0);
			}*/

		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // material ���� ���� �Ϸ�

	// �޽��� ����ȭ �մϴ�.
	hr = mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	d3d::Release<ID3DXBuffer*>(adjBuffer); // adjacent ���� ���� �Ϸ�

	if (FAILED(hr))
	{
		::MessageBox(0, "Optimize failed", 0, 0);
		return false;
	}

	//::MessageBox(0, "�����Ϸ�", 0, 0); //������ �޼��� �ڽ����

	return true;
}

// ���� ȭ�鿡�� �Ҹ��Ŵ
void CBorder::destroy()
{
	if (mesh != NULL) {
		mesh->Release();
		mesh = NULL;
	}
}

// �׵θ��� ȭ�鿡 �׷���
void CBorder::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice) return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);

	for (int i = 0; i < Mtrls.size(); i++)
	{
		pDevice->SetMaterial(&Mtrls[i]);
		//pDevice->SetTexture(0, Textures[i]);		// ���ÿ����� �ؽ��ı��� �׸��� �ʽ��ϴ�.
		mesh->DrawSubset(i);
	}
}


// �׵θ��� ��ġ�� �ٲ�
void CBorder::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->m_x = x;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

void CBorder::setLocalTransform(const D3DXMATRIX& mLocal)
{
	m_mLocal = mLocal;
}