#include "utils.h"

std::string _utf8ansi874(const std::string &utf8)
{
    int lenW = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    std::wstring wstr(lenW, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], lenW);

    int lenA = WideCharToMultiByte(874, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL); // 874 = Thai ANSI
    std::string ansi(lenA, 0);
    WideCharToMultiByte(874, 0, wstr.c_str(), -1, &ansi[0], lenA, NULL, NULL);

    return ansi;
}
