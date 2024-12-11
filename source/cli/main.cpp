#include "main.h"

int main(int argc, char** argv)
{
    std::unordered_map<std::string, std::string> options;
    std::unordered_map<std::string, bool> flags;
    if (!parse_param(argc, argv, options, flags)) {
        return 1;
    }

    if (flags["version"]) {
        print_version();
        return 0;
    }

    if (flags["help"]) {
        print_help();
        return 0;
    }

    MaaToolkitInitOptionConfig("./", "{}");

    print_help();
    print_version();

    ConfigMgr& config = ConfigMgr::get_instance();
    ControlConfig& control = config.get_control_config();
    DeviceConfig& device = config.get_device_config();
    TasksConfig& tasks = config.get_tasks_config();

    std::string package = "com.miHoYo.enterprise.NGHSoD";
    std::string activity = "com.miHoYo.enterprise.NGHSoD/com.miHoYo.overridenativeactivity.OverrideNativeActivity";
    MaaAdbControllerType ctrl_type = 0;
    std::string adb_config;

    auto arg_device = options.find("device");
    if (arg_device != options.end()) {
        config.set_config_target(ConfigOption_Device, arg_device->second);
    }
    if (!device.exists()) {
        if (arg_device != options.end()) {
            std::cerr << "device config: " << arg_device->second << std::endl;
            return 1;
        }

        if (!default_device_init(device)) {
            mpause();
            return 1;
        }
        device.dump();
    }

    auto arg_control = options.find("control");
    if (arg_control != options.end()) {
        config.set_config_target(ConfigOption_Control, arg_control->second);
    }
    if (!control.exists()) {
        if (arg_control != options.end()) {
            std::cerr << "control config: " << arg_control->second << std::endl;
            return 1;
        }

        if (!default_control_init(control)) {
            mpause();
            return 1;
        }
        control.dump();
    }

    auto arg_tasks = options.find("tasks");
    if (arg_tasks != options.end()) {
        config.set_config_target(ConfigOption_Tasks, arg_tasks->second);
    }
    if (!tasks.exists()) {
        if (arg_tasks != options.end()) {
            std::cerr << "tasks config: " << arg_tasks->second << std::endl;
            return 1;
        }

        if (!default_tasks_init(tasks)) {
            mpause();
            return 1;
        }
        tasks.dump();
    }

    config.init();

    adb_config = get_default_adb_config();

    bool identified = app_package_and_activity(control.get_config_server(), package, activity);
    if (!identified) {
        std::cerr << "Failed to identify the server type" << std::endl;
        mpause();
        return 1;
    }

    std::vector<Task> tasklist = tasks.get_config_tasklist();

    if (tasklist.empty()) {
        std::cout << "Tasklist Empty" << std::endl;
        return 0;
    }

    const auto cur_dir = std::filesystem::path(argv[0]).parent_path();
    std::string resource_dir = (cur_dir / "resource").string();
    std::string agent_dir = (cur_dir / "MaaAgentBinary").string();

    int key = 1;
    ctrl_type = control.get_config_touch() << 0 | key << 8 | control.get_config_screencap() << 16;

    auto maa_handle = MaaCreate(nullptr, nullptr);
    auto resource_handle = MaaResourceCreate(nullptr, nullptr);
    auto controller_handle =
        MaaAdbControllerCreateV2(device.get_config_adb().c_str(), device.get_config_device_SN().c_str(), ctrl_type,
                                 adb_config.c_str(), agent_dir.c_str(), nullptr, nullptr);

    MaaBindResource(maa_handle, resource_handle);
    MaaBindController(maa_handle, controller_handle);
    int height = 720;
    MaaControllerSetOption(controller_handle, MaaCtrlOption_ScreenshotTargetShortSide, reinterpret_cast<void*>(&height),
                           sizeof(int));
    MaaControllerSetOption(controller_handle, MaaCtrlOption_DefaultAppPackageEntry, (void*)activity.c_str(),
                           activity.size());
    MaaControllerSetOption(controller_handle, MaaCtrlOption_DefaultAppPackage, (void*)package.c_str(), package.size());

    auto resource_id = MaaResourcePostPath(resource_handle, resource_dir.c_str());
    auto connection_id = MaaControllerPostConnection(controller_handle);

    MaaResourceWait(resource_handle, resource_id);
    MaaControllerWait(controller_handle, connection_id);

    auto destroy = [&]() {
        MaaDestroy(maa_handle);
        MaaResourceDestroy(resource_handle);
        MaaControllerDestroy(controller_handle);
    };

    if (!MaaInited(maa_handle)) {
        destroy();
        std::cout << "Failed to init Maa instance, a connection error or resource file corruption occurred, please "
                     "refer to the log."
                  << std::endl;
        mpause();
        return 1;
    }

    CustomActionRegistrar action_registerar;
    CustomRecognizerRegistrar recognizer_registerar;
    register_custom_action(maa_handle, action_registerar);
    register_custom_recognizer(maa_handle, recognizer_registerar);

    MaaTaskId task_id = 0;
    for (const auto& task : tasklist) {
        if (!task.status) {
            continue;
        }
        std::cout << task.type << " Start" << std::endl;
        task_id = MaaPostTask(maa_handle, task.type.c_str(), task.param.to_string().c_str());
        std::cout << task.type << " Running..." << std::endl;
        auto end_status = MaaWaitTask(maa_handle, task_id);
        std::cout << task.type << " End" << std::endl
                  << task.type << " Result: " << TaskStatus(end_status) << std::endl;
    }

    std::cout << std::endl << "All Tasks Over" << std::endl;

    destroy();

    return 0;
}

bool parse_param(int argc, char** argv, std::unordered_map<std::string, std::string>& options,
                 std::unordered_map<std::string, bool>& flags)
{
    std::unordered_set<std::string> allowed_param = { "control", "device", "tasks", "init", "help", "version" };
    std::unordered_set<std::string> flags_key = { "init", "help", "version" };
    for (auto& key : flags_key) {
        flags[key] = false;
    }

    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];

        if (arg.size() < 2 || arg.substr(0, 2) != "--") {
            std::cerr << "Invalid argument format: " << arg << std::endl;
            return false;
        }

        std::string arg_name = arg.substr(2);

        if (arg_name.empty()) {
            std::cerr << "Invalid argument name: " << arg << std::endl;
            return false;
        }

        if (allowed_param.count(arg_name) == 0) {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return false;
        }

        if (flags_key.count(arg_name)) {
            flags[arg_name] = true;
            i--;
            continue;
        }

        if (i + 1 >= argc || argv[i + 1][0] == '-') {
            std::cerr << "Missing argument value: " << arg << std::endl;
            return false;
        }

        std::string arg_value = argv[i + 1];
        options[arg_name] = arg_value;
    }

    return true;
}

void print_help()
{
    std::cout <<
        R"(MAABH3 CLI
Github: https://github.com/MaaXYZ/MAABH3

Usage: MAABH3_CLI [param]

Options:
  --control [config]    Specify the control config
  --device [config]     Specify the device config
  --tasks [config]      Specify the tasks config

Flags:
  --help                Display this help message
  --version             Display this version message

Modify ./config/tasks/default.json to configure tasks.

Welcome to come and create a GUI for us! :)
)" << std::endl;
}

void print_version()
{
    std::cout << "MaaFramework Version: " << MaaVersion() << std::endl
              << "MAABH3 Version: " << MAABH3_VERSION << std::endl
              << std::endl;
}

std::string get_default_adb_config()
{
    const std::filesystem::path local_dir = ".";
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

void mpause()
{
    std::ignore = getchar();
}
