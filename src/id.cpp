#include "id.hpp"



namespace gla {

	Id32::Id32(const std::string str) {
		bool ended = false;

		for(unsigned char i = 0; i < sizeof(chars); i+=1) {
			if(str.size() <= i) {
				ended = true;
			}

			if(ended) {
				chars[i] = '.';
			} else {
				chars[i] = str[i];
			}
		}
	}


	Id32::operator std::string() const {
		return std::string(chars, sizeof(chars));
	}


	bool Id32::operator == (const Id32 & r) const {
		for(unsigned char i = 0; i < sizeof(chars); i+=1) {
			if(chars[i] != r.chars[i])
				return false;
		}
		return true;
	}

	bool Id32::operator == (const std::string & r) const {
		return operator == (Id32(r));
	}

	bool Id32::operator == (const char * r) const {
		return operator == (Id32(r));
	}

	bool Id32::operator == (std::uint32_t r) const {
		for(unsigned char i = 0; i < sizeof(chars); i+=1) {
			if(static_cast<unsigned char>(chars[i]) != ((r >> (i*8)) & 0xFF))
				return false;
		}
		return true;
	}

}
