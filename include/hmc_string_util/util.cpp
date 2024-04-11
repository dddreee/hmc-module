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

#include <hmc_string_util/util.h>

std::vector<std::string> hmc_string_util::split(const std::string &source, const std::string &splitter)
{
    std::vector<std::string> result;
    if (source.empty())
    {
        return result;
    }
    size_t start = 0, index = 0;

    while ((index = source.find(splitter, start)) != std::string::npos)
    {
        if (index > start)
        {
            result.push_back(source.substr(start, index - start));
        }

        start = index + splitter.size();
        if (start == std::string::npos)
        {
            return result;
        }
    }

    result.push_back(source.substr(start));

    return result;
}

std::vector<std::wstring> hmc_string_util::split(const std::wstring &source, const std::wstring &splitter)
{
    std::vector<std::wstring> result;
    if (source.empty())
    {
        return result;
    }
    size_t start = 0, index = 0;

    while ((index = source.find(splitter, start)) != std::wstring::npos)
    {
        if (index > start)
        {
            result.push_back(source.substr(start, index - start));
        }

        start = index + splitter.size();
        if (start == std::wstring::npos)
        {
            return result;
        }
    }

    result.push_back(source.substr(start));

    return result;
}

std::vector<std::string> hmc_string_util::split(const std::string &source, const char &splitter)
{
    std::vector<std::string> result;
    if (source.empty())
    {
        return result;
    }

    size_t count = source.size();

    std::wstring temp = std::wstring();

    for (size_t i = 0; i < count; i++)
    {
        auto &&it = source.at(i);

        if (it == splitter)
        {
            if (!temp.empty())
            {
                result.emplace_back(std::move(temp));
                temp.clear();
            }
        }
        else
        {
            temp.push_back(it);
        }
    }
    if (!temp.empty())
    {
        result.emplace_back(std::move(temp));
    }

    return result;
}

std::vector<std::wstring> hmc_string_util::split(const std::wstring &source, const wchar_t &splitter)
{
    std::vector<std::wstring> result;
    if (source.empty())
    {
        return result;
    }

    size_t count = source.size();

    std::wstring temp = std::wstring();

    for (size_t i = 0; i < count; i++)
    {
        auto &&it = source.at(i);

        if (it == splitter)
        {
            if (!temp.empty())
            {
                result.emplace_back(std::move(temp));
                temp.clear();
            }
        }
        else
        {
            temp.push_back(it);
        }
    }
    if (!temp.empty())
    {
        result.emplace_back(std::move(temp));
    }
    return result;
}

std::vector<std::string> hmc_string_util::split(const std::string &source, const std::vector<char> &splitterList)
{
    std::vector<std::string> result;
    if (source.empty())
    {
        return result;
    }

    size_t count = source.size();

    std::wstring temp = std::wstring();

    for (size_t i = 0; i < count; i++)
    {
        auto &&it = source.at(i);

        for (auto &&splitter : splitterList)
        {
            if (it == splitter)
            {
                if (!temp.empty())
                {
                    result.emplace_back(std::move(temp));
                    temp.clear();
                }
            }
            else
            {
                temp.push_back(it);
            }
        }
    }
    if (!temp.empty())
    {
        result.emplace_back(std::move(temp));
    }
    return result;
}

std::vector<std::wstring> hmc_string_util::split(const std::wstring &source, const std::vector<wchar_t> &splitterList)
{
    std::vector<std::wstring> result;
    if (source.empty())
    {
        return result;
    }

    size_t count = source.size();
    for (auto &&splitter : splitterList)
    {
        std::wstring temp = std::wstring();

        for (size_t i = 0; i < count; i++)
        {
            auto &&it = source.at(i);

            for (auto &&splitter : splitterList)
            {
                if (it == splitter)
                {
                    if (!temp.empty())
                    {
                        result.emplace_back(std::move(temp));
                        temp.clear();
                    }
                }
                else
                {
                    temp.push_back(it);
                }
            }
        }
        if (!temp.empty())
        {
            result.emplace_back(std::move(temp));
        }
    }

    return result;
}

