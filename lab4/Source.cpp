#include <Windows.h>
#include <float.h>
#include <math.h>
#include <windowsx.h>
#include <tchar.h>

int time = 0;
wchar_t outTime[256];
int timeAll[] = { 0, 0, 0, 0, 0, 0,0,0 };
bool pause = false;

//������� ��� ������
#define CIRCLEBUTTON 1500
#define CIRCLESTOPBUTTON 1501
#define DATEBUTTON 1600
#define DATESTOPBUTTON 1601

#define M_PI 3.14159265358979323846 //������ ��� ����� �� (����� ������������ sin ��� ��������� �������� �����


HANDLE hThread1; //���������� ��������
HANDLE hThread2;


SYSTEMTIME TIME;


		//���������� ���� SYSTEMTIME ������ � ���� ����� (�� ��������) ���� �������� .wHour ; .wMinute � �.�

//������� ��� ������� ������

bool CircleDrawing = false;//���� ������ ������ ��������� �����


DWORD WINAPI Thread2(LPVOID t) {
	HDC hdc = GetDC((HWND)t);  // HDC - Device Context Handle - -�������� �� ����� ������ ����-����  (����� ��� ����)
	//����������� ����(����� ��� ����,����� �����������)
	SelectBrush(hdc, CreateSolidBrush(RGB(120, 40, 255))); //����� ����� HBRUSH ,������� ����� ������������ Ellipse(...) ��� �������� ���������� �����
	SelectPen(hdc, CreatePen(0, 3, RGB(255, 255, 255))); // ����� ����� HPEN ,������� ����� ������������ Ellipse(...) ��� �������

	//��� ��� ������ �������� ������ (�������� x ���� � ������ ��������� �������� ���� �� 0.01) ������� ������ ����� ����� ����������� ���������� ����������
	//���� ������ ����������� ����� ��� ������� - �������� �� ����� ��������� ��������
	while (true) {
		//��� ��������� �������� ������������ sin ��� ��� ��� ����������� ������� , ������� �������� ��� �������������� ��������� ������� �����
		//���� M_PI - ������ ��������� ���� - ����� ��
		for (double x = M_PI / 2; x < (5 * M_PI) / 2; x += 0.01) {
			//FillRect(hdc, &black, CreateSolidBrush(RGB(255, 255, 255)));
			HBRUSH brush = CreateSolidBrush(RGB(120 * (sin(x) + 1), 120 * (cos(x) + 1), 120 * (cos(x) + 1)));
			SelectObject(hdc, brush);
			Ellipse(hdc, 100 - 50 * (sin(x) + 1), 100 - 50 * (sin(x) + 1), 200 + 50 * (sin(x) + 1), 200 + 50 * (sin(x) + 1));
			//������ �������� �� 4 ������ - (����� ������� �������������� �� x , ������� ������� �������������� �� y, ������ ������� �� x, ������ �� y)
			//������� ������������� , ������� ������ ������ ���� ������ ,��� ��� � ������ ������ ������������� �������� ��������� - ���������� ���� 
			Sleep(5); //�������� ����� ��������� ������� ����� - 5 ���� , ����� ��������� ��� ������ ,����� �������� �������� 
					  //��� ��������� ,����� ���������	
		}
	}
	return 0;
}


