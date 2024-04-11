﻿#include "./hmc_shell_util.h"
#include "./hmc_windows_util.h"
#include "./hmc_string_util.h"
#include "./hmc_util.h"

// ?GetTaryIconList

// 预赋值头
namespace hmc_shell_util
{
    bool copyFile(const std::wstring &filePath, const std::wstring &newFilePath, bool isShowConfirm = true, bool isShow = true, bool isShowProgress = true);
    bool moveFile(const std::wstring &filePath, const std::wstring &newFilePath, bool isShowConfirm = true, bool isShow = true, bool isShowProgress = true);
    void winRunApplication(const std::wstring &Path);
    bool getThumbnailPngFile(const std::wstring source, const std::wstring target, int size = 256);
    std::vector<std::uint8_t> getThumbnailPng(const std::wstring source, int size = 256);
    bool SetFolderIcon(const std::wstring &folderPath, const std::wstring &iconPath, int iconIndex = 0);
    bool setStartup(const std::wstring &key, const std::wstring &execPath, const std::wstring &cmd = L"");
    bool removeStartup(const std::wstring &key);
    bool hasStartup(const std::wstring &key);
    bool SelectFolderV1(const std::wstring &Title, const std::wstring &SelectFolderPath);
    bool SelectFolder(std::wstring &folderPath);
    bool SelectFolders(std::vector<std::wstring> &folderPaths);
    bool SelectFiles(std::vector<std::wstring> &filePaths);
    bool SelectFile(std::wstring &FilePath);
    bool SelectFile(std::wstring &FilePath, std::map<std::wstring, std::wstring> filtr);
    bool SelectFiles(std::vector<std::wstring> &FilePaths, const std::map<std::wstring, std::wstring> &filtr);
    long long int getTimestamp(bool is_Millisecond = false);
    long long int getTimestamp(const SYSTEMTIME &st, bool is_Millisecond = false);

}

std::vector<hmc_shell_util::GetTaryIconList::TaryIcon> hmc_shell_util::GetTaryIconList::getTrayList()
{
    std::vector<TaryIcon> result = {};

    // win11
    if (::FindWindowW(L"TopLevelWindowForOverflowXamlIsland", NULL))
    {
        return result;
    }

    std::vector<HWND> hWndList = {GetSystemTrayHwnd(), GetNotifyOverflowTryHwnd()};

    for (size_t i = 0; i < hWndList.size(); i++)
    {
        HWND hwnd = hWndList[i];

        if (hwnd == NULL)
            continue;

        auto list = EnumCommctrlList(hwnd);

        for (size_t i = 0; i < list.size(); i++)
        {
            result.push_back(list[i]);
        }
    }

    return result;
}

std::wstring hmc_shell_util::GetTaryIconList::getTrayListJsonW()
{
    std::wstring result = L"[";
    result.reserve(2);

    auto data_list = getTrayList();

    if (data_list.empty())
    {
        result.push_back(']');
        return result;
    }

    result.reserve(4096);

    for (size_t i = 0; i < data_list.size(); i++)
    {
        auto data = data_list[i];
        std::wstring obj = LR"({ "info":"{info}","path":"{path}","handle":{handle} })";
        obj.reserve(54 + MAX_PATH * 2);

        obj.replace(obj.find(L"{info}"), sizeof("{info}") - 1, hmc_string_util::escapeJsonString(data.title));
        obj.replace(obj.find(L"{path}"), sizeof("{path}") - 1, hmc_string_util::escapeJsonString(data.path));
        obj.replace(obj.find(L"{handle}"), sizeof("{handle}") - 1, std::to_wstring((long long)data.hwnd));

        if (i + 1 < result.size())
        {
            result.push_back(',');
        }

        result.append(obj);
    }

    result.push_back(']');

    return result;
}

