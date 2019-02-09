#ifndef SONOGLPG_TRANSITION_HPP
#define SONOGLPG_TRANSITION_HPP



namespace gla {

	using transition_func = void (*)(float);


	class Transition {
	protected:
		float progress = 0.0f;
		float target;

		transition_func action;

	public:
		Transition(transition_func);
		Transition(transition_func, float max);

		// Return 'false' if the transition overflows, 'true' otherwise.
		bool advance(float);
		bool advance_no_excess(float);

		// Returns the excess before the last transition reset.
		float excess();
	};

}

#endif
