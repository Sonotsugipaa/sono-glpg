#ifndef SNEKA_ASSET_EXCEPT_HPP
#define SNEKA_ASSET_EXCEPT_HPP

#include "sneka/except.hpp"

#include "sneka/asset/ari.hpp"



namespace sneka {

	class AssetException: public Exception {
	public:
		Ari ari;
	};

	class AriException: public Exception {
	public:
	};

}

#endif
