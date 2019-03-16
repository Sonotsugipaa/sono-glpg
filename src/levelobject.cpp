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

	float temp_parse_float(const std::string str) {
		int i;
		read_dec_int(str.c_str(), &i);
		return static_cast<float>(i) / 100.0f;
	}

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
		#pragma GCC warning "missing proper exception handling: ifstream, mesh loader"
		std::ifstream is = std::ifstream(nm);
		LevelFile file = LevelFile(is);

		gla::Id32 type = (std::string) file["type"];
		Mesh& mesh = mesh_loader.get(file["mesh"]);
		glm::vec4 color = glm::vec4(1.0f);

		{
			std::string color_str = file["color"];
			if(color_str.size() > 0) {
				std::smatch sm;
				if(std::regex_search(color_str, sm, color_regex)) {
					#pragma GCC warning "missing float parser, using a temp subst"
					color = glm::vec4(
							temp_parse_float(sm[1].str()),
							temp_parse_float(sm[4].str()),
							temp_parse_float(sm[7].str()),
							1.0f );
					std::string alpha = sm[11].str();
					if(alpha.size() > 0)
						color[3] = temp_parse_float(sm[11].str());
				}
			}
		}

		return new LevelObjectTemplate(nm, mesh, type, color);
	}

}
