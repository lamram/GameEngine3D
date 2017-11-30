#include <Windows.h>
#include "Direct3D.h"
#include "AssetLoader.h"

SE_BEGIN_NAMESPACE

//#define VERTEX_FORMAT (D3DFVF_XYZ | D3DFVF_TEX1)

//struct Vertex {
//	float x, y, z;
//	float tu, tv;
//};

Direct3D::Direct3D(HWND hWnd) {
	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_device);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //D3DCULL_CCW
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//Vertex vertices[] = {
	//	{ -1.0f, 1.0f, 0.0f,	0.0f, 0.0f },		//Top-left
	//	{ 1.0f, 1.0f, 0.0f,		1.0f, 0.0f },		//Top-right
	//	{ -1.0f,-1.0f, 0.0f,	0.0f, 1.0f },		//Bottom-left
	//	{ 1.0f,-1.0f, 0.0f,		1.0f, 1.0f },		//Bottom-right
	//};

	//int vertCount = sizeof(vertices) / sizeof(Vertex);
	//int byteCount = vertCount * sizeof(Vertex);
	//if (FAILED(m_device->CreateVertexBuffer(byteCount, 0, VERTEX_FORMAT, D3DPOOL_MANAGED, &m_vBuffer, NULL))) {
	//	return;
	//}

	//VOID* pVertices;
	//m_vBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//memcpy(pVertices, vertices, byteCount);
	//m_vBuffer->Unlock();

	//
	//std::string src = "Assets\\texture.jpg";
	//D3DXCreateTextureFromFile(m_device, src.c_str(), &m_texture);
	//m_device->SetTexture(0, m_texture);
	
	AssetLoader *a = AssetLoader::GetInstance();
	a->Init(m_device);
	a->AddMesh("airplane.x");
	a->AddMesh("tiger.x");

	m_airplane.Init("airplane.x");
	m_tiger.Init("tiger.x");

	m_airplane.ChangePosition(2.0f, 0.0f, -5.0f);
	m_tiger.ChangePosition(-2.0f, 0.0f, 0.0f);
}

Direct3D::~Direct3D() {
	//texture->Release();
	//m_vBuffer->Release();
	AssetLoader::GetInstance()->Clean("tiger.x");
	AssetLoader::GetInstance()->Clean("airplane.x");
	m_device->Release();
	m_d3d->Release();
}

void Direct3D::Update(float delta) {
	//World (object update)
	/*unsigned int iTime = (int)delta / 10 % 1000;
	D3DXMATRIX matRotate;
	D3DXMATRIX matTranslate;
	D3DXMatrixRotationY(&matRotate, iTime * (2.0f * D3DX_PI) / 1000.0f);
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, 0.0f);
	m_device->SetTransform(D3DTS_WORLD, &(matRotate * matTranslate));*/

	
	//View (camera)
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 0.0f, 10.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_device->SetTransform(D3DTS_VIEW, &matView);

	//Projection
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	m_device->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Direct3D::Render() {
	m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	m_device->BeginScene();
	
	//m_device->SetFVF(VERTEX_FORMAT);
	//m_device->SetStreamSource(0, m_vBuffer, 0, sizeof(Vertex));
	//m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_airplane.Update(m_device);
	m_airplane.Render();
	m_tiger.Update(m_device);
	m_tiger.Render();

	m_device->EndScene();
	m_device->Present(NULL, NULL, NULL, NULL);
}

SE_END_NAMESPACE