#include "AdbConfigCache.h"

bool AdbConfigCache::has(const std::string& device_name, const std::string& device_SN)
{
    std::string cache_key = generate_cache_key(device_name, device_SN);
    std::filesystem::path file_path = adb_config_cache_dir / cache_key;
    if (!std::filesystem::exists(file_path)) {
        return false;
    }
    return true;
}

bool AdbConfigCache::set_adb_config(const std::string& device_name, const std::string& device_SN,
                                    const std::string& adb_config)
{
    std::string cache_key = generate_cache_key(device_name, device_SN);
    if (!save(cache_key, adb_config)) {
        return false;
    }
    return true;
}

std::string AdbConfigCache::get_adb_config(const std::string& device_name, const std::string& device_SN)
{
    std::string cache_key = generate_cache_key(device_name, device_SN);
    std::string adb_config;
    if (!load(cache_key, adb_config)) {
        return std::string();
    }
    return adb_config;
}

std::string AdbConfigCache::get_default_adb_config()
{
    std::filesystem::path file_path = local_dir / "resource" / "adb_config.json";
    std::ifstream ifs(file_path, std::ios::in);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open default adb config file: " << file_path << std::endl;
        return std::string();
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    return buffer.str();
}

std::string AdbConfigCache::generate_cache_key(const std::string& device_name, const std::string& device_SN)
{
    std::string cache_key = device_name + "_" + device_SN;
    size_t hash = std::hash<std::string> {}(cache_key);
    cache_key = std::to_string(hash);
    return cache_key;
}

bool AdbConfigCache::save(const std::string& cache_key, const std::string& adb_config)
{
    std::filesystem::create_directories(adb_config_cache_dir);
    std::filesystem::path file_path = adb_config_cache_dir / cache_key;
    std::ofstream ofs(file_path, std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open cache file: " << file_path << std::endl;
        return false;
    }
    ofs << adb_config;
    ofs.close();

    // Update cache map
    cache_map[cache_key] = adb_config;
    return true;
}

bool AdbConfigCache::load(const std::string& cache_key, std::string& adb_config)
{
    if (cache_map.count(cache_key) > 0) {
        adb_config = cache_map[cache_key];
        return true;
    }

    std::filesystem::path file_path = adb_config_cache_dir / cache_key;
    std::ifstream ifs(file_path, std::ios::in);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open cache file: " << file_path << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    adb_config = buffer.str();
    ifs.close();

    // Update cache map
    cache_map[cache_key] = adb_config;
    return true;
}