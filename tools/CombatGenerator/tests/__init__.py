import unittest
from pathlib import Path

from combatgenerator import (
    default_delay,
    generate_json_from_combat,
    read_file,
    Combat,
)


class TestCombatMethods(unittest.TestCase):
    def test_default_delay(self):
        """测试 default_delay 函数是否返回正确的预延迟和后延迟。"""
        test_cases = [
            {"action": "Forward", "pre_delay": 0, "post_delay": 50},
            {"action": "Backward", "pre_delay": 0, "post_delay": 50},
            {"action": "Left", "pre_delay": 0, "post_delay": 50},
            {"action": "Right", "pre_delay": 0, "post_delay": 50},
            {"action": "Left Forward", "pre_delay": 0, "post_delay": 50},
            {"action": "Left Backward", "pre_delay": 0, "post_delay": 50},
            {"action": "Right Forward", "pre_delay": 0, "post_delay": 50},
            {"action": "Right Backward", "pre_delay": 0, "post_delay": 50},
            {"action": "BasicATK", "pre_delay": 0, "post_delay": 200},
            {"action": "BasicATKCharge", "pre_delay": 0, "post_delay": 100},
            {"action": "Evade", "pre_delay": 0, "post_delay": 150},
            {"action": "Ultimate", "pre_delay": 0, "post_delay": 150},
            {"action": "UltimateCharge", "pre_delay": 0, "post_delay": 100},
            {"action": "WeaponSkill", "pre_delay": 0, "post_delay": 150},
            {"action": "ELFSkill", "pre_delay": 0, "post_delay": 150},
            {"action": "ExtraSkill", "pre_delay": 0, "post_delay": 100},
        ]

        for test_case in test_cases:
            action = default_delay(str(test_case["action"]))
            self.assertEqual(action.pre_delay, test_case["pre_delay"])
            self.assertEqual(action.post_delay, test_case["post_delay"])

    def test_generate_json_from_combat(self):
        """测试 generate_json_from_combat 函数是否返回正确的 JSON 结构。"""
        combat_list = [
            "Forward",
            "Backward",
            {"BasicATK": [0, 30]},
            {"ExtraSkill": [30, 0]},
        ]
        combat_path = Path(__file__).parent / "data" / "test.json"
        data = Combat.model_validate_json(read_file(combat_path))
        generated_json = generate_json_from_combat(data.combat, data.mode, data.role)
        self.assertIsNotNone(generated_json)

        # 检查 "UniversalMirageCombatGenericPreheat" 步骤
        self.assertIn(f"{data.mode}Combat{data.role}Preheat", generated_json)
        self.assertEqual(
            generated_json[f"{data.mode}Combat{data.role}Preheat"]["pre_delay"], 500
        )
        self.assertEqual(
            generated_json[f"{data.mode}Combat{data.role}Preheat"]["post_delay"], 1500
        )

        # 检查 "next" 字段和下一个步骤的键是否匹配
        keys = list(generated_json.keys())
        for i in range(len(keys) - 1):  # 跳过最后一个元素
            current_key = keys[i]
            next_key = keys[i + 1]
            self.assertIn(next_key, generated_json[current_key]["next"])

        # 检查最后一个步骤的 "next" 字段应该只包含 "UniversalMirageCombatFinish"
        self.assertEqual(
            generated_json[keys[-1]]["next"], [f"{data.mode}Combat{data.role}Finish"]
        )


if __name__ == "__main__":
    unittest.main()