HFONT font = CreateFont(60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Times New");

RECT timePlace = { 50,400,800,800 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg) {
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, CreateSolidBrush(RGB(128, 0, 0)));
		SetTextColor(hdc, RGB(128, 0, 0));
		SetTextCharacterExtra(hdc, 3);
		GetSystemTime(&TIME);
		SelectObject(hdc, font);
		
		timeAll[0] = (TIME.wHour / 10);
		timeAll[1] = (TIME.wHour % 10);
		timeAll[2] = (TIME.wMinute / 10); // dec min
		timeAll[3] = (TIME.wMinute % 10); // un min
		timeAll[4] = (TIME.wSecond / 10); // dec sec
		timeAll[5] = (TIME.wSecond % 10);
		swprintf(outTime, 10, L"%i%i:%i%i:%i%i\0", timeAll[0], timeAll[1], timeAll[2], timeAll[3], timeAll[4], timeAll[5]);
		TextOut(hdc, 0, 400, L"����� �� ��������", 17);
		TextOut(hdc,
			100, 470,
			outTime, _tcslen(outTime));
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
	}
	return 0;
	case WM_RBUTTONDOWN:
		for (int i = 0; i < sizeof(time); i++)
		{
			time = 0;
		}
		KillTimer(hWnd, 1);
		pause = true;
		InvalidateRect(hWnd, &timePlace, TRUE);
		break;

	case WM_TIMER:
		time++;
		InvalidateRect(hWnd, &timePlace, TRUE);
		break;


	case WM_COMMAND:
	{

		//��������� ������� �� ������� , ������ ������� ������������� � ����������� ����� (������� - ��� �������� , ������ DATESTOPBUTTON ����� �������� 1601) 
		//��� ������� �� ������� ��������� ������ ������� WndProc � ����������� HWND ���� , � ������� ������ ������ 
		// uMsg - WM_COMMAND
		// LOWORD(wParam) - ������������� ������ 
		// LOWORD - ������ ����� , ����� ����� - ���-�� �� ���� AH � AL � assembly
		if (LOWORD(wParam) == DATESTOPBUTTON) {
			KillTimer(hWnd, 1);
			pause = true;
		}
		if (LOWORD(wParam) == CIRCLESTOPBUTTON) {
			TerminateThread(hThread2, 0); //����������� ������ � ����� 0 (��� 0 ������ ��� ��������/������� , ����� �������� ����� �����)
			CircleDrawing = false;
		}
		if (LOWORD(wParam) == CIRCLEBUTTON) {
			if (!CircleDrawing) {
				hThread2 = CreateThread(NULL, 0, Thread2, hWnd, 0, NULL); //�������� ������ , ����� ����� ������������� � ������������ ������� 
				CircleDrawing = true;
			}
			//� ������ ������ ������������� � ������� Thread2 ��������� ���� , ����� ����� ��������� �� ����������� �� �������� ������ ���������
// hThread2 ���������� � �������� ���-�� ����
		}
		if (LOWORD(wParam) == DATEBUTTON) {
			struct timeval tp;
			SetTimer(hWnd, 1, 1, NULL);
			time += TIME.wMilliseconds + TIME.wMinute * 60000 + TIME.wSecond * 1000 + TIME.wHour * 3600000;
			pause = false;
		}

	}
	return 0;
	case WM_CREATE:
		//WM_CREATE ����������� ����� ���� ���������
	{

		RECT Window;
		GetClientRect(hWnd, &Window); //��������� �������������� rect �������� �� ���������� �������� ����
		//�������� ������ - ������ ������������� � �������������� (HMENU)DATEBUTTON , ����� ��� ������� �������� � WM_COMMAND � � �����. if
		HWND CreateDButton = CreateWindow(L"BUTTON", L"����� � �����", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 500, 200, 50, hWnd, (HMENU)DATEBUTTON, NULL, NULL);
		HWND CreateCButton = CreateWindow(L"BUTTON", L"���������� ����", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 600, 200, 50, hWnd, (HMENU)CIRCLEBUTTON, NULL, NULL);
		HWND DeleteDButton = CreateWindow(L"BUTTON", L"���������� ����� � �����", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 550, 200, 50, hWnd, (HMENU)DATESTOPBUTTON, NULL, NULL);
		HWND DeleteCButton = CreateWindow(L"BUTTON", L"���������� ����", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 650, 200, 50, hWnd, (HMENU)CIRCLESTOPBUTTON, NULL, NULL);
	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
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
	hwnd = CreateWindow(wc.lpszClassName, L"���4 ��������", WS_OVERLAPPEDWINDOW, 0, 0, 600, 700, NULL, NULL, wc.hInstance, NULL);
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
	return static_cast<int>(msg.wParam);
}

