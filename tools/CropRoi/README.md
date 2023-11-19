# 截图工具

## 环境

需要 `python` 环境

## 依赖

在./tools/CropRoi下执行以下命令

(如果已经安装过pipx和pdm可以跳过直接使用pdm install)

```shell
python -m pip install pipx
pipx ensurepath
```

重启终端

```shell
pipx install pdm
pdm install
```

## 使用

截图需预先保存到 `./src/` 路径下, 产物将会保存在 `./dst/` 路径下

打开cmd, 执行以下命令

```shell
.venv\Scripts\Activate.ps1
pdm run python main.py
```

在弹出窗口中使用鼠标左键画框, `s` 键保存, `q` 键退出
