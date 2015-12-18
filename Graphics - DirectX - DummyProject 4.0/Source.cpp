#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>

#include "d3d9.h"
#pragma comment(lib, "d3d9.lib")

#include "d3dx9.h"
#pragma comment(lib, "d3dx9.lib")

LPDIRECT3D9 pDirect3D = NULL;
LPDIRECT3DDEVICE9 pDirect3DDevice = NULL;
LPDIRECT3DINDEXBUFFER9 pIndexBuffer = NULL;
LPDIRECT3DTEXTURE9 pTexture001;

struct CUSTOMVERTEX {
	float x, y, z;
	float nx, ny, nz;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;

void Matrix() {
	D3DXMATRIX MatrixWorld;
	D3DXMATRIX MatrixWorldX;
	D3DXMATRIX MatrixWorldY;
	D3DXMATRIX MatrixWorldZ;
	D3DXMATRIX MatrixView;
	D3DXMATRIX MatrixProjection;

	// MatrixWorld

	static double Angle = 0;
	Angle += 0.01;

	D3DXMatrixRotationX(&MatrixWorldX, Angle);
	D3DXMatrixRotationY(&MatrixWorldY, Angle);
	D3DXMatrixRotationZ(&MatrixWorldZ, Angle);

	D3DXMatrixMultiply(&MatrixWorld, &MatrixWorldX, &MatrixWorldY);
	pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorld);

	/*D3DXMatrixRotationX(&MatrixWorldX, Angle);
	pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorldX);
*/

	D3DXMatrixRotationY(&MatrixWorldY, Angle);
	pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorldY);

	/*D3DXMatrixRotationZ(&MatrixWorldZ, Angle);
	pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorldZ);*/

	// MatrixView
	D3DXMatrixLookAtLH(
		&MatrixView,
		&D3DXVECTOR3(0.f, 0.f, -18.f),
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		&D3DXVECTOR3(0.f, 1.f, 0.f));


	pDirect3DDevice->SetTransform(D3DTS_VIEW, &MatrixView);

	// MatrixProjection
	D3DXMatrixPerspectiveFovLH(
		&MatrixProjection,
		D3DX_PI / 2.f, 1.f,
		1.f,
		100.f);


	pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &MatrixProjection);

}

void LightMaterial() {
	D3DMATERIAL9 Material;
	D3DLIGHT9 Light;

	// ��������
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Diffuse.r = Material.Ambient.r = 1.0f;
	Material.Diffuse.g = Material.Ambient.g = 1.0f;
	Material.Diffuse.b = Material.Ambient.b = 1.0f;
	Material.Diffuse.a = Material.Ambient.a = 1.0f;
	pDirect3DDevice->SetMaterial(&Material);

	// ����
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// ���� � ����� �����������
	Light.Type = D3DLIGHT_DIRECTIONAL;
	// ����� ����
	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;
	// ���������
	Light.Range = 2000.0f;

	// ������ ��� ������� ����������� ���������
	D3DXVECTOR3 VectorDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &VectorDir);

	pDirect3DDevice->SetLight(0, &Light);

	pDirect3DDevice->LightEnable(0, TRUE);

	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, FALSE);

}

