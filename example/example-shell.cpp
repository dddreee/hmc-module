#include "../include/hmc_sock_util.h"
#include "../include/hmc_util.h"
#include "../include/hmc_console_util.h"

#include <iostream>
#include "../include/hmc_shell_util.h"
#include <windows.h>

#include <string>

int main()
{

   GetThumbnail: {
        std::cout << "\n\n\n--------------------------- < hmc_shell_util::GetThumbnail > ---------------------\n\n" << std::endl;

        const std::vector<BYTE> buff_list = hmc_shell_util::GetThumbnail::GetBuff(L"C:\\Windows\\explorer.exe", 256);

        // 获取二进制
        if (!buff_list.empty() && (buff_list[1] == 'P' || buff_list[2] == 'N' || buff_list[3] == 'G'))
        {
            std::cout << "GetBuff >> [is png ok] " << std::endl;
        }
        else
        {
            std::cout << "GetBuff >> [error] " << std::endl;
        }

        // 获取到文件
        const bool GetThumbnail_is_ok = hmc_shell_util::GetThumbnail::toFile(L"C:\\Windows\\explorer.exe", L"C:\\1.png", 256);
        if (GetThumbnail_is_ok)
        {
            std::cout << "toFile >> [is png ok] " << std::endl;
        }
        else
        {
            std::cout << "toFile >> [error] " << std::endl;
        }
    }

    SetFolderIcon: {
        std::cout << "\n\n\n--------------------------- < hmc_shell_util::SetFolderIcon > ---------------------\n\n" << std::endl;

        const bool t01 = hmc_shell_util::SetFolderIcon(L"C:\\TestFolder", L"C:\\Windows\\explorer.exe", 0);
        std::cout << "SetFolderIcon  << " << std::boolalpha << t01 << std::endl;
    }

    SetShortcutLink: {
        std::cout << "\n\n\n--------------------------- < hmc_shell_util::setShortcutLink > ---------------------\n\n" << std::endl;

        const bool t02 = hmc_shell_util::setShortcutLink(L"C:\\setShortcutLink.ink", L"C:\\Windows\\explorer.exe", L"C:\\Windows\\", L"desc", L"cmd", SW_SHOW, L"C:\\Windows\\regedit.exe", 0);
        std::cout << "SetShortcutLink  << " << std::boolalpha << t02 << std::endl;
    }

    GetShortcutLink: {
        std::cout << "\n\n\n--------------------------- < hmc_shell_util::getShortcutLink > ---------------------\n\n" << std::endl;

        auto temp_lnk = hmc_shell_util::getShortcutLink(L"C:\\setShortcutLink.ink");

        std::cout << "isValid  << " << temp_lnk.isValid() << std::endl;

        hmc_console_util::start(true,true,true);
        std::wcout << "path     << " << temp_lnk.path << std::endl;
        std::wcout << "cwd      << " << temp_lnk.cwd << std::endl;
        std::wcout << "desc     << " << temp_lnk.desc << std::endl;
        std::wcout << "icon     << " << temp_lnk.icon << "       [ " << temp_lnk.iconIndex << " ] " << std::endl;
        hmc_console_util::stop(true,true,true);
        
        std::cout << "showCmd  << " << temp_lnk.showCmd << std::endl;
        std::cout << "hotkey   << " << temp_lnk.hotkey << std::endl;

    }

    CopyFile:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::CopyFile > ---------------------\n\n" << std::endl;

        const bool t01 = hmc_shell_util::copyFile(L"C:\\1.png", L"C:\\copy_ok.png", true , true ,true );
        std::cout << "CopyFile  << " << std::boolalpha << t01 << std::endl;
    }


    MoveFile:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::MoveFile > ---------------------\n\n" << std::endl;

        const bool t01 = hmc_shell_util::moveFile(L"C:\\copy_ok.png", L"C:\\copy_move.png", true , true ,true );
        std::cout << "MoveFile  << " << std::boolalpha << t01 << std::endl;
    }


    TrashFile:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::TrashFile > ---------------------\n\n" << std::endl;

        const bool t01 = hmc_shell_util::trashFile( L"C:\\copy_ok.png" , true ,true );
        std::cout << "TrashFile  << " << std::boolalpha << t01 << std::endl;
    }


    GetTimestamp:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::GetTimestamp > ---------------------\n\n" << std::endl;

        const auto t01 = hmc_shell_util::getTimestamp(false);
        std::cout << "getTimestamp  << " << t01 << std::endl;
    }


    GetSystemIdleTime:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::GetSystemIdleTime > ---------------------\n\n" << std::endl;

        const auto t01 = hmc_shell_util::getSystemIdleTime();
        std::cout << "getTimestamp  << " << t01 << std::endl;
    }


    Beep:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::beep > ---------------------\n\n" << std::endl;

         hmc_shell_util::beep();

    }


    GetTaryIconList:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::GetTaryIconList > ---------------------\n\n" << std::endl;

        hmc_console_util::start(true,true,true);
        const auto t01 = hmc_shell_util::GetTaryIconList::getTrayListJsonW();
        std::wcout << "GetTaryIconList  << " << t01 << std::endl;
        hmc_console_util::stop(true,true,true);

    }


    IsSystemFor64bit:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::IsSystemFor64bit > ---------------------\n\n" << std::endl;

        const auto t01 = hmc_shell_util::isSystemFor64bit();
        std::wcout << "IsSystemFor64bit  << " << t01 << std::endl;
        
    }
    

    ShowContextMenu:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::showContextMenu > ---------------------\n\n" << std::endl;

        const auto t01 = hmc_shell_util::showContextMenu(NULL,L"C:\\1.png",0,0);
        std::wcout << "ShowContextMenu  << " << t01 << std::endl;
        
    }


    ShowItemInFolder:{
         std::cout << "\n\n\n--------------------------- < hmc_shell_util::showItemInFolder > ---------------------\n\n" << std::endl;

        const auto t01 = hmc_shell_util::ShellOpen::showItemInFolder(L"C:\\1.png",true);
        std::wcout << "ShowItemInFolder  << " << t01 << std::endl;
        
    }


    HardLink:{
        std::cout << "\n\n\n--------------------------- < hmc_shell_util::HardLink > ---------------------\n\n" << std::endl;
        std::wstring HardLink = L"C:\\Windows\\explorer.exe";
        
        std::cout << "isHardLink  << " << std::boolalpha << hmc_shell_util::Symlink::isHardLink(HardLink) << std::endl;
        
        const auto t01 = hmc_shell_util::Symlink::createHardLink(HardLink,L"C:\\1.exe");
        std::cout << "createHardLink  << " << std::boolalpha << t01 << std::endl;
        
        auto HardLinkList = hmc_shell_util::Symlink::getHardLinkList(HardLink);

        hmc_console_util::start(true,true,true);
        
        size_t count = HardLinkList.size();

        for (size_t i = 0; i < count; i++)
        {
            auto link = HardLinkList[i];
            std::wcout << link << std::endl;
        }
        

        hmc_console_util::stop(true,true,true);
    }



    std::cin.get();
}
