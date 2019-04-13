#include "sneka/asset.hpp"



namespace sneka {

	AssetLoadException::AssetLoadException(
			const std::string & asset,
			const std::string & cause
	):
			message("couldn't load \""+asset+"\" - "+cause),
			asset_name(asset), description(cause)
	{ }

	AssetLoadException::AssetLoadException(
			const std::string & asset,
			const char * cause
	):
			AssetLoadException::AssetLoadException(asset, std::string(cause))
	{ }

	const char * AssetLoadException::what() const noexcept {
		return message.c_str();
	}


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
