#pragma once
#include <deque>

/**
 * Helpers.
 */

// Some platform dependant code to get
// needed resolution for timer.
#ifndef _WIN32
	#include <unistd.h>
#endif
#ifdef _WIN32
	#include <windows.h>
	typedef LARGE_INTEGER timetype;
#elif _POSIX_TIMERS > 0
	#include <sys/time.h>
	typedef timespec timetype;
#else
	typedef clock_t timetype;
#endif

/// Timer
struct Timer {
	Timer(): m_duration(0.0) {
		#ifdef _WIN32
			QueryPerformanceCounter(&m_start_time);
		#elif _POSIX_TIMERS > 0
			clock_gettime(CLOCK_MONOTONIC, &m_start_time);
		#else
			m_start_time = clock();
		#endif
	}

	double interval() {
		#ifdef _WIN32
			LARGE_INTEGER stop_time;
			QueryPerformanceCounter(&stop_time);
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			m_duration = (double)(stop_time.QuadPart - m_start_time.QuadPart) / (double)freq.QuadPart;
			m_start_time = stop_time;
		#elif _POSIX_TIMERS > 0
			timetype t;
			clock_gettime(CLOCK_REALTIME, &t);
			m_duration = (t.tv_sec - m_start_time.tv_sec) + (t.tv_nsec - m_start_time.tv_nsec) / 1000000000.0;
			m_start_time = t;
		#else
			m_duration = double(clock() - m_start_time) / CLOCKS_PER_SEC;
			m_start_time = clock();
		#endif
		return m_duration;
	}

private:
	timetype m_start_time;
	double m_duration;
};

/// FPS Counter
struct FPSCounter {
	FPSCounter(int memLength = 5): m_memory(memLength), m_timer() { }

	void operator()() {
		m_memory.pop_back();
		m_memory.push_front(m_timer.interval());
	}

	double getFPS() const {
		double sum = 0;
		for (size_t i = 0; i < m_memory.size(); ++i)
			sum += m_memory.at(i);
		return (int)(m_memory.size() / sum); // 1 / average time to render one frame
	}

private:
	std::deque<double> m_memory;
	Timer m_timer;
};
