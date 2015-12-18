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

	// материал
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Diffuse.r = Material.Ambient.r = 1.0f;
	Material.Diffuse.g = Material.Ambient.g = 1.0f;
	Material.Diffuse.b = Material.Ambient.b = 1.0f;
	Material.Diffuse.a = Material.Ambient.a = 1.0f;
	pDirect3DDevice->SetMaterial(&Material);

	// свет
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// свет в одном направлении
	Light.Type = D3DLIGHT_DIRECTIONAL;
	// белый свет
	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;
	// растояние
	Light.Range = 2000.0f;

	// вектор для задания направления источника
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

	// создание буфера вершин
	if (FAILED(pDirect3DDevice->CreateVertexBuffer(
		totalPoints * sizeof(CUSTOMVERTEX),	// размер буфера вешир в байтах
		0,							// способ применения буфера вершин
		D3DFVF_CUSTOMVERTEX,		// формат вершин	
		D3DPOOL_DEFAULT,			// флаг описывающий формат буфера вершин для размещения в памяти
		&pVertexBuffer,				// адрес указателя, где будет храниться адрес создаваемого буфера вершин
		NULL						// зарезервированный параметр
		))) {
		return E_FAIL;
	}

	// блокировка буфера
	void *pVB = NULL;
	if (FAILED(pVertexBuffer->Lock(
		0,					// смещение
		totalPoints *sizeof(CUSTOMVERTEX),	// размер блокировки буфера вершин
		(void **)&pVB,		// адрес указателя на указатель, с данными буфера вершин
		0					// тип осуществляемой блокировки
		))) {
		return E_FAIL;
	}

	// копирование данных
	memcpy(pVB, Vertexes, totalPoints * sizeof(CUSTOMVERTEX));

	// разблокировка буфера
	pVertexBuffer->Unlock();


	if (FAILED(pDirect3DDevice->CreateIndexBuffer(
		// размер индексного буфера в байтах
		indexBufferSize * sizeof(unsigned short),
		// способ применения
		0,
		// формат
		D3DFMT_INDEX16,
		// фортма для размещения в памяти
		D3DPOOL_DEFAULT,
		// адрес указателя, в котором хранится создаваемый индексный буфер
		&pIndexBuffer,
		// зарезервинован
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

	// очищаем задний буфер
	pDirect3DDevice->Clear(
		0,							// количество прямоугольных областей
		NULL,						// адрес массива прямоугольников
									// очищаем поверхность заданыным цветом
									// очищаем буфер глубины
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255),	// цвет
		1.0f,						// значение ждя очистки Z-буфера[0-1]
		0							// значение для очистки буфера трафарета
		);

	// начало сцены
	pDirect3DDevice->BeginScene();

	Matrix();
	LightMaterial();


	// буфер вершин связывается с потоком данных устройства
	pDirect3DDevice->SetStreamSource(
		0,						// определение потока данных[0-1]
		pVertexBuffer,			// указатель на буфер вершин
		0,						// смещение от начала данных, до начала данных вершин
		sizeof(CUSTOMVERTEX)	// "шаг в байтах" от одной вершины до другой
		);

	// устновка формата вершин
	pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	pDirect3DDevice->SetIndices(pIndexBuffer);


	pDirect3DDevice->SetTexture(0, pTexture001);
	pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1,
		D3DTA_TEXTURE);
	pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,
		D3DTOP_MODULATE);


	for (int i = 0; i < countNurbs - 4; i++) {
		pDirect3DDevice->DrawIndexedPrimitive(
			D3DPT_LINESTRIP,	// тип примитива
								//D3DPT_TRIANGLELIST,	// тип примитива
			0,					// смещение индекса
			0,					// минимальный индекс
			0,					// количество рисуемых вершин
			countSegments * 3 * i,// стартовый индекс
			countSegments * 10	// количество рисуемых примитивов
			);
	}

	for (int i = 0; i < countNurbs; i++) {
		pDirect3DDevice->DrawIndexedPrimitive(
			D3DPT_LINESTRIP,	// тип примитива
								//D3DPT_TRIANGLELIST,	// тип примитива
			0,					// смещение индекса
			0,					// минимальный индекс
			0,					// количество рисуемых вершин
			countSegments * 3 * i + 50,// стартовый индекс
			countSegments * 5	// количество рисуемых примитивов
			);
	}

	// конец сцены
	pDirect3DDevice->EndScene();

	// копирование заднего буфера на передний
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

