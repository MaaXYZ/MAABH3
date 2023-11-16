import argparse
import json
import re
import sys
from json import JSONDecodeError
from pathlib import Path
from typing import Dict, List, Union, Optional

from pydantic import BaseModel
from rich.console import Console

console = Console()

default_input_path = Path(__file__).parent.parent.parent / "data" / "input.json"
default_output_path = Path(__file__).parent.parent.parent / "data" / "output.json"
default_pre_delay = 50
default_post_delay = 50
preheat_pre_delay = 1000
preheat_post_delay = 1000

json_template = {
    "recognition": "Custom",
    "custom_recognizer": "Combating",
    "action": "Custom",
    "pre_delay": 0,
    "post_delay": 0,
    "next": [],
}
action_list = [
    {"action": "Forward", "pre_delay": 0, "post_delay": 50},
    {"action": "Backward", "pre_delay": 0, "post_delay": 50},
    {"action": "Left", "pre_delay": 0, "post_delay": 50},
    {"action": "Right", "pre_delay": 0, "post_delay": 50},
    {"action": "LeftForward", "pre_delay": 0, "post_delay": 50},
    {"action": "LeftBackward", "pre_delay": 0, "post_delay": 50},
    {"action": "RightForward", "pre_delay": 0, "post_delay": 50},
    {"action": "RightBackward", "pre_delay": 0, "post_delay": 50},
    {"action": "BasicATK", "pre_delay": 0, "post_delay": 200},
    {"action": "BasicATKCharge", "pre_delay": 0, "post_delay": 100},
    {"action": "Evade", "pre_delay": 0, "post_delay": 150},
    {"action": "Ultimate", "pre_delay": 0, "post_delay": 150},
    {"action": "UltimateCharge", "pre_delay": 0, "post_delay": 100},
    {"action": "WeaponSkill", "pre_delay": 0, "post_delay": 150},
    {"action": "ELFSkill", "pre_delay": 0, "post_delay": 150},
    {"action": "ExtraSkill", "pre_delay": 0, "post_delay": 100},
]


class Action(BaseModel):
    pre_delay: int
    post_delay: int


class Combat(BaseModel):
    mode: str
    role: str
    version: str
    combat: List[Union[str, Dict[str, List[int]]]]


def default_delay(data: str) -> Action:
    """根据动作名称，返回对应的预延迟和后延迟。

    参数:
        data (str): 动作名称

    返回:
        Action: 包含预延迟和后延迟的对象
    """
    for action_dict in action_list:
        if action_dict["action"] == data:
            return Action(
                pre_delay=action_dict["pre_delay"], post_delay=action_dict["post_delay"]
            )

    # 如果没有找到匹配的动作，返回默认值
    return Action(pre_delay=default_pre_delay, post_delay=default_post_delay)


def generate_from_combat(combat: List, mode: str, role: str, template: dict) -> Dict:
    """
    根据 combat 列表生成 JSON 对象。

    参数:
        combat (List): 包含 "combat" 操作的列表。
        mode (str): 模式名称。
        role (str): 角色名称。

    返回:
        dict: 生成的 JSON 对象。
    """

    def _process_string_action(action: str) -> Dict:
        """
        处理字符串类型的动作，并返回一个包含动作信息的字典。

        参数:
            action (str): 动作名称。

        返回:
            dict: 包含动作名称和延迟信息的字典。
        """
        _data = template.copy()
        _data["custom_action"] = action
        delay = default_delay(action)
        _data["pre_delay"] = delay.pre_delay
        _data["post_delay"] = delay.post_delay
        return _data

    def _process_dict_action(action: Dict) -> Dict:
        """
        处理字典类型的动作，并返回一个包含动作信息的字典。

        参数:
            action (Dict): 包含动作名称和延迟信息的字典。

        返回:
            dict: 包含动作名称和延迟信息的字典。
        """
        _data = template.copy()
        for key, value in action.items():
            _data["custom_action"] = key
            if isinstance(value, list) and len(value) == 2:
                _data["pre_delay"] = value[0]
                _data["post_delay"] = value[1]
            else:
                delay = default_delay(key)
                _data["pre_delay"] = delay.pre_delay
                _data["post_delay"] = delay.post_delay
        return _data

    if template is None:
        template = json_template

    generated_json = {}
    next_index = 0

    for idx, item in enumerate(combat):
        data = None  # 初始化 data 变量
        if isinstance(item, str):
            data = _process_string_action(item)
        elif isinstance(item, dict):
            data = _process_dict_action(item)
        else:
            console.print(f"[bold red]未知的 item 类型: {type(item)}[/bold red]")
            continue

        next_step = f"{mode}Combat{role}_{str(next_index + 1).zfill(3)}"
        data["next"] = [f"{mode}Combat{role}Finish", next_step]

        if idx == 0:
            current_step = f"{mode}Combat{role}Preheat"
            data["pre_delay"] = preheat_pre_delay
            data["post_delay"] = preheat_post_delay
        else:
            current_step = f"{mode}Combat{role}_{str(next_index).zfill(3)}"

        generated_json[current_step] = data

        next_index += 1

    try:
        if generated_json:
            last_step = f"{mode}Combat{role}_{str(next_index - 1).zfill(3)}"
            generated_json[last_step]["next"] = [f"{mode}CombatFinish"]
    except KeyError:
        console.print_exception()
        sys.exit(1)

    return generated_json


