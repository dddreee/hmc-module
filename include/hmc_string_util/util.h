/**
 * @file hmc_string_util/util.h
 * @author kiic
 * @brief 本模块是文本处理相关简单功能的合集
 * @version 0.1
 * @date 2024-01-25
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

// 防止重复导入
#ifndef MODE_INTERNAL_INCLUDE_HMC_STRING_UTIL_UG_HPP
#define MODE_INTERNAL_INCLUDE_HMC_STRING_UTIL_UG_HPP

#include <string>
#include <regex>
#include <vector>
#include <optional>
#include <functional>

class hmc_string_util
{
private:
public:

    enum IGNORE_CHAR_POINT_TYPE
    {
        // 不忽略出现的指定字符
        IGNORE_NOT = 0,
        // 任意地方出现的此字符直接忽略
        IGNORE_ALL = 10,
        // 开头是此字符时候请忽略
        IGNORE_START = 1,
        // 结尾是此字符时候请忽略他的存在
        IGNORE_END = 2,
        // 只忽略中间出现的此字符 如果他在开头结尾出现请继续执行而不是忽略
        IGNORE_MIDDLE = 3
    };

    /**
     * @brief 替换单次
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    static inline void replace(std::wstring &sourcePtr, const std::wstring from, const std::wstring to, bool isRepAll = false);

    /**
     * @brief 替换单次
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    static inline void replace(std::string &sourcePtr, std::string from, std::string to, bool isRepAll = false);

    /**
     * @brief 替换单次 但是批量
     *
     * @param sourcePtr 源文本
     * @param list 处理的内容
     */
    static void replace(std::string &sourcePtr, std::vector<std::tuple<std::string, std::string>> list, bool isRepAll = false);

    /**
     * @brief 替换单次 但是批量
     *
     * @param sourcePtr 源文本
     * @param list 处理的内容
     */
    static void replace(std::wstring &sourcePtr, std::vector<std::tuple<std::wstring, std::wstring>> list, bool isRepAll = false);

    /**
     * @brief 替换单次 但是批量
     *
     * @param sourcePtr 源文本
     * @param list 处理的内容
     */
    static void replace(std::string &sourcePtr, std::vector<std::tuple<char, char>> list, bool isRepAll = false);

    /**
     * @brief 替换单次 但是批量
     *
     * @param sourcePtr 源文本
     * @param list 处理的内容
     */
    static void replace(std::wstring &sourcePtr, std::vector<std::tuple<wchar_t, wchar_t>> list, bool isRepAll = false);

    /**
     * @brief 替换单次
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    static void replace(std::wstring &sourcePtr, const wchar_t from, const wchar_t to, bool isRepAll = false);

    /**
     * @brief 替换单次
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    static void replace(std::string &sourcePtr, char from, char to, bool isRepAll = false);

    /**
     * @brief 按照指定 字符/字符串 分割文本
     *
     * @param s
     * @param res
     * @return std::vector<std::string>
     */
    static std::vector<std::string> split(const std::string &source, const std::string &splitter);

    /**
     * @brief 按照指定 字符/字符串 分割文本
     *
     * @param s
     * @param res
     * @return std::vector<std::wstring>
     */
    static std::vector<std::wstring> split(const std::wstring &source, const std::wstring &splitter);

    /**
     * @brief 按照指定 字符/字符串 分割文本
     *
     * @param s
     * @param res
     * @return std::vector<std::string>
     */
    static inline std::vector<std::string> split(const std::string &source, const char &splitter);

    /**
     * @brief 按照指定 字符/字符串 分割文本
     *
     * @param s
     * @param res
     * @return std::vector<std::wstring>
     */
    static inline std::vector<std::wstring> split(const std::wstring &source, const wchar_t &splitter);

    /**
     * @brief 按照指定 字符/字符串 分割文本
     *
     * @param s
     * @param res
     * @return std::vector<std::string>
     */
    static inline std::vector<std::string> split(const std::string &source, const std::vector<char> &splitter);

    /**
     * @brief 按照指定 字符/字符串 分割文本
     *
     * @param s
     * @param res
     * @return std::vector<std::wstring>
     */
    static inline std::vector<std::wstring> split(const std::wstring &source, const std::vector<wchar_t> &splitter);

    /**
     * @brief 转大写
     *
     * @param data
     * @return std::string
     */
    static inline std::string to_upper(std::string data);

    /**
     * @brief 转大写
     *
     * @param data
     * @return std::wstring
     */
    static inline std::wstring to_upper(std::wstring data);

    /**
     * @brief 转小写
     *
     * @param data
     * @return std::string
     */
    static inline std::string to_lower(std::string data);

    /**
     * @brief 转小写
     *
     * @param data
     * @return std::wstring
     */
    static inline std::wstring to_lower(std::wstring data);

    /**
     * @brief 拼接文本
     *
     * @param item_list
     * @param splitter
     * @return std::string
     */
    static std::string join(std::vector<std::string> &item_list, std::string splitter = std::string());

    /**
     * @brief 拼接文本
     *
     * @param item_list
     * @param splitter
     * @return std::wstring
     */
    static std::wstring join(std::vector<std::wstring> &item_list, std::wstring splitter = std::wstring());

    /**
     * @brief 删除匹配到的字符 但是不复制内存
     *
     * @param inputPtr 需要处理的文本指针
     * @param match_char 需要匹配的字符
     * @param Ignore 忽略类型 [默认全部忽略]
     * - 10 / nullopt / None / all_ignore  全部忽略  无论他出现在哪里都不视为是需要对比的文本
     * - 2 / start_ignore 只忽略开头的空白字符
     * - 2 / end_ignore 只忽略尾部的空白字符
     * - 3 / middle_ignore 只忽略中间出现的空白字符 如果开头出现的则不管
     * @return true
     * @return false
     */
    static inline bool trim(std::string &inputPtr, const std::string &match, IGNORE_CHAR_POINT_TYPE Ignore = IGNORE_ALL);

    /**
     * @brief 删除匹配到的字符 但是不复制内存
     *
     * @param inputPtr 需要处理的文本指针
     * @param match_char 需要匹配的字符
     * @param Ignore 忽略类型 [默认全部忽略]
     * - 10 / nullopt / None / all_ignore  全部忽略  无论他出现在哪里都不视为是需要对比的文本
     * - 2 / start_ignore 只忽略开头的空白字符
     * - 2 / end_ignore 只忽略尾部的空白字符
     * - 3 / middle_ignore 只忽略中间出现的空白字符 如果开头出现的则不管
     * @return true
     * @return false
     */
    static inline bool trim(std::wstring &inputPtr, const std::wstring &match, IGNORE_CHAR_POINT_TYPE Ignore = IGNORE_ALL);

    /**
     * @brief 删除匹配到的字符 但是不复制内存
     *
     * @param inputPtr 需要处理的文本指针
     * @param match_char 需要匹配的字符
     * @param Ignore 忽略类型 [默认全部忽略]
     * - 10 / nullopt / None / all_ignore  全部忽略  无论他出现在哪里都不视为是需要对比的文本
     * - 2 / start_ignore 只忽略开头的空白字符
     * - 2 / end_ignore 只忽略尾部的空白字符
     * - 3 / middle_ignore 只忽略中间出现的空白字符 如果开头出现的则不管
     * @return true
     * @return false
     */
    static inline bool trim(std::string &inputPtr, const char &match, IGNORE_CHAR_POINT_TYPE Ignore = IGNORE_ALL);

    /**
     * @brief 删除匹配到的字符 但是不复制内存
     *
     * @param inputPtr 需要处理的文本指针
     * @param match_char 需要匹配的字符
     * @param Ignore 忽略类型 [默认全部忽略]
     * - 10 / nullopt / None / all_ignore  全部忽略  无论他出现在哪里都不视为是需要对比的文本
     * - 2 / start_ignore 只忽略开头的空白字符
     * - 2 / end_ignore 只忽略尾部的空白字符
     * - 3 / middle_ignore 只忽略中间出现的空白字符 如果开头出现的则不管
     * @return true
     * @return false
     */
    static inline bool trim(std::wstring &inputPtr, const wchar_t &match, IGNORE_CHAR_POINT_TYPE Ignore = IGNORE_ALL);

    /**
     * @brief 对比文本 但是忽略指定字符
     *
     * @param str1 文本1
     * @param str2 文本2
     * @param Ignore 忽略类型 [默认全部忽略]
     * - 10 / nullopt / None / all_ignore  全部忽略  无论他出现在哪里都不视为是需要对比的文本
     * - 2 / start_ignore 只忽略开头的空白字符
     * - 2 / end_ignore 只忽略尾部的空白字符
     * - 3 / middle_ignore 只忽略中间出现的空白字符 如果开头出现的则不管
     * @return true
     * @return false
     */
    static bool diffNulCharacters(const std::wstring str1, const std::wstring str2, IGNORE_CHAR_POINT_TYPE Ignore = IGNORE_ALL);

    /**
     * @brief 对比文本 但是忽略指定字符
     *
     * @param str1 文本1
     * @param str2 文本2
     * @param Ignore 忽略类型 [默认全部忽略]
     * - 10 / nullopt / None / all_ignore  全部忽略  无论他出现在哪里都不视为是需要对比的文本
     * - 2 / start_ignore 只忽略开头的空白字符
     * - 2 / end_ignore 只忽略尾部的空白字符
     * - 3 / middle_ignore 只忽略中间出现的空白字符 如果开头出现的则不管
     * @return true
     * @return false
     */
    static bool diffNulCharacters(const std::string str1, const std::string str2, IGNORE_CHAR_POINT_TYPE Ignore = IGNORE_ALL);

};

class lpString
{
private:
public:
};

class StringVarint
{
private:
    std::string _value_u8;
    std::wstring _value_u16;
    int _type_index;

public:
    StringVarint(std::string _value_u8);
    StringVarint(std::wstring _value_u16);
    StringVarint(char *_value_u8);
    StringVarint(wchar_t *_value_u16);
    StringVarint(char _value_u8);
    StringVarint(wchar_t _value_u16);
    StringVarint(std::vector<char> _value_u8);
    StringVarint(std::vector<wchar_t> _value_u16);
    StringVarint(char16_t *_value_u16);
    StringVarint(char16_t _value_u16);
    StringVarint(std::vector<char16_t> _value_u16);
    StringVarint(int size, char _value_u8);
    StringVarint(int size, wchar_t _value_u16);
    StringVarint(int size, char16_t _value_u16);
    bool isWide();
    bool isAscii();
    bool isUtf8();
    bool isAnsi();
};

#endif // MODE_INTERNAL_INCLUDE_HMC_STRING_UTIL_UG_HPP