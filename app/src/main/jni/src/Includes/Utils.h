#ifndef UTILS
#define UTILS

#include <jni.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include "Logger.h"

extern const char* libName;

typedef unsigned long DWORD;
static uintptr_t libBase;

DWORD findLibrary(const char *library);

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr);

bool isLibraryLoaded(const char *libraryName);

void MakeToast(JNIEnv *env, jobject thiz, const char *text, int length);

std::string s(const std::string &str);

uintptr_t string2Offset(const char *c);

void writeToFile(std::string dir, std::string buf);

namespace Toast {
    inline const int LENGTH_LONG = 1;
    inline const int LENGTH_SHORT = 0;
}


template<typename ... Args>
std::string stringFormat(const std::string& format, Args ... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    if(size <= 0) { return ""; }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

#endif