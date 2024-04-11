/**
 * @file hmc_path_util.h
 * @author kiic / 凌原 / nuo / https://github.com/hypercore-cxx (未知授权) / node.js
 * @brief node 标准的路径处理工具
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

// 防止重复导入
#ifndef MODE_INTERNAL_INCLUDE_HMC_PATH_UTIL_H
#define MODE_INTERNAL_INCLUDE_HMC_PATH_UTIL_H

#include <vector>
#include <string>
#include <regex>

class hmc_path_util
{
private:
    using String = std::string;

    struct chParse
    {
        String dir;
        String root;
        String base;
        String ext;
        String name;
    };

#ifdef _WIN32 || defined __CYGWIN__
    static const bool IS_WIN32 = true;
    static const char SEPARATOR = '\\';
    static const char DELIMITER = ';';
#else
    static const bool IS_WIN32 = false;
    static const char DELIMITER = ':';
    static const char SEPARATOR = '/';
#endif

public:
    /**
     * @brief 方法使用特定于平台的分隔符作为定界符将所有给定的 path 片段连接在一起，然后规范化生成的路径
     *
     * @tparam Args
     * @param argsT
     * @return String
     */
    template <typename... Args>
    static String join(Args... argsT)
    {
        std::vector<String> args = {(String)argsT...};
        return joinVector(args);
    }

    /**
     * @brief  方法使用特定于平台的分隔符作为定界符将所有给定的 path 片段连接在一起，然后规范化生成的路径
     *
     * @param args
     * @return String
     */
    static String joinVector(const std::vector<String> &args);

    /**
     * @brief 方法使用特定于平台的分隔符作为定界符将所有给定的 path 片段连接在一起，然后规范化生成的路径 同时进行格式化
     * 
     * @param args 
     * @return String 
     */
    static String resolveVector(const std::vector<String>& args);

    /**
     *
     * @brief 将路径或路径片段的序列解析为绝对路径
     *
     * @tparam Args
     * @param argsT
     * @return String
     */
    template <typename... Args>
    static String resolve(Args... argsT)
    {
        std::vector<String> args = {(String)argsT...};
        return resolveVector(args);
    }

    /**
     * @brief 判断是否是路径分隔符号
     *
     * @param s
     * @return true
     * @return false
     */
    static bool isPathSeparator(const wchar_t s);

    /**
     * @brief 判断是否是路径分隔符号
     *
     * @param s
     * @return true
     * @return false
     */
    static bool isPathSeparator(const char s);

    /**
     * @brief 方法返回 path 的目录名，类似于 Unix dirname 命令。尾随的目录分隔符被忽略，见 path.sep
     *
     * @param path
     * @return String
     */
    static String dirName(const String path);

    /**
     * @brief 反格式化parse
     *
     * @param input
     * @return String
     */
    static String format(const chParse input);

    /**
     * @brief 路径解析
     *
     * @param path
     * @return chParse
     */
    static chParse parse(const String path);

    /**
     * @brief 提供特定于平台的路径定界符：
     *
     * @return String
     */
    static String delimiter();

    /**
     * @brief 方法返回 path 的扩展名
     * @details 即 path 的最后一部分中从最后一次出现的 .（句点）字符到字符串的结尾。如果 path 的最后一部分中没有 .，或者除了 path 的基本名称（参见 path.basename()）的第一个字符之外没有 . 个字符，则返回空字符串
     * @param path
     * @return String
     */
    static String extName(const String path);

    /**
     * @brief 将路径或路径片段的序列解析为绝对路径
     *
     * @param args
     * @return String
     */
    static String resolveFromVector(const std::vector<String> &args);

    /**
     * @brief 获取文件名 全名
     *
     * @param path
     * @return String
     */
    static String baseName(const String path);

    /**
     * @brief 分割路径为数组
     *
     * @param path
     * @return std::string
     */
    static std::vector<String> splitPath(const String path);

    /**
     * @brief 合并路径 并且不对其进行其他处理
     *
     * @param list
     * @return String
     */
    static String joinPathList(const std::vector<String> &list);

    /**
     * @brief 格式化路径
     *
     * @param path
     * @return String
     */
    static String resolve(const String path);

    /**
     * @brief 确定 path 是否为绝对路径。
     *
     * @param path
     * @return true
     * @return false
     */
    static bool isAbsolute(const String &path);

    /**
     * @brief 法规范化给定的 path，解析 '..' 和 '.' 片段
     * @details 当找到多个连续的路径片段分隔符（例如 POSIX 上的 / 和 Windows 上的 \ 或 /）时，则它们将被平台特定路径片段分隔符（POSIX 上的 / 和 Windows 上的 \）的单个实例替换。保留尾随的分隔符
     * @param parts
     * @param allowAboveRoot
     * @return std::vector<String>
     */
    static std::vector<String> normalizePathList(std::vector<String> parts, bool allowAboveRoot);

    /**
     * @brief 法规范化给定的 path，解析 '..' 和 '.' 片段
     * @details 当找到多个连续的路径片段分隔符（例如 POSIX 上的 / 和 Windows 上的 \ 或 /）时，则它们将被平台特定路径片段分隔符（POSIX 上的 / 和 Windows 上的 \）的单个实例替换。保留尾随的分隔符
     * @return std::vector<String>
     */
    static String normalize(const String &path);

    /**
     * @brief 当前平台的路径分割符号
     *
     * @return char
     */
    static char sep();
};

#endif // MODE_INTERNAL_INCLUDE_HMC_PATH_UTIL_H