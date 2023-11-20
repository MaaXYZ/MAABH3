#pragma once

#include <filesystem>
#include <string>

class Cache
{
public:
    virtual ~Cache() = default;

public:
    inline static const std::filesystem::path local_dir = ".";
    inline static const std::filesystem::path cache_dir = local_dir / "cache";
};