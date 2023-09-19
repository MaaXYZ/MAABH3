<div align="center">

# MAABH3

基于MAA全新架构的 蹦蹦蹦 小助手

图像技术 + 模拟控制，解放双手！

由 [MaaFramework](https://github.com/MaaAssistantArknights/MaaFramework) 强力驱动！

</div>

## How to use

> 1. [下载](https://github.com/MaaAssistantArknights/MAABH3/releases) 对应平台的压缩包
> 2. 将压缩包解压到没有中文的目录下

### 基本说明

1. 请根据 [模拟器支持情况](https://maa.plus/docs/1.3-模拟器支持.html)，进行对应的操作。
2. 修改模拟器分辨率为 `16:9` 比例，最低 `1280 * 720`，更高不限。

### 直接使用 

> 以 Windows 用户为主，其他系统请照葫芦画瓢。

1. 首次使用，双击打开 `MAABH3_CLI.exe` 或 通过 CMD 执行 `MAABH3_CLI.exe`
2. 等待扫描设备（设备越多等待时间越长）
3. 选择客户端
4. 输入需要执行的任务，以空格分隔，例如 `1 2 3 4 5`
5. 选择需要连接的设备
6. 开始使用吧！（后续使用无需再次选择客户端和输入需要执行的任务）

### 作为MAA结束后脚本使用

> 以 Windows 用户为主，其他系统请照葫芦画瓢。

1. 先按照 [直接使用](#直接使用) 进行操作
2. 编写批处理文件
//Todo
3. 在MAA结束后脚本处填写批处理文件路径
4. 开始使用吧！

## 任务说明

| 任务 | 说明 |
| ---- | ---- |
| Homelad | 家园；自动收家园金币、体力、打工和远征 |
| MaterialEvent | 材料远征；自动一键减负 |
| Armada | 舰团；自动提交委托回收和领取舰团贡献奖励 |
| Shop | 商店；用金币购买后勤终端的角色碎片 |
| Awards | 奖励；领取每日奖励 |

## How to build

**如果你要编译源码才看这节，否则直接 [下载](https://github.com/MaaAssistantArknights/MAABH3/releases) 即可**

_欢迎大佬们来带带~_

1. 下载 MaaFramework 的 [Release 包](https://github.com/MaaAssistantArknights/MaaFramework/releases)，解压到 `deps` 文件夹中
2. 配置 cmake

    - Windows  

    ```bash
    cmake --preset "MSVC 2022"
    ```

    - Linux / macOS

    ```bash
    cmake --preset "NinjaMulti"
    ```

3. 使用 cmake 构建工程  

    ```bash
    cmake --build build --config Release
    cmake --install build --prefix install
    ```

    生成的二进制及相关资源文件在 `install` 目录下

## 开发相关

- `tools/CropRoi` 可以用来裁剪图片和获取 ROI
- Pipeline 协议请参考 [MaaFramework 的文档](https://github.com/MaaAssistantArknights/MaaFramework/blob/main/docs/zh_cn/3.3-%E4%BB%BB%E5%8A%A1%E6%B5%81%E6%B0%B4%E7%BA%BF%E5%8D%8F%E8%AE%AE.md)

## Join us

吹水 QQ 群：672372860

开发 QQ 群：566868495

## 其他

* 芝士[MAA1999](https://github.com/MaaAssistantArknights/MAA1999)，基于MAA全新架构的 亿韭韭韭 小助手。

## Todo

* [x] 启动
  * [x] 签到

* [x] 家园
  * [x] 家园金币和体力
  * [x] 家园打工
  * [x] 家园远征

* [x] 材料远征一键减负

* [x] 舰团
  * [x] 委托回收
  * [x] 舰团贡献

* [ ] 商店
  * [x] 后勤终端金币碎片
  * [ ] 虚境商店1星石时序通行证
  * [ ] 吼姆秘宝

* [ ] 奖励
  * [x] 每日奖励
  * [ ] 邮件

* [ ] 杂项
  * [ ] 每周分享