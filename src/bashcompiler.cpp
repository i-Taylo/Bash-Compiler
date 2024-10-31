
/***********************************************
# ==============================================
# | Author: Taylo @ https://github.com/i-Taylo |
# ==============================================
*///////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <getopt.h>
#include <random>
#include "include/obfuscate.h" // Author: Adam Yaxley | check the header for more info.
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <sys/stat.h>

#define __BASHCOMPILER_VERSION__ "0.1"
#define __BASHCOMPILER_PROGRAM__ "BashCompiler"
#define __BASHCOMPILER_MAINTAINER__ "Taylox"

auto commandExists(const std::string &command) -> bool {
    const char *pathEnv = std::getenv("PATH");
    if (!pathEnv) {
        return false;
    }

    std::string pathStr(pathEnv);
    std::istringstream pathStream(pathStr);
    std::string dir;

    while (std::getline(pathStream, dir, ':')) {
        std::string fullPath = dir + "/" + command;

        struct stat buffer;
        if (stat(fullPath.c_str(), &buffer) == 0 && (buffer.st_mode & S_IXUSR)) {
            return true;
        }
    }

    return false;
}

// BashCompiler Class
class BashCompiler {
private:
    std::string scriptFile;
    std::string outputFile;
    bool optimize;
    
    bool fileExists(const std::string& filename) {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }

    std::string generateCode(const std::string& script) {
        // Create a unique key based on script content for additional entropy
        uint64_t scriptKey = 0;
        for (char c : script) {
            scriptKey = (scriptKey * 31 + c) | 0x0101010101010101ull;
        }
        
        std::string escapedScript;
        for (char c : script) {
            if (c == '\n') escapedScript += "\\n";
            else if (c == '\r') escapedScript += "\\r";
            else if (c == '\t') escapedScript += "\\t";
            else if (c == '\"') escapedScript += "\\\"";
            else if (c == '\\') escapedScript += "\\\\";
            else escapedScript += c;
        }

        #include "include/bashcompiler.h"
        return part1 + part2 + part3 + part4;
    }

public:
    BashCompiler(const std::string& script, const std::string& output, bool opt) 
        : scriptFile(script), outputFile(output), optimize(opt) {}

    void compile() {
        if (!fileExists(scriptFile)) {
            throw std::runtime_error("Script file does not exist");
        }

        std::ifstream script(scriptFile);
        if (!script.is_open()) {
            throw std::runtime_error("Could not open script file");
        }

        std::string shellScript((std::istreambuf_iterator<char>(script)), 
                               std::istreambuf_iterator<char>());
        script.close();

        std::string cppCode = generateCode(shellScript);

        std::string tempFile = outputFile + ".cpp";
        std::ofstream outFile(tempFile);
        if (!outFile.is_open()) {
            throw std::runtime_error("Could not create output file");
        }
        outFile << cppCode;
        outFile.close();

        size_t lastSlash = outputFile.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            std::string dir = outputFile.substr(0, lastSlash);
            system(("mkdir -p " + dir).c_str());
        }

        std::string compileCommand = "g++ -O3 -Wall -Wextra -pedantic -std=c++17 ";
        if (optimize) {
            compileCommand += "-flto -march=native ";
        }
        compileCommand += tempFile + " -o " + outputFile;

        if (system(compileCommand.c_str()) != 0) {
            std::remove(tempFile.c_str());
            throw std::runtime_error("Compilation failed");
        }

        std::remove(tempFile.c_str()); // Cleanup
    }
};


void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <script.sh> <output_binary>\n"
              << "Options:\n"
              << "  --optim, -o\t\tEnable optimizations (default: false)\n"
              << "  --version, -v\t\tShow "<<__BASHCOMPILER_PROGRAM__<<" version.\n"
              << "  --help, -h\t\tShow this help message\n";
}

void printVersion() {
    std::cout << __BASHCOMPILER_PROGRAM__ <<": "<<__BASHCOMPILER_VERSION__ 
    << ", <author:" << __BASHCOMPILER_MAINTAINER__<<">"<< std::endl;
    return;
}

auto abort(const std::string &errMessage) -> void {
    std::cout << errMessage << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {

    std::vector<std::string> DEPS = {
        "g++"
    };
    for(auto &dep : DEPS) {
        if (!commandExists(dep)) {
            abort(dep +": Not installed! please install it or run build script to install dependencies.\n");
        }
    }

    std::string scriptFile;
    std::string outputFile;
    bool optimize = false;

    static struct option long_options[] = {
        {"optim", no_argument, nullptr, 'o'},
        {"version", no_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "ovh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'o': 
                optimize = true;
                break;
            case 'v':
                printVersion();
                return 0;
            case 'h':
                printUsage(argv[0]);
                return 0;
            case '?':
                printUsage(argv[0]);
                return 1;
            default:
                printUsage(argv[0]);
                return 1;
        }
    }

    if (optind + 2 != argc) {
        printUsage(argv[0]);
        return 1;
    }

    scriptFile = argv[optind];
    outputFile = argv[optind + 1];

    try {
        BashCompiler compiler(scriptFile, outputFile, optimize);
        compiler.compile();
        std::cout << "Compilation complete. Output: " << outputFile << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}