#include <bits/stdc++.h>
#include<fstream>
#include<sstream>

using namespace std;

enum class TokenType{
    _return,
    int_lit,
    semi
};

struct Token{
    TokenType type;
    optional<string>value{};
};

vector<Token>tokenize(string &str){
    string buf;
    vector<Token>tokens;
    for(int i=0;i<str.size();i++){
        char c=str[i];
        if(isalpha(c)){
            buf.push_back(c);
            i++;
            while(isalnum(str[i])){
                buf.push_back(str[i]);
                i++;
            }
            i--;
            if(buf=="return"){
                tokens.push_back({TokenType::_return});
                buf.clear();
            }else{
                cerr<<"You messes up!"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(isdigit(c)){
            buf.push_back(c);
            i++;
            while(isdigit(str[i])){
                buf.push_back(str[i]);
                i++;
            }
            i--;
            tokens.push_back({TokenType::int_lit, buf});
            buf.clear();
        }
        else if(c==';'){
            tokens.push_back({TokenType::semi});
            buf.clear();
        }
        else if(isspace(c)){
            continue;
        }else{
                cerr<<"You messes up!"<<endl;
                exit(EXIT_FAILURE);
            }
    }
    return tokens;
}

string tokens_to_asm(vector<Token>&tokens){
    stringstream output;
    output<<"global _start\n_start:\n";
    
    for(int i=0;i<tokens.size();i++){
        const Token& token=tokens[i];

        if(token.type==TokenType::_return){
            if(i+1<tokens.size() && tokens[i+1].type==TokenType::int_lit){
                if(i+2<tokens.size() && tokens[i+2].type==TokenType::semi){
                    output<<"    mov rax, 60\n";
                    output<<"    mov rdi, "<<tokens[i+1].value.value()<<endl;
                    output<<"    syscall";
                }
            }
        }
    }
    return output.str();
}

int main(int argc, char* argv[]) {
    if(argc!=2){
        cerr<<"Incorrect usage. Correct Usage is... "<<endl;
        cerr<<"hydro <input.hy> "<<endl;
        return EXIT_FAILURE;
    }

    string contents;
    {
        stringstream contents_stream;
        fstream input(argv[1], ios::in);
        contents_stream<<input.rdbuf();
        contents=contents_stream.str();
   }

   vector<Token> tokens = tokenize(contents);

    {
        fstream file("out.asm",ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");


    return EXIT_SUCCESS;
}