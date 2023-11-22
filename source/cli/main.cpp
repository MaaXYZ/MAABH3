#include "main.h"

int main(int argc, char** argv)
{
    std::unordered_map<std::string, std::string> args;
    if (!parse_args(argc, argv, args)) {
        return 0;
    }

    MaaToolKitInit();

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

    auto arg_device = args.find("device");
    if (arg_device != args.end()) {
        config.set_config_target(ConfigOption_Device, arg_device->second);
    }
    if (!device.exists()) {
        if (arg_device != args.end()) {
            std::cerr << "device config: " << arg_device->second << std::endl;
            return 1;
        }

        if (!default_device_init(device)) {
            mpause();
            return 1;
        }
        device.dump();
    }

    auto arg_control = args.find("control");
    if (arg_control != args.end()) {
        config.set_config_target(ConfigOption_Control, arg_control->second);
    }
    if (!control.exists()) {
        if (arg_control != args.end()) {
            std::cerr << "control config: " << arg_control->second << std::endl;
            return 1;
        }

        if (!default_control_init(control)) {
            mpause();
            return 1;
        }
        control.dump();
    }

    auto arg_tasks = args.find("tasks");
    if (arg_tasks != args.end()) {
        config.set_config_target(ConfigOption_Tasks, arg_tasks->second);
    }
    if (!tasks.exists()) {
        if (arg_tasks != args.end()) {
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

    AdbConfigCache adb_config_cache;
    if (!adb_config_cache.has(device.get_config_device_name(), device.get_config_device_SN())) {
        adb_config = adb_config_cache.get_default_adb_config();
    }
    else {
        adb_config = adb_config_cache.get_adb_config(device.get_config_device_name(), device.get_config_device_SN());
    }

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
        MaaToolKitUninit();
    };

    if (!MaaInited(maa_handle)) {
        destroy();
        std::cout << "Failed to init Maa instance, a connection error or resource file corruption occurred, please "
                     "refer to the log."
                  << std::endl;
        mpause();
        return -1;
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

bool parse_args(int argc, char** argv, std::unordered_map<std::string, std::string>& args)
{
    std::unordered_set<std::string> allowed_args = { "control", "device", "tasks" };

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

        if (allowed_args.count(arg_name) == 0) {
            std::cerr << "Unknown argument: " << arg << std::endl;

        }

        if (i + 1 >= argc || argv[i + 1][0] == '-') {
            std::cerr << "Missing argument value: " << arg << std::endl;
            return false;
        }

        std::string arg_value = argv[i + 1];
        args[arg_name] = arg_value;
    }

    return true;
}

void print_help()
{
    std::cout <<
        R"(MAABH3 CLI
Github: https://github.com/MaaAssistantArknights/MAABH3

Usage: MAABH3_CLI.exe

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

MaaSize scanning_devices()
{
    std::cout << "Scanning for Devices..." << std::endl;
    auto device_size = MaaToolKitFindDevice();
    if (device_size == 0) {
        std::cout << "No Devices Found" << std::endl;
        return 0;
    }
    std::cout << "Scanning Finished" << std::endl;
    return device_size;
}

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
    name = MaaToolKitGetDeviceName(device_index);
    SN = MaaToolKitGetDeviceAdbSerial(device_index);
    adb = MaaToolKitGetDeviceAdbPath(device_index);
    AdbConfigCache adb_config_cache;
    if (!adb_config_cache.has(name, SN)) {
        adb_config_cache.set_adb_config(name, SN, MaaToolKitGetDeviceAdbConfig(device_index));
    }
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
        std::cout << "  " << i + 1 << ". " << MaaToolKitGetDeviceName(i) << " (" << MaaToolKitGetDeviceAdbSerial(i)
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

bool default_tasks_init(TasksConfig& tasks)
{
    std::vector<Task> tasklist;
    if (!select_tasks(tasklist)) {
        return false;
    }
    tasks.set_config_tasklist(tasklist);
    return true;
}

bool select_tasks(std::vector<Task>& tasklist)
{
    std::cout << std::endl
              << "Please select tasks: " << std::endl
              << std::endl
              << "  1. Dorm\n"
                 "  2. MaterialEvent\n"
                 "  3. Armada\n"
                 "  4. Shop\n"
                 "  5. Awards\n"
                 "  6. UniversalMirage\n"
              << std::endl
              << "Please enter the task numbers to be executed: " << std::endl;
    std::vector<int> task_ids;
    std::string line;

    std::cin.ignore();
    std::getline(std::cin, line);

    std::istringstream iss(line);
    int task_id;
    while (iss >> task_id) {
        task_ids.emplace_back(task_id);
    }

    int index = 1;
    for (auto id : task_ids) {
        Task task_obj;
        task_obj.name = "MyTask" + std::to_string(index++);

        switch (id) {
        case 1:
            task_obj.type = "Dorm";
            task_obj.param = dorm_param();
            break;
        case 2:
            task_obj.type = "MaterialEvent";
            break;
        case 3:
            task_obj.type = "Armada";
            break;
        case 4:
            task_obj.type = "Shop";
            break;
        case 5:
            task_obj.type = "Awards";
            break;
        case 6:
            task_obj.type = "UniversalMirage";
            task_obj.param = universal_mirage_param();
            break;

        default:
            std::cout << "Unknown task: " << id << std::endl;
            return false;
        }
        tasklist.emplace_back(std::move(task_obj));
    }
    Task close_game_task;
    close_game_task.name = "CloseGame";
    close_game_task.status = false;
    close_game_task.param = close_game_param();
    close_game_task.type = "CloseBH3";

    tasklist.emplace_back(std::move(close_game_task));

    return true;
}

bool app_package_and_activity(int client_type, std::string& package, std::string& activity)
{
    switch (client_type) {
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

bool match_adb_address(const std::string& adb_address, MaaSize& index, const MaaSize& device_size)
{
    for (MaaSize i = 0; i < device_size; i++) {
        if (adb_address == MaaToolKitGetDeviceAdbSerial(i)) {
            index = i;
            return true;
        }
    }
    return false;
}

json::value dorm_param()
{
    json::value param;
    auto& diff = param["diff_task"];
    auto& doc = diff["Sub_SwitchToFragmentPage"]["doc"];
    auto& enabled = diff["Sub_SwitchToFragmentPage"]["enabled"];

    doc = "切换刷最近碎片；默认false，刷最近材料";
    enabled = false;

    return param;
}

json::value universal_mirage_param()
{
    json::value param;
    auto& diff = param["diff_task"];
    auto& costing_time_swirl_passes_doc = diff["UniversalMirageCostingTimeSwirlPasses"]["doc"];
    auto& costing_time_swirl_passes_enabled = diff["UniversalMirageCostingTimeSwirlPasses"]["enabled"];
    auto& glacial_fences_doc = diff["Sub_EnterGlacialFencesPage"]["doc"];
    auto& glacial_fences_enabled = diff["Sub_EnterGlacialFencesPage"]["enabled"];
    auto& illuminated_land_doc = diff["Sub_EnterIlluminatedLandPage"]["doc"];
    auto& illuminated_land_enabled = diff["Sub_EnterIlluminatedLandPage"]["enabled"];
    auto& roaring_palace_doc = diff["Sub_EnterRoaringPalacePage"]["doc"];
    auto& roaring_palace_enabled = diff["Sub_EnterRoaringPalacePage"]["enabled"];
    auto& blade_grave_doc = diff["Sub_EnterBladeGravePage"]["doc"];
    auto& blade_grave_enabled = diff["Sub_EnterBladeGravePage"]["enabled"];
    auto& stage_f2_doc = diff["UniversalMirageStageF2"]["doc"];
    auto& stage_f2_enabled = diff["UniversalMirageStageF2"]["enabled"];

    costing_time_swirl_passes_doc = "花费时序通行证；默认false";
    costing_time_swirl_passes_enabled = false;
    glacial_fences_doc = "幽寒之槛";
    glacial_fences_enabled = true;
    illuminated_land_doc = "煌然之地";
    illuminated_land_enabled = false;
    roaring_palace_doc = "轰鸣之殿";
    roaring_palace_enabled = false;
    blade_grave_doc = "千刃之冢";
    blade_grave_enabled = false;
    stage_f2_doc = "第二层";
    stage_f2_enabled = true;

    return param;
}

json::value close_game_param()
{
    json::value param;
    auto& diff = param["diff_task"];
    auto& close_bh3 = diff["CloseBH3"]["enabled"];
    auto& close_bh3_doc = diff["CloseBH3"]["doc"];

    close_bh3 = false;
    close_bh3_doc = "关闭崩坏3；默认false";

    return param;
}

std::string TaskStatus(MaaStatus status)
{
    std::string task_status;
    switch (status) {
    case MaaStatus_Invalid:
        task_status = "Invalid";
        break;
    case MaaStatus_Pending:
        task_status = "Pending";
        break;
    case MaaStatus_Running:
        task_status = "Running";
        break;
    case MaaStatus_Success:
        task_status = "Success";
        break;
    case MaaStatus_Failed:
        task_status = "Failed";
        break;
    default:
        task_status = "Unknown";
    }
    return task_status;
}

void mpause()
{
    std::ignore = getchar();
}