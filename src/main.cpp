#include <bits/stdc++.h>
#include<fstream>
#include<sstream>
#include"./tokenization.hpp"
#include"./parser.hpp"
#include"./generation.hpp"

using namespace std;


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
   Tokenizer tokenizer(move(contents));
   vector<Token> tokens = tokenizer.tokenize();

   //parsing the tokens
   Parser parser(move(tokens));
   optional<NodeExit>tree=parser.parse();
   if(!tree.has_value()){
    cerr<<"No Exit Statement Found"<<endl;
    exit(EXIT_FAILURE);
   }

   //generating asembly code
   Generator generator(tree.value());
   //create and open that file
    {
        fstream file("out.asm",ios::out); 
        file << generator.generate();
    }

    system("nasm -felf64 out.asm"); //assembler gives object file
    system("ld -o out out.o"); //its a linker gives executable code


    return EXIT_SUCCESS;
}