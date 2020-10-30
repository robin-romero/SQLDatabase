//Robin Romero, CS8, 12/11/18
#pragma once
#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <iostream>
#include <algorithm>
#include "constants.h"

using namespace std;

class Token
{
public:
	//CTORs
	//resets the entire token(string = "", type = 0)
	Token();
	//initializes the private members to arguments being passed
	Token(string str, int type);
	//couts the token using outs
	friend ostream& operator <<(ostream& outs, const Token& t);
	//returns the type from private memeber variable
	int type();
	//returns the type that is used for start state ie: Double, Alpha
	string type_string() const;
	//returns the token string
	string token_str() const ;

	void remove_quotes();

private:
	string _token;
	int _type;
};

#endif // !TOKEN_H
