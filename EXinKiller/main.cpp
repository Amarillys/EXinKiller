/*
EXin(Hubei University Version)
By Maikaze 2017/03/25

referred from http://www.cnblogs.com/LCCRNblog/p/4652374.html
*/
#include <iostream>  
#include <string>  
#include <windows.h>  
#include <TlHelp32.h>

using stdstr = std::string;
//For example: "21067290-D949-471E-A3E4-099FD470989A.exe" + "\0" = 41
const int eXinEXELength = 41;
const char eXinEXEUpdateName[11] = "update.exe";

int main()
{
    //create the snapshot of all the process
    HANDLE hPS = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);    
    if (hPS == INVALID_HANDLE_VALUE)
    {
            std::cout << "CreateToolhelp32Snapshot Error!" << std::endl;;
            return -1;
    }

    //init the loop
    PROCESSENTRY32 pe;
    //very important to avoid the error
    pe.dwSize = sizeof(pe);

    int bRsl = Process32First(hPS, &pe);

    stdstr eXin;
    unsigned int eXinID = 0, eXinUpdateID = 0;
    char tmpstr[255];
    size_t p_len = 0;

    //loop and find the target process
    while (bRsl > 0)
    {
        //convert wchar to char
        wcstombs_s(&p_len, tmpstr, pe.szExeFile, wcslen(pe.szExeFile));

        if (p_len == eXinEXELength)
        {
            eXin = stdstr(tmpstr);
            eXinID = pe.th32ProcessID;
        }

        //the name "update.exe" is so common so that i just compare its pid with exin .
        if (strncmp(tmpstr, eXinEXEUpdateName, 11) == 0)
        {
            if (pe.th32ProcessID > eXinID)
            {
                eXinUpdateID = pe.th32ProcessID;
            }
        }
        bRsl = Process32Next(hPS, &pe);
    }
    //Run the command, i think it need the admin permission.
    stdstr taskkillCmd;
    
    if (eXin != "")
    {
        taskkillCmd = "taskkill /F /IM " + eXin;
        system(taskkillCmd.c_str());
    }

    if (eXinUpdateID > 0)
    {
        taskkillCmd = "taskkill /F /PID " + std::to_string(eXinUpdateID);
        system(taskkillCmd.c_str());
    }
    return 0;
}
