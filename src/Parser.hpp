#pragma once

#include <string>
#include <vector>
#include "Token.hpp"
#include "Conf.hpp"

class Configuration;

class Parser
{
	public:
	    Parser(const std::vector<Token>& tokens, Configuration& config);

		void parseConfig();
		void parseServerBlock();
		void parseInstruction(std::vector<std::string>& args);

		bool check(tokenType type) const;
		bool checkNext(tokenType type) const;
		bool checkWord(const std::string &str) const;
		bool isAtEnd() const;
		void advance();

		Token consume(tokenType type, const std::string& errorMessage);
		Token consumeWord(const std::string &str, const std::string& errorMessage);
		const std::string& currentTokenValue() const;

		void setupHandlersInstruction();
		void setupHandlersLocation();

		void parseListen(VirtualServer& server, const std::vector<std::string>& args);
		void parseHost(VirtualServer& server, const std::vector<std::string>& args);
		void parseServerName(VirtualServer& server, const std::vector<std::string>& args);
		void parseErrorPage(VirtualServer& server, const std::vector<std::string>& args);
		void parseMaxBodySize(VirtualServer& server, const std::vector<std::string>& args);
		
		void parseLocation(VirtualServer& server);
		void parseLimit(Route& route, const std::vector<std::string>& args);
		void parseRedirection(Route& route, const std::vector<std::string>& args);
		void parseRoot(Route& route, const std::vector<std::string>& args);
		void parseAutoIndex(Route& route, const std::vector<std::string>& args);
		void parseIndex(Route& route, const std::vector<std::string>& args);
		void parseCgiPath(Route& route, const std::vector<std::string>& args);

		int parseNumber(const std::string& str);

		void throwParseError(const std::string& message);
	private:
		size_t _current;
	    const std::vector<Token>& _tokens;
		Configuration& _config;

		typedef void (Parser::*HandlerFunction)(VirtualServer& server, const std::vector<std::string>&);
		typedef void (Parser::*HandlerFunctionLoc)(Route& route, const std::vector<std::string>&);
		std::map<std::string, HandlerFunction> _directiveHandlers;
		std::map<std::string, HandlerFunctionLoc> _directiveHandlersLocation;
};