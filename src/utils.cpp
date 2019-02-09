#include "utils.tpp"

using namespace std::chrono;



namespace gla {

	template<>
	float within_bounds<float>(float original, float lower, float higher) {
		while(original > higher) original += lower - higher;
		while(original < lower) original += higher - lower;
		return original;
	}


	Timer::Timer() {
		reset();
	}

	void Timer::reset() {
		time = system_clock::now();
	}

	float Timer::millis() {
		duration<double, std::micro> micros = system_clock::now() - time;
		return micros.count() / 1000.0;
	}

}
