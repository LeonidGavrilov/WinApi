#include <Windows.h>
#include <float.h>
#include <math.h>
#include <windowsx.h>
#include <tchar.h>

int time = 0;
wchar_t outTime[256];
int timeAll[] = { 0, 0, 0, 0, 0, 0,0,0 };
bool pause = false;

//макросы для кнопок
#define CIRCLEBUTTON 1500
#define CIRCLESTOPBUTTON 1501
#define DATEBUTTON 1600
#define DATESTOPBUTTON 1601

#define M_PI 3.14159265358979323846 //макрос для числа пи (далее используется sin для изменения размеров круга


HANDLE hThread1; //управление потоками
HANDLE hThread2;


SYSTEMTIME TIME;


		//переменная типа SYSTEMTIME хранит в себе время (по гринвичу) есть элементы .wHour ; .wMinute и т.д

//функция для первого потока

bool CircleDrawing = false;//флаг работы потока отрисовки круга


DWORD WINAPI Thread2(LPVOID t) {
	HDC hdc = GetDC((HWND)t);  // HDC - Device Context Handle - -отвечает за вывод данных куда-либо  (также как выше)
	//бесконечный цикл(также как выше,также параллельно)
	SelectBrush(hdc, CreateSolidBrush(RGB(120, 40, 255))); //выбор кисти HBRUSH ,которую будет использовать Ellipse(...) для закраски внутренней части
	SelectPen(hdc, CreatePen(0, 3, RGB(255, 255, 255))); // выбор ручки HPEN ,которую будет использовать Ellipse(...) для границы

	//так как размер меняется плавно (значение x ниже с каждой итерацией меняется лишь на 0.01) граница нового круга будет заркашивать постепенно предыдущий
	//если убрать собственную кисть для границы - анимация не будет правильно работать
	while (true) {
		//для изменения размеров используется sin так как это периодичная функция , которая подходит для периодического изменения размера круга
		//ниже M_PI - макрос описанный выше - число пи
		for (double x = M_PI / 2; x < (5 * M_PI) / 2; x += 0.01) {
			//FillRect(hdc, &black, CreateSolidBrush(RGB(255, 255, 255)));
			HBRUSH brush = CreateSolidBrush(RGB(120 * (sin(x) + 1), 120 * (cos(x) + 1), 120 * (cos(x) + 1)));
			SelectObject(hdc, brush);
			Ellipse(hdc, 100 - 50 * (sin(x) + 1), 100 - 50 * (sin(x) + 1), 200 + 50 * (sin(x) + 1), 200 + 50 * (sin(x) + 1));
			//Эллипс рисуется по 4 точкам - (левая граница прямоугольника по x , верхняя граница прямоугольника по y, правая граница по x, нижняя по y)
			//получив прямоугольник , функция строит внутри него эллипс ,так как в данном случае прямоугольник является квадратом - происходит круг 
			Sleep(5); //задержка после отрисовки каждого кадра - 5 мсек , можно уменьшить еще больше ,чтобы ускорить анимацию 
					  //или увеличить ,чтобы замедлить	
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
		TextOut(hdc, 0, 400, L"Время по гринвичу", 17);
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

		//обработка нажатий на клавиши , каждая клавиша привязывается к уникальному числу (макросы - для удобства , вместо DATESTOPBUTTON можно написать 1601) 
		//при нажатии на клавишу вызовется данная функция WndProc с параметрами HWND окна , в котором нажали кнопку 
		// uMsg - WM_COMMAND
		// LOWORD(wParam) - идентификатор кнопки 
		// LOWORD - нижнее слово , часть слова - что-то по типу AH и AL в assembly
		if (LOWORD(wParam) == DATESTOPBUTTON) {
			KillTimer(hWnd, 1);
			pause = true;
		}
		if (LOWORD(wParam) == CIRCLESTOPBUTTON) {
			TerminateThread(hThread2, 0); //уничтожение потока с кодом 0 (код 0 только для удобства/отлдаки , можно написать любое число)
			CircleDrawing = false;
		}
		if (LOWORD(wParam) == CIRCLEBUTTON) {
			if (!CircleDrawing) {
				hThread2 = CreateThread(NULL, 0, Thread2, hWnd, 0, NULL); //создание потока , здесь поток привязывается к определенной функции 
				CircleDrawing = true;
			}
			//в данном случае привязывается к функции Thread2 описанной выше , поток будет выполнять ее параллельно от основого потока программы
// hThread2 глобальный и объявлен где-то выше
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
		//WM_CREATE выполняется когда окно создалось
	{

		RECT Window;
		GetClientRect(hWnd, &Window); //параметры прямоугольника rect беруться из актуальных размеров окна
		//создание кнопок - кнопка привзяывается к идентификатору (HMENU)DATEBUTTON , далее при нажатии поподаем в WM_COMMAND и в соотв. if
		HWND CreateDButton = CreateWindow(L"BUTTON", L"Поток с датой", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 500, 200, 50, hWnd, (HMENU)DATEBUTTON, NULL, NULL);
		HWND CreateCButton = CreateWindow(L"BUTTON", L"Меняющийся круг", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 600, 200, 50, hWnd, (HMENU)CIRCLEBUTTON, NULL, NULL);
		HWND DeleteDButton = CreateWindow(L"BUTTON", L"Остановить поток с датой", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 550, 200, 50, hWnd, (HMENU)DATESTOPBUTTON, NULL, NULL);
		HWND DeleteCButton = CreateWindow(L"BUTTON", L"Остановить круг", WS_CHILD | WS_VISIBLE, Window.right - 200, Window.bottom - 650, 200, 50, hWnd, (HMENU)CIRCLESTOPBUTTON, NULL, NULL);
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
	hwnd = CreateWindow(wc.lpszClassName, L"Лаб4 Гаврилов", WS_OVERLAPPEDWINDOW, 0, 0, 600, 700, NULL, NULL, wc.hInstance, NULL);
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

