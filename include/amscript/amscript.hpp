#ifndef AMS_AMSCRIPT_HPP
#define AMS_AMSCRIPT_HPP

#define AMSCRIPT_VERSION "1.1.4"

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


	/* Thrown DIRECTLY if an unexpected internal error
	 * occurs during the parsing phase */
	class Exception : public std::exception {
	private:
		char * const message;

	public:
		Exception(const std::string &);
		Exception(const char *);
		constexpr const char * what() const noexcept { return message; };
	};


	/* Thrown when the script has an invalid syntax */
	class LogicException : public Exception {
	private:
		std::size_t _line;

	public:
		LogicException(const std::string &, std::size_t line = 0);
		LogicException(const char *, std::size_t line = 0);
		constexpr std::size_t line() const { return _line; }
	};


	/* Thrown when a script function encounters an error */
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
		using ptr_t = std::vector<std::string> (*)(
				Amscript*,
				std::vector<std::string> );

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
				std::vector<std::string> arguments,
				std::vector<std::string> body,
				ptr_t func_ptr = nullptr );

		inline std::string getName() const { return name; }
		inline const std::vector<std::string> & getArguments() const { return args; }
		inline const std::vector<std::string> & getBody() const { return body; }
		inline ptr_t getNative() const { return pointer; }
		inline bool isBuiltin() const { return is_builtin; }
		inline operator bool() const { return ! name.empty(); }

		std::vector<std::string> eval(
				Amscript& script,
				const std::vector<std::string> & arguments,
				const std::map<std::string, std::string> * local_arguments = nullptr );

		std::vector<std::string> eval(
				Amscript& script,
				std::string arguments,
				const std::map<std::string, std::string> * local_arguments = nullptr );
	};


	class Amscript {
	public:
		using map_t = std::map<std::string, std::string>;

		class Getter {
		private:
			union {
				Amscript* amscript;
				const Amscript * amscript_const;
			};
			std::string reference;

		public:
			Getter(Amscript& amscript, std::string ref_name);
			Getter(const Amscript & amscript, std::string ref_name);

			std::vector<std::string> operator () (std::string arguments);
			operator std::string() const;
			operator double() const;
			operator float() const;
			operator long long int() const;
			operator int() const;
			operator bool() const;
		};

	private:
		map_t symbols_indirect;
		map_t symbols_direct;
		std::map<std::string, Function> functions;

		void parse(std::string str);

	public:
		unsigned short recursion_limit = 0xFFF;

		Amscript();

		Amscript(const std::string & input);
		Amscript(std::string&& input);

		Amscript(
				std::istream& input_stream,
				std::size_t max_size = ASCRIPT_FILE_SIZE_MAX );

		Amscript(
				std::istream& input_tream,
				const Amscript & import_source,
				std::size_t max_size = ASCRIPT_FILE_SIZE_MAX );

		/* Attempts to fetch all tokens in a function;
		 * throws a RuntimeException on non-trivial failure. */
		std::vector<std::string> compute(
				std::string function_name,
				std::string arguments );

		void setFunction(
				std::string name,
				std::vector<std::string> arguments,
				std::vector<std::string> body );

		void setFunction(
				std::string name,
				unsigned int max_args,
				Function::ptr_t function_ptr );

		void setFunction(
				std::string name,
				Function::ptr_t function_ptr );

		/* Try to remove a function; returns 'false' if the function
		 * was not found. */
		bool removeFunction(std::string name);

		Function getFunction(std::string name);
		const Function getFunction(std::string name) const;

		void setSymbol(std::string symbol_name, Symbol symbol);
		void removeSymbol(std::string symbol_name);

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

		Getter operator [] (std::string reference_name);
		const Getter operator [] (std::string reference_name) const;

		Amscript& import(const Amscript & import_source);
	};

}

#endif
