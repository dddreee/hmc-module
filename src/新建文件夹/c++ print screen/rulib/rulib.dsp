# Microsoft Developer Studio Project File - Name="rulib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rulib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rulib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rulib.mak" CFG="rulib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rulib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "rulib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rulib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\rulib.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=doxygen dox/doxygen.cfg
# End Special Build Tool

!ELSEIF  "$(CFG)" == "rulib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\db_rulib.lib"

!ENDIF 

# Begin Target

# Name "rulib - Win32 Release"
# Name "rulib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AsyncWinSock.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoReg.cpp
# End Source File
# Begin Source File

SOURCE=.\AviEncode.cpp
# End Source File
# Begin Source File

SOURCE=.\Base64.cpp
# End Source File
# Begin Source File

SOURCE=.\Bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\Capture.cpp
# End Source File
# Begin Source File

SOURCE=.\CfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CircBuf.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientDde.cpp
# End Source File
# Begin Source File

SOURCE=.\CmpFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ComCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Comp2.cpp
# End Source File
# Begin Source File

SOURCE=.\CsvFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DataPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMover.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrLog.cpp
# End Source File
# Begin Source File

SOURCE=.\FifoSync.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\FindFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\GrBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\GrDC.cpp
# End Source File
# Begin Source File

SOURCE=.\GrPen.cpp
# End Source File
# Begin Source File

SOURCE=.\HList.cpp
# End Source File
# Begin Source File

SOURCE=.\HookStdio.cpp
# End Source File
# Begin Source File

SOURCE=.\HqTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\LList.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\md5rsa.cpp
# End Source File
# Begin Source File

SOURCE=.\Mime.cpp
# End Source File
# Begin Source File

SOURCE=.\Module.cpp
# End Source File
# Begin Source File

SOURCE=.\MovableObj.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiStore.cpp
# End Source File
# Begin Source File

SOURCE=.\NetCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\NetCom.cpp
# End Source File
# Begin Source File

SOURCE=.\NetDispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\NetFile.cpp
# End Source File
# Begin Source File

SOURCE=.\NetMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\NetMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Params.cpp
# End Source File
# Begin Source File

SOURCE=.\PFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp
# End Source File
# Begin Source File

SOURCE=.\Reg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResType.cpp
# End Source File
# Begin Source File

SOURCE=.\RKey.cpp
# End Source File
# Begin Source File

SOURCE=.\StackReport.cpp
# End Source File
# Begin Source File

SOURCE=.\StackTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Str.cpp
# End Source File
# Begin Source File

SOURCE=.\StrParse.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskList.cpp
# End Source File
# Begin Source File

SOURCE=.\Text.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Tooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\Var.cpp
# End Source File
# Begin Source File

SOURCE=.\vErr.cpp
# End Source File
# Begin Source File

SOURCE=.\Win.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAsyncSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAvi.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDc.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDcQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\WinFile.cpp
# End Source File
# Begin Source File

SOURCE=.\WinImg.cpp
# End Source File
# Begin Source File

SOURCE=.\WinPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\WinRect.cpp
# End Source File
# Begin Source File

SOURCE=.\WinSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\WinTime.cpp
# End Source File
# Begin Source File

SOURCE=.\WinTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\WspStream.cpp
# End Source File
# Begin Source File

SOURCE=.\XpTheme.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\AsyncWinSock.h
# End Source File
# Begin Source File

SOURCE=.\inc\AutoReg.h
# End Source File
# Begin Source File

SOURCE=.\inc\AviEncode.h
# End Source File
# Begin Source File

SOURCE=.\inc\Base64.h
# End Source File
# Begin Source File

SOURCE=.\inc\Bmp.h
# End Source File
# Begin Source File

SOURCE=.\inc\Capture.h
# End Source File
# Begin Source File

SOURCE=.\inc\CfgFile.h
# End Source File
# Begin Source File

SOURCE=.\inc\CircBuf.h
# End Source File
# Begin Source File

SOURCE=.\inc\ClientDde.h
# End Source File
# Begin Source File

SOURCE=.\inc\CmpFile.h
# End Source File
# Begin Source File

SOURCE=.\inc\ComCtrl.h
# End Source File
# Begin Source File

SOURCE=.\inc\Comp2.h
# End Source File
# Begin Source File

SOURCE=.\inc\CsvFile.h
# End Source File
# Begin Source File

SOURCE=.\inc\DataPacket.h
# End Source File
# Begin Source File

SOURCE=.\inc\DdeSz.h
# End Source File
# Begin Source File

