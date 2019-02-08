#ifndef SONOGLPG_UTILS_HPP
#define SONOGLPG_UTILS_HPP



template<typename T>
T within_bounds(T original, T lower, T higher) {
	while(original > higher) original += lower - higher;
	while(original < lower) original += higher - lower;
	return original;
}

template<>
float within_bounds<float>(float original, float lower, float higher);

#endif
