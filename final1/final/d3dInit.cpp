                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                //////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dinit.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates how to initialize Direct3D, how to use the book's framework
//       functions, and how to clear the screen to black.  Note that the Direct3D
//       initialization code is in the d3dUtility.h/.cpp files.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"

IDirect3DDevice9* Device = 0; 

const int Width = 640;
const int Height = 480;

IDirect3DVertexBuffer9* Triangle = 0;

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x; _y = y; _z = z; _c = c;
	}
	float _x, _y, _z;
	D3DCOLOR _c;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

bool Setup()
{
	Device -> CreateVertexBuffer(
		3 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&Triangle,
		0);

	Vertex* v;
	Triangle -> Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-2.0f, 0.0f, 0.0f,d3d::RED);
	v[1] = Vertex(0.0f, 2.0f, 0.0f,d3d::GREEN);
	v[2] = Vertex(2.0f, 0.0f, 0.0f,d3d::BLUE);

	Triangle -> Unlock();

	D3DXVECTOR3 position(0.0f, 0.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	Device -> SetTransform(D3DTS_VIEW, &V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device -> SetTransform(D3DTS_PROJECTION, &proj);
	
	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		Device->BeginScene();
		Device->SetStreamSource(0,Triangle,0,sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		D3DXMATRIX w1;
		D3DXMatrixTranslation(&w1,-5,0,0);
		Device->SetTransform(D3DTS_WORLD,&w1);
		Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);

		Device->SetRenderState(D3DRS_LIGHTING,false);
		Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

		D3DXMATRIX w2;
		D3DXMatrixTranslation(&w2,0,0,0);
		Device->SetTransform(D3DTS_WORLD,&w2);
		Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);

		D3DXMATRIX w3;
		D3DXMatrixTranslation(&w3,5,0,0);
		Device->SetTransform(D3DTS_WORLD,&w3);
		Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);
		
		Device->SetRenderState(D3DRS_LIGHTING,true);


		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		640, 480, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                