void hmc_string_util::replace(std::wstring &sourcePtr, const std::wstring from, const std::wstring to, bool isRepAll)
{
    size_t start_pos = 0;
    while ((start_pos = sourcePtr.find(from, start_pos)) != std::string::npos)
    {
        sourcePtr.replace(start_pos, from.length(), to);
        start_pos += to.length();
        if (!isRepAll)
        {
            return;
        }
    }
}

void hmc_string_util::replace(std::string &sourcePtr, std::string from, std::string to, bool isRepAll)
{
    size_t start_pos = 0;
    while ((start_pos = sourcePtr.find(from, start_pos)) != std::string::npos)
    {
        sourcePtr.replace(start_pos, from.length(), to);
        start_pos += to.length();
        if (!isRepAll)
        {
            return;
        }
    }
}

void hmc_string_util::replace(std::string &sourcePtr, std::vector<std::tuple<std::string, std::string>> list, bool isRepAll)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        const auto &pair = list[i];
        const auto &from = std::get<0>(pair);
        const auto &to = std::get<1>(pair);
        replace(sourcePtr, from, to, isRepAll);
    }
}

void hmc_string_util::replace(std::wstring &sourcePtr, std::vector<std::tuple<std::wstring, std::wstring>> list, bool isRepAll)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        const auto &pair = list[i];
        const auto &from = std::get<0>(pair);
        const auto &to = std::get<1>(pair);
        replace(sourcePtr, from, to, isRepAll);
    }
}

void hmc_string_util::replace(std::string &sourcePtr, std::vector<std::tuple<char, char>> list, bool isRepAll)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        const auto &pair = list[i];
        const auto &from = std::get<0>(pair);
        const auto &to = std::get<1>(pair);
        replace(sourcePtr, from, to, isRepAll);
    }
}

void hmc_string_util::replace(std::wstring &sourcePtr, std::vector<std::tuple<wchar_t, wchar_t>> list, bool isRepAll)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        const auto &pair = list[i];
        const auto &from = std::get<0>(pair);
        const auto &to = std::get<1>(pair);
        replace(sourcePtr, from, to, isRepAll);
    }
}

void hmc_string_util::replace(std::wstring &sourcePtr, const wchar_t from, const wchar_t to, bool isRepAll)
{
    size_t count = sourcePtr.size();
    for (size_t i = 0; i < count; i++)
    {
        auto it = sourcePtr[i];
        if (it == from)
        {
            sourcePtr[i] = to;
            if (!isRepAll)
            {
                return;
            }
        }
    }
}

void hmc_string_util::replace(std::string &sourcePtr, char from, char to, bool isRepAll)
{
    size_t count = sourcePtr.size();
    for (size_t i = 0; i < count; i++)
    {
        auto it = sourcePtr[i];
        if (it == from)
        {
            sourcePtr[i] = to;
            if (!isRepAll)
            {
                return;
            }
        }
    }
}

std::string hmc_string_util::to_upper(std::string data)
{
    std::string Result;

    Result.reserve(data.size() + 1);

    for (char &c : data)
    {
        if (std::isalpha(static_cast<int>(c)))
        {
            Result.push_back(std::toupper(c));
        }
        else
        {
            Result.push_back(c);
        }
    }

    Result.shrink_to_fit();
    return Result;
}

std::wstring hmc_string_util::to_upper(std::wstring data)
{
    std::wstring Result;

    Result.reserve(data.size() + 1);

    for (wchar_t &c : data)
    {
        if (std::isalpha(static_cast<int>(c)))
        {
            Result.push_back(std::toupper(c));
        }
        else
        {
            Result.push_back(c);
        }
    }

    Result.shrink_to_fit();
    return Result;
}

std::string hmc_string_util::to_lower(std::string data)
{
    std::string Result;

    Result.reserve(data.size() + 1);

    for (char &c : data)
    {
        if (std::isalpha(static_cast<int>(c)))
        {
            Result.push_back(std::tolower(c));
        }
        else
        {
            Result.push_back(c);
        }
    }

    Result.shrink_to_fit();
    return Result;
}

