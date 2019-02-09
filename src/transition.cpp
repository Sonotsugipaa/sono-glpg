#include "transition.hpp"



namespace gla {

	Transition::Transition(transition_func f, float max):
			target(max), action(f)
	{
		action(progress);
	}

	Transition::Transition(transition_func f):
			Transition::Transition(f, 1.0f)
	{ }


	bool Transition::advance(float adv) {
		progress += adv;
		action(progress);
		if(progress >= target) {
			do {
				progress -= target;
			} while(progress >= target);
			return false;
		}
		return true;
	}

	bool Transition::advance_no_excess(float adv) {
		progress += adv;
		if(progress >= target) {
			progress = 0.0f;
			action(progress);
			return false;
		}
		action(progress);
		return true;
	}

}
