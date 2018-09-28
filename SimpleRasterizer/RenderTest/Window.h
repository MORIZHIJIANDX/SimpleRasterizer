#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "PCH.h"

class Window
{
public:
	Window(HINSTANCE pHinstance, const std::string &pName, const size_t &pClientWidth, const size_t &pClientHeight);
	~Window();

	void SetClientArea(const size_t &pClientWidth, const size_t &pClientHeight);
	void GetClientArea(size_t &pClientWidth, size_t &pClientHeight);

	void SetWindowPos(const size_t &pX, const size_t &pY);
	void GetWindowPos(size_t &pX, size_t &pY);

	void SetTitle(const std::string &pContent);

	BOOL IsAlive();

	BOOL IsMinimized();

	HWND GetHWND();
	HINSTANCE GetHinstance();

	void MessageLoop();

	void Destroy();
private:
	void MakeWindow();

	static LRESULT WINAPI WndProc(HWND pHwnd, UINT pMsg, WPARAM pWParam, LPARAM pLParam);

	HINSTANCE m_hinstance;
	HWND m_hwnd;
	std::string m_name;
};
#endif // !WINDOW_H
