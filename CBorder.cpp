#include "d3dUtility.h"
#include "CBorder.h"

// 벽의 생성자
CBorder::CBorder()
{
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	mesh = NULL;
}

// 벽의 소멸자
CBorder::~CBorder()
{
	d3d::Release<ID3DXMesh*>(mesh);

	for (int i = 0; i < Textures.size(); i++)
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
}

bool CBorder::create(IDirect3DDevice9* pDevice, float ix, float iz, float idepth, D3DXCOLOR color)
//벽을 화면에 생성함
{
	if (NULL == pDevice) return false;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;

	//if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
	//	return false;

	// MESH
	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	hr = D3DXLoadMeshFromX(
		"t.x", D3DXMESH_MANAGED, pDevice,
		&adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);

	if (FAILED(hr)){
		::MessageBox(0, "X file load fail", 0, 0);
		return false;
	}

	// Extract the materials, and load textures.
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = color;
			mtrls[i].MatD3D.Diffuse = color;
			mtrls[i].MatD3D.Specular = color;
			mtrls[i].MatD3D.Emissive = d3d::BLACK;

			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back(mtrls[i].MatD3D);

			// check if the ith material has an associative texture
			/*if (mtrls[i].pTextureFilename != 0)
			{
			// yes, load the texture for the ith subset
			IDirect3DTexture9* tex = 0;
			D3DXCreateTextureFromFile(
			pDevice,
			mtrls[i].pTextureFilename,
			&tex);

			// save the loaded texture
			Textures.push_back(tex);
			}
			else
			{
			// no texture for the ith subset
			Textures.push_back(0);
			}*/

		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	// Optimize the mesh.
	hr = mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	d3d::Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if (FAILED(hr))
	{
		::MessageBox(0, "Optimize failed", 0, 0);
		return false;
	}

	::MessageBox(0, "생성완료", 0, 0);

	return true;
}

// 벽을 화면에서 소멸시킴
void CBorder::destroy()
{
	if (mesh != NULL) {
		mesh->Release();
		mesh = NULL;
	}
}

// 테두리를 화면에 그려냄
void CBorder::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice) return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);

	for (int i = 0; i < Mtrls.size(); i++)
	{
		pDevice->SetMaterial(&Mtrls[i]);
		//pDevice->SetTexture(0, Textures[i]);
		mesh->DrawSubset(i);
	}

	//pDevice->SetMaterial(&m_mtrl);
	mesh->DrawSubset(0);
}


// 테두리의 위치를 바꿈
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