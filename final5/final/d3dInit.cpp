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

IDirect3DVertexBuffer9* VB = 0;

IDirect3DTexture9* WallTex = 0;
IDirect3DTexture9* MirrorTex = 0;

D3DMATERIAL9 WallMtrl = d3d::WHITE_MTRL;
D3DMATERIAL9 MirrorMtrl = d3d::WHITE_MTRL;

ID3DXMesh* Torus = 0;
D3DMATERIAL9 TorusMtrl = d3d::MAGENTA_MTRL;

float Rx = 0.0f;
float Ry = 0.0f;

D3DXVECTOR3 TorusPos(0.0f, 0.0f, -7.0f);

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

bool Setup()
{
	WallMtrl.Specular = d3d::WHITE * 0.5f;
	MirrorMtrl.Specular = d3d::WHITE * 0.1f;

	D3DXCreateTorus(Device, 0.5f,1.5f,30,30,&Torus,0);

	Device->CreateVertexBuffer(
		54 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v = 0;
	VB->Lock(0, 0, (void**)&v, 0);

	// wall
	v[0] = Vertex(-18.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-18.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex( -6.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-18.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex( -6.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex( -6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//左上

	v[6] = Vertex(-6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7] = Vertex(-6.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex( 6.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9] = Vertex(-6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(6.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//中上

	v[12] = Vertex(6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(6.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(18.0f, 15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = Vertex( 6.0f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(18.0f,15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(18.0f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//右上

	v[18] = Vertex(-18.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-18.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex( -6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-18.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex( -6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex( -6.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//左中

	v[24] = Vertex( 6.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[25] = Vertex( 6.0f,   5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[26] = Vertex( 18.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[27] = Vertex( 6.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[28] = Vertex( 18.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[29] = Vertex( 18.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//右中

	v[30] = Vertex(-18.0f, -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[31] = Vertex(-18.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[32] = Vertex( -6.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[33] = Vertex(-18.0f, -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[34] = Vertex( -6.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[35] = Vertex( -6.0f, -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//左下

	v[36] = Vertex(-6.0f, -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[37] = Vertex(-6.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[38] = Vertex( 6.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[39] = Vertex(-6.0f, -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[40] = Vertex( 6.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[41] = Vertex( 6.0f, -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//中下

	v[42] = Vertex(6.0f,  -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[43] = Vertex(6.0f,   -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[44] = Vertex(18.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[45] = Vertex(6.0f,  -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[46] = Vertex(18.0f,  -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[47] = Vertex(18.0f, -15.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//右下

	//mirror
	v[48] = Vertex(-6.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[49] = Vertex(-6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[50] = Vertex( 6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[51] = Vertex(-6.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[52] = Vertex( 6.0f,  5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[53] = Vertex( 6.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	VB->Unlock();


	D3DXCreateTextureFromFile(Device, "wall.jpg", &WallTex);
	D3DXCreateTextureFromFile(Device, "window4.jpg", &MirrorTex);

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &color);

	Device->SetLight(0, &light);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXVECTOR3 position(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 1.0f);

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
	d3d::Release<ID3DXMesh*>(Torus);
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DTexture9*>(WallTex);
	d3d::Release<IDirect3DTexture9*>(MirrorTex);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		Device->BeginScene();

		Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		//draw wall
		D3DXMATRIX W1;
		D3DXMatrixTranslation(&W1,0,0,0);
		Device->SetTransform(D3DTS_WORLD,&W1);
		Device->SetMaterial(&WallMtrl);
		Device->SetTexture(0, WallTex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 16);

		Device->SetRenderState(D3DRS_STENCILENABLE,true);
		Device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
		Device->SetRenderState(D3DRS_STENCILREF,0x1);
		Device->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);
		
		//draw window
		
		D3DXMATRIX W;
		D3DXMatrixTranslation(&W,0,0,0);
		Device->SetTransform(D3DTS_WORLD,&W);
		Device->SetMaterial(&MirrorMtrl);
		Device->SetTexture(0, MirrorTex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 48, 2);

		Device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
		Device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
		
		//draw torus
		D3DXMatrixTranslation(&W,TorusPos.x,TorusPos.y,TorusPos.z);
		
		D3DXMATRIX R,p;
		D3DXPLANE plane(0.0f,0.0f,1.0f,0.0f);
		D3DXMatrixReflect(&R,&plane);
		p = W * R;
		Device->SetTransform(D3DTS_WORLD,&p);
		Device->SetMaterial(&TorusMtrl);

		Device->Clear(0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
		Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
		Torus->DrawSubset(0);
		
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		Device->SetRenderState(D3DRS_STENCILENABLE,false);
		Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

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
		if ( wParam == VK_LEFT )
			TorusPos.x -= 0.1f;
			//Rx -= 0.1f;
		if ( wParam == VK_RIGHT )
			TorusPos.x += 0.1f;
			//Rx += 0.1f;
		if( wParam == VK_UP )
			TorusPos.y += 0.1f;
			//Ry += 0.1f;
		if( wParam == VK_DOWN )
			TorusPos.y -= 0.1f;
			//Ry -= 0.1f;

		if( wParam == 0x65 )//if ( ::GetAsyncKeyState('a') )
			Rx -= 0.1f;
		if( wParam == 0x68 )//if ( ::GetAsyncKeyState('d') )
			Rx += 0.1f;
		if( wParam == 0x87 )//if( ::GetAsyncKeyState('w') )
			Ry += 0.1f;
		if( wParam == 0x83 )//if( ::GetAsyncKeyState('s') )
			Ry -= 0.1f;
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