#ifndef SNEKA_ASSET_ARI_HPP
#define SNEKA_ASSET_ARI_HPP

#include <vector>
#include <string>



namespace sneka {

	class Ari {
	public:
		struct Type {
			enum type_enum_t {
				NONE, UNRECOGNIZED,
				LEVEL, MATERIAL, MESH,
				GAME_STATE, CHUNK
			} enumerated;
			inline Type(): enumerated (NONE) { }
			inline Type(type_enum_t enumd): enumerated (enumd) { }
			Type(std::string);
			operator std::string () const;
			inline bool operator == (type_enum_t eq) const { return eq == enumerated; }
		};

		Type type;
		std::string location;
		std::vector<std::string> path;

		Ari(Type, std::vector<std::string> path);
		Ari(Type, std::string path);
		Ari(std::string serial);

		inline Ari(): type (Type::NONE), location (), path () { }

		std::string getSerial() const;
		std::string getPathString() const;
		inline bool isNull() const { return location.empty() && path.empty(); }
		inline bool isRoot() const { return path.empty(); }
	};

}

#endif