std::wstring hmc_string_util::to_lower(std::wstring data)
{
    std::wstring Result;

    Result.reserve(data.size() + 1);

    for (wchar_t &c : data)
    {
        if (std::isalpha(static_cast<int>(c)))
        {
            Result.push_back(std::tolower(c));
        }
        else
        {
            Result.push_back(c);
        }
    }

    Result.shrink_to_fit();

    return Result;
}

bool hmc_string_util::trim(std::string &inputPtr, const std::string &match, IGNORE_CHAR_POINT_TYPE Ignore) {
    
}

bool hmc_string_util::trim(std::wstring &inputPtr, const std::wstring &match, IGNORE_CHAR_POINT_TYPE Ignore){
    
}

bool hmc_string_util::trim(std::string &inputPtr, const char &match_char, IGNORE_CHAR_POINT_TYPE ignore){
    
    bool result = false;

    size_t the_size = inputPtr.size();

    if (the_size == 0)
    {
        return false;
    }

    // 全部删除
    if (ignore == IGNORE_CHAR_POINT_TYPE::IGNORE_ALL)
    {
        inputPtr.erase(std::remove(inputPtr.begin(), inputPtr.end(), match_char),
                    inputPtr.end());
        return the_size != inputPtr.size();
    }

    // 判断是否全是此字符
    if (ignore == IGNORE_CHAR_POINT_TYPE::IGNORE_END || ignore == IGNORE_CHAR_POINT_TYPE::IGNORE_START)
    {
        size_t length = inputPtr.size();
        bool is_all_match_char = true;

        for (size_t i = 0; i < length; i++)
        {
            wchar_t it = inputPtr[i];
            if (it != match_char)
            {
                is_all_match_char = false;
                break;
            }
        }

        if (is_all_match_char)
        {
            inputPtr.clear();
            return the_size != inputPtr.size();
        }
    }

    // 只删除开头
    if (ignore == IGNORE_CHAR_POINT_TYPE::IGNORE_START)
    {

        size_t front_index = 0;
        size_t length = inputPtr.size();
        for (size_t i = 0; i < length; i++)
        {
            wchar_t it = inputPtr[i];
            if (i != 0 && it != match_char)
            {
                front_index = i - 1;
                break;
            }
        }

        if (front_index > 0)
        {
            inputPtr.erase(0, front_index);
        }

        return the_size != inputPtr.size();
    }

    // 只删除结尾
    if (ignore == IGNORE_CHAR_POINT_TYPE::IGNORE_END)
    {

        size_t length = inputPtr.size();
        size_t end_index = length;

        for (size_t i = length - 1; i >= 0; i--)
        {
            wchar_t it = inputPtr[i];
            if (i != 0 && it != match_char)
            {
                end_index = i + 1;
                break;
            }
        }

        if (end_index < length)
        {
            inputPtr.erase(end_index, length);
        }

        return the_size != inputPtr.size();
    }

    // 只删除中间 并忽略开头结尾出现的
    if (ignore == IGNORE_CHAR_POINT_TYPE::IGNORE_MIDDLE)
    {

        size_t length = inputPtr.size();
        // 开头结尾 都不等于此字符 删除全部此字符
        if (inputPtr.front() != match_char && inputPtr.back() != match_char)
        {
            inputPtr.erase(std::remove(inputPtr.begin(), inputPtr.end(), match_char),
                        inputPtr.end());
            return the_size != inputPtr.size();
        }
        // 新的开头
        size_t ready_front_index = 0;
        // 尾部需要忽略多少个字符
        size_t ready_end_size = 0;

        // 全部都是 match_char 应当拒绝处理
        if (inputPtr.size() <= ready_end_size + 1)
        {
            return the_size != inputPtr.size();
        }

        // 计算出开头
        if (inputPtr.front() == match_char)
        {
            for (size_t i = 0; i < length; i++)
            {
                wchar_t it = inputPtr[i];
                if (it != match_char)
                {
                    ready_front_index = i;
                    break;
                }
            }
        }

        // 计算出结尾
        if (inputPtr.front() == match_char)
        {
            for (size_t i = length - 1; i >= 0; i--)
            {
                wchar_t it = inputPtr[i];
                if (it != match_char)
                {
                    break;
                }
                ready_end_size++;
            }
        }

        for (size_t i = ready_front_index; i < inputPtr.size(); i++)
        {

            // 结尾完成
            if (i + 1 > inputPtr.size() - ready_end_size)
            {
                break;
            }

            if (inputPtr[i] == match_char)
            {
                inputPtr.erase(i, 1);
                i--;
            }
        }
    }

    return result;
}

