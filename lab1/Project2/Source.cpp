#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");


int WINAPI _tWinMain(HINSTANCE This,		// Дескриптор текущего приложения 
						HINSTANCE Prev,		// В современных системах всегда 0 
						LPTSTR cmd,			// Командная строка 
						int mode)			// Режим отображения окна
{
	HWND hWnd;		// Дескриптор главного окна программы 
	MSG msg; 		// Структура для хранения сообщения 
	WNDCLASS wc; 	// Класс окна
	// Определение класса окна 
	wc.hInstance = This;
	wc.lpszClassName = WinName; 					// Имя класса окна 
	wc.lpfnWndProc = WndProc; 						// Функция окна 
	wc.style = CS_HREDRAW | CS_VREDRAW; 			// Стиль окна 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 	// Стандартная иконка 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// Стандартный курсор 
	wc.lpszMenuName = NULL; 						// Нет меню 
	wc.cbClsExtra = 0; 								// Нет дополнительных данных класса 
	wc.cbWndExtra = 0; 								// Нет дополнительных данных окна 
	wc.hbrBackground = CreateSolidBrush(RGB(48, 213, 200));


	// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;

	// Создание окна 
	hWnd = CreateWindow(WinName,			// Имя класса окна 
		_T("LAB1"), 		// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна 
		CW_USEDEFAULT,				// x 
		CW_USEDEFAULT, 				// y	 Размеры окна 
		400, 				// width 
		400, 				// Height 
		HWND_DESKTOP, 				// Дескриптор родительского окна 
		NULL, 						// Нет меню 
		This, 						// Дескриптор приложения 
		NULL); 					// Дополнительной информации нет 

	ShowWindow(hWnd, mode); 				// Показать окно

	// Цикл обработки сообщений 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 		// Функция трансляции кодов нажатой клавиши 
		DispatchMessage(&msg); 		// Посылает сообщение функции WndProc() 
	}
	return 0;
}

int time = 0;
wchar_t outTime[256];
int timeAll[] = { 0, 0, 0, 0, 0, 0};
HFONT font;
bool pause = false;

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;	// Дескриптор контекста устройства. 

	/*TCHAR top_clock[] = _T("0");
	TCHAR left_clock[] = _T("45");
	TCHAR right_clock[] = _T("15");
	TCHAR bottom_clock[] = _T("30");*/

	/*double x_arrow = 250;
	double y_arrow = 100;
	double radius = 75;*/


	switch (message)		 // Обработчик сообщений
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
		hdc = BeginPaint(hWnd, &ps);	// Функция BeginPaint подготавливает указанное окно для рисования и заполняет структуру PAINTSTRUCT информацией о картине.
		//SelectObject(hdc, CreateSolidBrush(RGB(128, 0, 0)));
		SetTextColor(hdc, RGB(0, 128, 0));	// Функция SetTextColor устанавливает цвет текста для указанного контекста устройства в указанный цвет.
		SetTextCharacterExtra(hdc, 3);	/* Функция SetTextCharacterExtra задает межхарактеристический интервал. Межхарактеристический интервал добавляется к каждому символу,
		включая символы разрыва, когда система записывает строку текста.*/

		timeAll[0] = (time / 60000); // dec min
		timeAll[1] = (time / 6000) % 10; // un min
		timeAll[2] = (time / 1000) % 6; // dec sec
		timeAll[3] = (time / 100) % 10; // un sec
		timeAll[4] = (time / 10) % 10; // dec millisec
		timeAll[5] = (time % 10); // un millisec

		swprintf(outTime, L"%i%i:%i%i:%i%i\0", timeAll[0], timeAll[1], timeAll[2], timeAll[3], timeAll[4], timeAll[5]);	// Запись форматированных данных в строку. 
		SetTextColor(hdc, RGB(0, 0, 128));
		SetBkColor(hdc, RGB(48, 213, 200));
		font = CreateFont(30, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Comic Sans MS");
		SelectObject(hdc, font);
		
		TextOut(hdc, 130, 150, outTime, _tcslen(outTime));	// Функция TextOut записывает символьную строку в указанное место, используя текущий выбранный шрифт, цвет фона и цвет текста.

		ReleaseDC(hWnd, hdc);	// Функция ReleaseDC освобождает контекст устройства 

		EndPaint(hWnd, &ps);	// Функция EndPaint отмечает окончание рисования в указанном окне.
		break;


	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);

		break; 			// Завершение программы 
	default: 			// Обработка сообщения по умолчанию 
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}