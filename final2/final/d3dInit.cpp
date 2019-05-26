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

ID3DXMesh* sphere = 0;

IDirect3DVertexBuffer9* VB  = 0;

D3DMATERIAL9 mtrl1 = d3d::RED_MTRL;
D3DMATERIAL9 mtrl2 = d3d::BLUE_MTRL;

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x; _y = y; _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;

bool Setup()
{
	D3DXCreateSphere(Device,0.5f,20,20,&sphere,0);

	Device -> CreateVertexBuffer(
		12 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v;
	VB  -> Lock(0, 0, (void**)&v, 0);
	v[0] = Vertex(-1.0f,0.0f,-1.0f);
	v[1] = Vertex(0.0f,1.0f,0.0f);
	v[2] = Vertex(1.0f, 0.0f, -1.0f);

	v[3] = Vertex(1.0f,0.0f,-1.0f);
	v[4] = Vertex(0.0f, 1.0f, 0.0f);
	v[5] = Vertex(0.0f,0.0f,1.0f);

	v[6] = Vertex(-1.0f,0.0f,-1.0f);
	v[7] = Vertex(0.0f, 0.0f, 1.0f);
	v[8] = Vertex(0.0f, 1.0f, 0.0f);

	v[9] = Vertex(-1.0f, 0.0f, -1.0f);
	v[10] = Vertex(1.0f,0.0f,-1.0f);
	v[11] = Vertex(0.0f, 0.0f, 1.0f);
	VB  -> Unlock();

	D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &color);

	Device->SetLight(0, &light);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
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

	
	Device->SetRenderState(D3DRS_LIGHTING,true);
	
	
	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<ID3DXMesh*>(sphere);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		Device->BeginScene();

		Device->SetStreamSource(0,VB,0,sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		D3DXMATRIX Tz1,Tz2,Ry;
		D3DXMatrixTranslation(&Tz1,0.0f,-1.0f,0.0f);
		Device->SetTransform(D3DTS_WORLD,&Tz1);
		Device->SetMaterial(&mtrl1);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);
		
		D3DXMatrixTranslation(&Tz2,0.0f,0.5f,0.0f);
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry,y);
		y+=timeDelta;
		D3DXMATRIX p = Tz2 * Ry;
		Device->SetTransform(D3DTS_WORLD,&p);
		Device->SetMaterial(&mtrl2);
		sphere->DrawSubset(0);


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