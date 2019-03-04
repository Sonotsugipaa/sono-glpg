#ifndef SONOGLPG_BOOLARRAY_HPP
#define SONOGLPG_BOOLARRAY_HPP

#include <ostream>
#include <exception>
#include <limits>
#include <string>

#define segm_t long long unsigned int



namespace gla {

	constexpr segm_t mask_set(std::size_t index) {
		return 1 << index;  }
	constexpr segm_t mask_clr(std::size_t index) {
		return std::numeric_limits<segm_t>::max() ^ (1 << index);  }


	class BoolArrayReference {
	protected:
		segm_t * ref;
		std::size_t index;
		bool val;

	public:
		BoolArrayReference(segm_t *, std::size_t index);
		BoolArrayReference(const BoolArrayReference &);
		BoolArrayReference(BoolArrayReference&&);


		BoolArrayReference& operator = (const BoolArrayReference &);
		BoolArrayReference& operator = (BoolArrayReference&&);

		constexpr operator bool () const { return val; }

		bool operator = (bool right_op);
	};

	std::ostream& operator << (
			std::ostream& o_stream,
			BoolArrayReference );


	class BoolArray {
		friend class BoolArrayReference;

	private:

		std::size_t size;
		segm_t * data;

	public:
		BoolArray(std::size_t size, bool init = false);
		BoolArray(const BoolArray &);
		BoolArray(BoolArray&&);
		~BoolArray();

		template<typename int_t>
		BoolArray(int_t* copy_data, std::size_t size):
				BoolArray(size)
		{
			std::size_t alloc_size =
					(size + (sizeof(std::size_t) - 1)) / sizeof(std::size_t);

			for(std::size_t i=0; i < alloc_size; i+=1) {
				data[i] = copy_data[i];
			}
		}

		BoolArray& operator = (const BoolArray & right_op);
		BoolArray& operator = (BoolArray&& right_op);

		/* Note: using a BoolArrayReference for a single R/W operation
		 * is much less efficient than get(...) or set(...), therefore
		 * it's best to only use it when actual references are needed
		 * (since there is no physical memory address that contains less
		 * than std::numeric_limits<T>::max() bits). */
		BoolArrayReference operator [] (unsigned int index);
		const BoolArrayReference operator [] (unsigned int index) const;
		void set(std::size_t index, bool bit);
		bool get(std::size_t index);

		constexpr std::size_t getSize() const { return size; }

		static BoolArray random(std::size_t size, segm_t seed);
	};


	class BoolArrayOverflow : public std::exception {
	private:
		const std::string what_str;

	public:
		const std::size_t index;
		const std::size_t array_size;

		BoolArrayOverflow(std::size_t index, std::size_t array_size);
		const char * what() const noexcept;
	};

}

#undef segm_t

#endif // #ifndef SONOGLPG_BOOLARRAY_HPP
