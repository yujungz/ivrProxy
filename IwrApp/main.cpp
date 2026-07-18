#include "IwrApp.h"
#include <QtWidgets/QApplication>

#pragma comment(lib,"ws2_32.lib")

CAppCfg* gpCfg = nullptr;
int main(int argc, char *argv[])
{
    qputenv("QT_SSL_USE_TEMPORARY_KEYCHAIN", "1");
    qSetMessagePattern("%{time hh:mm:ss.zzz}: %{message}");

    CAppCfg*& pCfg = gpCfg;
    pCfg = new CAppCfg(argc, (const char**&)argv);

    QApplication a(pCfg->argc, pCfg->argv);

    if (!pCfg->testReload()) {   //add
        a.exit(-1);
        return NULL;
    }

    IwrApp w(pCfg);
    w.show();
    return a.exec();
}
