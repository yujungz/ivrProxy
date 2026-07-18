#include "stdafx.h"
#include "wshProxy.h"
#include <QtWidgets/QApplication>

#pragma comment(lib,"ws2_32.lib")

CAppCfg* gpCfg = nullptr ;

int main(int argc, char *argv[])
{
    CAppCfg*& pCfg = gpCfg;
    pCfg = new CAppCfg(argc, (const char**&)argv);

    QApplication a(pCfg->argc, pCfg->argv);
    if (!pCfg->testReload()) {   //add
        a.exit(-1);
        return NULL;
    }
    wshProxy w(pCfg);
    w.show();
    return a.exec();
}
