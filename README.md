# MAABH3

基于MAA全新架构的 蹦蹦蹦 小助手。图像技术 + 模拟控制，解放双手！

由 [MaaFramework](https://github.com/MaaAssistantArknights/MaaFramework) 强力驱动！

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