BOOL hmc_shell_util::GetTaryIconList::IsSystemFor64bit()
{
    SYSTEM_INFO SystemInfo;
    ::GetNativeSystemInfo(&SystemInfo);
    if (SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 || SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        return TRUE;
    else
        return FALSE;
}

HWND hmc_shell_util::GetTaryIconList::GetNotifyOverflowTryHwnd()
{
    HWND hWnd = ::FindWindowW(L"NotifyIconOverflowWindow", NULL);

    if (hWnd != NULL)
    {
        hWnd = ::FindWindowExW(hWnd, NULL, L"ToolbarWindow32", NULL);
    }

    return hWnd;
}

HWND hmc_shell_util::GetTaryIconList::GetSystemTrayHwnd()
{
    HWND hWnd = ::FindWindowW(L"Shell_TrayWnd", NULL);
    if (hWnd)
    {
        hWnd = ::FindWindowExW(hWnd, NULL, L"TrayNotifyWnd", NULL);
        if (hWnd)
        {
            hWnd = ::FindWindowExW(hWnd, NULL, L"SysPager", NULL);
            if (hWnd)
                hWnd = ::FindWindowExW(hWnd, NULL, L"ToolbarWindow32", NULL);
        }
    }
    if (!hWnd)
        return NULL;
    return hWnd;
};

std::vector<hmc_shell_util::GetTaryIconList::TaryIcon> hmc_shell_util::GetTaryIconList::EnumCommctrlList(HWND hWnd)
{
    std::vector<TaryIcon> result = {};
    if (hWnd == NULL)
        return result;

    // 获取托盘进程ID
    DWORD dwProcessId = 0;
    ::GetWindowThreadProcessId(hWnd, &dwProcessId);
    if (dwProcessId == 0)
        return result;

    // 获取托盘进程句柄
    HANDLE hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId);
    if (hProcess == NULL)
        return result;

    // 分配内存，用来接收 TBBUTTON 结构体
    LPVOID Pointer_Icon_Buttons = ::VirtualAllocEx(hProcess, 0, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    // gc
    std::shared_ptr<void> _shared_free_handle(nullptr, [&](void *)
                                              {
            if (Pointer_Icon_Buttons != NULL) {
                ::VirtualFreeEx(hProcess, Pointer_Icon_Buttons, 0, MEM_RELEASE);
            }
            if (hProcess != NULL) {
                ::CloseHandle(hProcess);
            } });

    if (Pointer_Icon_Buttons == NULL)
        return result;

    // 初始化
    BYTE Byte_Buffer_Pointer[1024] = {0};
    DWORD Index_Icon_Buttons = 0;
    HWND Main_HWND = NULL;

    long Index_Icon_Buttons_id = 0;
    int Index_data_Close_Set = 12;
    int Index_String_Close_Set = 18;

    // 判断 x64(64位和32位字数不一样)
    if (IsSystemFor64bit())
    {
        Index_data_Close_Set = Index_data_Close_Set + 4;
        Index_String_Close_Set = Index_String_Close_Set + 6;
    }

    // 获取托盘图标个数
    size_t Tray_Length = 0;
    Tray_Length = ::SendMessage(hWnd, _TB_BUTTONCOUNT, 0, 0);

    if (Tray_Length == 0)
        return result;

    // 遍历托盘
    for (size_t i = 0; i < Tray_Length; i++)
    {
        // 获取 TBBUTTON 结构
        if (!::SendMessage(hWnd, _TB_GETBUTTON, i, (LPARAM)Pointer_Icon_Buttons))
            break;

        // 获取 TBBUTTON.dwData
        if (!::ReadProcessMemory(hProcess, (LPVOID)((DWORD)Pointer_Icon_Buttons + Index_data_Close_Set), &Index_Icon_Buttons, 4, 0))
            break;

        // 解析文本
        if (Index_Icon_Buttons)
        {
            if (!::ReadProcessMemory(hProcess, (LPCVOID)Index_Icon_Buttons, Byte_Buffer_Pointer, 1024, 0))
                break;

            HWND icon_hwnd = (*((HWND *)Byte_Buffer_Pointer));

            if (icon_hwnd == NULL)
                continue;

            std::wstring path = (WCHAR *)Byte_Buffer_Pointer + Index_String_Close_Set;
            std::wstring title = (WCHAR *)Byte_Buffer_Pointer + Index_String_Close_Set + MAX_PATH;
            long index = Index_Icon_Buttons_id;

            result.push_back(TaryIcon(icon_hwnd, path, title, index));
        }

        Index_Icon_Buttons = 0;
    }

    return result;
}

// ? API  function

bool hmc_shell_util::isSystemFor64bit()
{
    SYSTEM_INFO SystemInfo;
    ::GetNativeSystemInfo(&SystemInfo);
    if (SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 || SystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        return TRUE;
    else
        return FALSE;
}

int hmc_shell_util::trashFile(std::wstring FromPath, bool bRecycle, bool isShow)
{
    SHFILEOPSTRUCTW FileOp = {0};
    if (bRecycle)
    {
        if (isShow)
        {
            FileOp.fFlags |= FOF_ALLOWUNDO;
        }
        else
        {
            FileOp.fFlags |= FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
        }
    }
    FileOp.pFrom = FromPath.c_str();
    FileOp.pTo = NULL;        // 一定要是NULL
    FileOp.wFunc = FO_DELETE; // 删除操作
    return ::SHFileOperationW(&FileOp);
}

namespace hmc_shell_util
{

    bool hmc_shell_util::clearTrash(HWND hwnd = NULL, std::wstring RootPath = L"", DWORD dwFlags = SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND)
    {
        return S_OK == ::SHEmptyRecycleBinW(hwnd, RootPath.empty() ? NULL : RootPath.c_str(), dwFlags);
    }

    bool setConversionStatus(HWND hwnd = NULL, DWORD sentencePatterns = IME_CMODE_ALPHANUMERIC, DWORD conversionModeValue = IME_SMODE_AUTOMATIC)
    {
        auto is_ok = false;

        // 获取输入法上下文
        HIMC hImc = ImmGetContext(hwnd);

        if (hImc == nullptr)
        {
            return false;
        }

        is_ok = ::ImmSetConversionStatus(hImc, sentencePatterns, conversionModeValue) != 0;

        if (is_ok)
        {
            is_ok = ::ImmReleaseContext(hwnd, hImc) != 0;
        }

        return is_ok;
    }

    bool hmc_shell_util::setShortcutLink(std::wstring lnkPath, std::wstring FilePath, std::wstring work_dir = L"", std::wstring desc = L"", std::wstring args = L"", DWORD iShowCmd = 0, std::wstring icon = L"", DWORD iconIndex = 0)
    {

        bool result = false;

        HRESULT hRes = ::CoInitialize(NULL);
        if (!SUCCEEDED(hRes))
        {
            return result;
        }

        IShellLinkW *pIShellLink;
        hRes = ::CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&pIShellLink);
        if (!SUCCEEDED(hRes))
        {
            return result;
        }
        hRes = pIShellLink->SetPath(FilePath.c_str());
        if (!SUCCEEDED(hRes))
        {
            return result;
        }
        hRes = pIShellLink->SetDescription(desc.c_str());
        if (!SUCCEEDED(hRes))
        {
            return result;
        }
        hRes = pIShellLink->SetWorkingDirectory(work_dir.c_str());
        if (!SUCCEEDED(hRes))
        {
            return result;
        }
        hRes = pIShellLink->SetArguments(args.c_str());
        if (!SUCCEEDED(hRes))
        {
            return result;
        }

        hRes = pIShellLink->SetShowCmd(iShowCmd);
        if (!SUCCEEDED(hRes))
        {
            return result;
        }

        hRes = pIShellLink->SetIconLocation(icon.c_str(), iconIndex);
        if (!SUCCEEDED(hRes))
        {
            return result;
        }
        IPersistFile *pIPersistFile;
        hRes = pIShellLink->QueryInterface(IID_IPersistFile, (void **)&pIPersistFile);
        if (!SUCCEEDED(hRes))
        {

            return result;
        }

        hRes = pIPersistFile->Save(lnkPath.c_str(), FALSE);
        if (!SUCCEEDED(hRes))
        {

            return result;
        }

        pIPersistFile->Release();
        pIShellLink->Release();

        result = true;

        return result;
    }

}

int hmc_shell_util::ShellOpen::openInExplorer(std::wstring Path)
{

    HINSTANCE hResult = ShellExecuteW(NULL, L"open", L"explorer.exe", Path.c_str(), NULL, SW_SHOWNORMAL);

    return (int)(UINT_PTR)hResult;
}

int hmc_shell_util::ShellOpen::showItemInFolder(std::wstring Path, bool isSelect)
{
    std::wstring explor_cmd = L"/select, ";

    explor_cmd.reserve(Path.size() + 15);

    if (!isSelect)
    {
        explor_cmd.clear();
    }

    explor_cmd.push_back('"');
    explor_cmd.append(Path.c_str());
    explor_cmd.push_back('"');

    HINSTANCE hResult = ShellExecuteW(NULL, L"open", L"explorer.exe", explor_cmd.c_str(), NULL, SW_SHOWNORMAL);

    return (int)(UINT_PTR)hResult;
}

int hmc_shell_util::ShellOpen::openUrl(std::wstring Url, bool isCurrentBrowser)
{
    int result = 0;

    // if (!isCurrentBrowser)
    // {
    HINSTANCE hResult = ShellExecuteW(NULL, L"open", Url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    result = (int)(UINT_PTR)hResult;
    // }

    return result;
}

int hmc_shell_util::ShellOpen::openApp(std::wstring AppPath, std::wstring Command, std::wstring cwd, bool isHide, bool isAdmin)
{
    const LPCWSTR flag = isAdmin ? L"runas" : L"open";
    const int sw_command = isHide ? 0 : 5;

    HINSTANCE hResult = ShellExecuteW(
        // hwnd
        NULL,
        // lpOperation
        flag,
        // lpFile
        AppPath.c_str(),
        // lpParameters
        Command.empty() ? NULL : Command.c_str(),
        // lpDirectory
        cwd.empty() ? NULL : cwd.c_str(),
        // nShowCmd
        sw_command);

    return (int)(UINT_PTR)hResult;
}

bool hmc_shell_util::chShortcutLinkItem::isValid()
{
    return !path.empty();
}

hmc_shell_util::chShortcutLinkItem::chShortcutLinkItem()
{
    // 目标路径
    path = L"";
    // 启动属性
    showCmd = 0;
    // 启动命令
    args = L"";
    // 描述
    desc = L"";
    // 进程图标(例如来自1.dll)
    icon = L"";
    // 图标索引位置
    iconIndex = 0;
    // 工作环境文件夹
    cwd = L"";
    // hotkey
    hotkey = 0;
}

hmc_shell_util::chShortcutLinkItem hmc_shell_util::getShortcutLink(std::wstring lnkPath)
{
    auto lnkItem = chShortcutLinkItem();

    IShellLinkW *GetShellLinkContent = NULL;

    auto temp_coinit = ::CoInitialize(0);

    LPWSTR temp_GetPathString = new wchar_t[MAX_PATH + 1];
    LPWSTR temp_GetArgumentsString = new wchar_t[MAX_PATH + 1];
    int temp_GetShowCmdNum = 0;
    LPWSTR temp_GetGetIconLocationString = new wchar_t[MAX_PATH + 1];
    int piIcon = 0;
    LPWSTR temp_GetGetDescriptionString = new wchar_t[MAX_PATH + 1];
    LPWSTR temp_GetWorkingDirectoryString = new wchar_t[MAX_PATH + 1];
    WORD pwHotkey = 0;

    HRESULT CreateHRESULT = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&GetShellLinkContent);
    DWORD dwReserved = 0;

    if (SUCCEEDED(CreateHRESULT))
    {
        IPersistFile *PersistFile;
        CreateHRESULT = GetShellLinkContent->QueryInterface(IID_IPersistFile, (LPVOID *)&PersistFile);
        if (SUCCEEDED(CreateHRESULT))
        {
            CreateHRESULT = PersistFile->Load(lnkPath.c_str(), STGM_READ);

            if (SUCCEEDED(CreateHRESULT))
            {
                WIN32_FIND_DATAW wfd = WIN32_FIND_DATAW();

                GetShellLinkContent->GetPath(temp_GetPathString, MAX_PATH, &wfd, SLGP_UNCPRIORITY | SLGP_RAWPATH);

                GetShellLinkContent->GetShowCmd(&temp_GetShowCmdNum);
                GetShellLinkContent->GetArguments(temp_GetArgumentsString, MAX_PATH);
                GetShellLinkContent->GetDescription(temp_GetGetDescriptionString, MAX_PATH);
                GetShellLinkContent->GetIconLocation(temp_GetGetIconLocationString, MAX_PATH, &piIcon);
                GetShellLinkContent->GetWorkingDirectory(temp_GetWorkingDirectoryString, MAX_PATH);
                GetShellLinkContent->GetHotkey(&pwHotkey);
            }
        }
    }

    if (GetShellLinkContent != NULL)
    {
        GetShellLinkContent->Release();
    }

    CreateHRESULT = NULL;

    lnkItem.args.append(temp_GetArgumentsString);
    lnkItem.cwd.append(temp_GetWorkingDirectoryString);
    lnkItem.desc.append(temp_GetGetDescriptionString);
    lnkItem.path.append(temp_GetPathString);
    lnkItem.icon.append(temp_GetGetIconLocationString);

    lnkItem.hotkey = pwHotkey;
    lnkItem.iconIndex = piIcon;
    lnkItem.showCmd = temp_GetShowCmdNum;

    if (CreateHRESULT > 0)
    {

        ::CoUninitialize();
    }

    return lnkItem;
}

class CComInterfaceReleaser {
public:
	explicit CComInterfaceReleaser(IUnknown * i) : _i(i) {}
	~CComInterfaceReleaser() { if (_i) _i->Release(); }
private:
	IUnknown * _i;
};

struct ComInitializer {
    bool isValid ;
	ComInitializer() {
		const auto result = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		isValid = SUCCEEDED(result);
	}

	~ComInitializer() {
		if(isValid)::CoUninitialize();
	}
};

class CItemIdArrayReleaser {
public:
	explicit CItemIdArrayReleaser(const std::vector<ITEMIDLIST*>& idArray) : _array(idArray) {}
	~CItemIdArrayReleaser() {
		for (ITEMIDLIST* item: _array)
			CoTaskMemFree(item);
	}

	CItemIdArrayReleaser& operator=(const CItemIdArrayReleaser&) = delete;
private:
	const std::vector<ITEMIDLIST*>& _array;
};

bool prepareContextMenuForObjects(std::vector<std::wstring> objects, void * parentWindow, HMENU& hmenu, IContextMenu*& imenu)
{
	ComInitializer comInitializer;

	if (objects.empty())
		return false;

	std::vector<ITEMIDLIST*> ids;
	std::vector<LPCITEMIDLIST> relativeIds;
	IShellFolder * ifolder = 0;
	for (size_t i = 0; i < objects.size(); ++i)
	{
        std::wstring filePath = std::wstring(objects[i]);

        hmc_util::replaceAll(filePath, L"/", L"\\");

        ids.push_back(0);
		HRESULT result = SHParseDisplayName(filePath.c_str(), 0, &ids.back(), 0, 0);
		if (!SUCCEEDED(result) || !ids.back())
		{
			ids.pop_back();
			continue;
		}

		relativeIds.push_back(0);
		result = SHBindToParent(ids.back(), IID_IShellFolder, (void**)&ifolder, &relativeIds.back());
		if (!SUCCEEDED(result) || !relativeIds.back())
			relativeIds.pop_back();
		else if (i < objects.size() - 1 && ifolder)
		{
			ifolder->Release();
			ifolder = nullptr;
		}
	}

	CItemIdArrayReleaser arrayReleaser(ids);

	imenu = 0;
	HRESULT result = ifolder->GetUIObjectOf((HWND)parentWindow, (UINT)relativeIds.size(), (const ITEMIDLIST **)relativeIds.data(), IID_IContextMenu, 0, (void**)&imenu);
	if (!SUCCEEDED(result) || !imenu)
		return false;

	hmenu = CreatePopupMenu();
	if (!hmenu)
		return false;
	return (SUCCEEDED(imenu->QueryContextMenu(hmenu, 0, 1, 0x7FFF, CMF_NORMAL)));
}

// 此 函数传入非法地址会出现错误
bool hmc_shell_util::showContextMenu(HWND hwnd, const std::vector<std::wstring> filePathList, int x, int y)
{

  ComInitializer comInitializer;

	IContextMenu * imenu = 0;
	HMENU hMenu = NULL;
	if (!prepareContextMenuForObjects(filePathList, hwnd, hMenu, imenu) || !hMenu || !imenu)
		return false;

	CComInterfaceReleaser menuReleaser(imenu);

    int command = ::TrackPopupMenuEx(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, x, y, static_cast<HWND>(hwnd), NULL);
    if (command > 0)
    {
      
        int command_temp = command - 1;
        CMINVOKECOMMANDINFOEX info = { 0 };
        info.cbSize = sizeof(info);
        info.hwnd = static_cast<HWND>(hwnd);
        info.lpVerb = MAKEINTRESOURCEA(command - 1);
        info.nShow = SW_NORMAL;
        imenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);

    }

	::DestroyMenu(hMenu);

	return command > 0;
    
}

