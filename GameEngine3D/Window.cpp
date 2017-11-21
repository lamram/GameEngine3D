#include <Windows.h>
#include "Window.h"

BEGIN_NAMESPACE

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window(const std::string &title) : m_title(title)
{
	m_width = 800;
	m_height = 500;
	m_x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_width / 2);
	m_y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (m_height / 2);
}

HWND Window::OpenWindow(const HINSTANCE &hInstance, const int &cmdShow) {
	ATOM atom = RegisterWindowProc(hInstance, WindowProc, "window");
	if (!atom) {
		return NULL;
	}
	HWND hWnd = CreateWindowEx(NULL, "window", m_title.c_str(), WS_OVERLAPPEDWINDOW, m_x, m_y, m_width, m_height, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, cmdShow);
	return hWnd;
}

void Window::SetSize(int width, int height) {
	m_width = width;
	m_height = height;
}

ATOM Window::RegisterWindowProc(const HINSTANCE &hInstance, const WNDPROC &wndProc, const std::string &className) {
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = wndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = className.c_str();
	return RegisterClassEx(&wc);
}

END_NAMESPACE