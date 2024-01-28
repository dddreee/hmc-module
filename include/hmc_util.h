/**
 * @file hmc_util.h
 * @author kiic，kihlh
 * @brief 本模块是高频调用的函数的方法合集 要求 轻且快
 * @version 0.1
 * @date 2024-01-25
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

// 防止重复导入
#ifndef MODE_INTERNAL_INCLUDE_HMC_UTIL_HPP
#define MODE_INTERNAL_INCLUDE_HMC_UTIL_HPP

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

#ifndef _WINTERNL_
typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING;
#endif // _WINTERNL_

// 自动释放dll
#ifndef FreeLibraryAuto
#define FreeLibraryAuto(hModule) \
    std::shared_ptr<void>##hModule##_shared_close_Library_(nullptr, [&](void *) {\
    try\
    {\
        if (hModule != NULL) {::FreeLibrary(hModule);}\
    }\
    catch(...){} });
#endif // FreeLibraryAuto

// 自动释放Handle
#ifndef FreeHandleAuto
#define FreeHandleAuto(hModule) \
    std::shared_ptr<void>##hModule##_shared_close_Handle_(nullptr, [&](void *) {\
    try\
    {\
        if (hModule != NULL) {::CloseHandle(hModule);}\
    }\
    catch(...){} });
#endif // FreeHandleAuto

// 释放vc内存开辟
#ifndef VcFree
#define VcFree(Virtua) \
    if (Virtua != NULL)    \
        ::VirtualFree(Virtua, 0, MEM_RELEASE);
#endif // VcFree

// 自动释放dll
#ifndef VcAlloc
#define VcAlloc(Type, leng) \
    (reinterpret_cast<Type>(::VirtualAlloc(nullptr, static_cast<DWORD>(leng), MEM_COMMIT, PAGE_READWRITE)))
#endif // VcAlloc


// 自动释放内存
#ifndef VcFreeAuto
#define VcFreeAuto(Virtua) \
	std::shared_ptr<void>##Virtua##_shared_close_FreeVSAuto_(nullptr, [&](void *) {if (Virtua != NULL) {::VirtualFree(Virtua, 0, MEM_RELEASE);} });
#endif // VsFreeAuto


// 自动释放 注册表key
#ifndef FreeRegKeyAuto
        // 关闭注册表键
#define FreeRegKeyAuto(subHKey) \
    std::shared_ptr<void>##subHKey##_shared_close_Library_(nullptr, [&](void *) {\
    try\
    {\
        if (subHKey != nullptr) {\
            ::RegCloseKey(subHKey);\
        subHKey = nullptr;}\
    }\
    catch(...){} });
#endif // FreeRegKeyAuto


// 自动释放 自定义释放
#ifndef FreeAnyAuto
        // 自动释放 自定义释放
#define FreeAnyAuto(code) \
    std::shared_ptr<void>##hModule##_shared_close_Library_(nullptr, [&](void *) {\
    try\
    {\
       code\
    }\
    catch(...){} });
#endif // FreeAnyAuto


namespace hmc_util
{
    namespace hmc_define_util
    {

    }

    /**
     * @brief 删除指定的字符
     *
     * @param input 输入
     * @param matching 匹配的字符
     * @return size_t
     */
    extern inline size_t removeAllCharPtr(std::wstring &input, wchar_t matching);

    /**
     * @brief 删除指定的字符
     *
     * @param input 输入
     * @param matching 匹配的字符
     * @return size_t
     */
    extern inline size_t removeAllCharPtr(std::string &input, char matching);

    /**
     * @brief 替换指定文本
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    extern inline void replace(std::wstring &sourcePtr, const std::wstring from, const std::wstring to);

    /**
     * @brief 替换指定文本
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    extern inline void replace(std::string &sourcePtr, const std::string from, const std::string to);

    /**
     * @brief 替换指定文本 全部
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    extern inline void replaceAll(std::wstring &sourcePtr, const std::wstring from, const std::wstring to);

    /**
     * @brief 替换指定文本 全部
     *
     * @param sourcePtr 源文本
     * @param from 查找内容
     * @param to 替换到
     */
    extern inline void replaceAll(std::string &sourcePtr, const std::string from, const std::string to);

    /**
     * @brief 转义json文本
     *
     * @param input 内容
     * @param is_to_value 如果為true 請加上"
     * @return std::wstring
     */
    extern inline std::wstring escapeJsonString(const std::wstring &input, bool is_to_value = false);

    /**
     * @brief 转义json文本
     *
     * @param input 内容
     * @param is_to_value 如果為true 請加上"
     * @return std::wstring
     */
    extern inline std::string escapeJsonString(const std::string &input, bool is_to_value = false);

    /**
     * @brief Nt文本轉爲寬文本
     *
     * @param unicodeString
     * @return std::wstring
     */
    extern inline std::wstring unicodeStringToWString(UNICODE_STRING unicodeString);
    
    /**
     * @brief 宽窄文本转码 但是仅限于ascii
     * 
     * @param input 
     * @return std::wstring 
     */
    extern inline std::wstring asciiConvers(std::string input);
   
    /**
     * @brief 宽窄文本转码 但是仅限于ascii
     * 
     * @param input 
     * @return std::wstring 
     */
    extern inline std::string asciiConvers(std::wstring input);
    
    /**
     * @brief 拼接文本对象进json
     * 
     * @param input 
     * @return std::wstring 
     */
    extern inline std::wstring vec2ar(std::vector<std::wstring> input);

    /**
     * @brief 拼接文本对象进json
     * 
     * @param input 
     * @return std::string 
     */
    extern inline std::string vec2ar(std::vector<std::string> input);

}

