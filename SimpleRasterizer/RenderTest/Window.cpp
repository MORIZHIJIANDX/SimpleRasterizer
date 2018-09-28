#include "Window.h"

Window::Window(HINSTANCE pHinstance, const std::string &pName, const size_t &pClientWidth, const size_t &pClientHeight): m_hinstance(pHinstance), m_name(pName)
{
	if (pHinstance == NULL)
	{
		this->m_hinstance = GetModuleHandle(NULL);
	}

	MakeWindow();
	SetClientArea(pClientWidth, pClientHeight);
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

Window::~Window()
{
	::DestroyWindow(m_hwnd);
	UnregisterClass(m_name.c_str(), m_hinstance);
}

void Window::MakeWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = "RasterizerWindow";

	ATOM windowClass = RegisterClass(&wc);

	m_hwnd = CreateWindow(LPCSTR(windowClass), m_name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, nullptr, nullptr, m_hinstance, nullptr);
}

void Window::SetClientArea(const size_t &pClientWidth, const size_t &pClientHeight)
{
	RECT client_rect;
	SetRect(&client_rect, 0, 0, static_cast<int>(pClientWidth), static_cast<int>(pClientHeight));

	AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, false);

	::SetWindowPos(m_hwnd, HWND_NOTOPMOST, 0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top, SWP_NOMOVE);
}

void Window::GetClientArea(size_t &pClientWidth, size_t &pClientHeight)
{
	RECT client_rect;
	
	GetClientRect(m_hwnd, &client_rect);

	pClientWidth = client_rect.right - client_rect.left;
	pClientHeight = client_rect.bottom - client_rect.top;
}

void Window::SetWindowPos(const size_t &pX, const size_t &pY)
{
	::SetWindowPos(m_hwnd, HWND_NOTOPMOST, static_cast<int>(pX), static_cast<int>(pY), 0, 0, SWP_NOSIZE);
}

void Window::GetWindowPos(size_t &pX, size_t &pY)
{
	RECT window_rect;
	::GetWindowRect(m_hwnd, &window_rect);
	pX = window_rect.left;
	pY = window_rect.top;
}

void Window::SetTitle(const std::string &pContent)
{
	std::string title = m_name + " " + pContent;
	::SetWindowText(m_hwnd, title.c_str());
}

BOOL Window::IsAlive()
{
	return ::IsWindow(m_hwnd);
}

BOOL Window::IsMinimized()
{
	return ::IsIconic(m_hwnd);
}

HWND Window::GetHWND()
{
	return m_hwnd;
}

HINSTANCE Window::GetHinstance()
{
	return m_hinstance;
}

void Window::MessageLoop()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::Destroy()
{
	::DestroyWindow(m_hwnd);
	UnregisterClass(m_name.c_str(), m_hinstance);
}

LRESULT WINAPI Window::WndProc(HWND pHwnd, UINT pMsg, WPARAM pWParam, LPARAM pLParam)
{
	switch (pMsg)
	{
	case WM_CLOSE:
		DestroyWindow(pHwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(pHwnd, pMsg, pWParam, pLParam);
	}

	return 0;
}