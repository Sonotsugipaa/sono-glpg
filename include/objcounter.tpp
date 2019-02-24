#ifndef SONOGLPG_OBJCOUNTER_TPP
#define SONOGLPG_OBJCOUNTER_TPP

#include <stdexcept>

/*
 * Utility class used to count instances of objects.
 *
 * Creating a new counter will initialize it as an original counter
 * with value 0;
 * Constructing a counter from another one (original or not) will
 * increment the original counter's value, and deleting it will decrease
 * the counter.
 *
 * NOTE: none of this is thread-safe.
*/



namespace gla {

	template<typename value_t = unsigned int>
	class ObjectCounter {
	protected:
		union {
			value_t counter;
			ObjectCounter<value_t>* reference;
		};

	public:
		const bool isOriginal;

		ObjectCounter(value_t init = value_t()):
				counter(init),
				isOriginal(true)
		{ }

		ObjectCounter(ObjectCounter& cpy):
				reference(&cpy),
				isOriginal(false)
		{
			if(! reference->isOriginal) {
				reference = reference->reference;
			}

			reference->counter += 1;
		}

		ObjectCounter(ObjectCounter&&) = delete;

		ObjectCounter& operator = (ObjectCounter&) = delete;
		ObjectCounter& operator = (ObjectCounter&&) = delete;

		~ObjectCounter() {
			if(! isOriginal)
				reference->counter -= 1;
		}

		value_t value() const {
			if(isOriginal) return counter;
			return reference->counter;
		}

		operator value_t () const {
			return value();
		}
	};

	#ifndef SONOGLPG_OBJCOUNTER_EXTERN_TEMPLATE
		extern template class ObjectCounter<unsigned int>;
	#endif

}

#endif
