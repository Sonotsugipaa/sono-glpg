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
				if(c == PATH_SEPARATOR) {
					if(first_path_sep) {
						location = std::move(buffer);
						first_path_sep = false;
					} else {
						path.push_back(std::move(buffer));
					}
					buffer.clear();
				} else {
					buffer.push_back(c);
				}
			}
			if(first_path_sep) {
				throw ex_invalid_format(std::move(src));
			} else {
				/* The ":/" and ":level/" ARIs have null paths, but all ARIs with more than
				 * one path separator have an empty last value, to differentiate between
				 * files and directories. To identify the root directory AS A DIRECTORY,
				 * "://" is used. */
				//if((! path.empty()) && (! buffer.empty())) {
				if(! (buffer.empty() && path.empty())) {
					path.push_back(std::move(buffer));
				}
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


	bool Ari::operator < (const Ari& cmp) const {
		if(location != cmp.location)  return (location < cmp.location);
		size_t least_size = path.size();
		if(cmp.path.size() < least_size)  least_size = cmp.path.size();
		for(size_t i=0; i < least_size; ++i) {
			if(path[i] != cmp.path[i])  return (path[i] < cmp.path[i]);
		}
		return path.size() < cmp.path.size();
	}


	std::string Ari::getSerial() const {
		std::string r = static_cast<std::string>(type) + ':';
		if(! location.empty())  r += location;
		r += PATH_SEPARATOR + getPathString();
		return r;
	}

	std::string Ari::getPathString() const {
		std::string r;
		r.reserve(path_to_str_heuristic(path.size()));
		if(! path.empty()) {
			r += path[0];
		}
		for(size_t i=1; i < path.size(); ++i) {
			r += '/' + path[i];
		}
		return r;
	}

}
