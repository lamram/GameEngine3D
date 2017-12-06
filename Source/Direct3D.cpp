#include <Windows.h>
#include "Direct3D.h"
#include "AssetLoader.h"
#include "SceneLoader.h"
#include "Terrain.h"
#include "Entity.h"

SE_BEGIN_NAMESPACE

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
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_device->SetRenderState(D3DRS_CLIPPING, TRUE); //By default true

	AssetLoader::GetInstance()->Init(m_device);
	AssetLoader::GetInstance()->AddMesh("airplane.x");
	AssetLoader::GetInstance()->AddMesh("tiger.x");

	Terrain *terrain = new Terrain(m_device);
	SceneLoader::GetInstance()->AddScene("heightmap");
	SceneLoader::GetInstance()->GetScene("heightmap")->AddEntity(terrain);

}

Direct3D::~Direct3D() {
	AssetLoader::GetInstance()->ReleaseMesh("tiger.x");
	AssetLoader::GetInstance()->ReleaseMesh("airplane.x");
	m_device->Release();
	m_d3d->Release();
}

void Direct3D::Update(float delta) {
	//World (object update)
	SceneLoader::GetInstance()->GetCurrentScene()->Update(delta);

	//View (camera)
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(m_camera.GetPosition().X, m_camera.GetPosition().Y, m_camera.GetPosition().Z),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_device->SetTransform(D3DTS_VIEW, &matView);

	//Projection
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 800 / 500, 1.0f, 100.0f);
	m_device->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Direct3D::Render() {
	m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	m_device->BeginScene();

	std::vector<Entity*> m_currentSceneObjects = SceneLoader::GetInstance()->GetCurrentScene()->GetEntities();
	for (int i = 0; i < m_currentSceneObjects.size(); i++) {
		Vector3<float> rotation = m_currentSceneObjects[i]->GetRotation();
		Vector3<float> position = m_currentSceneObjects[i]->GetPosition();
		D3DXMatrixRotationYawPitchRoll(&m_matRotate, rotation.X, rotation.Y, rotation.Z);
		D3DXMatrixTranslation(&m_matTranslate, position.X, position.Y, position.Z);
		m_device->SetTransform(D3DTS_WORLD, &(m_matRotate * m_matTranslate));
		if (m_currentSceneObjects[i]->GetFilename() != "") {
			Mesh m_currentMesh = AssetLoader::GetInstance()->GetMeshes()->at(m_currentSceneObjects[i]->GetFilename());
			m_currentMesh.Render();
		}
	}

	SceneLoader::GetInstance()->GetCurrentScene()->Render();

	m_device->EndScene();
	m_device->Present(NULL, NULL, NULL, NULL);
}

SE_END_NAMESPACE