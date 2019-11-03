#ifndef SNEKA_ASSET_ASSET_HPP
#define SNEKA_ASSET_ASSET_HPP

#include "sneka/asset/ari.hpp"



namespace sneka {

	class Asset {
	protected:
		Ari _ari;

	public:
		Asset(Ari);
		virtual ~Asset();

		inline const Ari& getAri() const { return _ari; }
	};

}

#endif
