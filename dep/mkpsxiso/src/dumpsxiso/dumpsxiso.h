#pragma once

#ifdef _WIN32
int dumpsxiso(int argc, wchar_t* argv[]);
#else
int dumpsxiso(int argc, char* argv[]);
#endif