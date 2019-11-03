#ifndef SNEKA_EXCEPT_HPP
#define SNEKA_EXCEPT_HPP



namespace sneka {

	enum class ExceptionType {
		UNDEFINED,
		INVALID_ARI, INVALID_ARI_TYPE,
		RESOURCE_NOT_FOUND
	};


	class Exception {
	public:
		ExceptionType type;
		std::string description;
	};

}

#endif
