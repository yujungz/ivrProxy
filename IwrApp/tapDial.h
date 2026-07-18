#pragma once

#include <QString>


struct stTapDial {

    char        InCallID[80];
    char        InCaller[80];
    char        InCalled[80];
    char        InCallerShow[80];
    int         ServerType;
    int         CallType;
    char        InParam[260];
    char        ReportUrl[260];

    char        appId[40];
    quint64     timestamp;
    char        sign_name[200];
    char        ServiceId[80];
    int         idx;
    quint64     tranID;
    quint64     stpHookOffCaller;
    quint64     stpHookOffCalled;
  //  quint64     endTimestamp;
};
