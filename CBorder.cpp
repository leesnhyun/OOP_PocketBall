#include "d3dUtility.h"
#include "CBorder.h"

// �׵θ��� ������
CBorder::CBorder(D3DXCOLOR color){

	D3DXMatrixIdentity(&mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	mesh = nullptr;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;
}

// �׵θ��� �Ҹ���
CBorder::~CBorder()
{
	d3d::Release<ID3DXMesh*>(mesh);

	for (int i = 0; i < Textures.size(); i++){
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
	}
}

bool CBorder::create(IDirect3DDevice9* pDevice){

	if (pDevice == nullptr) return false;

	// �޽��� �����ϱ� ���� adj���ۿ� material���۸� ����.
	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	// t.x ������ �ҷ��� �޽��� ����ϴ�.
	hr = D3DXLoadMeshFromX(
		"./res/t.x", D3DXMESH_MANAGED, pDevice,
		&adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);

	// ����ó��
	if (FAILED(hr)){
		::MessageBox(0, "X file load fail :: Please Read README.txt", 0, 0);
		return false;
	}

	// x���Ϸκ��� ���͸���(material)�� �ؽ���(texture)�� �����մϴ�.
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = static_cast<D3DXMATERIAL*>(mtrlBuffer->GetBufferPointer());

		for (int i = 0; i < numMtrls; i++)
		{
			// DX�� .X���Ϸκ��� �޽� ������ �����ö� ambient value�� ������ ���� �����Ƿ�, 
			// ���⼭ ������ ���ش�.
			// ������ OOP���ÿ����� �ؽ��� ��������� �ʿ䰡 ��� ���ڷ� �Ѿ�� color������ ����.
			mtrls[i].MatD3D.Ambient = d3d::BLACK;
			mtrls[i].MatD3D.Diffuse = d3d::BLACK;
			mtrls[i].MatD3D.Specular = d3d::BLACK;
			mtrls[i].MatD3D.Emissive = d3d::BLACK;

			//mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// i��° material�� �����մϴ�.
			Mtrls.push_back(mtrls[i].MatD3D);

			// .x���Ϸκ��� �ؽ��������� �����ɴϴ�.
			if (mtrls[i].pTextureFilename != 0)
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
			}

		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // material ���� ���� �Ϸ�

	// �޽��� ����ȭ �մϴ�.
	hr = mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		static_cast<DWORD*>(adjBuffer->GetBufferPointer()),
		nullptr, nullptr, nullptr);

	d3d::Release<ID3DXBuffer*>(adjBuffer); // adjacent ���� ���� �Ϸ�

	if (FAILED(hr))
	{
		::MessageBox(nullptr, "Optimize failed", nullptr, 0);
		return false;
	}

	//::MessageBox(0, "�����Ϸ�", 0, 0); //������ �޼��� �ڽ����

	return true;
}

// ���� ȭ�鿡�� �Ҹ��Ŵ
void CBorder::destroy()
{
	if (mesh != nullptr) {
		mesh->Release();
		mesh = nullptr;
	}
}

// �׵θ��� ȭ�鿡 �׷���
void CBorder::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (pDevice == nullptr) return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);

	for (int i = 0; i < Mtrls.size(); i++)
	{
		pDevice->SetMaterial(&d3d::WOOD_MTRL);
		pDevice->SetTexture(0, Textures[i]);		// ���ÿ����� �ؽ��ı��� �׸��� �ʽ��ϴ�.
		mesh->DrawSubset(i);
	}
}


// �׵θ��� ��ġ�� �ٲ�
void CBorder::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->center_x = x;
	this->center_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}