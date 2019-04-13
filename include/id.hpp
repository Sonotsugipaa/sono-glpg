#include <cstdint>

#include <string>



namespace gla {

	/* Id32 is a short name of the type, which
	 * can also be represented by a 32-bit integer. */
	class Id32 {
	public:
		using uint_t = uint32_t;

	private:
		char chars[sizeof(uint_t)] = { };

	public:
		Id32(const std::string str);

		constexpr Id32(const char * str) {
			bool ended = false;
			for(unsigned char i = 0; i < sizeof(chars); i+=1) {
				if(str[i] == '\0') {
					ended = true;
				}
				if(ended) {
					chars[i] = '.';
				} else {
					chars[i] = str[i];
				}
			}
		}

		constexpr Id32(uint_t n) {
			for(unsigned char i = 0; i < sizeof(chars); i+=1) {
				chars[i] = (n >> (i*8)) & 0xFF;
			}
		}


		operator std::string() const;

		constexpr operator uint_t() const {
			std::uint64_t retn = 0;
			for(unsigned char i = 0; i < sizeof(chars); i+=1) {
				retn |= chars[i] << (i*8);
			}
			return retn;
		}

		bool operator == (const Id32 & right_operand) const;
		bool operator == (const std::string & right_operand) const;
		bool operator == (const char * right_operand) const;
		bool operator == (uint_t right_operand) const;

		inline bool operator != (const Id32 & right_operand) {
			return ! (operator == (right_operand));  }
		inline bool operator != (std::string right_operand) {
			return ! (operator == (right_operand));  }
		inline bool operator != (const char * right_operand) {
			return ! (operator == (right_operand));  }
		inline bool operator != (uint_t right_operand) {
			return ! (operator == (right_operand));  }
	};

}