bool hmc_string_util::trim(std::wstring &inputPtr, const wchar_t &match, IGNORE_CHAR_POINT_TYPE Ignore){
    
}

bool hmc_string_util::diffNulCharacters(const std::wstring str1, const std::wstring str2, IGNORE_CHAR_POINT_TYPE Ignore){
    
}

bool hmc_string_util::diffNulCharacters(const std::string str1, const std::string str2, IGNORE_CHAR_POINT_TYPE Ignore){

}

bool hmc_string_util::removeMatchCharPtr(std::wstring &input, const wchar_t match_char, IGNORE_CHAR_POINT_TYPE ignore)
{
    bool result = false;

    size_t the_size = input.size();

    if (the_size == 0)
    {
        return false;
    }

    // 全部删除
    if (ignore == IGNORE_CHAR_POINT_TYPE::ALL_IGNORE)
    {
        input.erase(std::remove(input.begin(), input.end(), match_char),
                    input.end());
        return the_size != input.size();
    }

    // 判断是否全是此字符
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE || ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {
        size_t length = input.size();
        bool is_all_match_char = true;

        for (size_t i = 0; i < length; i++)
        {
            wchar_t it = input[i];
            if (it != match_char)
            {
                is_all_match_char = false;
                break;
            }
        }

        if (is_all_match_char)
        {
            input.clear();
            return the_size != input.size();
        }
    }

    // 只删除开头
    if (ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {

        size_t front_index = 0;
        size_t length = input.size();
        for (size_t i = 0; i < length; i++)
        {
            wchar_t it = input[i];
            if (i != 0 && it != match_char)
            {
                front_index = i - 1;
                break;
            }
        }

        if (front_index > 0)
        {
            input.erase(0, front_index);
        }

        return the_size != input.size();
    }

    // 只删除结尾
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE)
    {

        size_t length = input.size();
        size_t end_index = length;

        for (size_t i = length - 1; i >= 0; i--)
        {
            wchar_t it = input[i];
            if (i != 0 && it != match_char)
            {
                end_index = i + 1;
                break;
            }
        }

        if (end_index < length)
        {
            input.erase(end_index, length);
        }

        return the_size != input.size();
    }

    // 只删除中间 并忽略开头结尾出现的
    if (ignore == IGNORE_CHAR_POINT_TYPE::MIDDLE_IGNORE)
    {

        size_t length = input.size();
        // 开头结尾 都不等于此字符 删除全部此字符
        if (input.front() != match_char && input.back() != match_char)
        {
            input.erase(std::remove(input.begin(), input.end(), match_char),
                        input.end());
            return the_size != input.size();
        }
        // 新的开头
        size_t ready_front_index = 0;
        // 尾部需要忽略多少个字符
        size_t ready_end_size = 0;

        // 全部都是 match_char 应当拒绝处理
        if (input.size() <= ready_end_size + 1)
        {
            return the_size != input.size();
        }

        // 计算出开头
        if (input.front() == match_char)
        {
            for (size_t i = 0; i < length; i++)
            {
                wchar_t it = input[i];
                if (it != match_char)
                {
                    ready_front_index = i;
                    break;
                }
            }
        }

        // 计算出结尾
        if (input.front() == match_char)
        {
            for (size_t i = length - 1; i >= 0; i--)
            {
                wchar_t it = input[i];
                if (it != match_char)
                {
                    break;
                }
                ready_end_size++;
            }
        }

        for (size_t i = ready_front_index; i < input.size(); i++)
        {

            // 结尾完成
            if (i + 1 > input.size() - ready_end_size)
            {
                break;
            }

            if (input[i] == match_char)
            {
                input.erase(i, 1);
                i--;
            }
        }
    }

    return result;
}

