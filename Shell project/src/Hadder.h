#ifndef HEADER_H
#define HEADER_H

// Standard C headers common to all platforms
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)

// POSIX / Linux / macOS specific headers

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/sysinfo.h>

#include <pwd.h>
#include <grp.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// For features like uname, hostname, uptime etc.
#include <sys/utsname.h>
#include <sys/time.h>

#elif defined(_WIN32) || defined(_WIN64)

// Windows-specific headers
#include <windows.h>
#include <winbase.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <direct.h>   // _mkdir, _rmdir
#include <io.h>       // _open, _close, _read, _write
#include <process.h>  // _spawn, _exec
#include <tchar.h>    // Portable strings
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>

// Windows networking initialization requires linking Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#else
#error "Platform not supported. Define includes for your platform."
#endif

#endif // SHELL_CMD_HEADERS_H
