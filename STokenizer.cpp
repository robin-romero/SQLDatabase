//Robin Romero, CS8, 12/11/18
#include "STokenizer.h"
//debugs used to see whats happening throughout the code
bool debug = false;

STokenizer::STokenizer(){
	_pos = 0;
	make_table(_table);
}

STokenizer::STokenizer(char str[]){
	make_table(_table);
	_pos = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		_buffer[i] = str[i];
		size = i;
	}
	if (debug) {
		cout << "This is original string: ";
		for (int i = 0; str[i] != '\0'; i++) {
			cout << str[i];
		}
		cout << endl;
		cout << "This is buffer: ";
		for (int i = 0; str[i] != '\0'; i++) {
			cout << _buffer[i];
		}
		cout << endl;
	}
}

bool STokenizer::done()
{
	if (_pos == size) 
		return true;
	else
		return false;
}

bool STokenizer::more()
{
	if (_pos <= size )
		return true;
	else
		return false;
}

void STokenizer::set_string(const char str[]){
	_pos = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		size = i;
		_buffer[i] = str[i];
	}
	//make_table(_table);
}

int STokenizer::_table[MAX_ROW][MAX_COL];

void STokenizer::make_table(int _table[][MAX_COL]){
	init_table(_table);

	//marks doubles to success states
	mark_cells(0, _table, DIGITS, 1);
	mark_cells(1, _table, DIGITS, 1);
	mark_cells(2, _table, DIGITS, 3);
	mark_cells(3, _table, DIGITS, 3);

	//marks to state 2 doubles
	mark_cells(0, _table, '.', '.', 2);
	mark_cells(1, _table, '.', '.', 2);

	//mark alpha to success states
	mark_cells(5, _table, ALPHA, 6);
	mark_cells(6, _table, ALPHA, 6);//success
	mark_cells(6, _table, '_', '_', 12);
	mark_cells(12, _table, ALPHA, 6);
	mark_cells(5, _table, '"', '"', 7);
	mark_cells(7, _table, ALPHA, 8);
	mark_cells(8, _table, ' ', ' ', 9);
	mark_cells(9, _table, ALPHA, 8);
	mark_cells(8, _table, ALPHA, 8);
	mark_cells(8, _table, '.', '.', 11);
	mark_cells(11, _table, '"', '"', 10);
	mark_cells(8, _table, '"', '"', 10);//success

	//mark punct to success states
	mark_cells(18, _table, PUNCT, 19);//change 12-13
	mark_cells(19, _table, PUNCT, 19);//13-13

	//mark spaces to success states
	mark_cells(20, _table, SPACE, 21);//20-21
	mark_cells(21, _table, SPACE, 21);//21-21

	//mark symbols to success states
	mark_cells(25, _table, SYMBOL, 26);//25-26
	mark_cells(26, _table, SYMBOL, 26);//26-26

	//marking columns and rows with fail and success states
	//doubles success
	mark_success(_table, 1);
	mark_success(_table, 3);
	//double fail
	mark_fail(_table, 0);
	mark_fail(_table, 2);
	//alpha success
	mark_success(_table, 6);
	mark_success(_table, 10);
	//alpha fail
	mark_fail(_table, 5);
	mark_fail(_table, 7);
	mark_fail(_table, 8);
	mark_fail(_table, 9);
	mark_fail(_table, 11);
	mark_fail(_table, 12);
	//Punct success
	mark_success(_table, 19);
	//punct fail
	mark_fail(_table, 18);
	//space success
	mark_success(_table, 21);
	//space fail
	mark_fail(_table, 20);
	//symbol success
	mark_success(_table, 26);
	//symbol fail
	mark_fail(_table, 25);
}

bool STokenizer::get_token(int start_state,	string& token) {
	bool success = false;
	string success_token;
	int i = _pos;
	if (_buffer[_pos] > MAX_COL || _buffer[_pos] < 0)
	{
		return false;
	}
	while (start_state != -1 && i <=size) {
		success_token += _buffer[i];
		//if (debug) {
		//	cout << "Success_Token: " << success_token << endl;
		//	cout << "Previous state: " << start_state << endl;
		//}
		if (_buffer[i] < MAX_COL && _buffer[i] > 0) {
			start_state = _table[start_state][_buffer[i]];
		}
		//if (debug) {
		//	cout << "i: " << i << ", char: " << _buffer[i] << ", Current state: " << start_state << endl;
		//}
		if (is_success(_table, start_state)) {
			token = success_token;
			if (_pos <= size) {
				_pos = i + 1;
			}
			else {
				_pos = i;
			}
			//if (debug) {
			//	cout << "Token_update: " << token << endl;
			//}
			success = true;
		}
		i++;
	}
	if (success) {
		if(debug)
			cout << "This is current position: " << _pos << " at: " <<  _buffer[_pos] << endl;
		return true;
	}	
	else {
		return false;
	}
}

STokenizer & operator>>(STokenizer & s, Token & t)
{
	string tk;
	if (s.get_token(DOUBLE_START, tk)) {
		t = Token(tk, DOUBLE_START);
		return s;
	}
	else if (s.get_token(ALPHA_START, tk)) {
		t = Token(tk, ALPHA_START);
		t.remove_quotes();
		return s;
	}
	else if (s.get_token(PUNCT_START, tk)) {
		t = Token(tk, PUNCT_START);
		return s;
	}
	else if (s.get_token(SPACE_START, tk)) {
		t = Token(tk, SPACE_START);
		return s;
	}
	else if (s.get_token(SYMBOL_START, tk)) {
		t = Token(tk, SYMBOL_START);
		return s;
	}
	else {
		tk += s._buffer[s._pos];
		t = Token(tk, UNKNOWN);
		s._pos++;
		return s;
	}
}