float p[][2] = {
	{ 2.777f, 1.54f },
	{ -0.4f, 3.918f },
	{ -1.276f, 6.606f },
	{ 0.594f, 8.399f },

	{ 0.594f, 8.399f },
	{ 3.479f, 8.555f },
	{ 5.882f, 6.801f },
	{ 6.684f, 4.872f },

	{ 6.684f, 4.872f },
	{ 7.142f, 4.326f },
	{ 8.994f, 5.593f },
	{ 10.475f, 7.006f },

	{ 10.475f, 7.006f },
	{ 10.371f, 7.464f },
	{ 10.143f, 7.854f },
	{ 9.636f, 8.01f },

	{ 9.636f, 8.01f },
	{ 5.427f, 10.885f },
	{ -2.056f, 9.023f },
	{ -2.758f, 8.204f },

	{ -2.758f, 8.204f },
	{ -4.628f, 6.334f },
	{ -5.252f, 4.852f },
	{ -5.291f, 3.293f },

	{ -5.291f, 3.293f },
	{ -4.122f, 0.76f },
	{ 0.088f, -2.553f },
	{ 2.426f, -4.268f },

	{ 2.426f, -4.268f },
	{ 3.089f, -5.242f },
	{ 3.206f, -6.373f },
	{ 2.738f, -7.698f },

	{ 2.738f, -7.698f },
	{ 0.633f, -9.179f },
	{ -2.446f, -9.569f },
	{ -4.667f, -9.023f },

	{ -4.667f, -9.023f },
	{ -5.876f, -7.854f },
	{ -6.265f, -6.723f },
	{ -6.46f, -5.164f },

	{ -6.46f, -5.164f },
	{ -7.006f, -4.463f },
	{ -7.707f, -4.424f },
	{ -8.409f, -4.852f },

	{ -8.409f, -4.852f },
	{ -9.929f, -6.606f },
	{ -10.475f, -7.62f },
	{ -10.28f, -8.438f },

	{ -10.28f, -8.438f },
	{ -7.707f, -9.997f },
	{ -4.784f, -10.855f },
	{ -0.692f, -10.738f },

	{ -0.692f, -10.738f },
	{ 3.518f, -9.53f },
	{ 6.986f, -6.918f },
	{ 7.805f, -4.151f },

	{ 7.805f, -4.151f },
	{ 8.324f, -2.389f },
	{ 2.777f, 1.54f },
	{ 2.777f, 1.54f }

};


// sizes
int countNurbs = 14;
int countSegments = 500;
int totalPoints = countNurbs * (countSegments + 1) + 1;
int indexBufferSize = 0;

HRESULT InitialVertexBuffer() {

	int i, j;
	float t;
	float step = 1.f / (countSegments);


	indexBufferSize = countNurbs * countSegments * 3;
	unsigned short * Indexes = new unsigned short[indexBufferSize];

	j = 1;
	for (i = 0; i < indexBufferSize - 2; i += 3) {
		Indexes[i] = 0;
		Indexes[i + 1] = j;
		Indexes[i + 2] = j + 1;
		j++;
	}


	CUSTOMVERTEX* Vertexes = new CUSTOMVERTEX[totalPoints];

	Vertexes[0].x = 0.0f;
	Vertexes[0].y = 0.0f;
	Vertexes[0].z = 3.0f;

	Vertexes[0].nx = 0.0f;
	Vertexes[0].ny = 0.0f;
	Vertexes[0].nz = 1.0f;




	for (i = 1; i < totalPoints; i++)
	{
		Vertexes[i].z = 0.0f;

	}


	int c = 0;
	int w = 1;

	for (i = 0; i < countNurbs; i++) {

		c = i * 4;


		float a[4] = {
			-p[c][0] + 3.f*p[c + 1][0] - 3.f*p[c + 2][0] + p[c + 3][0],
			3.f*p[c][0] - 6.f*p[c + 1][0] + 3.f*p[c + 2][0],
			-3.f*p[c][0] + 3.f*p[c + 1][0],
			p[c][0]
		};

		float b[4] = {
			-p[c][1] + 3.f*p[c + 1][1] - 3.f*p[c + 2][1] + p[c + 3][1],
			3.f*p[c][1] - 6.f*p[c + 1][1] + 3.f*p[c + 2][1],
			-3.f*p[c][1] + 3.f*p[c + 1][1],
			p[c][1]
		};



		t = 0.f;
		for (j = 0; j < countSegments + 1; j++) {
			//t += step;
			Vertexes[w].x = ((a[0] * t + a[1])*t + a[2])*t + a[3];
			Vertexes[w].y = ((b[0] * t + b[1])*t + b[2])*t + b[3];

			D3DXVECTOR3 n;
			n.x = 3 * b[0] * t * t + 2 * b[1] * t + b[2];;
			n.y = 3 * a[0] * t*t + 2 * a[1] * t + a[2];
			n.z = (3 * a[0] * t*t + 2 * a[1] * t + a[2]) * (b[0] * t* t* t + b[1] * t *t + b[1] * t + b[2])
				- (a[0] * t* t* t + a[1] * t* t + a[2] * t + a[3] - (3 * b[0] * t* t + 2 * b[1] * t + b[2]));

			D3DXVECTOR3 nn;
			D3DXVec3Normalize((D3DXVECTOR3*)&nn, &n);

			Vertexes[w].nx = nn.x;
			Vertexes[w].ny = nn.y;
			Vertexes[w].nz = nn.z;

			w++;
			t += step;
		}
	}


	D3DXCreateTextureFromFile(pDirect3DDevice, _T("me.png"), &pTexture001);

	// �������� ������ ������
	if (FAILED(pDirect3DDevice->CreateVertexBuffer(
		totalPoints * sizeof(CUSTOMVERTEX),	// ������ ������ ����� � ������
		0,							// ������ ���������� ������ ������
		D3DFVF_CUSTOMVERTEX,		// ������ ������	
		D3DPOOL_DEFAULT,			// ���� ����������� ������ ������ ������ ��� ���������� � ������
		&pVertexBuffer,				// ����� ���������, ��� ����� ��������� ����� ������������ ������ ������
		NULL						// ����������������� ��������
		))) {
		return E_FAIL;
	}

	// ���������� ������
	void *pVB = NULL;
	if (FAILED(pVertexBuffer->Lock(
		0,					// ��������
		totalPoints *sizeof(CUSTOMVERTEX),	// ������ ���������� ������ ������
		(void **)&pVB,		// ����� ��������� �� ���������, � ������� ������ ������
		0					// ��� �������������� ����������
		))) {
		return E_FAIL;
	}

	// ����������� ������
	memcpy(pVB, Vertexes, totalPoints * sizeof(CUSTOMVERTEX));

	// ������������� ������
	pVertexBuffer->Unlock();


	if (FAILED(pDirect3DDevice->CreateIndexBuffer(
		// ������ ���������� ������ � ������
		indexBufferSize * sizeof(unsigned short),
		// ������ ����������
		0,
		// ������
		D3DFMT_INDEX16,
		// ������ ��� ���������� � ������
		D3DPOOL_DEFAULT,
		// ����� ���������, � ������� �������� ����������� ��������� �����
		&pIndexBuffer,
		// ��������������
		NULL
		))) {
		return E_FAIL;
	}

	void *pIB = NULL;
	if (FAILED(pIndexBuffer->Lock(
		0,
		indexBufferSize * sizeof(unsigned short),
		(void**)&pIB, 0
		))) {
		return E_FAIL;
	}
	memcpy(pIB, Indexes, indexBufferSize * sizeof(unsigned short));
	pIndexBuffer->Unlock();

	return S_OK;
}