template <typename T>
class VcRef
{

public:
    VcRef()
    {
        m_data = reinterpret_cast<T *>(::VirtualAlloc(nullptr, sizeof(T), MEM_COMMIT, PAGE_READWRITE));

        if (m_data != NULL)
        {
            m_size = sizeof(T);
            is_data = true;
        }
    }

    VcRef(std::size_t size)
    {
        m_data = reinterpret_cast<T *>(::VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE));

        if (m_data != NULL)
        {
            m_size = size;
            is_data = true;
        }
    }

    // 只增不减的内存
    bool reserve(std::size_t size){
         bool is_ok = false;

        if (size <= m_size) {
            return is_ok;
        }

        release();
        if (size > 0)
        {
            m_data = reinterpret_cast<T *>(::VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE));
            is_data = true;
            m_size = size;
            is_ok = true;
        }

        return is_ok;
    }

    // 调整内存块大小
    bool resize(std::size_t size)
    {
        bool is_ok = false;

        if (size > 0 && size == m_size) {
            return is_ok;
        }

        release();
        if (size > 0)
        {
            m_data = reinterpret_cast<T *>(::VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE));
            is_data = true;
            m_size = size;
            is_ok = true;
        }

        return is_ok;
    }

    size_t size(){
        return m_size;
    }

    void release()
    {

        if (is_data || m_data != NULL)
        {

            if (!::VirtualFree(m_data, 0, MEM_RELEASE))
            {
                ::Sleep(5);
                ::VirtualFree(m_data, 0, MEM_RELEASE);
                is_data = false;
            }

            is_data = false;
            m_size = 0;
        }

        is_data = false;
        m_size = 0;
    }

    T *data() const { return m_data; }

    // if
    operator bool() const
    {
        return is_data || reinterpret_cast<void *>(m_data) != nullptr;
    }

    // *
    operator T *() const
    {
        return m_data;
    }

    // &
    operator T &() const
    {
        return m_data;
    }

    // !
    bool operator!() const
    {
        return !is_data || reinterpret_cast<void *>(m_data) == nullptr;
    }

    T *operator->() const
    {
        return m_data;
    }

    ~VcRef()
    {
        release();
    }

private:
    bool is_data = false;
    size_t m_size = 0;
    T *m_data;
};

#endif // MODE_INTERNAL_INCLUDE_HMC_UTIL_HPP