#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");


int WINAPI _tWinMain(HINSTANCE This,		// ���������� �������� ���������� 
						HINSTANCE Prev,		// � ����������� �������� ������ 0 
						LPTSTR cmd,			// ��������� ������ 
						int mode)			// ����� ����������� ����
{
	HWND hWnd;		// ���������� �������� ���� ��������� 
	MSG msg; 		// ��������� ��� �������� ��������� 
	WNDCLASS wc; 	// ����� ����
	// ����������� ������ ���� 
	wc.hInstance = This;
	wc.lpszClassName = WinName; 					// ��� ������ ���� 
	wc.lpfnWndProc = WndProc; 						// ������� ���� 
	wc.style = CS_HREDRAW | CS_VREDRAW; 			// ����� ���� 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 	// ����������� ������ 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// ����������� ������ 
	wc.lpszMenuName = NULL; 						// ��� ���� 
	wc.cbClsExtra = 0; 								// ��� �������������� ������ ������ 
	wc.cbWndExtra = 0; 								// ��� �������������� ������ ���� 
	wc.hbrBackground = CreateSolidBrush(RGB(48, 213, 200));


	// ����������� ������ ����
	if (!RegisterClass(&wc)) return 0;

	// �������� ���� 
	hWnd = CreateWindow(WinName,			// ��� ������ ���� 
		_T("LAB1"), 		// ��������� ���� 
		WS_OVERLAPPEDWINDOW, 		// ����� ���� 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 ������� ���� 
		400, 				// width 
		400, 				// Height 
		HWND_DESKTOP, 				// ���������� ������������� ���� 
		NULL, 						// ��� ���� 
		This, 						// ���������� ���������� 
		NULL); 					// �������������� ���������� ��� 

	ShowWindow(hWnd, mode); 				// �������� ����

	// ���� ��������� ��������� 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 		// ������� ���������� ����� ������� ������� 
		DispatchMessage(&msg); 		// �������� ��������� ������� WndProc() 
	}
	return 0;
}

int time = 0;
wchar_t outTime[256];
int timeAll[] = { 0, 0, 0, 0, 0, 0};
HFONT font;
bool pause = false;

// ������� ������� ���������� ������������ ��������
// � �������� ��������� �� ������� ��� ������� ����������

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;	// ���������� ��������� ����������. 

	/*TCHAR top_clock[] = _T("0");
	TCHAR left_clock[] = _T("45");
	TCHAR right_clock[] = _T("15");
	TCHAR bottom_clock[] = _T("30");*/

	/*double x_arrow = 250;
	double y_arrow = 100;
	double radius = 75;*/


	switch (message)		 // ���������� ���������
	{
	case WM_LBUTTONDOWN:
		if (pause)
		{
			SetTimer(hWnd, 1, 1, NULL);
			pause = false;
		}
		else 
		{
			KillTimer(hWnd, 1);
			pause = true;
		}
		break;

	case WM_RBUTTONDOWN:
		for (int i = 0; i < sizeof(time); i++)
		{
			time = 0;
		}
		KillTimer(hWnd, 1);
		pause = true;
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_TIMER:
		time++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;


	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);	// ������� BeginPaint �������������� ��������� ���� ��� ��������� � ��������� ��������� PAINTSTRUCT ����������� � �������.
		//SelectObject(hdc, CreateSolidBrush(RGB(128, 0, 0)));
		SetTextColor(hdc, RGB(0, 128, 0));	// ������� SetTextColor ������������� ���� ������ ��� ���������� ��������� ���������� � ��������� ����.
		SetTextCharacterExtra(hdc, 3);	/* ������� SetTextCharacterExtra ������ ��������������������� ��������. ��������������������� �������� ����������� � ������� �������,
		������� ������� �������, ����� ������� ���������� ������ ������.*/

		timeAll[0] = (time / 60000); // dec min
		timeAll[1] = (time / 6000) % 10; // un min
		timeAll[2] = (time / 1000) % 6; // dec sec
		timeAll[3] = (time / 100) % 10; // un sec
		timeAll[4] = (time / 10) % 10; // dec millisec
		timeAll[5] = (time % 10); // un millisec

		swprintf(outTime, L"%i%i:%i%i:%i%i\0", timeAll[0], timeAll[1], timeAll[2], timeAll[3], timeAll[4], timeAll[5]);	// ������ ��������������� ������ � ������. 
		SetTextColor(hdc, RGB(0, 0, 128));
		SetBkColor(hdc, RGB(48, 213, 200));
		font = CreateFont(30, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Comic Sans MS");
		SelectObject(hdc, font);
		
		TextOut(hdc, 130, 150, outTime, _tcslen(outTime));	// ������� TextOut ���������� ���������� ������ � ��������� �����, ��������� ������� ��������� �����, ���� ���� � ���� ������.

		ReleaseDC(hWnd, hdc);	// ������� ReleaseDC ����������� �������� ���������� 

		EndPaint(hWnd, &ps);	// ������� EndPaint �������� ��������� ��������� � ��������� ����.
		break;


	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);

		break; 			// ���������� ��������� 
	default: 			// ��������� ��������� �� ��������� 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}