HRESULT RenderingDirect3D() {

	if (pDirect3DDevice == NULL) {
		return E_FAIL;
	}

	// ������� ������ �����
	pDirect3DDevice->Clear(
		0,							// ���������� ������������� ��������
		NULL,						// ����� ������� ���������������
									// ������� ����������� ��������� ������
									// ������� ����� �������
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255),	// ����
		1.0f,						// �������� ��� ������� Z-������[0-1]
		0							// �������� ��� ������� ������ ���������
		);

	// ������ �����
	pDirect3DDevice->BeginScene();

	Matrix();
	LightMaterial();


	// ����� ������ ����������� � ������� ������ ����������
	pDirect3DDevice->SetStreamSource(
		0,						// ����������� ������ ������[0-1]
		pVertexBuffer,			// ��������� �� ����� ������
		0,						// �������� �� ������ ������, �� ������ ������ ������
		sizeof(CUSTOMVERTEX)	// "��� � ������" �� ����� ������� �� ������
		);

	// �������� ������� ������
	pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	pDirect3DDevice->SetIndices(pIndexBuffer);


	pDirect3DDevice->SetTexture(0, pTexture001);
	pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1,
		D3DTA_TEXTURE);
	pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,
		D3DTOP_MODULATE);


	for (int i = 0; i < countNurbs - 4; i++) {
		pDirect3DDevice->DrawIndexedPrimitive(
			D3DPT_LINESTRIP,	// ��� ���������
								//D3DPT_TRIANGLELIST,	// ��� ���������
			0,					// �������� �������
			0,					// ����������� ������
			0,					// ���������� �������� ������
			countSegments * 3 * i,// ��������� ������
			countSegments * 10	// ���������� �������� ����������
			);
	}

	for (int i = 0; i < countNurbs; i++) {
		pDirect3DDevice->DrawIndexedPrimitive(
			D3DPT_LINESTRIP,	// ��� ���������
								//D3DPT_TRIANGLELIST,	// ��� ���������
			0,					// �������� �������
			0,					// ����������� ������
			0,					// ���������� �������� ������
			countSegments * 3 * i + 50,// ��������� ������
			countSegments * 5	// ���������� �������� ����������
			);
	}

	// ����� �����
	pDirect3DDevice->EndScene();

	// ����������� ������� ������ �� ��������
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

