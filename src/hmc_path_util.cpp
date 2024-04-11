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
#include <hmc_path_util.h>

std::vector<hmc_path_util::String> hmc_path_util::splitPath(const String path)
{
    std::vector<String> result;

    size_t count = path.size();

    String temp = String();

    for (size_t i = 0; i < count; i++)
    {
        auto &&it = path.at(i);

        if (it == '\\' || it == '/' || it == '\0')
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

hmc_path_util::String hmc_path_util::baseName(const String path)
{
    String result = String();

    size_t length = path.size();

    for (int i = length - 1; i >= 0; i--)
    {
        auto it = path.at(i);

        if (it == '\0')
        {
            continue;
        }

        if (it == '\\' || it == '/')
        {
            break;
        }

        result.insert(0, 1, it);
    }

    return result;
}

bool hmc_path_util::isAbsolute(const String &path)
{
    bool result = false;
    if (path.empty())
        return false;

    if (IS_WIN32)
    {
        if (path[0] == '\\' || path[0] == '/' || (path.size() >= 2 && path[1] == ':'))
            return true;
    }
    else
    {
        // 对于POSIX路径
        // 如果路径以 '/' 开头，则认为是绝对路径
        if (path[0] == '/')
            return true;
    }

    return result;
}

hmc_path_util::String hmc_path_util::format(const chParse input)
{
    String dir = input.dir.length() > 0 ? input.dir + SEPARATOR : String();
    return dir + input.base;
}

hmc_path_util::String hmc_path_util::delimiter()
{
    String temp = String();
    temp.push_back(IS_WIN32 ? ';' : ':');
    return temp;
}

hmc_path_util::String hmc_path_util::extName(const String path)
{
    String result = String();

    if (path.empty() || path.back() == '\\' || path.back() == '/')
    {
        return result;
    }

    auto temp = hmc_path_util::baseName(path);

    if (temp.empty())
    {
        return result;
    }

    size_t dotIdx = temp.find_last_of(String(1, '.'));

    if (dotIdx != String::npos)
    {
        result.assign(result.begin() + dotIdx, result.end());
    }

    return result;
}

hmc_path_util::String hmc_path_util::resolveFromVector(const std::vector<String> &args)
{

    String resolvedPath = String();
    bool resolvedAbsolute = false;

    for (int i = args.size() - 1; i >= -1 && !resolvedAbsolute; i--)
    {
        String path = args[i];

        if (path.empty())
        {
            continue;
        }

        resolvedPath = path + SEPARATOR + resolvedPath;
        resolvedAbsolute = path[0] == SEPARATOR;
    }

    // At this point the path should be resolved to a full absolute path, but
    // handle relative paths to be safe (might happen when process.cwd() fails)

    // Normalize the path
    auto split = splitPath(resolvedPath);
    auto normalized = normalizePathList(split, !resolvedAbsolute);

    resolvedPath = joinPathList(normalized);

    auto sep = String(1, SEPARATOR);
    String p = ((resolvedAbsolute ? sep : String()) + resolvedPath);

    if (p.empty())
    {
        return String(1, '.');
    }

    return p;
}

std::vector<hmc_path_util::String> hmc_path_util::normalizePathList(std::vector<String> parts, bool allowAboveRoot)
{
    std::vector<String> res;
    int len = parts.size();

    String upOneLevel = String(2, '.');

    for (int i = 0; i < len; i++)
    {
        auto p = parts[i];

        if ((p.length() == 0) || (p.size() == 1 && p[0] == '.'))
        {
            continue;
        }

        if (p == upOneLevel)
        {
            if (!res.empty() && res.back() != upOneLevel)
            {
                res.pop_back();
            }
            else if (allowAboveRoot)
            {
                res.emplace_back(String(2, '.'));
            }
        }
        else
        {
            res.push_back(p);
        }
    }
    return res;
}

hmc_path_util::String hmc_path_util::normalize(const String &path)
{
    String result = String();

    if (path.empty())
    {
        return result;
    }

    bool isAbs = isAbsolute(path);
    bool trailingSlash = path[path.length() - 1] == SEPARATOR;

    auto split = splitPath(path);
    auto normalized = normalizePathList(split, !isAbs);
    auto tmp = joinPathList(normalized);

    if (tmp.length() == 0 && !isAbs)
    {
        tmp = String(1, '.');
    }

    if (path.length() > 0 && trailingSlash)
    {
        tmp += SEPARATOR;
    }

    return (isAbs ? String(1, SEPARATOR) : String()) + tmp;
}

char hmc_path_util::sep()
{
    return SEPARATOR + 0;
}

hmc_path_util::String hmc_path_util::joinPathList(const std::vector<String> &list)
{
    String res;
    int len = list.size();
    int index = 0;

    for (const auto &el : list)
    {
        res += el;
        if (++index != len)
        {
            res += SEPARATOR;
        }
    }

    return res;
}

bool hmc_path_util::isPathSeparator(const char s)
{
    return s == '/' || s == '\\';
}

bool hmc_path_util::isPathSeparator(const wchar_t s)
{
    return s == '/' || s == '\\';
}

hmc_path_util::String hmc_path_util::joinVector(const std::vector<String> &args)
{
    String path;

    if (args.size() == 1 && (args[0].size() == 1 && args[0][0] == '/'))
    {
        path.push_back('/');
        return path;
    }

    for (const auto &el : args)
    {
        if (el.length() > 0)
        {
            if (path.length() == 0)
            {
                path += el;
            }
            else
            {
                path += SEPARATOR + el;
            }
        }
    }
    return normalize(path);
}

hmc_path_util::String hmc_path_util::dirName(const String path)
{

    size_t len = path.length();

    if (len == 0)
    {
        return String(1, '.');
    }

    size_t rootEnd = -1;
    size_t offset = 0;
    const char ch = path[0];

    if (len == 1)
    {
        // `path` contains just a path separator, exit early to avoid
        // unnecessary work or a dot.
        return isPathSeparator(ch) ? path : String(1, '.');
    }

    // Try to match a root
    if (isPathSeparator(ch))
    {
        // Possible UNC root

        rootEnd = offset = 1;

        if (isPathSeparator(path[1]))
        {
            // Matched double path separator at beginning
            size_t j = 2;
            size_t last = j;

            // Match 1 or more non-path separators
            while (j < len && !isPathSeparator(path[j]))
            {
                j++;
            }

            if (j < len && j != last)
            {
                // Matched!
                last = j;
                // Match 1 or more path separators
                while (j < len && isPathSeparator(path[j]))
                {
                    j++;
                }

                if (j < len && j != last)
                {
                    // Matched!
                    last = j;
                    // Match 1 or more non-path separators
                    while (j < len && !isPathSeparator(path[j]))
                    {
                        j++;
                    }
                    if (j == len)
                    {
                        // We matched a UNC root only
                        return path;
                    }
                    if (j != last)
                    {
                        // We matched a UNC root with leftovers

                        // Offset by 1 to include the separator after the UNC root to
                        // treat it as a "normal root" on top of a (UNC) root
                        rootEnd = offset = j + 1;
                    }
                }
            }
        }
        // Possible device root
    }
    else if (((ch > 'a' && ch < 'z') && (ch > 'A' && ch < 'Z')) && (path.size() >= 2 && path[1] == ':'))
    {
        rootEnd = len > 2 && isPathSeparator(path[2]) ? 3 : 2;
        offset = rootEnd;
    }

    size_t end = -1;
    size_t matchedSlash = true;

    for (size_t i = len - 1; i >= offset; --i)
    {
        if (isPathSeparator(path[i]))
        {
            if (!matchedSlash)
            {
                end = i;
                break;
            }
        }
        else
        {
            // We saw the first non-path separator
            matchedSlash = false;
        }
    }

    if (end == -1)
    {
        if (rootEnd == -1)
        {
            return String(1, '.');
        }

        end = rootEnd;
    }

    return path.substr(0, end);
}

hmc_path_util::String hmc_path_util::resolveVector(const std::vector<hmc_path_util::String> &args)
{
    String resolvedPath = String();
    bool resolvedAbsolute = false;

    if (args.size() < 1)
    {
        return resolvedPath;
    }

    for (int i = args.size() - 1; i >= -1 && !resolvedAbsolute; i--)
    {
        if (i < 0 || i == args.size())
        {
            break;
        }

        String path = args[i];

        if (path.empty())
        {
            continue;
        }

        resolvedPath = path + SEPARATOR + resolvedPath;
        resolvedAbsolute = path[0] == SEPARATOR;
    }

    // At this point the path should be resolved to a full absolute path, but
    // handle relative paths to be safe (might happen when process.cwd() fails)

    // Normalize the path
    auto split = splitPath(resolvedPath);
    auto normalized = normalizePathList(split, !resolvedAbsolute);

    resolvedPath = joinPathList(normalized);

    auto sep = String(1, SEPARATOR);
    String p = ((resolvedAbsolute ? sep : String()) + resolvedPath);

    if (p.empty())
    {
        return String(1, '.');
    }

    return p;
}

hmc_path_util::chParse hmc_path_util::parse(const String path)
{
    chParse result = chParse();

    if (path.empty())
    {
        return result;
    }

    result.root = path[0] == sep() ? String(1, sep()) : String();
    result.dir = dirName(path);
    result.base = baseName(path);
    result.ext = extName(path);
    result.name = result.base.substr(0, result.base.length() - result.ext.length());
    return result;
}