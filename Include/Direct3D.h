#ifndef DIRECT3D_H
#define DIRECT3D_H

#include "std.h"
#include "Object.h"
#include <d3dx9.h>

SE_BEGIN_NAMESPACE

class Direct3D {
public:
	Direct3D(HWND hWnd);
	~Direct3D();
	void Update(float delta);
	void Render();
private:
	Object m_airplane;
	Object m_tiger;
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_device;
};

SE_END_NAMESPACE

#endif