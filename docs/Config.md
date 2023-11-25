# Config

## 配置文件目录结构

> 默认情况下

```
config
├─control
|  └─default.json
├─device
|  └─default.json
├─tasks
|  └─default.json
└─maa_toolkit.json
```

`./config/control` 目录下放置 [control](#control) 相关配置

`./config/device` 目录下放置 [device](#device) 相关配置

`./config/tasks` 目录下放置 [tasks](#tasks) 相关配置

`maa_toolkit.json` 用于管理 maa_toolkit 相关配置

## control

`./config/control/default.json` 为成功初始化后生成的 `control` 默认配置文件

## device

> CLI成功配置目标连接设备后，会生成一个名为 `default.json` 的 device 默认配置文件。你可以在 `./config/control` 目录下找到此文件。该配置文件用于定义默认连接的设备。请注意，根据系统的要求和需求，你可能需要修改此文件以适应特定的配置。

`./config/device/default.json` 内容一般如下

```json
{
    "adb": "D:/MuMuPlayer-12.0/shell/adb.exe",
    "device_SN": "127.0.0.1:16384",
    "device_name": "MuMuPlayer12"
}
```

### device 配置属性

| 属性名 | 类型 | 描述 |
| ---- | ---- | ---- |
| device_name | string | 设备名称 |
| device_SN | string | 设备序列号(serial number) |
| adb | srting | adb路径 |

## tasks

`./config/tasks/default.json` 为成功初始化后生成的 `tasks` 默认配置文件