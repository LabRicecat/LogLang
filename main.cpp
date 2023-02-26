
#define LL_ADV_CONFIGS
#include "loglang.hpp"
#include "catmods/argparser/argparser.h"

std::tuple<std::string,std::string> split_sh(std::string src) {
    int i = 0;
    while(i < src.size() && (src[i] != ' ' || src[i] == '\t')) ++i;
    std::string c1 = src.substr(0,i);
    std::string c2 = src.substr(i,src.size()-1);
    c1.erase(c1.find_last_not_of(" \t\n\r\f\v") + 1);
    c1.erase(0, c1.find_first_not_of(" \t\n\r\f\v"));
    c2.erase(c2.find_last_not_of(" \t\n\r\f\v") + 1);
    c2.erase(0, c2.find_first_not_of(" \t\n\r\f\v"));

    return std::make_tuple(c1,c2);
}

std::string rd(std::string file) {
    std::ifstream ifs(file);
    std::string r;
    while(ifs.good()) r += ifs.get();
    if(!r.empty()) r.pop_back();

    return r;
}

template<typename ...Targs>
bool is_of(std::string c, Targs ...args) {
    std::vector<std::string> a = {args...};
    for(auto i : a) if(i == c) return true;
    return false;
}

#define errc() if(true) { if(_ll_err) { std::cout << "Error: " << ll_error() << "\n"; continue; } } else do {} while(0)
#define hasarg() if(true) { if(arg == "") { std::cout << "Command requires an argument!\n"; continue; } } else do {} while(0)
#define noarg() if(true) { if(arg != "") { std::cout << "Command does not allow an argument!\n"; continue; } } else do {} while(0)
int main(int argc, char** argv) {
    std::string layout = "$> ", input;
    std::vector<ll_function> functions;
    
    if(argc >= 2) {
        std::string arg;
        for(size_t i = 1; i < argc; ++i) {
            arg = std::string(argv[i]);
            if(arg.front() == '-') {
                arg.erase(arg.begin());
                for(auto c : arg) {
                    switch(c) {
                        case 'e':
                            ll::eval_show = true;
                            break;
                        case 'v':
                            ll::val_show = true;
                            break;
                        case 'r':
                            ll::run_show = true;
                            break;
                        case 'a':
                            ll::eval_show = true;
                            ll::val_show = true;
                            ll::run_show = true;
                            break;
                        case 'h':
                            std::cout << "# LogLang shell\n\n"
                                << "  Usage: loglang [options] [files]\n\n"
                                << "Loads FILES if any, and opens the loglang shell.\n"
                                << "Options: \n"
                                << " -e : shows all eval() steps\n"
                                << " -v : shows all val() steps\n"
                                << " -r : shows all runf() steps\n"
                                << " -a : all the 3 above\n"
                                << " -h : this\n"
                                << "Note: `-v -e` is the same as `-ve`\n";
                            std::exit(0);
                            break;
                        default:
                            std::cout << "unknown option: " << c << "\n"
                            << "  try -h for help!\n";
                            std::exit(1);
                            break;
                    }
                }
            }
            else {
                auto fns = ll_parse(rd(arg));
                if(_ll_err) { std::cout << ll_error() << "\n"; std::exit(1); }
                for(auto i : fns) functions.push_back(i);
            }
        }
    }
    
    std::cout << " --- Loglang Shell ---\n"
            << "This is free software without any warranty!\n"
            << "Use `help` for help.\n"
            << "LabRicecat (c) 2023\n\n";

    while(true) {
        std::cout << layout;
        std::getline(std::cin,input);
        auto [command, arg] = split_sh(input);

        if(is_of(command,"file","f","fi")) {
            hasarg();
            auto fns = ll_parse(rd(arg));
            errc();
            for(auto i : fns) functions.push_back(i);
        }
        else if(is_of(command,"clear","c","cls")) {
            noarg();
            functions.clear();
            std::cout << "Cleared functions\n";
        }
        else if(is_of(command,"info","i","in")) {
            hasarg();
            ll_function f = ll_getf(arg,functions);
            errc();
            std::cout << f.name;
            for(size_t i = 0; i < f.body.size(); ++i) {
                for(auto j : f.body[i]) 
                    std::cout << " " << j;
                if(i + 1 < f.body.size()) std::cout << ",";
            }
            std::cout << "\n";
        }
        else if(is_of(command,"eval","e","ev")) {
            hasarg();
            std::cout << (ll_eval(arg,"",functions,{}) ? "TRUE" : "FALSE") << "\n";
            errc();
        }
        else if(is_of(command,"def","d","fn")) {
            auto fns = ll_parse(arg);
            errc();
            for(auto i : fns) functions.push_back(i);
        }
        else if(is_of(command,"quit","q","exit",":q",":q!")) {
            break;
        }
        else if(is_of(command,"help","h")) {
            std::cout << 
                "file <file.ll>    : loads a file\n" <<
                "clear             : clears all data\n" <<
                "eval <statement>  : evaluates a statement\n" <<
                "def|fn <function> : defined a function\n" <<
                "quit|exit         : exits the program\n" <<
                "help              : this\n";
        }
        else if(command != "") {
            std::cout << "Unknown \"" + command + "\"\n";
        }
    }
}