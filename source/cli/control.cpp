#include "control.h"

bool default_control_init(ControlConfig& control)
{
    int server = 0;
    if (!select_server(server)) {
        return false;
    }
    control.set_config_server(server);
    return true;
}

bool select_server(int& server)
{
    std::cout << std::endl
              << "Please select server number: " << std::endl
              << std::endl
              << "  1. Official(CN)\n"
                 "  2. Bilibili\n"
                 "  3. Vivo\n"
              << std::endl
              << "Please enter the server number: " << std::endl;

    std::cin >> server;

    if (server < 1 || server > 3) {
        std::cout << "Unknown server number: " << server << std::endl;
        return false;
    }

    return true;
}

bool app_package_and_activity(int server_type, std::string& package, std::string& activity)
{
    switch (server_type) {
    case 1:
        // "1. Official(CN)\n"
        package = "com.miHoYo.enterprise.NGHSoD";
        activity = "com.miHoYo.enterprise.NGHSoD/com.miHoYo.overridenativeactivity.OverrideNativeActivity";
        break;
    case 2:
        // "2. Bilibili\n"
        package = "com.miHoYo.bh3.bilibili";
        activity = "com.miHoYo.bh3.bilibili/com.miHoYo.overridenativeactivity.OverrideNativeActivity";
        break;
    case 3:
        // "2. Vivo\n"
        package = "com.miHoYo.bh3.vivo";
        activity = "com.miHoYo.bh3.vivo/com.miHoYo.overridenativeactivity.OverrideNativeActivity";
        break;
    default:
        return false;
    }

    return true;
}