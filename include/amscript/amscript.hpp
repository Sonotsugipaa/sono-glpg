#ifndef SCRPN_AMSCRIPT_HPP
#define SCRPN_AMSCRIPT_HPP

#include <exception>
#include <string>
#include <istream>
#include <map>
#include <vector>

#define ASCRIPT_FILE_SIZE_MAX (1024*30)



namespace amscript {

	// Class forward declarations
	class Exception;
	class LogicException;
	class RuntimeException;
	class UndefinedNameException;
	class Symbol;
	class Function;
	class Amscript;


	class Exception : public std::exception {
	private:
		char * const message;

	public:
		Exception(const std::string &);
		Exception(const char *);
		constexpr const char * what() const noexcept { return message; };
	};


	/* Thrown before parsing any script */
	class LogicException : public Exception {
	public:
		LogicException(const std::string &);
		LogicException(const char *);
	};


	/* Thrown when a script runtime encounters an error */
	class RuntimeException : public Exception {
	public:
		RuntimeException(const std::string &);
		RuntimeException(const char *);
	};


	/* Thrown when the parser can't find a necessary symbol */
	class UndefinedNameException : public Exception {
	public:
		UndefinedNameException(const std::string &);
		UndefinedNameException(const char *);
	};


	class Symbol {
	private:
		std::string str;
		bool is_ref;

	public:
		Symbol();
		Symbol(std::string value, bool is_reference);

		inline std::string value() const { return str; }
		inline bool isReference() const { return is_ref && (! isNull()); }
		inline bool isConst() const { return ! is_ref && (! isNull()); }
		inline bool isNull() const { return str.empty(); }

		inline operator std::string() const { return value(); }
	};


	class Function {
		friend Amscript;  // needed for Amscript::setFunction(...)
	public:
		using ptr_t = std::vector<std::string> (*)(std::vector<std::string>);

	private:
		std::string name;
		std::vector<std::string> args, body;
		ptr_t pointer;
		unsigned int args_n;
		bool is_builtin;

	public:
		Function();
		Function(
				std::string name,
				std::string arguments,
				std::string body,
				ptr_t func_ptr = nullptr );

		inline std::string getName() const { return name; }
		inline const std::vector<std::string> & getArguments() const { return args; }
		inline const std::vector<std::string> & getBody() const { return body; }
		inline ptr_t getFunctionPointer() const { return pointer; }
		inline bool isBuiltin() const { return is_builtin; }
		inline void setFunctionPointer(ptr_t ptr) { pointer = ptr; }
		inline operator bool() const { return name.empty(); }

		std::vector<std::string> eval(
				Amscript& script,
				const std::vector<std::string> & arguments );

		std::vector<std::string> eval(
				Amscript& script,
				std::string arguments );
	};


	class Amscript {
	public:
		using map_t = std::map<std::string, std::string>;

	private:
		map_t symbols_indirect;
		map_t symbols_direct;
		std::map<std::string, Function> functions;

		void parse(const std::string & str);

	public:
		Amscript(
				std::istream& input_stream,
				std::size_t max_size = ASCRIPT_FILE_SIZE_MAX );

		/* Attempts to fetch all tokens in a function;
		 * throws a RuntimeException on non-trivial failure. */
		std::vector<std::string> compute(
				std::string function_name,
				std::string arguments );

		void setFunction(
				std::string name,
				unsigned int max_args,
				Function::ptr_t function_ptr );

		/* Try to remove a function; returns 'false' if the function
		 * was not found. */
		bool removeFunction(std::string name);

		Function* getFunction(std::string name);
		const Function * getFunction(std::string name) const;

		void setSymbol(const Symbol * symbol);

		/* Returns the symbol pointed to by 'symbol_name'. */
		Symbol getSymbol(
				std::string symbol_name,
				const map_t * temp_defs_map = nullptr
		) const;

		/* Attempts to return the final value referenced by
		 * the given symbol name; returns an empty string on
		 * failure. */
		std::string resolveSymbol(
				std::string symbol_name,
				const map_t * temp_defs_map = nullptr
		) const;

		const map_t & getSymbols(bool get_indirect = false) const;
	};

}

#endif
