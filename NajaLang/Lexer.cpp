#include "Lexer.h"
#include <unordered_map>
#include <iostream>
namespace NajaLang
{

	static std::unordered_map<std::string, TokenType> keywords =
		{
			{"var", TOKEN_VAR},
			{"if", TOKEN_IF},
			{"else", TOKEN_ELSE},
			{"true", TOKEN_TRUE},
			{"false", TOKEN_FALSE},
			{"null", TOKEN_NULL},
			{"while", TOKEN_WHILE},
			{"for", TOKEN_FOR},
			{"break", TOKEN_BREAK},
			{"continue", TOKEN_CONTINUE},
			{"function", TOKEN_FUNCTION},
			{"class", TOKEN_CLASS},
			{"public", TOKEN_PUBLIC},
			{"protected", TOKEN_PROTECTED},
			{"private", TOKEN_PRIVATE},
			{"this", TOKEN_THIS},
			{"base", TOKEN_BASE},
			{"operator", TOKEN_OPERATOR},
			{"return", TOKEN_RETURN},
	};

	Lexer::Lexer()
	{
		ResetStatus();
	}
	Lexer::~Lexer()
	{
	}

	const std::vector<Token> &Lexer::ScanTokens(std::string src)
	{
		ResetStatus();
		m_Source = src;
		while (!IsAtEnd())
		{
			m_StartPos = m_CurPos;
			ScanToken();
		}

		AddToken(TOKEN_EOF, "");

		return m_Tokens;
	}
	void Lexer::ScanToken()
	{
		char c = GetCurCharAndStepOnce();

		switch (c)
		{
		case '(':
			AddToken(TOKEN_LEFT_PAREN);
			break;
		case ')':
			AddToken(TOKEN_RIGHT_PAREN);
			break;
		case '[':
			AddToken(TOKEN_LEFT_BRACKET);
			break;
		case ']':
			AddToken(TOKEN_RIGHT_BRACKET);
			break;
		case '{':
			AddToken(TOKEN_LEFT_BRACE);
			break;
		case '}':
			AddToken(TOKEN_RIGHT_BRACE);
			break;
		case ',':
			AddToken(TOKEN_COMMA);
			break;
		case '.':
			AddToken(TOKEN_DOT);
			break;
		case ':':
			AddToken(TOKEN_COLON);
			break;
		case ';':
			AddToken(TOKEN_SEMICOLON);
			break;
		case '?':
			AddToken(TOKEN_QUESTION);
			break;
		case '\"':
			String();
			break;
		case ' ':
		case '\t':
		case '\r':
			break;
		case '\n':
			m_Line++;
			break;
		case '+':
			if (IsMatchCurCharAndStepOnce('+'))
				AddToken(TOKEN_PLUS_PLUS);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_PLUS_EQUAL);
			else
				AddToken(TOKEN_PLUS);
			break;
		case '-':
			if (IsMatchCurCharAndStepOnce('-'))
				AddToken(TOKEN_MINUS_MINUS);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_MINUS_EQUAL);
			else
				AddToken(TOKEN_MINUS);
			break;
		case '*':
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_ASTERISK_EQUAL);
			else
				AddToken(TOKEN_ASTERISK);
			break;
		case '/':
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_SLASH_EQUAL);
			else if (IsMatchCurCharAndStepOnce('/'))
			{
				while (!IsMatchCurChar('\n') && !IsAtEnd())
					GetCurCharAndStepOnce();
				m_Line++;
			}
			else if (IsMatchCurCharAndStepOnce('*'))
			{
				while (!IsAtEnd())
				{
					if (IsMatchCurChar('\n'))
						m_Line++;
					else if (IsMatchCurChar('*') && IsMatchNextChar('/'))
					{
						GetCurCharAndStepOnce();
						GetCurCharAndStepOnce();
						break;
					}
					else
						GetCurCharAndStepOnce();
				}
			}
			else
				AddToken(TOKEN_SLASH);

			break;
		case '%':
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_PERCENT_EQUAL);
			else
				AddToken(TOKEN_PERCENT);
			break;
		case '!':
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_BANG_EQUAL);
			else
				AddToken(TOKEN_BANG);
			break;
		case '&':
			if (IsMatchCurCharAndStepOnce('&'))
				AddToken(TOKEN_AMPERSAND_AMPERSAND);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_AMPERSAND_EQUAL);
			else
				AddToken(TOKEN_AMPERSAND);
			break;
		case '|':
			if (IsMatchCurCharAndStepOnce('|'))
				AddToken(TOKEN_VBAR_VBAR);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_VBAR_EQUAL);
			else
				AddToken(TOKEN_VBAR);
			break;
		case '^':
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_CARET_EQUAL);
			else
				AddToken(TOKEN_CARET);
			break;
		case '~':
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_TILDE_EQUAL);
			else
				AddToken(TOKEN_TILDE);
			break;
		case '<':
			if (IsMatchCurCharAndStepOnce('<'))
				if (IsMatchCurCharAndStepOnce('='))
					AddToken(TOKEN_LESS_LESS_EQUAL);
				else
					AddToken(TOKEN_LESS_LESS);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_LESS_EQUAL);
			else
				AddToken(TOKEN_LESS);
			break;
		case '>':
			if (IsMatchCurCharAndStepOnce('>'))
				if (IsMatchCurCharAndStepOnce('='))
					AddToken(TOKEN_GREATER_GREATER_EQUAL);
				else
					AddToken(TOKEN_GREATER_GREATER);
			else if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_GREATER_EQUAL);
			else
				AddToken(TOKEN_GREATER);
			break;
		case '=':
			if (IsMatchCurCharAndStepOnce('='))
				AddToken(TOKEN_EQUAL_EQUAL);
			else
				AddToken(TOKEN_EQUAL);
			break;
		default:
			if (IsNumber(c))
				Number();
			else if (IsLetter(c))
				Identifier();
			else
				AddToken(TOKEN_UNDEFINED);
			break;
		}
	}

	void Lexer::ResetStatus()
	{
		m_StartPos = m_CurPos = 0;
		m_Line = 1;
		std::vector<Token>().swap(m_Tokens);
	}

	bool Lexer::IsMatchCurChar(char c)
	{
		return GetCurChar() == c;
	}
	bool Lexer::IsMatchCurCharAndStepOnce(char c)
	{
		bool result = GetCurChar() == c;
		if (result)
			m_CurPos++;
		return result;
	}

	bool Lexer::IsMatchNextChar(char c)
	{
		return GetNextChar() == c;
	}
	bool Lexer::IsMatchNextCharAndStepOnce(char c)
	{
		bool result = GetNextChar() == c;
		if (result)
			m_CurPos++;
		return result;
	}

	char Lexer::GetNextCharAndStepOnce()
	{
		if (m_CurPos + 1 < m_Source.size())
			return m_Source[++m_CurPos];
		return '\0';
	}
	char Lexer::GetNextChar()
	{
		if (m_CurPos + 1 < m_Source.size())
			return m_Source[m_CurPos + 1];
		return '\0';
	}
	char Lexer::GetCurCharAndStepOnce()
	{
		if (!IsAtEnd())
			return m_Source[m_CurPos++];
		return '\0';
	}

	char Lexer::GetCurChar()
	{
		if (!IsAtEnd())
			return m_Source[m_CurPos];
		return '\0';
	}

	void Lexer::AddToken(TokenType type)
	{
		auto literal = m_Source.substr(m_StartPos, m_CurPos - m_StartPos);
		m_Tokens.emplace_back(Token(type, literal, m_Line));
	}
	void Lexer::AddToken(TokenType type, std::string literal)
	{
		m_Tokens.emplace_back(Token(type, literal, m_Line));
	}

	bool Lexer::IsAtEnd()
	{
		return m_CurPos >= m_Source.size();
	}

	bool Lexer::IsNumber(char c)
	{
		return c >= '0' && c <= '9';
	}
	bool Lexer::IsLetter(char c)
	{
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
	}
	bool Lexer::IsLetterOrNumber(char c)
	{
		return IsLetter(c) || IsNumber(c);
	}

	void Lexer::Number()
	{
		while (IsNumber(GetCurChar()))
			GetCurCharAndStepOnce();

		if (IsMatchCurCharAndStepOnce('.'))
		{
			if (IsNumber(GetCurChar()))
				while (IsNumber(GetCurChar()))
					GetCurCharAndStepOnce();
			else
			{
				AddToken(TOKEN_NUMBER, m_Source.substr(m_StartPos, m_CurPos - m_StartPos - 1));
				AddToken(TOKEN_DOT, m_Source.substr(m_CurPos - 1, 1));
				return;
			}
		}

		AddToken(TOKEN_NUMBER);
	}

	void Lexer::Identifier()
	{
		while (IsLetterOrNumber(GetCurChar()))
			GetCurCharAndStepOnce();

		std::string literal = m_Source.substr(m_StartPos, m_CurPos - m_StartPos);

		bool isKeyWord = false;
		for (const auto &keyword : keywords)
			if (keyword.first.compare(literal) == 0)
			{
				AddToken(keyword.second, literal);
				isKeyWord = true;
				break;
			}

		if (!isKeyWord)
			AddToken(TOKEN_IDENTIFIER, literal);
	}

	void Lexer::String()
	{
		while (!IsMatchCurChar('\"') && !IsAtEnd())
		{
			if (IsMatchCurChar('\n'))
				m_Line++;
			GetCurCharAndStepOnce();
		}

		if (IsAtEnd())
			std::cout << "[line " << m_Line << "]:Uniterminated string." << std::endl;

		GetCurCharAndStepOnce(); //eat the second '\"'

		AddToken(TOKEN_STRING, m_Source.substr(m_StartPos + 1, m_CurPos - m_StartPos - 2));
	}

}