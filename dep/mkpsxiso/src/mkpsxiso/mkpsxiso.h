#pragma once

#ifdef _WIN32
int mkpsxiso(int argc, wchar_t* argv[]);
#else
int mkpsxiso(int argc, char* argv[]);
#endif