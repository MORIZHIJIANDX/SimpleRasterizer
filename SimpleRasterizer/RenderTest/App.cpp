#include "App.h"

std::shared_ptr<Context3D> App::m_context = nullptr;

App::App(const std::string &pName, HINSTANCE pHinstance, const size_t &pWidth, const size_t &pHeight): m_window(pHinstance, pName, pWidth, pHeight), m_timer()
{
	m_device = std::make_shared<Device3D>();
	m_context = std::make_shared<Context3D>();

	m_swap_chain = std::make_shared<SwapChain>(m_window.GetHWND(), pWidth, pHeight);

	for (size_t i = 0; i < m_timer_delta_sample_num; i++)
	{
		m_time_delta_buffer[i] = 0;
	}
}

App::~App()
{
	m_device = nullptr;
	m_context = nullptr;
	m_swap_chain = nullptr;
}

int App::Run()
{
	try
	{
		float update_frequency = 1 / 30.0f;
		float delta = 0.0f;

		Initialize();
		
		while (m_window.IsAlive())
		{
			if (!m_window.IsMinimized())
			{
				
				m_timer.Update();
				delta += m_timer.GetDeltaSecondF();

				m_swap_chain->ClearBackBuffer(Vec4<uint8_t>(0, 0, 0, 0));
				m_context->ClearDepthBuffer();

				if (delta >= update_frequency)
				{
					delta -= update_frequency;

					Update(update_frequency);

				}
				CalculateFPS();
				Render(m_timer.GetDeltaSecondF());
				m_swap_chain->Present();		
			}
			
			std::string fps_str = "FPS: " + std::to_string(m_fps);
			m_window.SetTitle(fps_str);
			m_window.MessageLoop();
		}
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}

void App::Initialize()
{

}

void App::Exit()
{
	m_window.Destroy();
}

void App::CalculateFPS()
{
	m_time_delta_buffer[m_curr_timer_sample] = m_timer.GetDeltaSecondF();
	m_curr_timer_sample = (m_curr_timer_sample + 1) % m_timer_delta_sample_num;

	float averageDelta = 0;
	for (size_t i = 0; i < m_timer_delta_sample_num; ++i)
		averageDelta += m_time_delta_buffer[i];
	averageDelta /= m_timer_delta_sample_num;

	m_fps = static_cast<size_t>(std::floor((1.0f / averageDelta) + 0.5f));
}

