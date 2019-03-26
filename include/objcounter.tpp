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

		bool original;

	public:

		ObjectCounter(value_t init = value_t()):
				counter(init),
				original(true)
		{ }

		ObjectCounter(ObjectCounter& cpy):
				reference(&cpy),
				original(false)
		{
			if(! reference->original) {
				reference = reference->reference;
			}

			reference->counter += 1;
		}

		ObjectCounter(ObjectCounter&&) = default;

		~ObjectCounter() {
			if(! original)
				reference->counter -= 1;
		}


		ObjectCounter& operator = (ObjectCounter& cpy) {
			if(original) {
				--counter;
			} else {
				--(reference->counter);
			}

			if(cpy.original) {
				reference = &cpy;
			} else {
				reference = cpy.reference;
			}

			original = false;
			++(reference->counter);

			return *this;
		}

		ObjectCounter& operator = (ObjectCounter&&) = default;


		bool isOriginal() const { return original; }

		value_t value() const {
			if(original) return counter;
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
