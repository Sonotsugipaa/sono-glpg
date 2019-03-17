#include "sneka/levelobject.hpp"

#include <fstream>
#include <regex>

#include "read_utils.hpp"

#include "sneka/levelfile.hpp"



namespace {

	sneka::LevelObjectCounter shcounter;
	const std::regex color_regex = std::regex(
		"(\\d+(\\.(\\d+))?)\\s+"     //  1,  3
		"(\\d+(\\.(\\d+))?)\\s+"     //  4,  6
		"(\\d+(\\.(\\d+))?)"         //  7,  9
		"(\\s+(\\d+(\\.(\\d+))?))?"  // 11, 13
	);

}


namespace sneka {

	LevelObjectTemplate::LevelObjectTemplate(
		std::string asset,
		const Mesh & mesh,
		gla::Id32 type,
		glm::vec4 color
	):
			Asset::Asset(asset),
			mesh(mesh), type(type), color(color)
	{ }

	LevelObjectTemplate::operator LevelObject() const {
		LevelObject retn = LevelObject(mesh, type);
		retn.setColor(color);
		return retn;
	}


	LevelObject::LevelObject(
			const Mesh & mesh, gla::Id32 type
	):
			GridObject::GridObject(mesh),
			counter(shcounter),
			type(type)
	{ }


	LevelObjectLoader::LevelObjectLoader(MeshLoader& ml):
			mesh_loader(ml)
	{ }


	LevelObjectTemplate* LevelObjectLoader::load(std::string nm) {
		std::ifstream is = std::ifstream(nm);
		LevelFile file = LevelFile(is);

		if(! is.is_open()) {
			throw AssetLoadException(nm, "could not open file \""+nm+"\"");
		}

		std::string mesh_str = file["mesh"];
		if(mesh_str.size() <= 0) {
			throw AssetLoadException(nm, "missing \"mesh\" value");
		}

		gla::Id32 type = (std::string) file["type"];
		try {
			Mesh& mesh = mesh_loader.get(mesh_str);

			glm::vec4 color = glm::vec4(1.0f);

			{
				std::string color_str = file["color"];
				if(color_str.size() > 0) {
					std::smatch sm;
					if(std::regex_search(color_str, sm, color_regex)) {
						try {
							color = glm::vec4(
									std::stof(sm[1].str()),
									std::stof(sm[4].str()),
									std::stof(sm[7].str()),
									1.0f );
							std::string alpha = sm[11].str();
							if(alpha.size() > 0)
								color[3] = std::stof(sm[11].str());
						} catch(std::logic_error err) {
							color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
						}
					}
				}
			}

			return new LevelObjectTemplate(nm, mesh, type, color);
		} catch(AssetLoadException& ex) {
			throw AssetLoadException(
					ex.asset_name,
					ex.description+" while loading "+nm+"" );
		}
	}

}
