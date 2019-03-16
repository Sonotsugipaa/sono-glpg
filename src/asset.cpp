#include "sneka/asset.hpp"



namespace {

	Asset::Asset(const char * name):
			asset_name(name)
	{ }

	Asset::Asset(const std::string & name):
			asset_name(name)
	{ }


	bool Asset::operator == (Asset& r) {
		return asset_name == r.asset_name;
	}

}