bool hmc_shell_util::showContextMenu(HWND hwnd, std::wstring file_Path, int x, int y)
{

    std::wstring filePath = std::wstring(file_Path.begin(), file_Path.end());
    hmc_util::replaceAll(filePath, L"/", L"\\");

    auto temp_coinit = ::CoInitialize(NULL);

    IShellItem *pItem;
    HRESULT hr = ::SHCreateItemFromParsingName(filePath.c_str(), NULL, IID_PPV_ARGS(&pItem));
    if (FAILED(hr))
    {
        return false;
    }

    IContextMenu *pContextMenu;
    hr = pItem->BindToHandler(NULL, BHID_SFUIObject, IID_PPV_ARGS(&pContextMenu));
    if (FAILED(hr))
    {
        pItem->Release();
        return false;
    }

    HMENU hMenu = ::CreatePopupMenu();
    if (hMenu == NULL)
    {
        pContextMenu->Release();
        pItem->Release();
        return false;
    }

    hr = pContextMenu->QueryContextMenu(hMenu, 0, 1, 0x7FFF, CMF_NORMAL);

    if (FAILED(hr))
    {
        ::DestroyMenu(hMenu);
        pContextMenu->Release();
        pItem->Release();
        return false;
    }


    int command = ::TrackPopupMenuEx(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, x, y, static_cast<HWND>(hwnd), NULL);
    if (command > 0)
    {
        // if ((command & 0xF000) == 0xF000)
        //{
        //     ::DestroyMenu(hMenu);
        //     return command & 0x0FFF;
        // }

        int command_temp = command - 1;
        CMINVOKECOMMANDINFOEX info = {0};
        info.cbSize = sizeof(info);
        info.hwnd = static_cast<HWND>(hwnd);
        info.lpVerb = MAKEINTRESOURCEA(command - 1);
        info.nShow = SW_NORMAL;
        pContextMenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
    }

    // 释放资源
    ::DestroyMenu(hMenu);
    pContextMenu->Release();
    pItem->Release();

    if (temp_coinit > 0)
    {
        ::CoUninitialize();
    }

    return command > 0;
}

