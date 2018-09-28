#pragma once
#ifndef TIMER_H
#define TIMER_H
#include "PCH.h"

class Timer
{
public:
	Timer()
	{
		__int64 frequency;
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
		m_frequencyf = static_cast<float>(frequency);
		m_frequencyd = static_cast<double>(frequency);

		m_second_per_countf = 1 / m_frequencyf;
		m_second_per_countd = 1 / m_frequencyd;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_start_time);
		m_elapsed_count = m_start_time - m_start_time;
		m_elapsed_countf = static_cast<float>(m_elapsed_count);
		m_elapsed_countd = static_cast<double>(m_elapsed_count);

		m_elapsed_secondf = m_elapsed_countf * m_second_per_countf;
		m_elapsed_secondd = m_elapsed_countd * m_second_per_countd;

		m_delta_count = 0;
		m_delta_countf = 0;
		m_delta_countd = 0;
	}

	~Timer() {}

	void Update()
	{
		LARGE_INTEGER largeInt;
		QueryPerformanceCounter(&largeInt);

		__int64 m_curr_count = largeInt.QuadPart - m_start_time;
		m_delta_count = m_curr_count - m_elapsed_count;
		m_delta_countf = static_cast<float>(m_delta_count);
		m_delta_countd = static_cast<float>(m_delta_count);

		m_delta_secondf = m_delta_countf * m_second_per_countf;
		m_delta_secondd = m_delta_countf * m_second_per_countd;

		m_elapsed_count = m_curr_count;
		m_elapsed_countf = static_cast<float>(m_elapsed_count);
		m_elapsed_countd = static_cast<double>(m_elapsed_count);

		m_elapsed_secondf = m_elapsed_countf * m_second_per_countf;
		m_elapsed_secondd = m_elapsed_countd * m_second_per_countd;
	}

	float GetDeltaSecondF() const
	{
		return m_delta_secondf;
	}

	double GetDeltaSecondD() const
	{
		return m_delta_secondd;
	}

	float GetElapsedSecondF() const
	{
		return m_elapsed_secondf;
	}

	double GetElapsedSecondD() const
	{
		return m_elapsed_secondd;
	}

private:
	__int64 m_start_time;
	__int64 m_elapsed_count;
	__int64 m_delta_count;
	__int64 m_curr_count;

	float m_elapsed_countf;
	double m_elapsed_countd;

	float m_frequencyf;
	double m_frequencyd;

	float m_second_per_countf;
	double m_second_per_countd;

	float m_elapsed_secondf;
	double m_elapsed_secondd;

	float m_delta_countf;
	double m_delta_countd;

	float m_delta_secondf;
	double m_delta_secondd;
};
#endif // !TIMER_H
