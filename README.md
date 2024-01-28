# hmc-module

 hmc-win32 是一个调用常用winapi的node模块，它包含了接近两百个API 本存储库的c++代码是hmc的核心功能支持库

但是他仅限于Windows平台

-----------------------------------------



#### hmc的开发宗旨永远在于

- 内存安全 [安全永远高于效率]
- 高效
- 代码可移植性
- 避免C语言语法  因为他的安全性很难控制



##### hmc目前已经包含了几个功能大类

- hmc_sock_util 本机网络io
- hmc_usb_util 一些简单的usb获取
- hmc_string_util 简单的文本处理
- hmc_registr_util 注册表封装
- hmc_shell_util 对文件系统/ 资源管理器的一些调用
- hmc_napi_value_util  纯node_api 的接口封装
- hmc_clip_util 对剪贴板的操作
- hmc_automation_util 自动化 例如键鼠操作
- hmc_byte_util 一些简单的进制转换 
- hmc_process_util 进程api封装
- hmc_windows_util 窗口操作

