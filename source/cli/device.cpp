#include "device.h"

bool default_device_init(DeviceConfig& device)
{
    std::string name, SN, adb;
    if (!select_device(name, SN, adb)) {
        return false;
    }
    device.set_config_device_name(name);
    device.set_config_device_SN(SN);
    device.set_config_adb(adb);
    return true;
}

MaaSize scanning_devices()
{
    std::cout << "Scanning for Devices..." << std::endl;
    auto device_size = MaaToolkitPostFindDevice();
    if (device_size == 0) {
        std::cout << "No Devices Found" << std::endl;
        return 0;
    }
    std::cout << "Scanning Finished" << std::endl;
    return device_size;
}

bool select_device(std::string& name, std::string& SN, std::string& adb)
{
    auto device_size = scanning_devices();
    if (!device_size) {
        if (!manually_enter_device_info(name, SN, adb)) {
            return false;
        }
        return true;
    }
    MaaSize number = 0;
    if (!select_device_number(device_size, number)) {
        return false;
    }
    if (!number) {
        if (!manually_enter_device_info(name, SN, adb)) {
            return false;
        }
        return true;
    }
    MaaSize device_index = number - 1;
    name = MaaToolkitGetDeviceName(device_index);
    SN = MaaToolkitGetDeviceAdbSerial(device_index);
    adb = MaaToolkitGetDeviceAdbPath(device_index);
    AdbConfigCache adb_config_cache;
    adb_config_cache.set_adb_config(name, SN, MaaToolkitGetDeviceAdbConfig(device_index));
    return true;
}

bool select_device_number(const MaaSize& device_size, MaaSize& number)
{
    std::cout << std::endl << "Please select a device:" << std::endl << std::endl;
    std::cout << "  0. Manual" << std::endl;
    print_device_list(device_size);
    std::cout << std::endl << "Please enter option number:" << std::endl;

    std::cin >> number;
    std::cin.ignore();

    if (number > device_size) {
        std::cout << std::endl << "Unknown option number: " << number << std::endl;
        return false;
    }

    return true;
}

void print_device_list(const MaaSize& device_size)
{
    for (MaaSize i = 0; i < device_size; i++) {
        std::cout << "  " << i + 1 << ". " << MaaToolkitGetDeviceName(i) << " (" << MaaToolkitGetDeviceAdbSerial(i)
                  << ")\n";
    }
}

bool manually_enter_device_info(std::string& name, std::string& SN, std::string& adb)
{
    if (!manually_enter_device_name(name)) {
        return false;
    }
    if (!manually_enter_device_SN(SN)) {
        return false;
    }
    if (!manually_enter_device_adb(adb)) {
        return false;
    }
    return true;
}

bool manually_enter_device_name(std::string& name)
{
    std::cout << std::endl << "Please enter device name: " << std::endl;
    std::getline(std::cin, name);
    if (name.empty()) {
        return false;
    }
    return true;
}

bool manually_enter_device_SN(std::string& SN)
{
    std::cout << std::endl << "Please enter device serial number: " << std::endl;
    std::getline(std::cin, SN);
    if (SN.empty()) {
        return false;
    }
    return true;
}

bool manually_enter_device_adb(std::string& adb)
{
    std::cout << std::endl << "Please enter adb path: " << std::endl;
    std::getline(std::cin, adb);
    if (adb.empty()) {
        return false;
    }
    return true;
}