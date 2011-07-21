#ifndef DXTCHAROFSTREAM_H
#define DXTCHAROFSTREAM_H

#include <fstream>

using namespace std;

#ifdef UNICODE

class dxTCHARofstream : public wofstream {
#else
class dxTCHARofstream : public ofstream {
#endif
};

#endif