#include "sneka/levelfile.hpp"

#include "read_utils.hpp"

#include <limits>
#include <regex>

#define ERROR_KEY "\033error"



namespace {

	using namespace sneka;

	const LevelFile::Line empty_line = LevelFile::Line();
	const std::regex line_regex = std::regex(
			"[\\s\\n\\r]*"
			"(\\w+)"
			"[\\s\\n\\r]*"
			":"
			"[\\s\\n\\r]*"
			"([[:print:]]+)"
			"[\\s\\n\\r]*" );
	const std::regex float_regex = std::regex("(\\d+)([\\.,](\\d+))?");
	const std::regex null_regex = std::regex("[\\s\\n\\r]*");

	bool c_delim(char c) {
		return
				(c == '\n') ||
				(c == '\r');
	}

	inline void add_line(
			LevelFile::map_t& values,
			LevelFile::Line& ln
	) {
		if(ln.key.size() > 0) {
			values[ln.key] = ln;
		}
	}

}


namespace sneka {

	LevelFile::Line::Line(const std::string & str) {
		std::smatch sm;

		if(std::regex_match(str, sm, line_regex)) {
			key = sm[1].str();
			value = sm[2].str();
		} else
		if(std::regex_match(str, sm, null_regex)) {
			key = value = "";
		} else {
			key = ERROR_KEY;
			value = "could not parse \"" + str + "\"";
		}

		{ // parse float
			if(std::regex_match(value, sm, float_regex)) {
				try {
					value_float = std::stof(value);
				} catch(std::logic_error err) {
					value_float = std::numeric_limits<float>::signaling_NaN();
				}
			} else {
				value_float = std::numeric_limits<float>::signaling_NaN();
			}
		}

		value_int = value_float;
	}

	LevelFile::Line::operator std::string() const {
		return value;
	}

	LevelFile::Line::operator int() const {
		return value_int;
	}

	LevelFile::Line::operator float() const {
		return value_float;
	}


	LevelFile::LevelFile(std::istream& is) {
		unsigned int pos = 0;
		unsigned int line = 1;
		std::stringstream* buffer = new std::stringstream();
		std::vector<std::string> errors;
		char c;

		while(is) {
			while(is.get(c)) {
				if(c_delim(c)) {
					break;
				} else {
					(*buffer) << c;
				}

				pos += 1;
				if(is.bad())
					throw LevelFileException("corrupt input stream", errors);
			}

			if(*buffer) {
				Line ln = Line(buffer->str());
				if(ln.key == ERROR_KEY) {
					errors.push_back(
							"invalid line \""+ln.value+"\""
							" ("+std::to_string(line)+")" );
				} else {
					add_line(values, ln);
				}
				delete buffer;
				if(is) {
					buffer = new std::stringstream();
					line += 1;
				}
			}
		}

		if(errors.size() > 0)
			throw LevelFileException("invalid file format", errors);
	}

	const LevelFile::Line & LevelFile::operator [] (std::string k) {
		map_t::iterator iter = values.find(k);
		if(iter == values.end()) {
			return empty_line;
		} else {
			return iter->second;
		}
	}


	LevelFileException::LevelFileException(const char * msg, const std::vector<std::string>& errors):
			error_messages(errors), message(msg)
	{ }

}
