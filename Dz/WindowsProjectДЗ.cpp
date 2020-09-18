#include <Windows.h>
//#include <float.h>
#include <math.h>
#include <windowsx.h>
#include <stdio.h>




HANDLE hThread1;

HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0,
	DEFAULT_CHARSET,
	0, 0, 0, 0,
	L"Comic Sans"
);


int stage1 = 0, stage2 = 0, stage3 = 0;

DWORD WINAPI Thread1(LPVOID t) {
	HDC hdc = GetDC((HWND)t);
	SelectObject(hdc, hFont);
	while (true) {
		if (stage1) SetTextColor(hdc, RGB(0, 255, 0));
		else SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 0, 0, L"0,1ln(a)", 9);

		if (stage2) SetTextColor(hdc, RGB(0, 255, 0));
		else SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 200, 0, L"sin(2.5a)", 10);

		if (stage3) SetTextColor(hdc, RGB(0, 255, 0));
		else SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 400, 0, L"4cos(5.5a^2)", 13);
		Sleep(250);
	}
	return 0;
}

float size=1;

LRESULT CALLBACK WindowProcess(HWND hWindow,
	UINT uMessage,
	WPARAM wParameter,
	LPARAM lParameter)
{

	switch (uMessage) {
	case WM_CREATE:
	{
		CreateThread(NULL, 0, Thread1, hWindow, 0, NULL);

	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWindow, &ps);
		MoveToEx(hdc, 500, 500, NULL);
		for (float a = 0; a < 4 * 3.14; a += 0.01) {
			float radius = 0.1;
			if (stage1)radius *= log(a);
			if (stage2)radius *= sin(2.5 * a);
			if (stage3)radius *= 4 * cos(5.5 * a * a);
			int asad = 0;
			float y = radius * sin(a) * size;
			float x = radius * cos(a) * size;
			if (a == 0)
				MoveToEx(hdc, 300 * x + 500, 300 * y + 500, NULL);
			else
				LineTo(hdc, 300 * x + 500, 300 * y + 500);
		}
		
		EndPaint(hWindow, &ps);
	}
	return 0;
	case WM_KEYDOWN:
		switch (wParameter) {
		case 0x31:
		{
			if (stage1 != 1)stage1 = 1;
			else stage1 = 0;
			break;
		}
		case 0x32:
		{
			if (stage2 != 1)stage2 = 1;
			else stage2 = 0;
			break;
		}
		case 0x33:
		{
			if (stage3 != 1)stage3 = 1;
			else stage3 = 0;
			break;
		}
		case VK_UP: {
			size += 0.1;
			break;
		}
		case VK_DOWN: {
			size -= 0.1;
			break;
		}
		}
		RECT rect;
		GetWindowRect(hWindow, &rect);
		InvalidateRect(hWindow, &rect, true);
		return 0;

		return 0;
		//StopDrawing(hWindow);
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
	}
	return NULL;
}




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

	wc.lpfnWndProc = WindowProcess;
	wc.lpszClassName = L"Class";
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}
	hwnd = CreateWindow(wc.lpszClassName, L"Lab_4",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
		, 0, 0, 1000, 1000, NULL, NULL, wc.hInstance, NULL);
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