bool hmc_shell_util::setShortcutLink(std::wstring lnkPath, chShortcutLinkItem shortcutLinkItem)
{
    if (!shortcutLinkItem.isValid())
    {
        return false;
    }

    std::wstring FilePath = shortcutLinkItem.path;
    std::wstring work_dir = shortcutLinkItem.cwd;
    std::wstring desc = shortcutLinkItem.desc;
    std::wstring args = shortcutLinkItem.args;
    int64_t iShowCmd = shortcutLinkItem.showCmd;
    std::wstring icon = shortcutLinkItem.icon;
    int64_t iconIndex = shortcutLinkItem.iconIndex;

    return setShortcutLink(lnkPath, FilePath, work_dir, desc, args, iShowCmd, icon, iconIndex);
}

/**
 * @brief 判断是否存在尺寸铺满整个屏幕的软件
 *
 * @return true
 * @return false
 */
bool hmc_shell_util::isFocusWindowFullScreen()
{
    // 全屏应用必定是一个前台窗口
    HWND focusWindow = ::GetForegroundWindow();

    if (focusWindow == NULL)
    {
        return false;
    }

    // 获取活动窗口的位置信息
    RECT windowRect = {0};
    ::GetWindowRect(focusWindow, &windowRect);

    // 获取包含活动窗口的显示器句柄
    HMONITOR monitor = ::MonitorFromWindow(focusWindow, MONITOR_DEFAULTTONEAREST);

    // 获取主显示信息
    MONITORINFO monitorInfo = {sizeof(MONITORINFO)};
    ::GetMonitorInfo(monitor, &monitorInfo);

    // 获取屏幕的尺寸
    int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

    if (windowRect.left <= 0 && windowRect.top <= 0 &&
        windowRect.right >= screenWidth && windowRect.bottom >= screenHeight &&
        monitorInfo.rcMonitor.left == 0 && monitorInfo.rcMonitor.top == 0 &&
        monitorInfo.rcMonitor.right == screenWidth && monitorInfo.rcMonitor.bottom == screenHeight)
    {
        return !hmc_windows_util::isDesktopWindow(focusWindow);
    }

    return false;
}

