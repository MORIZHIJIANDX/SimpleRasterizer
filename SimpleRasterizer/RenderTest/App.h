#pragma once
#ifndef APP_H
#define APP_H

#include "PCH.h"
#include "Window.h"
#include "RenderInterface.h"
#include "Timer.h"

class App
{
public:
	App(const std::string &pName = " ", HINSTANCE pHinstance = NULL, const size_t &pWidth = 512, const size_t &pHeight = 512);
	~App();

	int Run();

	static std::shared_ptr<Context3D> GetContext()
	{
		return m_context;
	}

protected:
	virtual void Initialize();

	virtual void Update(const float &pDelta) = 0;
	virtual void Render(const float &pDelta) = 0;

	void Exit();

	void CalculateFPS();

	Window m_window;
	Timer m_timer;
	std::shared_ptr<Device3D> m_device;
	static std::shared_ptr<Context3D> m_context;
	std::shared_ptr<SwapChain> m_swap_chain;

	static const size_t m_timer_delta_sample_num = 64;
	float m_time_delta_buffer[m_timer_delta_sample_num];
	size_t m_curr_timer_sample;
	size_t m_fps;

	std::string m_name;
};

#endif // !APP_H
