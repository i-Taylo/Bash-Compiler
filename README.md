[# Bash-Compiler
A tool for compiling bash scripts into c++ binary executable.
](https://github.com/i-Taylo/Bash-Compiler.git)# BashCompiler

## Overview

BashCompiler is a C++ utility that compiles Bash scripts into standalone binary executables with script obfuscation. This tool allows you to convert Bash scripts into secure, optimized, and portable executable files.

## Features

- 🔒 Script Obfuscation
  - Encrypts script content to prevent direct inspection
  - Uses a unique cryptographic key generation method
  - Provides runtime decryption of script content

- 🚀 Optimization Options
  - Optional compilation with `-O3` optimization level
  - Support for Link Time Optimization (LTO)
  - Native architecture optimization

- 🔧 Flexible Compilation
  - Supports various input Bash scripts
  - Generates standalone C++ executable
  - Manages temporary files and cleanup

- 🛡️ Error Handling
  - Comprehensive error checking
  - Detailed error messages
  - Signal handling for graceful termination

## Installation

Clone the repository and compile the BashCompiler:

```bash
git clone https://github.com/i-Taylo/Bash-Compiler.git
cd Bash-Compiler
chmod +x build.sh
./build.sh
```

## Usage

### Syntax

```bash
./bashcompiler [options] <script.sh> <output_binary>
```

### Options

- `--optim, -o`: Enable advanced optimizations
- `--version, -v`: Display BashCompiler version
- `--help, -h`: Show help message

### Examples

1. Basic Compilation:
```bash
./bashcompiler myscript.sh myapp
```

2. Compilation with Optimizations:
```bash
./bashcompiler -o myscript.sh myoptimizedapp
```

## How It Works

1. Read input Bash script
2. Generate obfuscated C++ code
3. Create temporary C++ source file
4. Compile with specified options
5. Generate standalone executable
6. Clean up temporary files

### Obfuscation Process

- Generates a unique cryptographic key based on script content
- Encrypts script using XOR-based cipher
- Embeds decryption logic in the generated executable
- Provides runtime script decryption with minimal overhead

## Security Considerations

- Temporary script files are created with random suffixes
- Executable permissions are strictly managed
- Files are immediately deleted after execution
- Obfuscation adds a layer of script protection

## Version

Current Version: 0.1
Maintainer: Taylo

## Disclaimer

This tool is provided as-is. Always review and understand the scripts you're compiling.
