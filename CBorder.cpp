#include "d3dUtility.h"
#include "CBorder.h"

// 테두리의 생성자
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

// 테두리의 소멸자
CBorder::~CBorder()
{
	d3d::Release<ID3DXMesh*>(mesh);

	for (int i = 0; i < Textures.size(); i++){
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
	}
}

bool CBorder::create(IDirect3DDevice9* pDevice){

	if (pDevice == nullptr) return false;

	// 메쉬를 생성하기 위해 adj버퍼와 material버퍼를 만듦.
	HRESULT hr = 0;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	// t.x 파일을 불러와 메쉬를 만듭니다.
	hr = D3DXLoadMeshFromX(
		"./res/t.x", D3DXMESH_MANAGED, pDevice,
		&adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);

	// 예외처리
	if (FAILED(hr)){
		::MessageBox(0, "X file load fail :: Please Read README.txt", 0, 0);
		return false;
	}

	// x파일로부터 메터리얼(material)과 텍스쳐(texture)를 추출합니다.
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = static_cast<D3DXMATERIAL*>(mtrlBuffer->GetBufferPointer());

		for (int i = 0; i < numMtrls; i++)
		{
			// DX가 .X파일로부터 메쉬 정보를 가져올때 ambient value를 가지고 있지 않으므로, 
			// 여기서 적용을 해준다.
			// 하지만 OOP팀플에서는 텍스쳐 적용까지는 필요가 없어서 인자로 넘어온 color값으로 고정.
			mtrls[i].MatD3D.Ambient = d3d::BLACK;
			mtrls[i].MatD3D.Diffuse = d3d::BLACK;
			mtrls[i].MatD3D.Specular = d3d::BLACK;
			mtrls[i].MatD3D.Emissive = d3d::BLACK;

			//mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// i번째 material을 저장합니다.
			Mtrls.push_back(mtrls[i].MatD3D);

			// .x파일로부터 텍스쳐정보를 가져옵니다.
			if (mtrls[i].pTextureFilename != 0)
			{
				// i번째 subset으로 저장
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(pDevice, mtrls[i].pTextureFilename,&tex);

				// 로딩된 텍스쳐 저장
				Textures.push_back(tex);
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back(0);
			}

		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // material 버퍼 쓰기 완료

	// 메쉬를 최적화 합니다.
	hr = mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		static_cast<DWORD*>(adjBuffer->GetBufferPointer()),
		nullptr, nullptr, nullptr);

	d3d::Release<ID3DXBuffer*>(adjBuffer); // adjacent 버퍼 쓰기 완료

	if (FAILED(hr))
	{
		::MessageBox(nullptr, "Optimize failed", nullptr, 0);
		return false;
	}

	//::MessageBox(0, "생성완료", 0, 0); //디버깅용 메세지 박스출력

	return true;
}

// 벽을 화면에서 소멸시킴
void CBorder::destroy()
{
	if (mesh != nullptr) {
		mesh->Release();
		mesh = nullptr;
	}
}

// 테두리를 화면에 그려냄
void CBorder::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (pDevice == nullptr) return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &mLocal);

	for (int i = 0; i < Mtrls.size(); i++)
	{
		pDevice->SetMaterial(&d3d::WOOD_MTRL);
		pDevice->SetTexture(0, Textures[i]);		// 팀플에서는 텍스쳐까지 그리지 않습니다.
		mesh->DrawSubset(i);
	}
}


// 테두리의 위치를 바꿈
void CBorder::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->center_x = x;
	this->center_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}