bool hmc_shell_util::isFullScreen()
{
    bool result = false;

    QUERY_USER_NOTIFICATION_STATE state;
    HRESULT hr = ::SHQueryUserNotificationState(&state);
    if (hr != S_OK)
        return false;

    switch (state)
    {

    // 全屏应用程序 第三方桌面可能也会导致识别错误
    case QUNS_BUSY:
        return isFocusWindowFullScreen();
    // Direct3D 应用程序正在运行) 全屏 (独占模式。
    case QUNS_RUNNING_D3D_FULL_SCREEN:
    // 用户已激活 Windows 演示文稿设置以阻止通知和弹出消息。
    case QUNS_PRESENTATION_MODE:
        return true;

    // 显示屏幕保护程序、锁定计算机或正在进行非活动快速用户切换会话。
    case QUNS_NOT_PRESENT:
    // 找不到其他状态，可以自由发送通知。
    case QUNS_ACCEPTS_NOTIFICATIONS:
    //  当前用户处于“静默时间”，这是新用户首次登录到其帐户后的第一小时
    case QUNS_QUIET_TIME:
    // Windows 应用商店应用正在运行
    case QUNS_APP:
        return false;
    };

    return false;
}

bool setStartup(const std::wstring &key, const std::wstring &execPath, const std::wstring &cmd);
bool removeStartup(const std::wstring &key);
bool hasStartup(const std::wstring &key);

long long int hmc_shell_util::getTimestamp(bool is_Millisecond)
{

    SYSTEMTIME sys;
    ::GetLocalTime(&sys);
    return getTimestamp(sys, is_Millisecond);
}

long long int hmc_shell_util::getTimestamp(const SYSTEMTIME &st, bool is_Millisecond)
{
    FILETIME ft;
    ::SystemTimeToFileTime(&st, &ft);

    // 将 FILETIME 转换为 64 位整数，表示 100 毫微秒为单位的时间数
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    // 将时间数转换为毫秒
    return static_cast<long long int>(is_Millisecond ? uli.QuadPart / 10000 : uli.QuadPart);
}

int64_t hmc_shell_util::getSystemIdleTime()
{

    LASTINPUTINFO lpi;
    lpi.cbSize = sizeof(lpi);
    ::GetLastInputInfo(&lpi);
    unsigned long long datetime = ::GetTickCount64() - lpi.dwTime;
    int64_t SystemIdleTime = datetime;

    return SystemIdleTime;
}

