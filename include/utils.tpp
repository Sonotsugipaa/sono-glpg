#ifndef SONOGLPG_UTILS_TPP
#define SONOGLPG_UTILS_TPP

#include <chrono>



namespace gla {

	class Timer {
	protected:
		std::chrono::steady_clock::time_point
			time;

	public:
		Timer();
		Timer(Timer const &) = default;
		Timer(Timer&&) = default;

		void reset();
		float s();
		float millis();
		float micros();

		Timer& operator = (const Timer &) = default;
		Timer& operator = (Timer&&) = default;
	};


	template<typename T>
	T within_bounds(T original, T lower, T higher) {
		while(original > higher) original += lower - higher;
		while(original < lower) original += higher - lower;
		return original;
	}

	template<>
	float within_bounds<float>(float original, float lower, float higher);


	#define UNIT(X)  (X*sizeof(T))
	template<typename T>
	T xorshift(T x) {
		x ^= (x << (UNIT(3)))   ^ (x >> (UNIT(3)+1));
		x ^= (x << (UNIT(2)))   ^ (x >> (UNIT(6)));
		x ^= (x >> (UNIT(2)+1)) ^ (x << (UNIT(6)-1));
		x ^= (x << (UNIT(5)-1)) ^ (x << (UNIT(4)+3));
		x ^= (x >> (UNIT(5)-5));
		return x;
	}
	#undef UNIT

}

#endif
