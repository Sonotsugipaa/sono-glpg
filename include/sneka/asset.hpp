#ifndef SNEKA_ASSET_HPP
#define SNEKA_ASSET_HPP

#include <string>
#include <map>



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
		virtual ~Asset() = default;

		/* By default, returns 'true' whenever the asset name is
		 * the same as the right operand - inheriting classes
		 * might choose to change or extend this behaviour. */
		virtual bool operator == (Asset&);
	};

	class AssetLoadException : public std::exception {
	public:
		const std::string msg;

		AssetLoadException(const std::string & asset_name, const char * cause_descr);
		AssetLoadException(const std::string & asset_name, const std::string & cause_descr);

		const char * what() const noexcept;
	};


	/* To anyone who needs to work on the AssetLoader<T> class:
	 * I sincerely apologize, and hope you find what you're
	 * looking for and accomplish whatever you're trying to do.
	 *
	 * Here's a list of pseudocode prototypes:
	   private virtual T* load(std::string asset_name);
	   public T& get(std::string asset_name);
	   public void clear();
	   public void unload(std::string asset_name);
	   public virtual void cleanup();
	 * Godspeed.
	 *
	 * P.S.
	 * DON'T TOUCH MY STUFF
	 */

	/* AssetLoader is meant to construct assets when needed,
	 * and holds a cache of already loaded assets.
	 *
	 * Note that the actual process of loading, allocating,
	 * unloading and freeing resources is meant to be hidden. */
	template<class T>
	class AssetLoader {
	public:
		// needed by destructor
		void clear() {
			auto iter = assets.begin();
			auto end = assets.end();

			while(iter != end) {
				delete iter->second;
				assets.erase(iter);
				iter = assets.begin();
			}
		}

	protected:
		std::map<std::string, T*> assets;

		/* Forces the asset to be loaded, even if it's already
		 * in the loaded assets container.
		 * Note that the return type is a pointer type, and may
		 * return a null pointer. */
		virtual T* load(std::string asset_name) = 0;
	public:
		virtual ~AssetLoader() { clear(); }

		T& get(std::string asset_name) {
			auto iter = assets.find(asset_name);

			if(iter == assets.end()) {
				T* loaded = load(asset_name);

				/* load(...) is not actually required to throw an
				 * exception when an object cannot be loaded
				 * (for some reason, I can't remember). */
				if(loaded != nullptr) {
					assets[asset_name] = loaded;
					iter = assets.find(asset_name);
				} else {
					throw AssetLoadException(asset_name, "unknown cause");
				}
			}

			return *(iter->second);
		}

		/* If the asset is not loaded, nothing happens. */
		void unload(std::string asset_name) {
			auto iter = assets.find(asset_name);
			if(iter != assets.end())
				assets.erase(iter);
		}

		/* Optionally implemented function that frees unneeded
		 * resources after loading - acts as a hint and no
		 * action is actually required to be performed. */
		virtual void cleanup() { }
	};

	using GenericAssetLoader = AssetLoader<Asset>;

}

#endif
