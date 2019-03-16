#include "sneka/levelfile.hpp"

#include "read_utils.hpp"

#include <limits>



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
		if(ln.key == "\033error") {
			#pragma GCC warning "add proper error handling"
		} else if(ln.key.size() > 0) {
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
			key = "\033error";
			value = "could not parse \"" + str + "\"";
		}

		{ // parse float
			if(std::regex_match(value, sm, float_regex)) {
				int n;
				float df;
				{
					int d;
					if(! read_dec_int(sm[1].str().c_str(), &n))  n = 0;
					if(! read_dec_int(sm[3].str().c_str(), &d))  d = 0;
					df = d;
					while(df >= 1.0f) {
						df /= 10.0f;
					}
				}

				value_float = static_cast<float>(n) + df;
			} else {
				value_float = std::numeric_limits<float>::quiet_NaN();
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
		std::stringstream* buffer = new std::stringstream();
		char c;

		while(is) {
			while(is) {
				c = is.get();
				if(c == -1) break;

				if(c_delim(c)) {
					break;
				} else {
					(*buffer) << c;
				}

				pos += 1;
			}

			if(*buffer) {
				Line ln = Line(buffer->str());
				add_line(values, ln);
				delete buffer;
				if(is) {
					buffer = new std::stringstream();
				}
			}
		}
	}

	const LevelFile::Line & LevelFile::operator [] (std::string k) {
		map_t::iterator iter = values.find(k);
		if(iter == values.end()) {
			return empty_line;
		} else {
			return iter->second;
		}
	}

}