std::wstring hmc_string_util::removeMatchChar(const std::wstring &Input_, const wchar_t match_char, IGNORE_CHAR_POINT_TYPE ignore)
{
    std::wstring input = std::wstring(Input_.begin(), Input_.end());

    bool result = false;

    size_t the_size = input.size();

    if (the_size == 0)
    {
        return input;
    }

    // 全部删除
    if (ignore == IGNORE_CHAR_POINT_TYPE::ALL_IGNORE)
    {
        input.erase(std::remove(input.begin(), input.end(), match_char),
                    input.end());
        return input;
    }

    // 判断是否全是此字符
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE || ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {
        size_t length = input.size();
        bool is_all_match_char = true;

        for (size_t i = 0; i < length; i++)
        {
            wchar_t it = input[i];
            if (it != match_char)
            {
                is_all_match_char = false;
                break;
            }
        }

        if (is_all_match_char)
        {
            input.clear();
            return input;
        }
    }

    // 只删除开头
    if (ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {

        size_t front_index = 0;
        size_t length = input.size();
        for (size_t i = 0; i < length; i++)
        {
            wchar_t it = input[i];
            if (i != 0 && it != match_char)
            {
                front_index = i - 1;
                break;
            }
        }

        if (front_index > 0)
        {
            input.erase(0, front_index);
        }

        return input;
    }

    // 只删除结尾
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE)
    {

        size_t length = input.size();
        size_t end_index = length;

        for (size_t i = length - 1; i >= 0; i--)
        {
            wchar_t it = input[i];
            if (i != 0 && it != match_char)
            {
                end_index = i + 1;
                break;
            }
        }

        if (end_index < length)
        {
            input.erase(end_index, length);
        }

        return input;
    }

    // 只删除中间 并忽略开头结尾出现的
    if (ignore == IGNORE_CHAR_POINT_TYPE::MIDDLE_IGNORE)
    {

        size_t length = input.size();
        // 开头结尾 都不等于此字符 删除全部此字符
        if (input.front() != match_char && input.back() != match_char)
        {
            input.erase(std::remove(input.begin(), input.end(), match_char),
                        input.end());
            return input;
        }
        // 新的开头
        size_t ready_front_index = 0;
        // 尾部需要忽略多少个字符
        size_t ready_end_size = 0;

        // 全部都是 match_char 应当拒绝处理
        if (input.size() <= ready_end_size + 1)
        {
            return input;
        }

        // 计算出开头
        if (input.front() == match_char)
        {
            for (size_t i = 0; i < length; i++)
            {
                wchar_t it = input[i];
                if (it != match_char)
                {
                    ready_front_index = i;
                    break;
                }
            }
        }

        // 计算出结尾
        if (input.front() == match_char)
        {
            for (size_t i = length - 1; i >= 0; i--)
            {
                wchar_t it = input[i];
                if (it != match_char)
                {
                    break;
                }
                ready_end_size++;
            }
        }

        for (size_t i = ready_front_index; i < input.size(); i++)
        {

            // 结尾完成
            if (i + 1 > input.size() - ready_end_size)
            {
                break;
            }

            if (input[i] == match_char)
            {
                input.erase(i, 1);
                i--;
            }
        }
    }

    return input;
}

