#include "sneka/asset/ari.hpp"

#include "sneka/asset/except.hpp"



namespace sneka {

	namespace {

		constexpr size_t str_to_path_heuristic(size_t path_length) {
			return path_length / 3;
		}

		constexpr size_t path_to_str_heuristic(size_t path_size) {
			return path_size * 8;
		}

		constexpr char PATH_SEPARATOR = '/';

		inline AriException ex_invalid_format(std::string str) {
			return AriException{
				ExceptionType::INVALID_ARI,
				"Invalid Asset Resource Identifier format: \"" + std::move(str) + '"'
			};
		}

		inline AriException ex_invalid_type(std::string str) {
			return AriException{
				ExceptionType::INVALID_ARI_TYPE,
				"Asset Resource Identifier type: \"" + std::move(str) + "\" not recognized"
			};
		}

		inline void separate_str(
				std::string src,
				std::string& location,
				std::vector<std::string>& path,
				size_t offset = 0
		) {
			std::string buffer;
			path.reserve(offset + str_to_path_heuristic(src.size()));
			bool first_path_sep = true;
			for(size_t i = offset; i < src.length(); ++i) {
				char c = src[i];
				if(c != PATH_SEPARATOR) {
					buffer.push_back(c);
				} else {
					if(first_path_sep) {
						location = std::move(buffer);
						first_path_sep = false;
					} else {
						path.push_back(std::move(buffer));
					}
					buffer.clear();
				}
			}
			if(first_path_sep) {
				throw ex_invalid_format(std::move(src));
			} else {
				path.push_back(std::move(buffer));
				first_path_sep = false;
			}
		}

	}


	Ari::Type::Type(std::string src) {
		if(src.empty())  enumerated = NONE;
		else if(src == "level")  enumerated = LEVEL;
		else if(src == "material")  enumerated = MATERIAL;
		else if(src == "mesh")  enumerated = MESH;
		else if(src == "game")  enumerated = GAME_STATE;
		else if(src == "chunk")  enumerated = CHUNK;
		else enumerated = UNRECOGNIZED;
	}

	Ari::Type::operator std::string () const {
		switch(enumerated) {
			case NONE:
			case UNRECOGNIZED:  return "";
			case LEVEL:  return "level";
			case MATERIAL:  return "material";
			case MESH:  return "mesh";
			case GAME_STATE:  return "game";
			case CHUNK:  return "chunk";
		}
		return "INTERNAL_ERROR";
	}


	Ari::Ari(Type type, std::vector<std::string> path_vec):
			type (std::move(type))
	{
		if(! path_vec.empty()) {
			path.reserve(path_vec.size() - 1);
			location = std::move(path_vec[0]);
			for(size_t i=1; i < path.size(); ++i) {
				path.push_back(std::move(path_vec[i]));
			}
		}
	}

	Ari::Ari(Type type, std::string path_str):
			type (std::move(type))
	{
		separate_str(std::move(path_str), location, path);
	}

	Ari::Ari(std::string serial) {
		size_t offset;
		{
			char c = '\0';
			for(offset=0; offset < serial.size(); ++offset) {
				c = serial[offset];
				if(c == ':')  break;
			}
			if(c == ':') {
				type = serial.substr(0, offset);
				if(type == Type::UNRECOGNIZED) {
					throw ex_invalid_type(serial.substr(0, offset));
				}
				++offset;
			} else {
				offset = 0;
			}
		}
		separate_str(std::move(serial), location, path, offset);
	}


	/* level:/
	 * level:/levels/lvl1.ams
	 * level:remote/ */
	std::string Ari::getSerial() const {
		std::string r = static_cast<std::string>(type) + ':';
		if(! location.empty())  r += PATH_SEPARATOR + location;
		r.reserve(r.size() + path_to_str_heuristic(path.size()));
		for(const std::string& str : path) {
			r += (PATH_SEPARATOR + str);
		}
		return r;
	}

}
