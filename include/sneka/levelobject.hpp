#ifndef SNEKA_LEVELOBJECT_HPP
#define SNEKA_LEVELOBJECT_HPP

#include "sneka/gridobject.hpp"

#include "objcounter.tpp"
#include "id.hpp"



namespace sneka {

	using counter_t = unsigned int;
	using LevelObjectCounter = gla::ObjectCounter<counter_t>;


	class LevelObject : public GridObject {
	protected:
	public:
		LevelObjectCounter counter;
		const gla::Id32 type;

		LevelObject(const Mesh &, gla::Id32 type);
		virtual ~LevelObject() = default;
	};


	class LevelObjectTemplate : public virtual Asset {
	public:
		const Mesh & mesh;
		const gla::Id32 type;
		const glm::vec4 color;

		LevelObjectTemplate(
				std::string asset_name,
				const Mesh & mesh,
				gla::Id32 type,
				glm::vec4 color = glm::vec4(1.0f) );

		operator LevelObject() const;
	};


	/* LevelObjectLoader handles LevelObjectTemplates instead of
	 * plain LevelObjects, because AssetLoaders provide references
	 * to already existing objects;
	 * each LevelObject is actually a distinct object in the level
	 * that shares the loaded properties.
	 * To instantiate a LevelObject, use LevelObjectTemplate's
	 * cast operator. */
	class LevelObjectLoader : public AssetLoader<LevelObjectTemplate> {
	private:
		MeshLoader& mesh_loader;

	public:
		LevelObjectLoader(MeshLoader& mesh_loader);

		virtual LevelObjectTemplate* load(std::string templ_file_name);
	};

}

#endif