bool hmc_string_util::removeMatchCharPtr(std::string &input, const char match_char, IGNORE_CHAR_POINT_TYPE ignore)
{
    bool result = false;

    size_t the_size = input.size();

    if (the_size == 0)
    {
        return false;
    }

    // 全部删除
    if (ignore == IGNORE_CHAR_POINT_TYPE::ALL_IGNORE)
    {
        input.erase(std::remove(input.begin(), input.end(), match_char),
                    input.end());
        return the_size != input.size();
    }

    // 判断是否全是此字符
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE || ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {
        size_t length = input.size();
        bool is_all_match_char = true;

        for (size_t i = 0; i < length; i++)
        {
            char it = input[i];
            if (it != match_char)
            {
                is_all_match_char = false;
                break;
            }
        }

        if (is_all_match_char)
        {
            input.clear();
            return the_size != input.size();
        }
    }

    // 只删除开头
    if (ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {

        size_t front_index = 0;
        size_t length = input.size();
        for (size_t i = 0; i < length; i++)
        {
            char it = input[i];
            if (i != 0 && it != match_char)
            {
                front_index = i - 1;
                break;
            }
        }

        if (front_index > 0)
        {
            input.erase(0, front_index);
        }

        return the_size != input.size();
    }

    // 只删除结尾
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE)
    {

        size_t length = input.size();
        size_t end_index = length;

        for (size_t i = length - 1; i >= 0; i--)
        {
            char it = input[i];
            if (i != 0 && it != match_char)
            {
                end_index = i + 1;
                break;
            }
        }

        if (end_index < length)
        {
            input.erase(end_index, length);
        }

        return the_size != input.size();
    }

    // 只删除中间 并忽略开头结尾出现的
    if (ignore == IGNORE_CHAR_POINT_TYPE::MIDDLE_IGNORE)
    {

        size_t length = input.size();
        // 开头结尾 都不等于此字符 删除全部此字符
        if (input.front() != match_char && input.back() != match_char)
        {
            input.erase(std::remove(input.begin(), input.end(), match_char),
                        input.end());
            return the_size != input.size();
        }
        // 新的开头
        size_t ready_front_index = 0;
        // 尾部需要忽略多少个字符
        size_t ready_end_size = 0;

        // 全部都是 match_char 应当拒绝处理
        if (input.size() <= ready_end_size + 1)
        {
            return the_size != input.size();
        }

        // 计算出开头
        if (input.front() == match_char)
        {
            for (size_t i = 0; i < length; i++)
            {
                char it = input[i];
                if (it != match_char)
                {
                    ready_front_index = i;
                    break;
                }
            }
        }

        // 计算出结尾
        if (input.front() == match_char)
        {
            for (size_t i = length - 1; i >= 0; i--)
            {
                char it = input[i];
                if (it != match_char)
                {
                    break;
                }
                ready_end_size++;
            }
        }

        for (size_t i = ready_front_index; i < input.size(); i++)
        {

            // 结尾完成
            if (i + 1 > input.size() - ready_end_size)
            {
                break;
            }

            if (input[i] == match_char)
            {
                input.erase(i, 1);
                i--;
            }
        }
    }

    return result;
}

