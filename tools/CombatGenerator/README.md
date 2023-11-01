# CombatGenerator

## 使用

    cd path/to/CombatGenerator
    pip install pipx
    pipx ensurepath
    pipx install pdm
    pdm install

重启终端后，直接运行`CombatGenerator`下`__init__.py`即可。

## 可选参数

### 反序列器 将生成的combatJson反序列化为输入的结构体

    -r

### 传入路径

    -i path/to/yourfile.json(绝对路径)

### 输出路径

    -o path/to/yourfile.json(绝对路径)

### e.g.

例如我想使用反序列器，传入路径为F:/1.json, 输出路径为F:/2.json

    python __init__.py -r -i F:/1.json -o F:/2.json

## input说明

`"BasicATK": [
0,
30
]`中[0, 30]表示`pre_delay=0`, `post_delay=30`。
若不填则根据下表填入默认值

| Action         | Pre Delay | Post Delay |
|----------------|-----------|------------|
| Forward        | 0         | 50         |
| Backward       | 0         | 50         |
| Left           | 0         | 50         |
| Right          | 0         | 50         |
| Left Forward   | 0         | 50         |
| Left Backward  | 0         | 50         |
| Right Forward  | 0         | 50         |
| Right Backward | 0         | 50         |
| BasicATK       | 0         | 200        |
| BasicATKCharge | 0         | 100        |
| Evade          | 0         | 150        |
| Ultimate       | 0         | 150        |
| UltimateCharge | 0         | 100        |
| WeaponSkill    | 0         | 150        |
| ELFSkill       | 0         | 150        |
| ExtraSkill     | 0         | 100        |
| QTE1           | 	0        | 	200       |
| QTE2           | 	0        | 	200       |

    {
        "mode": "[关卡类型]",
        "role": "[女武神名称]",
        "version": "[版本]",
        "combat": [
            "[Combat Action]",
            {"[Combat Action]": ["[pre delay]","[post delay]"]},
            "..."
        ]
    }

`prefix`: [mode]Combat[role]

    {
    "UniversalMirageCombatElysianPreheat": {
        "recognition": "Custom",
        "custom_recognizer": "Combating",
        "action": "Custom",
        "pre_delay": 1000,
        "post_delay": 1000,
        "next": [
            "UniversalMirageCombatElysianFinish",
            "UniversalMirageCombatElysian_001"
        ],
        "custom_action": "BasicATK"
    },
    "UniversalMirageCombatElysian_001": {
        "recognition": "Custom",
        "custom_recognizer": "Combating",
        "action": "Custom",
        "pre_delay": 0,
        "post_delay": 30,
        "next": [
            "UniversalMirageCombatElysianFinish",
            "UniversalMirageCombatElysian_002"
        ],
        "custom_action": "BasicATK"
    },
    "..."
    }

### 首项无论是什么延迟均为[1000, 1000]

具体样本可以去data下看

