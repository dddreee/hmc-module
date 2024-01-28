﻿/**
 * @file hmc_sock_util.h
 * @author kihlh, kiic
 * @brief 网络模块
 * @note windows.h 会默认把<winsock.h> 导入因此此模块需要在windows.h导入之前导入
 * @version 0.1
 * @date 2024-01-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

// 防止重复导入
#ifndef MODE_INTERNAL_INCLUDE_HMC_SOCK_UTIL_HPP
#define MODE_INTERNAL_INCLUDE_HMC_SOCK_UTIL_HPP

// WINSOCKAPI 会和winsock2.h 冲突 难以避免 索性提前禁用 WINSOCKAPI
// !  windows.h 会默认把<winsock.h> 放进来 如果在之前引用了需要把这句放进前面 但是 winsock.h 会和 winsock2.h 冲突导致无法使用ipv6
#define _WINSOCKAPI_

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include <windows.h>
#include <vector>
#include <set>
#include <variant>
#include <optional>

#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <ip2string.h>
#include <iphlpapi.h>
#include <Psapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Ntdll.lib")

namespace hmc_sock_util
{
    namespace hmc_define_util
    {

    }

    // 枚举本机ip本机局域网的ip(v4) 数据体
    struct chLocalVddrItemV4
    {
        // 广播
        std::wstring broadcast;
        // 掩码
        std::wstring netmask;
        // ip
        std::wstring ipaddress;
        std::wstring to_json();
    };

    struct chNetDnsServers
    {
        std::string IpAddress;
        std::string IpMask;
        DWORD context;
        std::string to_json();
    };

    // 主机网络信息
    struct chNetParams
    {
        std::string hostName;                    // 主机名称
        std::string domainName;                  // 域名名称
        std::vector<chNetDnsServers> dnsServers; // dns服务器列表
        /**
         * @brief 节点类型
         * - BROADCAST_NODETYPE;
         * - HYBRID_NODETYPE
         * - MIXED_NODETYPE
         * - PEER_TO_PEER_NODETYPE
         * - UNKNOWN:<UINT>
         */
        std::string nodeType;

        std::string dhcpScopeName; // dhcp 范围名称
        bool enableRouting;        // 是否启用路由选择
        bool enableArpProxy;       // 是否ARP代理
        bool enableDns;            // 是否启用dns
        std::string to_json();
    };

    // 检索 本机 连接表数据体
    // https://en.wikipedia.org/wiki/Transmission_Control_Protocol
    // https://learn.microsoft.com/zh-cn/windows/win32/api/iphlpapi/nf-iphlpapi-gettcp6table
    struct chConnectNet
    {
        std::wstring typeName; // "UDP" / "TCP" / "TCP6" / "UDP6"
        DWORD type = 0;        // 6 tcp / tcp6   17 udp / udp6
        DWORD ipAddr;          // dwLocalAddr
        std::wstring ip;       // 解析出来的实际ip
        DWORD port;            // 占用的端口
        DWORD pid;             // 占用此端口的进程
        bool existsProcess;    // 判断此进程是否存在
        std::wstring to_json();
    };

    struct chNetAdapterAddrList
    {
        // ip
        std::wstring ipaddress;
        // ip类型名称  ipv4 or ipv6
        std::wstring typeName;
        // ip类型 2(AF_INET) 23(AF_INET6)
        DWORD type = 0;
        std::wstring to_json();
    };

    struct chConnectNetTCP : public chConnectNet
    {
        DWORD remotePort;      // 远程的端口
        DWORD remoteIPAddr;    // 远程的ip (LocalAddr)
        std::wstring remoteIP; // 解析出来的实际远程ip
        std::wstring state;    // 状态码 "CLOSED"|"LISTEN"|"SYN-SENT"|"SYN-RECEIVED"|"ESTABLISHED"|"FIN-WAIT-1"|"FIN-WAIT-2"|"CLOSE-WAIT"|"CLOSING"|"LAST-ACK"|"TIME-WAIT"|"DELETE-TCB"|"UNKNOWN"
        std::wstring to_json();
    };

    // 获取本机局域网的所有ip ( 并按照适配器名称区分 ,v4,v6 )
    struct chNetAdapterAddrItem
    {
        // 网络适配器名称
        std::wstring name;
        // ip 列表
        std::vector<chNetAdapterAddrList> addr_item;
        std::wstring to_json();
    };

    // 格式化ip
    extern std::wstring formatIP(UCHAR ucLocalAddr[16]);
    // 格式化ip
    extern std::wstring formatIP(IN6_ADDR dwLocalAddr);
    // 格式化ip
    extern std::wstring formatIP(DWORD dwLocalAddr);

    /**
     * @brief 枚举本机连接 端口
     *
     * @param tcp 是否枚举 tcp v4 端口
     * @param udp 是否枚举 udp v4 端口
     * @param tcp6 是否枚举 tcp v6 端口
     * @param udp6 是否枚举 udp v6 端口
     * @return std::vector<std::variant<chConnectNet>>
     */
    extern std::vector<std::variant<chConnectNet, chConnectNetTCP>> enumConnectNet(bool tcp, bool udp, bool tcp6, bool udp6);

    /**
     * @brief 获取tcp6 端口关联进程
     *
     * @param port 端口
     * @return DWORD
     */
    extern DWORD getPortProcessTCP(DWORD port);

    /**
     * @brief 获取tcp6 端口关联进程
     *
     * @param port 端口
     * @return DWORD
     */
    extern DWORD getPortProcessUDP(DWORD port);

    /**
     * @brief 获取tcp6 端口关联进程
     *
     * @param port 端口
     * @return std::vector<DWORD>
     */
    extern std::vector<DWORD> getPortProcessUDP6(DWORD port);

    /**
     * @brief 获取tcp6 端口关联进程
     *
     * @param port 端口
     * @return std::vector<DWORD>
     */
    extern std::vector<DWORD> getPortProcessTCP6(DWORD port);

    /**
     * @brief 状态码转换
     *
     * @param dwState 状态码
     * @return std::wstring
     */
    extern std::wstring getStateName(DWORD dwState);

    /**
     * @brief 获取本机局域网的所有ip ( 并按照适配器名称区分 ,v4,v6 )
     *
     * @return std::vector<chNetAddrItem>
     */
    extern std::vector<chNetAdapterAddrItem> getAdapterIPList();

    /**
     * @brief 主机网络信息
     *
     * @return chNetParams
     */
    extern chNetParams getNetParams();

    /**
     * @brief 获取系统代理链接（仅在启用系统代理时）
     *
     * @return std::optional<std::wstring>
     */
    extern std::optional<std::wstring> getSystemProxyServer();

    /**
     * @brief 获取系统代理pac脚本链接
     *
     * @return std::optional<std::wstring>
     */
    extern std::optional<std::wstring> getSystemProxyPac();

    /**
     * 从默认dns(联网)解析域名主机ip
     * - √ goole.com
     * - √ api.goole.com
     * - × api.goole.com/services
     * - × https://goole.com
     * @param url
     * @returns ip列表
     */
    extern std::vector<std::wstring> getDomainAddress(std::wstring domainName);

    /**
     * @brief 获取hosts文件的路径
     *
     * @return std::wstring
     */
    extern std::wstring getHostsPath();

    /**
     * @brief ip 列表 转换函数 [内联]
     *
     * @param ua
     * @return chNetAdapterAddrList
     */
    extern chNetAdapterAddrList CpUaToAddrItem(PIP_ADAPTER_UNICAST_ADDRESS ua);

    /**
     * @brief 枚举本机TCP v6 当前端口列表
     *
     * @return std::vector<chConnectNetTCP>
     */
    extern std::vector<chConnectNetTCP> CpEnumConnectNetConnectTCP6();

    /**
     * @brief 枚举本机TCP v4 当前端口列表
     *
     * @return std::vector<chConnectNetTCP>
     */
    extern std::vector<chConnectNetTCP> CpEnumConnectNetConnectTCP4();

    /**
     * @brief 枚举本机UDP v6 当前端口列表
     *
     * @return std::vector<chConnectNet>
     */
    extern std::vector<chConnectNet> CpEnumConnectNetConnectUDP6();

    /**
     * @brief 枚举本机UDP v4 当前端口列表
     *
     * @return std::vector<chConnectNet>
     */
    extern std::vector<chConnectNet> CpEnumConnectNetConnectUDP4();

    typedef std::variant<
        chConnectNet, chConnectNetTCP, chNetAdapterAddrList, chNetParams,
        std::vector<chNetAdapterAddrItem>,
        std::vector<DWORD>,
        std::vector<std::variant<chConnectNet, chConnectNetTCP>>>
        SockJsonType;

    typedef std::vector<
        std::variant<chConnectNet, chConnectNetTCP>>
        ConnectNetList;

    extern std::variant<std::wstring, std::string> to_json(SockJsonType input);
};

#endif // MODE_INTERNAL_INCLUDE_HMC_SOCK_UTIL_HPP