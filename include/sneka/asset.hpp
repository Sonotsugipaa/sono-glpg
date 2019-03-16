#ifndef SNEKA_ASSET_HPP
#define SNEKA_ASSET_HPP

#include <string>



namespace sneka {

	/* This class is meant to be virtually inherited by anything
	 * that can be created by an AssetLoader;
	 * depending on the name, the AssetLoader can decide that it
	 * can't be loaded and throw a corresponding
	 * AssetLoadException.
	 *
	 * Ideally, the name is preceded by a folder name and a slash,
	 * but it is not a requirement; however, any AssetLoader must
	 * be able to determine the type of asset (partially, at least)
	 * by its file extension.
	 *
	 * The name must only contain alphanumeric characters, dots
	 * or forward slashes. Spaces are not forbidden, but any
	 * AssetLoader is allowed not to accept them. */
	class Asset {
	public:
		const std::string asset_name;

		Asset(const char * type);
		Asset(const std::string & name);

		/* By default, returns 'true' whenever the asset name is
		 * the same as the right operand - inheriting classes
		 * might choose to change or extend this behaviour. */
		virtual bool operator == (Asset&);
	};


	template<class T>
	class AssetLoader {
	public:
		virtual T load(std::string asset_name) = 0;
	};

}

#endif
