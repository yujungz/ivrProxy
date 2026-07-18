#include <bufPool.h>

CBufPool::CBufPool(const int n) : m_sBufs(nullptr), m_nBuf(n) , m_nIdx(0), m_nSizeCur(0) {
    this->init(n);
}


CBufPool::~CBufPool() {

    this->free();
}

int  CBufPool::bufLimit(const int n, const int nMax , const int nMin) {
    int nRet = n;
    if (nRet < nMin) nRet = nMin;
    else if (nRet > nMax) nRet = nMax;

    return nRet;
}

bool CBufPool::init(const int n) {
    if (n < 1) {
        return false;
    }
    if (nullptr != m_sBufs) {
        if (n == m_nBuf) {
            return true;
        }
        else {
            this->free();
        }
    }
    m_nBuf = this->bufLimit(n);

    int i;
    m_sBufs = new char* [n];
    for (i = 0; i < n; ++i) {
        m_sBufs[i] = nullptr;
    }
    return true;
}

void CBufPool::free(const bool bAll) {
    if (nullptr == m_sBufs || 1 > m_nBuf) {
        return;
    }

    int i, n = m_nBuf;
    for (i = 0; i < n; ++i) {
        YQ_FREE_VAR(m_sBufs[i]);
    }
    if (bAll) {
        YQ_FREE_VAR(m_sBufs);
    }

}

char* CBufPool::create(const long long nSize) {
    if (nSize < 1 || m_nBuf < 1 || m_nIdx < 0 || nullptr == m_sBufs) {
        return nullptr;
    }

    int n = m_nBuf;
    int& idx = m_nIdx;
    if (idx >= n) {
        idx  = 0;
    }
    bool bNew = nSize > m_nSizeCur;
    if (bNew) {
        m_nSizeCur = nSize;
        this->free(false);
    }
    char*& pBuf = m_sBufs[idx];
    if (nullptr == pBuf) {
        pBuf = new char[m_nSizeCur + 1];
    }
    ++idx;

    return pBuf;
}