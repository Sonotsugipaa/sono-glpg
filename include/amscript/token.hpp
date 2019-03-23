#ifndef SCRPN_TOKEN_HPP
#define SCRPN_TOKEN_HPP

#include <string>
#include <vector>



namespace amscript {

	class TokenType {
	private:
		long long unsigned int id;

	protected:
		std::size_t groups_n;
		std::string own_regex;

	public:
		TokenType(std::string regex);

		inline std::string getRegex() const { return own_regex; }
		inline std::size_t getRegexGroups() const { return groups_n; }

		constexpr long long unsigned int uid() const { return id; }

		constexpr bool operator == (const TokenType & r) const {
			return id == r.id;
		}
		constexpr bool operator != (const TokenType & r) const {
			return id != r.id;
		}

		operator bool() const;
	};


	class Token {
	private:
		std::vector<std::string> groups;
		TokenType type;

	public:
		Token();
		Token(const TokenType &, std::string source);

		constexpr const TokenType & getType() const { return type; }
		inline std::size_t size() const { return type.getRegexGroups(); }

		const std::string & operator [] (std::size_t index) const;
		operator bool() const;
	};


	class TokenBuilder {
	private:
		std::vector<TokenType> types;
		std::string regex_full;

	public:
		// tokens must not be deallocated before using this object
		TokenBuilder& operator << (const TokenType &);

		Token operator () (std::string source) const;
		const TokenType & getType(std::string source) const;
		std::string fullRegex() const;
	};

}

#endif
