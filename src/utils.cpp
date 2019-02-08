#include "utils.tpp"



template<>
float within_bounds<float>(float original, float lower, float higher) {
	while(original > higher) original += lower - higher;
	while(original < lower) original += higher - lower;
	return original;
}
