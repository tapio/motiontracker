#pragma once

/**
 * Helpers
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
	Timer(std::string msg): m_msg(msg), m_duration(0.0) {
		#ifdef _WIN32
			QueryPerformanceCounter(&m_start_time);
		#elif _POSIX_TIMERS > 0
			clock_gettime(CLOCK_REALTIME, &m_start_time);
		#else
			m_start_time = clock();
		#endif
	}
	double stop() {
		#ifdef _WIN32
			LARGE_INTEGER stop_time;
			QueryPerformanceCounter(&stop_time);
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			m_duration = (double)(stop_time.QuadPart - m_start_time.QuadPart) / (double)freq.QuadPart;
		#elif _POSIX_TIMERS > 0
			timetype t;
			clock_gettime(CLOCK_REALTIME, &t);
			m_duration = (t.tv_sec - m_start_time.tv_sec) + (t.tv_nsec - m_start_time.tv_nsec) / 1000000000.0;
		#else
			m_duration = double(clock() - m_start_time) / CLOCKS_PER_SEC;
		#endif
			m_start_time += m_duration;
		return m_duration;
	}

	~Timer() { if (!m_duration) stop(); std::cout << m_msg << " " <<  m_duration << "s" << std::endl; }
  private:
	std::string m_msg;
	timetype m_start_time;
	double m_duration;
};

/// FPS Counter
struct FPSCounter {
	FPSCounter(int memLength): memoryLength(memLength), timer(Timer("")) {
		for (int i = 0; i < memoryLength; ++i) {
			memory.push_back(0.0);
		}
	}

	FPSCounter::operator()() {
		memory.pop_back();
		memory.push_front(timer.stop());
	}

	double getFPS() {
		double sum = 0;
		for (int i = 0; i < memoryLength; ++i)
			sum += memory.at(i);
		return memoryLength / sum; // 1 / average time to render one frame
	}

	private:
		Timer timer;
		std::deque<double> memory;
		int memoryLength;
};
