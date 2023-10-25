import json


def calculate_PrePost_delay(name):
    match name.lower():
        case "forward" | "backward" | "left" | "right":
            return (0, 50)
        case "basicatk":
            return (0, 200)
        case "basicatkcharge" | "ultimatecharge" | "extraskill":
            return (0, 100)
        case "evade" | "weaponskill" | "ultimate" | "elfskill":
            return (0, 150)
        case _:
            # raise IOError  # for test check
            return (None, None)

def json_to_list(json_obj: dict):
    return [oper.get('custom_action', None) for oper in json_obj.values()]
    
def list_to_json(oper_names: list):
    total_line = {'sample': {'next':[None]}}
    last_oper = 'sample'
    for idx, name in enumerate(oper_names, start=1):
        sk_id = f"{name}_{idx:0>3}"
        total_line[last_oper]['next'].append(sk_id)
        last_oper = sk_id
        pre_delay, post_delay = calculate_PrePost_delay(name)
        
        total_line.update({
                sk_id: {
                    "recognition": "TemplateMatch",
                    "template": "Combat/StopCombat.png",
                    "roi": [
                        0,
                        0,
                        136,
                        140
                    ],
                    "action": "Custom",
                    "custom_action": name,
                    "pre_delay": pre_delay,
                    "post_delay": post_delay,
                    "next": [
                        "UniversalMirageCombatFinish"
                    ]
                }
            }
        )
            
    del total_line['sample']
    return total_line
            
def load_jsonfile(json_file: str, 
        mode: str = 'r', 
        json_obj: dict = None,
        encoding: str = 'utf-8',
        indent: int = 4):
    mode = mode.lower()
    
    with open(json_file, mode, encoding) as jf:
        match mode:
            case 'r':
                return json.load(jf)
            case 'w':
                if json_obj is None:
                    raise IOError("json obj can't be None while writing")
                json.dump(json_obj, indent)
            case _:
                raise ValueError(f'unkown open mode:{mode}')
    
if __name__ == '__main__':
    jo = {
        "XXX_001": {
             "recognition": "TemplateMatch",
            "template": "Combat/StopCombat.png",
            "roi": [
                0,
                0,
                136,
                140
            ],
            "action": "Custom",
            "custom_action": "Forward",
            "pre_delay": 0,
            "post_delay": 50,
            "next": [
                "UniversalMirageCombatFinish",
                "UniversalMirageCombatGeneric_002"
            ]
        },
        "XXX_002": {
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
            "pre_delay": 0,
            "post_delay": 200,
            "next": [
                "UniversalMirageCombatFinish",
                "UniversalMirageCombatGeneric_003"
            ]
        },
        "XXX_003": {
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
            "pre_delay": 0,
            "post_delay": 200,
            "next": [
                "UniversalMirageCombatFinish",
                "UniversalMirageCombatGeneric_004"
            ]
        },
        "XXX_004": {
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
            "pre_delay": 0,
            "post_delay": 200,
            "next": [
                "UniversalMirageCombatFinish",
                "UniversalMirageCombatGeneric_005"
            ]
        },
        "XXX_005": {
             "recognition": "TemplateMatch",
            "template": "Combat/StopCombat.png",
            "roi": [
                0,
                0,
                136,
                140
            ],
            "action": "Custom",
            "custom_action": "Ultimate",
            "pre_delay": 0,
            "post_delay": 150,
            "next": [
                "UniversalMirageCombatFinish"
            ]
        }
    }
    
    j = json_to_list(jo)
    print(j)
    print(list_to_json(j))