bool hmc_shell_util::copyFile(const std::wstring &filePath, const std::wstring &newFilePath, bool isShowConfirm, bool isShow, bool isShowProgress)
{
    SHFILEOPSTRUCTW fileOP = {0};
    ::ZeroMemory(&fileOP, sizeof(SHFILEOPSTRUCTW));
    fileOP.wFunc = FO_COPY;
    fileOP.pFrom = filePath.c_str();
    fileOP.pTo = newFilePath.c_str();

    fileOP.fFlags = (isShowConfirm ? 0 : (isShowProgress ? 0 : FOF_SILENT)) | FOF_NOCONFIRMATION | (isShow ? 0 : FOF_NO_UI);
    int fileOperation = ::SHFileOperationW(&fileOP);

    return fileOperation == 0;
}

bool hmc_shell_util::moveFile(const std::wstring &filePath, const std::wstring &newFilePath, bool isShowConfirm, bool isShow, bool isShowProgress)
{
    SHFILEOPSTRUCTW fileOP = {0};
    ::ZeroMemory(&fileOP, sizeof(SHFILEOPSTRUCTW));
    fileOP.wFunc = FO_MOVE;
    fileOP.pFrom = filePath.c_str();
    fileOP.pTo = newFilePath.c_str();
    fileOP.fFlags = (isShowConfirm ? 0 : (isShowProgress ? 0 : FOF_SILENT)) | FOF_NOCONFIRMATION | (isShow ? 0 : FOF_NO_UI);
    int fileOperation = ::SHFileOperationW(&fileOP);
    return fileOperation == 0;
}

bool hmc_shell_util::SetFolderIcon(const std::wstring &folderPath, const std::wstring &iconPath, int iconIndex)
{

    // 设置文件夹属性为只读，保护 desktop.ini 文件
    DWORD folderAttributes = ::GetFileAttributesW(folderPath.c_str());
    if (folderAttributes == INVALID_FILE_ATTRIBUTES)
        return false;

    if ((folderAttributes & FILE_ATTRIBUTE_READONLY) != FILE_ATTRIBUTE_READONLY)
        ::SetFileAttributesW(folderPath.c_str(), folderAttributes | FILE_ATTRIBUTE_READONLY);

    // 构建 desktop.ini 文件路径
    std::wstring desktopIniPath = folderPath + L"\\desktop.ini";

    // 写入 desktop.ini 文件内容
    std::wstring iniContent = L"[.ShellClassInfo]\nIconResource=" + iconPath + L"," + std::to_wstring(iconIndex) + L"\n";
    if (!::WritePrivateProfileStringW(L".ShellClassInfo", L"IconResource", (iconPath + L"," + std::to_wstring(iconIndex)).c_str(), desktopIniPath.c_str()))
        return false;

    // 设置 desktop.ini 文件属性为隐藏和系统
    ::SetFileAttributesW(desktopIniPath.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);

    // 将文件夹的只读属性恢复
    ::SetFileAttributesW(folderPath.c_str(), folderAttributes);

    // 使文件夹的缩略图设置生效
    ::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return true;
}

bool hmc_shell_util::powerControl(DWORD dwReason, bool aims)
{
    if (dwReason != EWX_LOGOFF && dwReason != EWX_REBOOT && dwReason != EWX_SHUTDOWN)
        return FALSE;
    OSVERSIONINFO osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    dwReason |= (aims != FALSE) ? EWX_FORCE : EWX_FORCEIFHUNG;
    return ::ExitWindowsEx(dwReason, 0);
}

void hmc_shell_util::powerControl(int flage)
{
    switch (flage)
    {
    case 1001: // 关机
        powerControl(EWX_SHUTDOWN, true);
        break;
    case 1002: // 重启
        powerControl(EWX_REBOOT, true);
        break;
    case 1003: // 注销
        powerControl(EWX_LOGOFF, false);
        break;
    case 1005: // 锁定
        ::LockWorkStation();
        break;
    case 1006: // 关闭显示器
        ::SendMessage(::FindWindow(0, 0), WM_SYSCOMMAND, SC_MONITORPOWER, 2);
        break;
    case 1007: // 打开显示器
        ::SendMessage(::FindWindow(0, 0), WM_SYSCOMMAND, SC_MONITORPOWER, -1);
        break;
    }
}

void hmc_shell_util::beep()
{
    ::MessageBeep(MB_OK);
}

hmc_shell_util::fileBrowser::Options::Options()
{
    filters = {};
    filters_extensions = {};
    title = L"";
    defaultPath = L"";
    buttonLabel = L"";
    properties = {};
}

bool hmc_shell_util::Symlink::isHardLink(const std::wstring &filePath)
{

    HANDLE fileHandle = ::CreateFileW(
        filePath.c_str(),
        0,                     // 不需要任何访问权限
        FILE_SHARE_READ,       // 允许其他进程读取文件
        NULL,                  // 不需要安全描述符
        OPEN_EXISTING,         // 文件必须存在
        FILE_ATTRIBUTE_NORMAL, // 常规文件
        NULL                   // 没有模板文件
    );

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    BY_HANDLE_FILE_INFORMATION fileInfo;
    if (!::GetFileInformationByHandle(fileHandle, &fileInfo))
    {
        ::CloseHandle(fileHandle);
        return false;
    }

    ::CloseHandle(fileHandle);

    // 不可能小于1 因为当前的文件就是其中一个
    return fileInfo.nNumberOfLinks > 1;
}

