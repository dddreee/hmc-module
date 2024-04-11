/**
 * @file hmc_string_reg_util.h
 * @author kiic
 * @brief 本模块是文本处理相关功能的合集
 * @version 0.1
 * @date 2024-01-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

// 防止重复导入
#ifndef MODE_INTERNAL_INCLUDE_HMC_STRING_REGEX_UTIL_HPP
#define MODE_INTERNAL_INCLUDE_HMC_STRING_REGEX_UTIL_HPP

#include <string>
#include <regex>
#include <vector>
#include <optional>
#include <functional>

namespace hmc_string_util
{
    using ReplaceCallback = std::function<std::string(const std::string &, int)>;
    inline std::vector<std::string> split(const std::string &s, const std::string &res);
    inline std::vector<std::wstring> split(const std::wstring &s, const std::wstring &res);
    inline std::vector<std::string> split(const std::string &s, const char &res);
    inline std::vector<std::wstring> split(const std::wstring &s, const wchar_t &res);
    inline std::vector<std::string> split(const std::string &s, const std::string &res, const std::string &flag);
    inline std::vector<std::wstring> split(const std::wstring &s, const std::wstring &res, const std::string &flag);

};

#endif // MODE_INTERNAL_INCLUDE_HMC_STRING_REGEX_UTIL_HPP