HRESULT InitialDirect3D(HWND hvnd) {

	// ������� ��������� �� ��������� IDerect3D9
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDirect3D == NULL) {
		return E_FAIL;
	}

	/*
	�������� ���������� � ������� ������ ����������� ����������� �������
	���������� ������, ��������� � ������ �����������
	*/
	D3DDISPLAYMODE Display;
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display))) {
		return E_FAIL;
	}

	// ������ ���������� �������������
	D3DPRESENT_PARAMETERS Direct3DParameter;
	// ������� ����������� ��������� �� ������������� "������"
	ZeroMemory(&Direct3DParameter, sizeof Direct3DParameter);

	// ����������. TRUE - ������� �����, FALSE - �������������
	Direct3DParameter.Windowed = TRUE;
	// ������� ������ ������. 
	Direct3DParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// ������ ����������� ������� ������
	Direct3DParameter.BackBufferFormat = Display.Format;

	// ����������� �������� � ��������� z-buffer
	Direct3DParameter.EnableAutoDepthStencil = TRUE;
	// ������ ����������� z-buffer
	Direct3DParameter.AutoDepthStencilFormat = D3DFMT_D16;

	// �������� ������� ��� ���������� ����������
	if (FAILED(pDirect3D->
		CreateDevice(
			D3DADAPTER_DEFAULT,						// ����������
			D3DDEVTYPE_HAL,							// ��� ����������(�� ��������� �������, ����� �� �������������� ����������� ����������)
													// D3DDEVTYPE_HALL - ������������� ����������� ����������� �����������
			hvnd,									// ���������� ����
			D3DCREATE_HARDWARE_VERTEXPROCESSING,	// ��� ����� ����������� ��������� ������
													// D3DCREATE_HARDWARE_VERTEXPROCESSING - ����������� ���������� � ���������� ���������� ��������� ������
			&Direct3DParameter,						// ��������� �� ��������� D3DPRESENT_PARAMTERS
			&pDirect3DDevice						// ����� ��������� ��� �������-���������� IDerect3DDevice9
			))) {
		return E_FAIL;
	}

	//activation of culling Direct3D
	pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


	// ��������� z-buffer
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	return S_OK;
}


void DeleteDirect3D() {

	if (pVertexBuffer != NULL) {
		pVertexBuffer->Release();
	}

	if (pDirect3DDevice != NULL) {
		pDirect3DDevice->Release();
	}

	if (pDirect3D != NULL) {
		pDirect3D->Release();
	}

	if (pTexture001 != NULL)
		pTexture001->Release();
}


int DisplayResourceNAMessageBox()
{
	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)L"Resource not available\nDo you want to try again?",
		(LPCWSTR)L"Account Details",
		MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
		);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;
	case IDTRYAGAIN:
		// TODO: add code
		break;
	case IDCONTINUE:
		// TODO: add code
		break;
	}

	return msgboxID;
}


