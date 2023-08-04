# MAABH3

由 [MaaFramework](https://github.com/MaaAssistantArknights/MaaFramework) 强力驱动！

## How to build

**如果你要编译源码才看这节，否则直接 [下载](https://github.com/MaaAssistantArknights/MAA1999/releases) 即可**

_只是一个临时的编译方法，因为新架构中的 [MaaCommon](https://github.com/MaaAssistantArknights/MaaCommon) 还没完成，所以先简单糊一下。欢迎大佬们来带带~_

1. 更新子模块 `git submodule update --init --recursive`
2. 下载 MaaFramework 的 [Release 包](https://github.com/MaaAssistantArknights/MaaFramework/releases)，解压到 `source/cli/x64/Release`
3. 使用 Visual Studio 编译 `source/cli/MAABH3_CLI.sln`
4. `tools/CropRoi` 是个小工具，可以用来裁剪图片和获取 ROI