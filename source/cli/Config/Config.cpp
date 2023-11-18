#include "Config.h"

bool Config::dump()
{
    return save(to_json());
}

void Config::set_target(std::string target)
{
    _target = target;
    if (!_target.ends_with(".json")) {
        _target += ".json";
    }
    build_target_path();
}