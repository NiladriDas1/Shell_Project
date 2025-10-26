# Shell_Project
The project provides a practical foundation for learning shell programming and system interaction, with scope for enhancements such as advanced parsing, built-in scripting, and optimizing locate functionality via database indexing.

# Custom Cross-Platform Shell Project

This project implements a basic shell command line interpreter in C supporting Unix-like commands on Linux, macOS, and Windows.

## Features

- Basic file and directory commands: ls, cd, pwd, mkdir, rmdir, cp, mv, rm, touch
- File content and text processing: cat, less, head, tail, cut, sort, uniq, grep, wc, diff
- File searching utilities: find, locate
- Process control: exit command
- Cross-platform implementation handling using conditional headers and APIs

## Project Structure

- `Hadder.h` - Includes all standard and platform-specific headers
- `Cmd.h` - Command prototypes and implementations
- `shell.c` - Main interactive shell loop reading and dispatching commands

## Build & Run

Requires GCC or compatible compiler.

<br><h1>gcc -o shell shell.c -Wall<h1><br>
<h1>./shell<br>

## Usage

Type shell commands at the prompt, e.g., `ls /home`, `cat file.txt`, `exit` to quit.

## Notes

- `locate` currently performs a recursive full filesystem scan and is slow.
- Windows support uses conditionally included WinAPI headers; further adaptations may be necessary.
- Extend by adding commands to `Cmd.h` and implementing corresponding functions.

## License

MIT License

