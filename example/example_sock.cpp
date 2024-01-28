#include "../include/hmc_sock_util.h"
#include "../include/hmc_util.h"
#include "../include/hmc_console_util.h"

#include <iostream>
#include <windows.h>

#include <string>

void fn_enumConnectNet()
{

    std::cout << "--------------------------------------------------------" << "\n" << std::endl;

    auto enumConnectNet_0o1111 = hmc_sock_util::enumConnectNet(true, true, true, true);
    auto enumConnectNet_0o0001 = hmc_sock_util::enumConnectNet(false, false, false, true);
    auto enumConnectNet_0o0010 = hmc_sock_util::enumConnectNet(false, false, true, false);
    auto enumConnectNet_0o0100 = hmc_sock_util::enumConnectNet(false, true, false, false);
    auto enumConnectNet_0o1000 = hmc_sock_util::enumConnectNet(true, false, false, false);

    std::cout << "[enumConnectNet_0o1111] << " << enumConnectNet_0o1111.size() << std::endl;
    std::cout << "[enumConnectNet_0o0001] << " << enumConnectNet_0o0001.size() << std::endl;
    std::cout << "[enumConnectNet_0o0010] << " << enumConnectNet_0o0010.size() << std::endl;
    std::cout << "[enumConnectNet_0o0100] << " << enumConnectNet_0o0100.size() << std::endl;
    std::cout << "[enumConnectNet_0o0001] << " << enumConnectNet_0o1000.size() << std::endl;
    hmc_console_util::start(true, true, true);

    for (auto &&Connect : enumConnectNet_0o1111)
    {
        if (Connect.index() == 0)
        {
            auto item = std::get<0>(Connect);
            std::wcout << item.to_json() << "\n";
        }

        if (Connect.index() == 1)
        {
            auto item = std::get<1>(Connect);
            std::wcout << item.to_json() << "\n";
        }
    }

    std::wcout << "--------------------------------------------------------" << "\n" << std::endl;

    hmc_console_util::stop(true, true, true);
}

void fn_eqProxyServer()
{
    hmc_console_util::start(true, true, true);

    std::wcout << "--------------------------------------------------------" << "\n" << std::endl;
    std::wcout << L"[getSystemProxyServer] << " << std::endl;

    auto SystemProxyServer = hmc_sock_util::getSystemProxyServer();

    if (SystemProxyServer.has_value())
    {
        std::wcout << SystemProxyServer.value() << "\n";
    }
    else
    {
        std::wcout << "none"
                   << "\n";
    }

    std::wcout << "--------------------------------------------------------" << "\n" << std::endl;

    std::wcout << L"[getSystemProxyPac] << " << std::endl;

    auto SystemProxyPac = hmc_sock_util::getSystemProxyPac();

    if (SystemProxyPac.has_value())
    {
        std::wcout << SystemProxyPac.value() << "\n";
    }
    else
    {
        std::wcout << "none"
                   << "\n";
    }

    std::wcout << "--------------------------------------------------------" << "\n" << std::endl;

    std::wcout << L"[getDomainAddress] << " << std::endl;

    for (auto &&i : hmc_sock_util::getDomainAddress(L"baidu.com"))
    {
        std::wcout << i << std::endl;
    }

    std::wcout << L"[getHostsPath] << " << std::endl;
    std::wcout << hmc_sock_util::getHostsPath() << std::endl;

    std::wcout << "--------------------------------------------------------" << "\n" << std::endl;

    hmc_console_util::stop(true, true, true);
}

void fm_getAdapterIPList()
{
    hmc_console_util::start(true, true, true);
    std::wcout << "--------------------------------------------------------" << "\n" << std::endl;

    auto list = hmc_sock_util::getAdapterIPList();

    for (size_t i = 0; i < list.size(); i++)
    {
        auto &it = list[i];

        std::wcout << it.to_json() << std::endl;
    }

    std::wcout << "--------------------------------------------------------" << "\n" << std::endl;

    hmc_console_util::stop(true, true, true);
}

int main()
{
    fn_enumConnectNet();
    fn_eqProxyServer();

    std::cout << "--------------------------------------------------------" << "\n" << std::endl;
    std::wcout << L"[getNetParams()] << " << std::endl;
    std::cout << hmc_sock_util::getNetParams().to_json() << std::endl;
    std::cout << "--------------------------------------------------------" << "\n" << std::endl;

    fm_getAdapterIPList();

    std::cout << std::endl;
}
