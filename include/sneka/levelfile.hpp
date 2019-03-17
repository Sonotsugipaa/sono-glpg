#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <exception>



namespace sneka {

	class LevelFile {
	public:
		class Line {
			friend LevelFile;
		private:
			Line(const std::string & c_string);
		public:
			std::string key, value;
			int value_int;
			float value_float;

			Line() { }

			operator std::string() const;
			operator int() const;
			operator float() const;
		};

		using map_t = std::map<std::string, Line>;

	private:
		map_t values;

	public:
		LevelFile(std::istream&);
		LevelFile(LevelFile&) = delete;

		const Line & operator [] (std::string key);
	};


	class LevelFileException : public std::exception {
	public:
		const std::vector<std::string> error_messages;
		char const * const message;

		LevelFileException(
				const char * msg,
				const std::vector<std::string> & errors );

		constexpr const char * what() const noexcept {
			return message;
		}
	};

}
