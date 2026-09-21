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
    int i=0;
    while(i<str.size()){
        char c=str[i];
        if(isalpha(c)){
            while(i < str.size() && isalnum(str[i])){
                buf.push_back(str[i]);
                i++;
            }
            if(buf=="return"){
                tokens.push_back({TokenType::_return});
                buf.clear();
            }else{
                cerr<<"You messes up!"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(isdigit(c)){
            while(i < str.size() && isdigit(str[i])){
                buf.push_back(str[i]);
                i++;
            }
            tokens.push_back({TokenType::int_lit, buf});
            buf.clear();
        }
        else if(c==';'){
            tokens.push_back({TokenType::semi});
            buf.clear();
            i++;
        }
        else if(isspace(c)){
            i++;
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

    //reading a puting the contest into string = contents 
    string contents;
    {
        stringstream contents_stream;
        fstream input_file_content(argv[1], ios::in);
        contents_stream<<input_file_content.rdbuf();
        contents=contents_stream.str();
    }
   // cout<<"the string we get is: "<<contents<<endl;

   //tokenizing the string
   vector<Token> tokens = tokenize(contents);
    
    {
        fstream file("out.asm",ios::out); //create and open that file
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm"); //assembler gives object file
    system("ld -o out out.o"); //its a linker gives executable code


    return EXIT_SUCCESS;
}