def reverse_to_combat(json_data: Dict) -> Combat:
    """
    将 JSON 对象解析为 Combat 对象。

    参数:
        json_data (Dict): 输入的 JSON 对象。

    返回:
        Combat: 解析后的 Combat 对象。
    """
    combat_list = []

    sorted_keys = sorted(json_data.keys())

    first_key = sorted_keys[0]
    match = re.search(r"(.+)Combat(.+)Preheat", first_key)
    if match is None:
        console.print(f"无法解析 {first_key}，请检查输入的 JSON 文件是否正确")
        sys.exit(1)
    mode, role = match.groups()

    for key in sorted_keys:
        action_data = json_data[key]
        custom_action = action_data["custom_action"]
        pre_delay = action_data["pre_delay"]
        post_delay = action_data["post_delay"]

        default_action = default_delay(custom_action)

        if (
            pre_delay == default_action.pre_delay
            and post_delay == default_action.post_delay
        ):
            combat_list.append(custom_action)
        elif pre_delay == preheat_pre_delay and post_delay == preheat_post_delay:
            combat_list.append(custom_action)
        else:
            combat_list.append({custom_action: [pre_delay, post_delay]})

    return Combat(mode=mode, role=role, version="debug", combat=combat_list)


def load_file(path: Optional[Path]) -> Dict:
    """读取指定路径的文件内容。

    参数:
        path (Path): 文件路径。

    返回:
        Optional[Dict]: 文件内容，如果发生异常则返回 None。

    异常:
        如果文件不存在、无权限或其他I/O错误，函数将返回 None。
    """

    def read_file(file_path: Path) -> Dict:
        """尝试读取指定的文件，返回文件内容或 None（如果发生错误）"""
        try:
            with file_path.open("r") as f:
                return json.load(f)
        except (FileNotFoundError, PermissionError, IOError, JSONDecodeError) as error:
            console.print_exception()
            sys.exit(1)

    if isinstance(path, (str, Path)):
        path = Path(path)
        return read_file(path)
    else:
        console.print("传入路径不是一个有效的字符串或 Path 对象")
        sys.exit(1)


def save_file(path: Union[Path, str], content) -> bool:
    """将内容保存到指定路径的文件中。

    参数:
        path (Path): 文件路径。
        content: 要保存的内容。

    返回:
        bool: 如果保存成功返回 True，否则返回 False。

    异常:
        如果没有写权限或其他I/O错误，函数将返回 False。
    """

    def write_file(file_path: Path) -> bool:
        """尝试将内容写入指定的文件，返回操作是否成功"""
        try:
            with file_path.open("w") as f:
                json.dump(content, f)
                console.print(f"保存文件成功：{file_path}")
            return True
        except (PermissionError, IOError, FileNotFoundError):
            console.print_exception()
            return False

    if isinstance(path, (str, Path)):
        path = Path(path)
        if write_file(path):
            return True
        console.print(f"无法保存到 {path}，尝试保存到默认路径 {default_output_path}", style="red")
        return write_file(default_output_path)
    else:
        console.print(f"传入路径{path}不是一个有效的字符串，尝试保存到默认路径 {default_output_path}")
        return write_file(default_output_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="反序列化生成的Combat文件")
    parser.add_argument("-r", "--reverse", action="store_true", help="反序列器")
    parser.add_argument(
        "-i",
        "--input",
        type=str,
        help="传入文件路径(绝对路径) e.g. F:\\sth.json",
        default=default_input_path,
    )

    parser.add_argument(
        "-o",
        "--output",
        type=str,
        help="保存文件路径(绝对路径) e.g. F:\\sth.json",
        default=default_output_path,
    )
    args = parser.parse_args()
    if args.reverse:
        output_model = reverse_to_combat(load_file(args.input))
        console.print(f"角色名{output_model.role}, 版本号{output_model.version}")
        save_file(
            args.output,
            output_model.model_dump(),
        )
    else:
        file = load_file(args.input)
        input_model = Combat.model_validate(file)
        console.print(f"角色名{input_model.role}, 版本号{input_model.version}")
        save_file(
            args.output,
            generate_from_combat(
                input_model.combat,
                input_model.mode,
                input_model.role,
                json_template,
            ),
        )
