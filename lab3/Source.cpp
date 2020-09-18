//#include <stdafx.h>
#include <Windows.h>
#include <cmath>
#include <stdio.h>


#define FIG1 1235
#define FIG2 1236
#define FIG3 1237
#define FIG4 1238

#define COLOR1 1239
#define COLOR2 1240
#define COLOR3 1241
#define COLOR4 1242

COLORREF color = RGB(255, 0, 0);

int mode = 1235;

wchar_t wbuf[100];

int sizeH = 20, sizeW = 20;

HWND htextBox1;
HWND htextBox2;

HWND hEdit;



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg) {
	case WM_CREATE:
	{

		CreateWindow(L"STATIC", L"Высота", WS_CHILD | WS_VISIBLE, 0, 40, 60, 15, hWnd, 0, 0, NULL);
		htextBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"50",
			WS_CHILD | WS_VISIBLE, 74, 38, 90, 20, hWnd, (HMENU)0, NULL, NULL);

		CreateWindow(L"STATIC", L"Ширина", WS_CHILD | WS_VISIBLE, 0, 60, 60, 15, hWnd, 0, 0, NULL);
		htextBox2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"30",
			WS_CHILD | WS_VISIBLE, 74, 64, 90, 20, hWnd, (HMENU)0, NULL, NULL);
		HWND BUTTON = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Применить", WS_CHILD | WS_VISIBLE, 0, 80, 80, 30, hWnd, (HMENU)1000, 0, NULL);

		HMENU hMenubar = CreateMenu();
		HMENU hMenu = CreateMenu();
		HMENU hSubMenuSpeed = CreatePopupMenu();
		HMENU hSubMenuColor = CreatePopupMenu();
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"Параметры");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenuSpeed, L"Фигуры");
		AppendMenu(hSubMenuSpeed, MF_POPUP, FIG1, L"Трапеция");
		AppendMenu(hSubMenuSpeed, MF_POPUP, FIG2, L"Квадрат");
		AppendMenu(hSubMenuSpeed, MF_POPUP, FIG3, L"Треугольник");
		AppendMenu(hSubMenuSpeed, MF_POPUP, FIG4, L"Ромб");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenuColor, L"Палитра");
		AppendMenu(hSubMenuColor, MF_POPUP, COLOR1, L"Красный");
		AppendMenu(hSubMenuColor, MF_POPUP, COLOR2, L"Зеленый");
		AppendMenu(hSubMenuColor, MF_POPUP, COLOR3, L"Желтый");
		AppendMenu(hSubMenuColor, MF_POPUP, COLOR4, L"Голубой");
		SetMenu(hWnd, hMenubar);
	}
	return 0;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == FIG1) {
			mode = FIG1;
		}

		if (LOWORD(wParam) == FIG2) {
			mode = FIG2;
		}

		if (LOWORD(wParam) == FIG3) {
			mode = FIG3;
		}

		if (LOWORD(wParam) == FIG4) {
			mode = FIG4;
		}
		if (LOWORD(wParam) == COLOR1) {
			color = RGB(255, 0, 0);
		}

		if (LOWORD(wParam) == COLOR2) {
			color = RGB(0, 255, 0);
		}

		if (LOWORD(wParam) == COLOR3) {
			color = RGB(255, 255, 0);
		}

		if (LOWORD(wParam) == COLOR4) {
			color = RGB(0, 0, 255);
		}
		if (LOWORD(wParam) == 1000) {
			GetWindowText(htextBox1, wbuf, 100);
			swscanf_s(wbuf, L"%d", &sizeW);
			GetWindowText(htextBox2, wbuf, 100);
			swscanf_s(wbuf, L"%d", &sizeH);
		}
	}
	return 0;
	case WM_LBUTTONDOWN:
	{
		POINT point;
		GetCursorPos(&point);
		point.y -= 40;
		point.x -= 5;
		switch (mode) {
		case FIG1:
		{
			HDC hdc;
			hdc = GetDC(hWnd);
			HBRUSH brush = CreateSolidBrush(color);
			SelectObject(hdc, brush);
			POINT vertices[] = { {point.x - (sizeW / 3),point.y - (sizeH / 2) },
								 {point.x + (sizeW / 3),point.y - (sizeH / 2)},
								{point.x + (sizeW / 2),point.y + (sizeH / 2) },
								point.x - (sizeW / 2),point.y + (sizeH / 2) };
			Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
			ReleaseDC(hWnd, hdc);
			return 0;
		}
		case FIG2: {
			HDC hdc;
			hdc = GetDC(hWnd);
			HBRUSH brush = CreateSolidBrush(color);
			SelectObject(hdc, brush);
			POINT vertices[] = { {point.x - (sizeW / 2),point.y - (sizeH / 2) },
								 {point.x + (sizeW / 2),point.y - (sizeH / 2)},
								{point.x + (sizeW / 2),point.y + (sizeH / 2) },
								point.x - (sizeW / 2),point.y + (sizeH / 2) };
			Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
			ReleaseDC(hWnd, hdc);
		}
				 return 0;
		case FIG3: {
			HDC hdc;
			hdc = GetDC(hWnd);
			HBRUSH brush = CreateSolidBrush(color);
			SelectObject(hdc, brush);
			POINT vertices[] = { {point.x + (sizeW / 2),point.y + (sizeH / 2) },
								point.x ,point.y - (sizeH / 2),
								point.x - (sizeW / 2),point.y + (sizeH / 2) };
			Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
			ReleaseDC(hWnd, hdc);
		}
				 return 0;
		case FIG4: {
			HDC hdc;
			hdc = GetDC(hWnd);
			HBRUSH brush = CreateSolidBrush(color);
			SelectObject(hdc, brush);
			POINT vertices[] = { point.x - (sizeW / 2),point.y,
								point.x ,point.y - (sizeH / 2),
								point.x + (sizeW / 2),point.y,
								point.x ,point.y + (sizeH / 2),
								point.x - (sizeW / 2),point.y
			};
			Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
			ReleaseDC(hWnd, hdc);
		}
		}
	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;
	case WM_SYSCOMMAND: {
		int command = int(wParam) & 0xfff0;
		if (command == SC_MOVE)
			return 0;
		break;
	}
					  return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};




int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow) {
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;

	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"Class";
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}
	hwnd = CreateWindow(wc.lpszClassName, L"Lab_3", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 1000, 1000, NULL, NULL, wc.hInstance, NULL);
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
	return NULL;
}