std::string hmc_string_util::removeMatchChar(const std::string &Input_, const char match_char, IGNORE_CHAR_POINT_TYPE ignore)
{
    std::string input = std::string(Input_.begin(), Input_.end());

    bool result = false;

    size_t the_size = input.size();

    if (the_size == 0)
    {
        return input;
    }

    // 全部删除
    if (ignore == IGNORE_CHAR_POINT_TYPE::ALL_IGNORE)
    {
        input.erase(std::remove(input.begin(), input.end(), match_char),
                    input.end());
        return input;
    }

    // 判断是否全是此字符
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE || ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {
        size_t length = input.size();
        bool is_all_match_char = true;

        for (size_t i = 0; i < length; i++)
        {
            char it = input[i];
            if (it != match_char)
            {
                is_all_match_char = false;
                break;
            }
        }

        if (is_all_match_char)
        {
            input.clear();
            return input;
        }
    }

    // 只删除开头
    if (ignore == IGNORE_CHAR_POINT_TYPE::START_IGNORE)
    {

        size_t front_index = 0;
        size_t length = input.size();
        for (size_t i = 0; i < length; i++)
        {
            char it = input[i];
            if (i != 0 && it != match_char)
            {
                front_index = i - 1;
                break;
            }
        }

        if (front_index > 0)
        {
            input.erase(0, front_index);
        }

        return input;
    }

    // 只删除结尾
    if (ignore == IGNORE_CHAR_POINT_TYPE::END_IGNORE)
    {

        size_t length = input.size();
        size_t end_index = length;

        for (size_t i = length - 1; i >= 0; i--)
        {
            char it = input[i];
            if (i != 0 && it != match_char)
            {
                end_index = i + 1;
                break;
            }
        }

        if (end_index < length)
        {
            input.erase(end_index, length);
        }

        return input;
    }

    // 只删除中间 并忽略开头结尾出现的
    if (ignore == IGNORE_CHAR_POINT_TYPE::MIDDLE_IGNORE)
    {

        size_t length = input.size();
        // 开头结尾 都不等于此字符 删除全部此字符
        if (input.front() != match_char && input.back() != match_char)
        {
            input.erase(std::remove(input.begin(), input.end(), match_char),
                        input.end());
            return input;
        }
        // 新的开头
        size_t ready_front_index = 0;
        // 尾部需要忽略多少个字符
        size_t ready_end_size = 0;

        // 全部都是 match_char 应当拒绝处理
        if (input.size() <= ready_end_size + 1)
        {
            return input;
        }

        // 计算出开头
        if (input.front() == match_char)
        {
            for (size_t i = 0; i < length; i++)
            {
                char it = input[i];
                if (it != match_char)
                {
                    ready_front_index = i;
                    break;
                }
            }
        }

        // 计算出结尾
        if (input.front() == match_char)
        {
            for (size_t i = length - 1; i >= 0; i--)
            {
                char it = input[i];
                if (it != match_char)
                {
                    break;
                }
                ready_end_size++;
            }
        }

        for (size_t i = ready_front_index; i < input.size(); i++)
        {

            // 结尾完成
            if (i + 1 > input.size() - ready_end_size)
            {
                break;
            }

            if (input[i] == match_char)
            {
                input.erase(i, 1);
                i--;
            }
        }
    }

    return input;
}

bool hmc_string_util::diffNullCharacters(const std::wstring str1, const std::wstring str2, IGNORE_CHAR_POINT_TYPE Ignore)
{
    std::wstring input_01 = std::wstring(str1.begin(), str1.end());
    input_01.reserve(input_01.size());
    std::wstring input_02 = std::wstring(str2.begin(), str2.end());
    input_02.reserve(input_02.size());

    hmc_string_util::removeMatchCharPtr(input_01, L'\0', Ignore);
    hmc_string_util::removeMatchCharPtr(input_02, L'\0', Ignore);
    if (input_01.size() != input_02.size())
    {
        return false;
    }

    const size_t length = input_01.size();
    for (size_t i = 0; i < length; i++)
    {
        const wchar_t it1 = input_01.at(i);
        const wchar_t it2 = input_02.at(i);

        if (it1 != it2)
        {
            return false;
        }
    }
    return true;
}

bool hmc_string_util::diffNullCharacters(const std::string str1, const std::string str2, IGNORE_CHAR_POINT_TYPE Ignore)
{
    std::string input_01 = std::string(str1.begin(), str1.end());
    input_01.reserve(input_01.size());
    std::string input_02 = std::string(str2.begin(), str2.end());
    input_02.reserve(input_02.size());

    hmc_string_util::removeMatchCharPtr(input_01, '\0', Ignore);
    hmc_string_util::removeMatchCharPtr(input_02, '\0', Ignore);

    if (input_01.size() != input_02.size())
    {
        return false;
    }

    const size_t length = input_01.size();
    for (size_t i = 0; i < length; i++)
    {
        const char it1 = input_01.at(i);
        const char it2 = input_02.at(i);

        if (it1 != it2)
        {
            return false;
        }
    }
    return true;
}