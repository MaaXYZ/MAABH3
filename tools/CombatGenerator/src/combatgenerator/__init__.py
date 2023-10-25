from pathlib import Path
from typing import Dict, List, Union, Optional
from rich import print

from pydantic import BaseModel

combat_path = Path(__file__).parent.parent.parent / "data" / "input.json"
json_template = {
    "recognition": "TemplateMatch",
    "template": "Combat/StopCombat.png",
    "roi": [0, 0, 136, 140],
    "action": "Custom",
    "pre_delay": 0,
    "post_delay": 0,
    "next": [],
}


class Action(BaseModel):
    pre_delay: int
    post_delay: int


class Combat(BaseModel):
    role: str
    version: str
    combat: List


def parsing_type(data: Union[str, Dict]) -> Union[str, Dict]:
    if isinstance(data, str):
        return data
    if isinstance(data, Dict):
        for key, value in data.items():
            return {key: Action(pre_delay=value[0], post_delay=value[1])}


def generate_json_from_combat(combat, start_index=0):
    """
    根据给定的 "combat" 列表生成对应的 JSON 结构。

    参数:
        combat_list (list): 包含 "combat" 操作的列表
        start_index (int): 生成的 JSON 对象的起始索引，默认为 0

    返回:
        tuple: 生成的 JSON 对象和下一个可用的索引
    """
    global json_template
    generated_json = {}
    next_index = start_index

    for idx, item in enumerate(combat):
        # 如果 item 是字符串，直接设置 custom_action
        json_template = json_template.copy()
        if isinstance(item, str):
            json_template["custom_action"] = item

        # 如果 item 是字典，设置 custom_action, pre_delay, 和 post_delay
        elif isinstance(item, dict):
            for key, value in item.items():
                json_template["custom_action"] = key
                json_template["pre_delay"] = value[0]
                json_template["post_delay"] = value[1]

        # 设置 next 字段
        next_step = f"UniversalMirageCombatGeneric_{next_index + 1}"
        json_template["next"] = ["UniversalMirageCombatFinish", next_step]

        # 对于第一个元素，特殊处理其名称
        if idx == 0:
            current_step = "UniversalMirageCombatGenericPreheat"
        else:
            current_step = f"UniversalMirageCombatGeneric_{next_index}"

        # 将新生成的 JSON 对象添加到 generated_json 中
        generated_json[current_step] = json_template

        # 更新 next_index
        next_index += 1

    # 更新最后一个对象的 next 字段
    if generated_json:
        last_step = f"UniversalMirageCombatGeneric_{next_index - 1}"
        generated_json[last_step]["next"] = ["UniversalMirageCombatFinish"]

    return generated_json, next_index


def read_file(path: Path) -> str:
    with open(path, "r") as f:
        return f.read()


if __name__ == "__main__":
    print(
        generate_json_from_combat(
            Combat.model_validate_json(read_file(combat_path)).combat
        )
    )