/*
������� ��������� �������
*/
LRESULT CALLBACK MainWinProc(
	HWND hwnd,								// ���������� ����
	UINT msg,								// ������������� �������
	WPARAM wparam,							// �������������� ���.
	LPARAM lparam) {						// �������������� ���.

	switch (msg) {
	case VK_LEFT: {
		DisplayResourceNAMessageBox();
		break;
	}
	case WM_PAINT:							// ������������� ����������� ����� ����, ������� ���� ����������, ��������� � ��� �����
		RenderingDirect3D();
		ValidateRect(hwnd, NULL);
		break;
	case WM_DESTROY:						// ���� ������ ���� �������(������������ ��������)
		DeleteDirect3D();
		// ����� ������������ �������� ���������� WM_QUIT - ����������� ����������
		// ������ � ������� WM_QUIT
		PostQuitMessage(0);
		return 0;
	}

	// ��������� ���������, ������� �� ����������
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/*
������� �������
*/
int WINAPI WinMain(
	HINSTANCE hinstance,		// ����������/��������� ����������
	HINSTANCE hprevinstance,	// �� ������������, ����� 0
	LPSTR lpCmdLine,			// ��������� �� ��������� ������, ������� ���� ����� �� ������ ����������� �������
	int nCmdShow) {				// ��������, ����������� - ��� ����� ������������ ���� �� ������ �������� ��� ��������


	WNDCLASSEX windowsclass;	// ����� ������������ ����

	windowsclass.cbSize = sizeof(WNDCLASSEX);								// ������ ����������� ���������
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;	// ���������� ������
																			/*
																			CS_VREDRAW - ���� ������ ������ ���� �������� ��� ���� ���� ����������, �� ��������� ����������� ����
																			CS_HREDRAW - ���� ������ ������ ���� �������� ��� ���� ���� ����������, �� ��������� ����������� ����
																			CS_OWNDC - ��� ������� ���� ������� ������ ������ ���� �������� ���������
																			CS_DBLCLKS - ��� ����������� � ���� ������� ������ ����� ���� ���������� ���������� � ������� ������
																			*/
	windowsclass.lpfnWndProc = MainWinProc;									// ��������� �� call back. ���������� ���� ��������� �������, ��� ������ �������� ���������� �����
	windowsclass.cbClsExtra = 0;											// ��� �������� ���. ����������
	windowsclass.cbWndExtra = 0;											// ��� �������� ���. ����������
	windowsclass.hInstance = hinstance;										// ����, ���������� �� ��������� ������������ ����������, ������������� � ������� WinMain
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// ������ ��� ����������� ����������� ����������
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);						// ����, �������� �� ������
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		// ���� ��������� �������� �� ���� ���� ����
	windowsclass.lpszMenuName = NULL;										// ����, ��� ����������� ������������ ���� � ����
	windowsclass.lpszClassName = _T("WINDOWSCLASS");						// �������� ������ ����
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					// ����� �����������

																			// ������������ ��������� �����
	if (!RegisterClassEx(&windowsclass)) {
		return 0;
	}


	// �������� ����
	HWND hwnd = CreateWindowEx(
		NULL,								// ����� ����(WS_EX_TOPMOST - ���� ���������� ������ ������ ����)
		_T("WINDOWSCLASS"),					// ��� ������ ������������ ����
		_T("����� �������� - ������������ ������ �3"),		// ��������� ����
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// ����� ����������� ����� � ��������� ����
											/*
											WS_OVERLAPPED - ���� � �������� ��������� � ������
											WS_VISIBLE - ���������� ������� ����
											WS_CAPTION - ���� � �������� ��������� (�������� � ���� ����� WS_BORDER)
											WS_BORDER - ���� � ������ �����
											WS_ICONIC - ���� ����������� ���������������
											WS_OVERLAPPEDWINDOW - ��������������� ����(��������: WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, WS_MAXIMIZEBOX)
											WS_MINIMIZE - ���������� ���������������
											WS_MAXIMIZE -������������ ����������������
											WS_MINIMIZEBOX - ���� � ������� �����������(������������ �� ������ WS_SYSMENU)
											WS_MAXIMIZEBOX -���� � ������� ������������(������������ �� ������ WS_SYSMENU)
											*/
		0, 0,								// ���������� ������ �������� �����
		1500, 800,							// ������ � ������ ����
		NULL,								// ���������� ������������� ����
		NULL,								// ���������� ����
		hinstance,							// ���������� ����������
		NULL);								// ��������� �� ������ ����

											// ��������, ������ �� �������
	if (!hwnd) {
		return 0;
	}


	// ������������� ���������
	MSG msg;
	if (SUCCEEDED(InitialDirect3D(hwnd))) {
		if (SUCCEEDED(InitialVertexBuffer())) {
			// ������� ����
			ShowWindow(hwnd, SW_SHOWDEFAULT);
			// ��������� ����
			UpdateWindow(hwnd);

			ZeroMemory(&msg, sizeof msg);
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					// �������������� ������������� �����
					TranslateMessage(&msg);
					// ��������� � ��������� ��������� � MainWinProc()
					DispatchMessage(&msg);
				}
				else {
					RenderingDirect3D();
				}
			}
		}
	}

	return msg.wParam;
}