HRESULT InitialDirect3D(HWND hvnd) {

	// создаем указатель на интерфейс IDerect3D9
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDirect3D == NULL) {
		return E_FAIL;
	}

	/*
	получаем информацию о текущем режиме визуального отображения дисплея
	разрешение экрана, цветность и формат поверхности
	*/
	D3DDISPLAYMODE Display;
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display))) {
		return E_FAIL;
	}

	// объект параметров представления
	D3DPRESENT_PARAMETERS Direct3DParameter;
	// ощичаем создаваемую структуру от всевозможного "мусора"
	ZeroMemory(&Direct3DParameter, sizeof Direct3DParameter);

	// видеорежим. TRUE - оконный режим, FALSE - полноэкранный
	Direct3DParameter.Windowed = TRUE;
	// задание буфера обмена. 
	Direct3DParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// формат поверхности заднего буфера
	Direct3DParameter.BackBufferFormat = Display.Format;

	// возможность создания и установки z-buffer
	Direct3DParameter.EnableAutoDepthStencil = TRUE;
	// формат поверхности z-buffer
	Direct3DParameter.AutoDepthStencilFormat = D3DFMT_D16;

	// создание объекта для интерфейса устройства
	if (FAILED(pDirect3D->
		CreateDevice(
			D3DADAPTER_DEFAULT,						// видеокарта
			D3DDEVTYPE_HAL,							// тип устройства(от парамента зависит, будет ли использоваться акселерация видеокарты)
													// D3DDEVTYPE_HALL - использование возможности аппаратного обеспечения
			hvnd,									// дескриптор окна
			D3DCREATE_HARDWARE_VERTEXPROCESSING,	// как будет происходить обработка вершин
													// D3DCREATE_HARDWARE_VERTEXPROCESSING - ипользуется видеокарта и происходит аппаратная обработка вершин
			&Direct3DParameter,						// указатель на структуру D3DPRESENT_PARAMTERS
			&pDirect3DDevice						// адрес указателя для объекта-интерфейса IDerect3DDevice9
			))) {
		return E_FAIL;
	}

	//activation of culling Direct3D
	pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


	// активация z-buffer
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
функция обработки событий
*/
LRESULT CALLBACK MainWinProc(
	HWND hwnd,								// дескриптор окна
	UINT msg,								// идентификатор события
	WPARAM wparam,							// дополнительная инф.
	LPARAM lparam) {						// дополнительная инф.

	switch (msg) {
	case VK_LEFT: {
		DisplayResourceNAMessageBox();
		break;
	}
	case WM_PAINT:							// необходимость перерисовки всего окна, которое было перемещено, увеличего и так далее
		RenderingDirect3D();
		ValidateRect(hwnd, NULL);
		break;
	case WM_DESTROY:						// окно должно быть закрыто(освобождение ресурсов)
		DeleteDirect3D();
		// после освобождения ресурсов вызывается WM_QUIT - закрывающее приложение
		// ставит в очередь WM_QUIT
		PostQuitMessage(0);
		return 0;
	}

	// обработка сообщений, которые не используем
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/*
главная функция
*/
int WINAPI WinMain(
	HINSTANCE hinstance,		// дескриптор/индикатор приложения
	HINSTANCE hprevinstance,	// не используется, равен 0
	LPSTR lpCmdLine,			// указатель на командную строку, которая идет сразу за именем запускаемой команды
	int nCmdShow) {				// парамент, указывающий - как будет отображаться окно на экране монитора при создании


	WNDCLASSEX windowsclass;	// класс создаваемого окна

	windowsclass.cbSize = sizeof(WNDCLASSEX);								// размер создаваемой структуры
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;	// комбинация флагов
																			/*
																			CS_VREDRAW - если размер высоты окна меняется или окно было перемещено, то требуется перерисовка окна
																			CS_HREDRAW - если размер ширины окна меняется или окно было перемещено, то требуется перерисовка окна
																			CS_OWNDC - для каждого окна данного класса дается свой контекст устройств
																			CS_DBLCLKS - при совершенном в окне двойном щелчке мышью окну посылается информация о двойном щелчке
																			*/
	windowsclass.lpfnWndProc = MainWinProc;									// указатель на call back. Существует цикл обработки событий, при работе которого происходит вызов
	windowsclass.cbClsExtra = 0;											// для кранения доп. информации
	windowsclass.cbWndExtra = 0;											// для кранения доп. информации
	windowsclass.hInstance = hinstance;										// поле, отвечающее за экземпляр создаваемого приложения, передаваемого в функцию WinMain
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// служит для определения пиктограммы приложения
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);						// поле, отвечает за курсор
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		// поле структуры отвечает за цвет фона окна
	windowsclass.lpszMenuName = NULL;										// поле, для подключения стандартного меню к окну
	windowsclass.lpszClassName = _T("WINDOWSCLASS");						// название класса окна
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					// малая пиктограмма

																			// регистрируем созданный класс
	if (!RegisterClassEx(&windowsclass)) {
		return 0;
	}


	// создание окна
	HWND hwnd = CreateWindowEx(
		NULL,								// стиль окна(WS_EX_TOPMOST - окно появляется поверх других окон)
		_T("WINDOWSCLASS"),					// имя класса создаваемого окна
		_T("Родин Владимир - Лабораторная работа №3"),		// заголовок окна
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// флаги описывающие стиль и поведение окна
											/*
											WS_OVERLAPPED - окно с полоской заголовка и рамкой
											WS_VISIBLE - изначально видимое окно
											WS_CAPTION - окно с полоской заголовка (включает в себя стиль WS_BORDER)
											WS_BORDER - окно в тонкой рамке
											WS_ICONIC - окно изаначально минимизированно
											WS_OVERLAPPEDWINDOW - перекрывающиеся окно(включает: WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, WS_MAXIMIZEBOX)
											WS_MINIMIZE - изканально минимизированно
											WS_MAXIMIZE -изканачально максимизированно
											WS_MINIMIZEBOX - окно с кнопкой минимизации(используется со стилем WS_SYSMENU)
											WS_MAXIMIZEBOX -окно с кнопкой максимизации(используется со стилем WS_SYSMENU)
											*/
		0, 0,								// координаты левого верхнего угола
		1500, 800,							// ширина и высота окна
		NULL,								// дескриптор родительского окна
		NULL,								// дескриптор меню
		hinstance,							// дескриптор приложения
		NULL);								// указатель на данное окно

											// проверка, смогли ли создать
	if (!hwnd) {
		return 0;
	}


	// идентификатор сообщения
	MSG msg;
	if (SUCCEEDED(InitialDirect3D(hwnd))) {
		if (SUCCEEDED(InitialVertexBuffer())) {
			// выводим окно
			ShowWindow(hwnd, SW_SHOWDEFAULT);
			// обновляем окно
			UpdateWindow(hwnd);

			ZeroMemory(&msg, sizeof msg);
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					// преобразование клавиатурного ввода
					TranslateMessage(&msg);
					// обработка и пересылка сообщений в MainWinProc()
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