std::vector<std::wstring> hmc_shell_util::Symlink::getHardLinkList(const std::wstring &filePath)
{
    std::vector<std::wstring> hardLinks;

    std::vector<wchar_t> buffer;

    if (filePath.size() < 3)
    {
        return hardLinks;
    }

    // 缓冲区大小 重置值
    size_t re_length = MAX_PATH + 1;
    // 缓冲区大小 获取值
    DWORD buff_size = MAX_PATH + 1;

    buffer.reserve(re_length);
    buffer.resize(re_length);

    bool next_read = false;

    HANDLE hFind = ::FindFirstFileNameW(filePath.c_str(), 0, &buff_size, &buffer[0]);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        // 使用了长路径 最长32768个
        if (::GetLastError() == ERROR_MORE_DATA)
        {
            re_length = 32768;
            buffer.reserve(re_length);
            buff_size = re_length;
            hFind = ::FindFirstFileNameW(filePath.c_str(), 0, &buff_size, &buffer[0]);

            // 无解
            if (hFind == INVALID_HANDLE_VALUE)
            {
                return hardLinks;
            }
        }
        else
            return hardLinks;
    }

    std::shared_ptr<void> open_hkey_close_key(nullptr, [&](void *)
                                              { ::FindClose(hFind); });

    std::wstring driveLetter = std::wstring();
    driveLetter.push_back(filePath[0]);
    driveLetter.push_back(filePath[1]);

    do
    {

        if (buff_size > 0)
        {

            std::wstring temp = std::wstring(driveLetter.begin(), driveLetter.end());

            for (size_t i = 0; i < buff_size; i++)
            {
                auto it = buffer[i];
                if (it != L'\0')
                    temp.push_back(it);
            }

            temp.push_back(L'\0');

            hardLinks.push_back(temp);
        }

        buff_size = re_length;
        next_read = ::FindNextFileNameW(hFind, &buff_size, &buffer[0]);

        if (!next_read)
        {
            // buff_size不等于0 但是提前停止了 那么意味着 此错误是 可能是因为使用了长路径 最长32768个

            if (buff_size > 1 && buff_size != re_length && ::GetLastError() == ERROR_MORE_DATA)
            {
                re_length = 32768;
                buffer.reserve(re_length);
                buff_size = re_length;
                buffer.resize(re_length);

                // 重试
                next_read = ::FindNextFileNameW(hFind, &buff_size, &buffer[0]);
                if (next_read)
                {
                    return hardLinks;
                }
            }
            else
            {
                return hardLinks;
            }
        }

    } while (next_read);

    return hardLinks;
}

bool hmc_shell_util::Symlink::isLink(const std::wstring &Path)
{
    DWORD dwAttr = ::GetFileAttributesW(Path.c_str());
    if (dwAttr == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    return dwAttr & FILE_ATTRIBUTE_REPARSE_POINT;
}

std::wstring hmc_shell_util::Symlink::getSymbolicLinkTarget(const std::wstring &symlinkPath)
{
    HANDLE hFile = ::CreateFileW(symlinkPath.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
    {
        return L"";
    }

    wchar_t *targetPath = new wchar_t[MAX_PATH + 1];
    DWORD pathSize = ::GetFinalPathNameByHandleW(hFile, targetPath, MAX_PATH, 0);
    if (pathSize == 0)
    {
        ::CloseHandle(hFile);
        return L"";
    }

    // '\\\\?\\F:\\source\\module\\hmc-win32\\source\\h2c-win32\\source\\node\\debug.js'
    std::wstring finalPath = std::wstring(targetPath, pathSize);

    const std::wstring prefix = L"\\\\?\\";

    if (finalPath.find(prefix) == 0)
    {
        finalPath.erase(0, prefix.size());
    }

    ::CloseHandle(hFile);
    return finalPath;
}

bool hmc_shell_util::Symlink::createDirSymlink(const std::wstring &targetPath, const std::wstring &sourcePath)
{
    return CreateSymbolicLinkW(targetPath.c_str(), sourcePath.c_str(), 0x1);
}

bool hmc_shell_util::Symlink::createSymlink(const std::wstring &targetPath, const std::wstring &sourcePath)
{
    return CreateSymbolicLinkW(targetPath.c_str(), sourcePath.c_str(), 0x1);
}

bool hmc_shell_util::Symlink::createHardLink(const std::wstring &targetPath, const std::wstring &sourcePath)
{
    return CreateHardLinkW(targetPath.c_str(), sourcePath.c_str(), NULL);
}

bool hmc_shell_util::Symlink::createSymbolicLink(const std::wstring &targetPath, const std::wstring &sourcePath)
{
    return CreateSymbolicLinkW(targetPath.c_str(), sourcePath.c_str(), SYMBOLIC_LINK_FLAG_DIRECTORY);
}

int hmc_shell_util::GetThumbnail::GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
    UINT num = 0;  // number of image encoders
    UINT size = 0; // size of the image encoder array in bytes

    Gdiplus::GetImageEncodersSize(&num, &size);

    if (size == NULL)
        return -1; // Failure

    auto pImageCodecInfo = VcRef<Gdiplus::ImageCodecInfo>(size);

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (::wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            return j; // Success
        }
    }

    return -1; // Failure
}

void hmc_shell_util::GetThumbnail::WriteToImageByte(Gdiplus::Bitmap *image, LPCWSTR formatMimeType, std::vector<BYTE> &result)
{

    CLSID myClsId;
    int retVal = GetEncoderClsid(formatMimeType, &myClsId);
    if (retVal == -1)
        return;

    IStream *istream = nullptr;
    if (::CreateStreamOnHGlobal(NULL, TRUE, &istream) != 0)
        return;

    image->Save(istream, &myClsId, nullptr);

    HGLOBAL hg = NULL;
    if (::GetHGlobalFromStream(istream, &hg) != S_OK)
        return;

    size_t bufsize = ::GlobalSize(hg);
    result.resize(bufsize);

    LPVOID pimage = ::GlobalLock(hg);

    if (!pimage)
    {
        result.clear();
        result.resize(0);
        return;
    }

    ::memcpy(&result[0], pimage, bufsize);

    istream->Release();

    ::GlobalUnlock(hg);

    return;
}

