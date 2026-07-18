#pragma once
#include <cmmdef.h>

#define BUF_POOL_MAX    64

class CBufPool {
public:
    CBufPool(const int n = 0);
    ~CBufPool();

    bool init(const int n = 1);
    void free(const bool bAll = true);
    char* create(const long long nSize);
private:
    int m_nBuf,m_nIdx;
    long long m_nSizeCur;
    char** m_sBufs;

    int  bufLimit(const int n, const int nMax = BUF_POOL_MAX, const int nMin = 1);
};