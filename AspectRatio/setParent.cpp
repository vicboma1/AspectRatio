// setParent.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "setParent.h"
#include <Windows.h>
#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int count = 0;
int* i = &count;
bool mutex = true;
BYTE prev_escape = 0, curr_escape = 0;
double threshold = 0.9999999999999999;

constexpr double res4_3  = 4.0 / 3.0;
constexpr double res16_9 = 16.0 / 9.0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SETPARENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	
    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;
  
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SETPARENT));

    MSG msg;

    // Bucle principal de mensajes:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SETPARENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SETPARENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
HWND child = NULL;
int w, h;

bool fin = false;

void setResolutionWindow(LPCWSTR str, double resolution ) {
	SetWindowText(child, str);

	auto w_ratio = w / resolution;
	auto margin_w = (h - w_ratio) / 2.0;

	auto h_ratio = h / resolution;
	auto margin_h = (w - h_ratio) / 2.0;

	//Verticales cuando se proyecta una resolución horizontal
	if (threshold > resolution)
		SetWindowPos(child, HWND_TOP,  (w + margin_w) / 2, 8, -margin_w, h, SWP_FRAMECHANGED | SWP_NOCOPYBITS);
	else 
		if (margin_w >= 0)
		SetWindowPos(child, HWND_TOP, 8, (h - w_ratio) / 2.0, w, w_ratio + 8, SWP_FRAMECHANGED | SWP_NOCOPYBITS);
	//Corregidos en 4/3
	else
		SetWindowPos(child, HWND_TOP, ((w - w_ratio) / 2.0)+8, 8, w_ratio, h, SWP_FRAMECHANGED | SWP_NOCOPYBITS);
}


void setMenuResolution(int** i) {

	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);

	switch (**i) {
	case 1:  setResolutionWindow(L"4:3  ", res4_3); break;
	case 2:  setResolutionWindow(L"16:9  ", res16_9); break;

	default:

		auto w_ratio = w / res16_10;
		auto h_ratio = h / res16_10;
		mutex = true;
		SetWindowText(child, L"Center Injection");
		SetWindowPos(child, HWND_TOP, (w - w_ratio) / 2.0, (h - h_ratio) / 2.0, w_ratio, h_ratio, SWP_FRAMECHANGED | SWP_NOCOPYBITS);

		**i = 0;
		break;

	}
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{ 
	hInst = hInstance; // Almacenar identificador de instancia en una variable global

   HWND parent = CreateWindowW(szWindowClass, L"Parent", WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   child = CreateWindowW(szWindowClass, L"Fullscreen", WS_OVERLAPPEDWINDOW,
	   8, 8, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, NULL, nullptr);
  
   DWORD id = 0;
   count++;
   CreateThread(nullptr, 0, [](LPVOID data) -> DWORD {
	   auto refInt = (int*)data;
	   while (!fin) {

		   curr_escape = GetAsyncKeyState(VK_RIGHT) ? 1 : 0;
			   if (prev_escape != curr_escape) {
				   if (curr_escape) {
					   ++(*refInt);
					   setMenuResolution(&refInt);
				   }
				   prev_escape = curr_escape;
			   }
		   
			   curr_escape = GetAsyncKeyState(VK_LEFT) ? 1 : 0;
			   if (prev_escape != curr_escape) {
				   if (curr_escape) {
					   --(*refInt);
					   setMenuResolution(&refInt);
				   }
				   prev_escape = curr_escape;
			   }
		   
		  
		   Sleep(250);
	   }

	   return 0; 
   } , (LPVOID)i, 0, &id);
      


   if (!parent)
   {
      return FALSE;
   }

   ShowWindow(parent, SW_MAXIMIZE);
   ShowWindow(child, SW_NORMAL);

   UpdateWindow(parent);
   UpdateWindow(child);

   SetParent(child, parent); //a will be the new parent b
  
   SetWindowLong(parent, GWL_STYLE, GetWindowLong(parent, GWL_STYLE) & ~((WS_BORDER | WS_DLGFRAME | WS_THICKFRAME)));
  
   UpdateWindow(parent);
   UpdateWindow(child);

   SetMenu(parent, NULL);
   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_EXIT:
				fin = true;
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
