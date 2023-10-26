# CombatGenerator

## 使用

    cd path/to/CombatGenerator
    pip install pipx
    pipx ensurepath
    pipx install pdm
    pdm install

重启终端后，直接运行`CombatGenerator`下`__init__.py`即可。

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
    "[prefix]Preheat": {
        "recognition": "TemplateMatch",
        "template": "Combat/StopCombat.png",
        "roi": [
            0,
            0,
            136,
            140
        ],
        "action": "Custom",
        "custom_action": "BasicATK",
        "pre_delay": 500,
        "post_delay": 1500,
        "next": [
            "[type]CombatFinish",
            "[prefix]_001"
        ]
    },
     "[prefix]_001": {
        "recognition": "TemplateMatch",
        "template": "Combat/StopCombat.png",
        "roi": [
            0,
            0,
            136,
            140
        ],
        "action": "Custom",
        "custom_action": "[Combat Action]",
        "pre_delay": "[pre delay]",
        "post_delay": "[post delay]",
        "next": [
            "[type]CombatFinish",
            "[prefix]_002"
        ]
    },
    "..."
}

### 首项无论是什么延迟均为[500, 1500]

具体样本可以去data下看

