import unittest
import json
from pathlib import Path
from combatgenerator import (
    Combat,
    generate_json_from_combat,
    read_file,
)  # 请替换 "your_module" 为你的实际模块名


class TestCombat(unittest.TestCase):
    def setUp(self):
        self.test_path = Path(__file__).parent.parent / "data" / "input.json"
        with open(self.test_path, "r") as f:
            self.data = json.load(f)
        self.combat_instance = Combat(**self.data)

    def test_generate_json_from_combat(self):
        generated_json, _ = generate_json_from_combat(self.combat_instance.combat)

        # 进行你需要的断言检查
        self.assertTrue(generated_json)  # 检查生成的 JSON 是否存在
        self.assertIn(
            "UniversalMirageCombatGenericPreheat", generated_json
        )  # 检查特定 key 是否存在
        # ... 添加更多断言

    def test_read_file(self):
        file_content = read_file(self.test_path)

        # 进行你需要的断言检查
        self.assertTrue(file_content)


if __name__ == "__main__":
    unittest.main()