SOURCE=.\inc\DlgMover.h
# End Source File
# Begin Source File

SOURCE=.\inc\ErrLog.h
# End Source File
# Begin Source File

SOURCE=.\inc\FifoSync.h
# End Source File
# Begin Source File

SOURCE=.\inc\FileSearch.h
# End Source File
# Begin Source File

SOURCE=.\inc\FindFiles.h
# End Source File
# Begin Source File

SOURCE=.\inc\GrBrush.h
# End Source File
# Begin Source File

SOURCE=.\inc\GrDC.h
# End Source File
# Begin Source File

SOURCE=.\inc\GrPen.h
# End Source File
# Begin Source File

SOURCE=.\inc\HList.h
# End Source File
# Begin Source File

SOURCE=.\inc\HookStdio.h
# End Source File
# Begin Source File

SOURCE=.\inc\HqTimer.h
# End Source File
# Begin Source File

SOURCE=.\inc\linked_ptr.h
# End Source File
# Begin Source File

SOURCE=.\inc\LList.h
# End Source File
# Begin Source File

SOURCE=.\inc\Macro.h
# End Source File
# Begin Source File

SOURCE=.\inc\md5.h
# End Source File
# Begin Source File

SOURCE=.\inc\md5rsa.h
# End Source File
# Begin Source File

SOURCE=.\inc\Message.h
# End Source File
# Begin Source File

SOURCE=.\inc\MessageList.h
# End Source File
# Begin Source File

SOURCE=.\inc\Mime.h
# End Source File
# Begin Source File

SOURCE=.\inc\Module.h
# End Source File
# Begin Source File

SOURCE=.\inc\MovableObj.h
# End Source File
# Begin Source File

SOURCE=.\inc\MultiStore.h
# End Source File
# Begin Source File

SOURCE=.\inc\NetCmd.h
# End Source File
# Begin Source File

SOURCE=.\inc\NetCom.h
# End Source File
# Begin Source File

SOURCE=.\inc\NetDispatch.h
# End Source File
# Begin Source File

SOURCE=.\inc\NetFile.h
# End Source File
# Begin Source File

SOURCE=.\inc\NetMgr.h
# End Source File
# Begin Source File

SOURCE=.\inc\NetMsg.h
# End Source File
# Begin Source File

SOURCE=.\inc\Params.h
# End Source File
# Begin Source File

SOURCE=.\inc\PFile.h
# End Source File
# Begin Source File

SOURCE=.\inc\Pid.h
# End Source File
# Begin Source File

SOURCE=.\inc\Pipe.h
# End Source File
# Begin Source File

SOURCE=.\inc\Reg.h
# End Source File
# Begin Source File

SOURCE=.\inc\RKey.h
# End Source File
# Begin Source File

SOURCE=.\inc\rulib.h
# End Source File
# Begin Source File

SOURCE=.\inc\StackReport.h
# End Source File
# Begin Source File

SOURCE=.\inc\StackTrace.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\inc\Str.h
# End Source File
# Begin Source File

SOURCE=.\inc\StrParse.h
# End Source File
# Begin Source File

SOURCE=.\inc\TaskList.h
# End Source File
# Begin Source File

SOURCE=.\inc\Text.h
# End Source File
# Begin Source File

SOURCE=.\inc\THList.h
# End Source File
# Begin Source File

SOURCE=.\inc\Thread.h
# End Source File
# Begin Source File

SOURCE=.\inc\ThreadSafe.h
# End Source File
# Begin Source File

SOURCE=.\inc\Timeout.h
# End Source File
# Begin Source File

SOURCE=.\inc\TLList.h
# End Source File
# Begin Source File

SOURCE=.\inc\TMem.h
# End Source File
# Begin Source File

SOURCE=.\inc\Tooltip.h
# End Source File
# Begin Source File

SOURCE=.\inc\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\inc\Var.h
# End Source File
# Begin Source File

SOURCE=.\inc\vErr.h
# End Source File
# Begin Source File

SOURCE=.\inc\Win.h
# End Source File
# Begin Source File

SOURCE=.\inc\Win32.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinAsyncSocket.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinAvi.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinDc.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinDcQueue.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinDlg.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinFile.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinImg.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinPolygon.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinRect.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinSocket.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinTime.h
# End Source File
# Begin Source File

SOURCE=.\inc\WinTimer.h
# End Source File
# Begin Source File

SOURCE=.\inc\WspStream.h
# End Source File
# Begin Source File

SOURCE=.\inc\XpTheme.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\dox\doxygen.cfg
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
