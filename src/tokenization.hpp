#pragma once
#include <bits/stdc++.h>
#include<fstream>
#include<sstream>
using namespace std;

enum class TokenType{
    exit,
    int_lit,
    semi
};

struct Token{
    TokenType type;
    optional<string>value{};
};

class Tokenizer{
public:
    inline explicit Tokenizer(string src)
        : m_src(move(src))
    {

    }

vector<Token>tokenize(){
        vector<Token>tokens;
        string buf;

    while(peak().has_value()){
        if(isalpha(peak().value())){
            buf.push_back(consume());
            while(peak().has_value() && isalnum(peak().value())){
                buf.push_back(consume());
            }
            if(buf == "exit"){
                tokens.push_back({TokenType::exit});
                buf.clear();
                continue;
            }else{
                cerr<<"You messes up!"<<endl;
                 exit(EXIT_FAILURE);
            }
        }
        else if(isdigit(peak().value())){
            buf.push_back(consume());
            while(peak().has_value() && isdigit(peak().value())){
                buf.push_back(consume());
            }
            tokens.push_back({TokenType::int_lit, buf});
             buf.clear();
             continue;
        }
        else if(peak().value() == ';' ){
            tokens.push_back({TokenType::semi});
            consume();
            continue;
        }
        else if(isspace(peak().value())){
            consume();
            continue;
        }else{
            cerr<<"You messes up!"<<endl;
             exit(EXIT_FAILURE);
        }
    }
        m_index=0;
        return tokens;
};



private:

    inline optional<char> peak(int ahead = 0) const {
        if(m_index + ahead >= m_src.length()){
            return {};
        }else{
            return m_src[m_index + ahead];
        }
    }
   inline char consume(){
        return m_src[m_index++];
    }
    const string m_src;
    size_t m_index = 0;
};