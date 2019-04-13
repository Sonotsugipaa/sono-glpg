#include "boolarray.hpp"

#include "utils.tpp"

#define segm_t long long unsigned int

using namespace gla;



template<typename T>
constexpr T alloc_size(T bits) {
	return (bits + (sizeof(T) - 1)) / sizeof(T);
}


namespace {

	bool get_bool(const segm_t * data, std::size_t i) {
		// index OF the segment (i's segm)
		std::size_t i_segm = i / (sizeof(segm_t) * 8);
		// offset WITHIN the segment (segm's i)
		std::size_t segm_i = i % (sizeof(segm_t) * 8);

		return data[i_segm] & mask_set(segm_i);
	}

	bool set_bool(segm_t * data, std::size_t i, bool v) {
		// index OF the segment (i's segm)
		std::size_t i_segm = i / (sizeof(segm_t) * 8);
		// offset WITHIN the segment (segm's i)
		std::size_t segm_i = i % (sizeof(segm_t) * 8);

		if(v) {
			data[i_segm] |= mask_set(segm_i);
		} else {
			data[i_segm] &= mask_clr(segm_i);
		}

		return v;
	}

}


namespace gla {


	/* ----------  BoolArrayReference  ---------- */


	BoolArrayReference::BoolArrayReference(
			segm_t * array,
			std::size_t i
	):
			ref(array), index(i),
			val(get_bool(array, i))
	{ }

	BoolArrayReference::BoolArrayReference(const BoolArrayReference &)
			= default;
	BoolArrayReference::BoolArrayReference(BoolArrayReference &&)
			= default;


	BoolArrayReference& BoolArrayReference::operator = (const BoolArrayReference &)
			= default;
	BoolArrayReference& BoolArrayReference::operator = (BoolArrayReference&&)
			= default;


	bool BoolArrayReference::operator = (bool r) {
		val = r;
		set_bool(ref, index, r);
		return r;
	}


	std::ostream& operator << (
			std::ostream& os,
			BoolArrayReference ref
	) {
		os << (bool) ref;
		return os;
	}


	/* ----------  BoolArray  ---------- */


	BoolArray::BoolArray(std::size_t sz, bool init):
			size(sz), data(new segm_t[alloc_size(sz)])
	{
		for(std::size_t i=0; i < alloc_size(sz); i+=1) {
			data[i] = init?
					std::numeric_limits<segm_t>::max() :
					0;
		}
	}

	BoolArray::BoolArray(const BoolArray & cpy):
			BoolArray::BoolArray(cpy.size, false)
	{
		for(std::size_t i=0; i < alloc_size(cpy.size); i+=1)
			data[i] = cpy.data[i];
	}

	BoolArray::BoolArray(BoolArray && mov):
			BoolArray::BoolArray(static_cast<const BoolArray &>(mov))
	{ }


	BoolArray::~BoolArray() {
		delete[] data;
	}


	BoolArray& BoolArray::operator = (const BoolArray & r) {
		delete[] data;
		data = new segm_t[alloc_size(r.size)];
		size = r.size;
		for(std::size_t i=0; i < alloc_size(r.size); i+=1)
			data[i] = r.data[i];
		return *this;
	}

	BoolArray& BoolArray::operator = (BoolArray&& r) {
		return operator = (static_cast<const BoolArray &>(r));
	}


	BoolArrayReference BoolArray::operator [] (unsigned int i) {
		if(i >= size)
			throw BoolArrayOverflow(i, size);
		return BoolArrayReference(data, i);
	}

	const BoolArrayReference BoolArray::operator [] (unsigned int i) const {
		if(i >= size)
			throw BoolArrayOverflow(i, size);
		return BoolArrayReference(data, i);
	}

	void BoolArray::set(std::size_t i, bool b) {
		if(i >= size)
			throw BoolArrayOverflow(i, size);
		set_bool(data, i, b);
	}

	bool BoolArray::get(std::size_t i) {
		if(i >= size)
			throw BoolArrayOverflow(i, size);
		return get_bool(data, i);
	}


	BoolArray BoolArray::random(std::size_t sz, segm_t seed) {
		BoolArray retn = BoolArray(sz);
		for(std::size_t i=0; i < alloc_size(sz); i+=1) {
			retn.data[i] = xorshift(seed ^ static_cast<segm_t>(i));
		}
		return retn;
	}

	/* ----------  BoolArrayOverflow  ---------- */


	BoolArrayOverflow::BoolArrayOverflow(std::size_t i, std::size_t sz):
			what_str(
				std::string("tried to access element ") + std::to_string(i) +
				" of a boolean array with " + std::to_string(sz) + " elements."
			),
			index(i), array_size(sz)
	{ }

	const char * BoolArrayOverflow::what() const noexcept {
		return what_str.c_str();
	}

}

#undef segm_t
