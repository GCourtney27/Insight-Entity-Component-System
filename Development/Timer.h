#pragma once

#include <chrono>


namespace Perf
{

	struct ManualTimer
	{
		ManualTimer(const char* ScopeName)
			: m_ScopeName(ScopeName),
			m_Time(0.0)
		{ }

		~ManualTimer() = default;

		/*
			Trackes the start time of the timer.
		*/
		inline void Start()
		{
			m_Start = std::chrono::steady_clock::now();
		}

		/*
			Tracks the end time of the timer and calculates the result.
		*/
		inline void Stop()
		{
			m_End = std::chrono::steady_clock::now();
			m_Time = static_cast<double>((m_End - m_Start).count());
			
			printf("Scope: (%s) executed in: (%f nano seconds)\n", m_ScopeName, m_Time);
		}

		/*
			Returns the amount of time the timer tracked in nanoseconds.
		*/
		inline double GetElapsedNanos()
		{
			return m_Time;
		}

		/*
			Returns the amount of time the timer tracked in miliseconds.
		*/
		inline double GetElapsedMilis()
		{
			return m_Time * 1000.0;
		}

	protected:
		std::chrono::steady_clock::time_point m_Start, m_End;
		const char* m_ScopeName;
		double m_Time;
	};
}
