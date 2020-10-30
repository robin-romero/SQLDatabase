//Robin Romero, CS8, 12/11/18
#include "Token.h"

Token::Token() {
	_token = "";
	_type = 16;
}

Token::Token(string str, int type) {
	_token = str;
	_type = type;
}

int Token::type() {
	return _type;
}

string Token::type_string() const {
	if (_type == DOUBLE_START)
		return string("DOUBLE");
	else if (_type == ALPHA_START)
		return string("ALPHA");
	else if (_type == PUNCT_START)
		return string("PUNCT");
	else if (_type == SPACE_START)
		return string("SPACE");
	else if (_type == SYMBOL_START)
		return string("SYMBOL");
	else if (_type == UNKNOWN)
		return string("UNKNOWN");
}

string Token::token_str() const {
	return string(_token);
}

void Token::remove_quotes() {
	_token.erase(remove(_token.begin(), _token.end(), '"'), _token.end());
}
ostream & operator<<(ostream & outs, const Token & t) {
	outs << "|" << t.token_str() << "|" << endl;
	return outs;
}