void hmc_shell_util::GetThumbnail::GetBitmapPixel(HBITMAP hbitmap, std::vector<BYTE> &result, Gdiplus::PixelFormat format, LPCWSTR formatMimeType)
{

    PALETTEENTRY pat = {255, 255, 255, 0};
    LOGPALETTE logpalt = {0, 1, pat};
    HPALETTE hpalette = ::CreatePalette(&logpalt);

    Gdiplus::Bitmap *bitmap = Gdiplus::Bitmap::FromHBITMAP(hbitmap, hpalette);
    Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
    Gdiplus::BitmapData bd;
    bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, bitmap->GetPixelFormat(), &bd);
    Gdiplus::Bitmap *bitmapWithAlpha = new Gdiplus::Bitmap(bd.Width, bd.Height, bd.Stride, format, (BYTE *)bd.Scan0);
    bitmap->UnlockBits(&bd);

    WriteToImageByte(bitmapWithAlpha, formatMimeType, result);
}

std::vector<BYTE> hmc_shell_util::GetThumbnail::GetBuff(std::wstring input, int nSize)
{
    std::vector<BYTE> result;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != 0)
    {
        return result;
    }

    FreeAnyAuto({
        Gdiplus::GdiplusShutdown(gdiplusToken);
    });

    HRESULT hr = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        FreeAnyAuto({ ::CoUninitialize(); });

        IShellItemImageFactory *pImageFactory;

        hr = ::SHCreateItemFromParsingName(input.c_str(), nullptr, IID_PPV_ARGS(&pImageFactory));

        if (SUCCEEDED(hr))
        {
            SIZE size = {nSize, nSize};

            FreeAnyAuto({ pImageFactory->Release(); });

            HBITMAP hbitmap;
            hr = pImageFactory->GetImage(size, SIIGBF_BIGGERSIZEOK | SIIGBF_ICONONLY | SIIGBF_SCALEUP, &hbitmap);
            if (SUCCEEDED(hr))
            {
                FreeAnyAuto({ ::DeleteObject(hbitmap); });

                PALETTEENTRY pat = {255, 255, 255, 0};
                LOGPALETTE logpalt = {0, 1, pat};
                HPALETTE hpalette = ::CreatePalette(&logpalt);

                Gdiplus::Bitmap *bitmap = Gdiplus::Bitmap::FromHBITMAP(hbitmap, hpalette);

                if (bitmap != NULL)
                {
                    Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
                    Gdiplus::BitmapData bd;
                    bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, bitmap->GetPixelFormat(), &bd);
                    Gdiplus::Bitmap *bitmapWithAlpha = new Gdiplus::Bitmap(bd.Width, bd.Height, bd.Stride, PixelFormat32bppARGB, (BYTE *)bd.Scan0);
                    bitmap->UnlockBits(&bd);
                    WriteToImageByte(bitmapWithAlpha, L"image/png", result);
                }
            }
        }
    }

    return result;
}

bool hmc_shell_util::GetThumbnail::toFile(std::wstring input, std::wstring output, int nSize)
{

    auto data = GetBuff(input, nSize);

    if (!data.empty())
    {
        std::ofstream OutFile(output.c_str(), std::ios::out | std::ios::binary);
        OutFile.write(reinterpret_cast<char *>(&data[0]), data.size());

        if (OutFile.fail())
        {
            return false;
        }

        OutFile.close();
        return true;
    }

    return false;
}

/*


void winRunApplication(const std::wstring &Path)
{
    std::thread([Path]() -> void
                {


            keybd_event(VK_LWIN, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(0x52, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(0x52, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);

            // 粘贴文本进入输入框中
            for (size_t i = 0; i < 100; i++)
            {

                Sleep(50);
                HWND focusWin = GetForegroundWindow();

                DWORD pid = 0;
                DWORD theardId = GetWindowThreadProcessId(focusWin, &pid);
                std::string FilePath;
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
                bool has_ok = false;
                char *lpFilename = new char[MAX_PATH+1];

                if (hProcess == nullptr)
                {
                    continue;
                }

                ::GetModuleBaseNameA(hProcess, NULL, lpFilename, MAX_PATH);
                ::CloseHandle(hProcess);
                FilePath.append(lpFilename);

                     std::string FilePath2upper;

            for (char &c : FilePath)
            {
                if (isalpha(static_cast<unsigned char>(c)))
                {
                    FilePath2upper.push_back(toupper(c));
                }
                else
                {
                    FilePath2upper.push_back(c);
                }
            }

                if (FilePath2upper == std::string("EXPLORER.EXE"))
                {
                    EnumChildWindows(
                        focusWin, [](HWND hWnd, LPARAM lParam) -> BOOL
                        {
                            int controlId = GetDlgCtrlID(hWnd);
                            if (controlId == 1001)
                            {
                                LPSTR lpClassName = {0};
                                SendMessageA(hWnd, WM_SETTEXT, 0,lParam);
                                Sleep(50);

                                keybd_event(VK_RETURN, 0, KEYEVENTF_EXTENDEDKEY, 0);
                                keybd_event(VK_RETURN, 0, KEYEVENTF_EXTENDEDKEY, 0);

                                Sleep(50);

                                return false;
                            }
                            return TRUE; // 返回 TRUE 以继续枚举下一个子控件
                        },
                        0);
                }
                else
                {
                    if (i > 80)
                    {
                        return;
                    }
                }
                if (has_ok)
                    return;
            } })
        .detach();
}


*/