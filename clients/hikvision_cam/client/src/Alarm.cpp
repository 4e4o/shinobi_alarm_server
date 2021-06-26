#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>

#include "public.h"
#include "Alarm.h"

void CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER */*pAlarmer*/,
                              char *pAlarmInfo, DWORD /*dwBufLen*/, void* /*pUser*/) {
    NET_DVR_ALARMINFO_V30 struAlarmInfo;
    memcpy(&struAlarmInfo, pAlarmInfo, sizeof(NET_DVR_ALARMINFO_V30));

    std::cout << "lCommand is " << lCommand << ", alarm type is " << struAlarmInfo.dwAlarmType << std::endl;

    switch(lCommand) {
    case COMM_ALARM_V30: {
        switch (struAlarmInfo.dwAlarmType) {
        case 3:
            std::cout << "Motion event" << std::endl;

            /*            for (int i=0; i<16; i++) {
                if (struAlarmInfo.byChannel[i] == 1) {
                    std::cout << "Motion detection " << i+1 << std::endl;
                }
            }*/
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
}

int Run_Alarm(const char* ip, const char *port, const char* login, const char* pass) {
    if (Run_AlarmFortify(ip, port, login, pass) == HPR_ERROR) {
        return HPR_ERROR;
    }

    return HPR_OK;
}

static int setupAlarm(LONG lUserID) {
    NET_DVR_SetDVRMessageCallBack_V51(0, MessageCallback, NULL);

    LONG lHandle;
    NET_DVR_SETUPALARM_PARAM_V50 struSetupAlarmParam{};
    struSetupAlarmParam.dwSize = sizeof(struSetupAlarmParam);
    struSetupAlarmParam.byRetVQDAlarmType = TRUE; // Prefer VQD Alarm type of NET_DVR_VQD_ALARM
    struSetupAlarmParam.byRetAlarmTypeV40 = TRUE;
    struSetupAlarmParam.byFaceAlarmDetection = 1;

    struSetupAlarmParam.byRetDevInfoVersion = TRUE;
    struSetupAlarmParam.byAlarmInfoType = 1;
    struSetupAlarmParam.bySupport = 4;
    lHandle = NET_DVR_SetupAlarmChan_V50(lUserID, &struSetupAlarmParam, NULL, 0);

    if (lHandle < 0) {
        std::cout << "NET_DVR_SetupAlarmChan_V50 error, " << NET_DVR_GetLastError() << std::endl;
        return HPR_ERROR;
    }

    //while(1) {
        sleep(5);   //second
    //}

    if (!NET_DVR_CloseAlarmChan_V30(lHandle)) {
        std::cout << "NET_DVR_CloseAlarmChan_V30 error, " << NET_DVR_GetLastError() << std::endl;
        return HPR_ERROR;
    }

    return HPR_OK;
}

static int Login(const char* ip, const char *port, const char* login, const char* pass) {
    LONG lUserID;
    NET_DVR_USER_LOGIN_INFO struLoginInfo{};
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40{};

    struLoginInfo.bUseAsynLogin = false;
    struLoginInfo.wPort = std::stoi(port);

    memcpy(struLoginInfo.sDeviceAddress, ip, NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(struLoginInfo.sUserName, login, NAME_LEN);
    memcpy(struLoginInfo.sPassword, pass, NAME_LEN);

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);

    if (lUserID < 0) {
        std::cout << "Login error, " << NET_DVR_GetLastError() << std::endl;
        fflush(0);
        return HPR_ERROR;
    }

    if (!NET_DVR_SetReconnect(5000, 1)) {
        return HPR_ERROR;
    }

    setupAlarm(lUserID);
    NET_DVR_Logout_V30(lUserID);
    return HPR_OK;
}

int Run_AlarmFortify(const char* ip, const char *port, const char* login, const char* pass) {
    if (NET_DVR_Init()) {
        Login(ip, port, login, pass);
        NET_DVR_Cleanup();
        return HPR_OK;
    }

    return HPR_ERROR;
}
