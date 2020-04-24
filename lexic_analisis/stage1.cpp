#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <exception>

using namespace std;

enum ST
{
	BEGIN, WORD, NUMBER, COMMENT, HEXADECIMAL, DECIMAL, OPERATOR, CONST
};

class lexem
{
public:
	string s;
	string type;
	lexem()
	{
		s = '\n';
		type = '\n';
	}
	string to_string()
	{
		ostringstream str;
		str << type << ": " << s;
		return str.str();
	}
	char gc()
	{
		char c;
		c = cin.get();
		return c;
	}
	vector<lexem> make_lexem()
	{
		ST L = BEGIN;
		vector<lexem> v;
		char c = gc();
		string s, s1;
		s += c;
		lexem x;
		while (c != EOF)
		{
			try
			{
				switch (L)
				{
				case BEGIN:
					if (((c >= 'a') && (c <= 'z')) || (c == '_') || ((c >= 'A') && (c <= 'Z')))
					{
						L = WORD;
						c = gc();
					}
					else if ((c >= '0') && (c <= '9'))
					{
						L = NUMBER;
						c = gc();
					}
					else if (c == '/')
					{
						c = gc();
						s = c;
						if (c != '*')
						{
							x.s = s;
							x.type = "OPERATOR";
							v.push_back(x);
							c = gc();
							s = c;
							L = BEGIN;
						}
						else
						{
							s = c;
							c = gc();
							L = COMMENT;
						}
						c = gc();
						s = '\0';
					}
					else if ((c == '*') || (c == '/') || (c == '+') || (c == '-') || (c == '<') || (c == '>') || (c == '=') || (c == '!'))
					{
						L = OPERATOR;
						s = c;
						c = gc();

					}

					else if ((c == ' ') || (c == ';') || (c == '\n') || (c == '{') || (c == '}') || (c == '(') || (c == ')'))
					{
						c = gc();
						s = c;
					}
					else if (c == '"')
					{
						L = CONST;
						c = gc();
						s = c;
						c = gc();
					}
					else
					{
						throw("incorrect symbol");
					}
					break;

				case WORD:
					if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '_') || ((c >= '0') && (c <= '9')))
					{
						s += c;
						c = gc();
					}
					else if ((c == ' ') || (c == ';') || (c == '\n') || (c == '{') || (c == '}') || (c == '(') || (c == ')'))
					{
						if ((s == "program") || (s == "int") || (s == "string") || (s == "real") || (s == "while") || (s == "read") || (s == "write") || (s == "continue")
								|| (s == "if") || (s == "else") || (s == "end"))
						{
							x.s = s;
							x.type = "KEYW";
							v.push_back(x);
							c = gc();
							s = c;
							L = BEGIN;
						}
						else if ((s == "or") || (s == "and") || (s == "not"))
						{
							x.s = s;
							x.type = "LOGIC_OPERATOR";
							v.push_back(x);
							c = gc();
							s = c;
							L = BEGIN;

						}
						else
						{
							x.s = s;
							x.type = "ID";
							v.push_back(x);
							c = gc();
							s = c;
							L = BEGIN;
						}
					}
					else if ((c == '*') || (c == '/') || (c == '+') || (c == '-') || (c == '<') || (c == '>') || (c == '=') || (c == '!'))
					{
						if ((s == "program") || (s == "int") || (s == "string") || (s == "real") || (s == "while") || (s == "read") || (s == "write") || (s == "continue")
								|| (s == "if") || (s == "else") || (s == "end"))
						{
							x.s = s;
							x.type = "KEYW";
							v.push_back(x);
							s = c;
							c = gc();
							L = OPERATOR;
						}
						else
						{
							x.s = s;
							x.type = "ID";
							v.push_back(x);
							s = c;
							c = gc();
							L = OPERATOR;
						}
					}
					break;

				case NUMBER:
					if ((s == "0") && ((c == 'x') || (c == 'X')))
					{
						L = HEXADECIMAL;
						c = gc();
						s = '\0';
					}
					else if ((c >= '0') && (c <= '9'))
					{
						L = DECIMAL;
						s += c;
						c = gc();
					}
					else if ((c == ' ') || (c == ';') || (c == '\n') || (c == '{') || (c == '}') || (c == '(') || (c == ')'))
					{
						x.s = s;
						x.type = "DECIMAL";
						v.push_back(x);
						s = c;
						L = BEGIN;
						c = gc();
					}
					else if ((c == '*') || (c == '/') || (c == '+') || (c == '-') || (c == '<') || (c == '>') || (c == '=') || (c == '!'))
					{
						x.s = s;
						x.type = "DECIMAL";
						v.push_back(x);
						s = c;
						c = gc();
						L = OPERATOR;
					}

					break;

				case DECIMAL:
					if ((c >= '0') && (c <= '9'))
					{
						s += c;
						c = gc();
					}
					else if ((c == ' ') || (c == ';') || (c == '\n') || (c == '{') || (c == '}') || (c == '(') || (c == ')'))
					{
						x.s = s;
						x.type = "DECIMAL";
						v.push_back(x);
						c = gc();
						s = c;
						L = BEGIN;
					}
					else if ((c == '*') || (c == '/') || (c == '+') || (c == '-') || (c == '<') || (c == '>') || (c == '=') || (c == '!'))
					{
						x.s = s;
						x.type = "DECIMAL";
						v.push_back(x);
						s = c;
						c = gc();
						L = OPERATOR;
					}
					else if(c=='.') {
						s += c;
						c = gc();
					}
					else
					{
						throw("error in decimal constant");
					}
					break;

				case HEXADECIMAL:
					if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')))
					{
						s += c;
						c = gc();
					}
					else if ((c == ' ') || (c == ';') || (c == '\n') || (c == '{') || (c == '}') || (c == '(') || (c == ')'))
					{
						x.s = s;
						x.type = "HEXADECIMAL";
						v.push_back(x);
						c = gc();
						s = c;
						L = BEGIN;
					}
					else if ((c == '*') || (c == '/') || (c == '+') || (c == '-') || (c == '<') || (c == '>') || (c == '=') || (c == '!'))
					{
						x.s = s;
						x.type = "HEXADECIMAL";
						v.push_back(x);
						s = c;
						c = gc();
						L = OPERATOR;
					}

					else
					{
						throw("error in hexadecimal constant");
					}
					break;

				case OPERATOR:
					s1 = s + c;
					if ((s1 == "<=") || (s1 == ">=") || (s1 == "==") || (s1 == "!="))
					{
						x.s = s1;
						x.type = "OPERATOR";
						v.push_back(x);
						L = BEGIN;
						c = gc();
						s = c;
					}
					else
					{
						x.s = s;
						x.type = "OPERATOR";
						v.push_back(x);
						s = c;
						L = BEGIN;
					}
					break;

				case COMMENT:
					if (c != '*')
					{
						s += c;
						c = gc();
					}
					else
					{
						c = gc();
						if (c == '/')
						{
							x.s = s;
							x.type = "COMMENT";
							v.push_back(x);
							c = gc();
							s = c;
							L = BEGIN;
						}
					}
					break;

				case CONST:
					if (c != '"')
					{
						s += c;
						c = gc();
					}
					else
					{
						x.type = "CONST";
						v.push_back(x);
						c = gc();
						s = c;
						L = BEGIN;
					}
				}
			}

			catch (char const* s)
			{
				cout << s<<endl;
				return v;
			}
		}
		return v;
	}
};

int main()
{
	lexem x;
	vector<lexem> v;
	v = x.make_lexem();
	cout << "Result:" << endl;
	for (unsigned i = 0; i < v.size(); i++)
	{
		cout << v[i].to_string() << endl;
	}

	return 0;
}
