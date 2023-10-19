#include "main.h"

int main(int argc, char** argv)
{
    MaaToolKitInit();

	print_help();
    print_version();

    bool debug = false;
	std::string adb;
	std::string adb_address;
    int client_type = 1;
    std::string package = "com.miHoYo.enterprise.NGHSoD";
    std::string activity = "com.miHoYo.enterprise.NGHSoD/com.miHoYo.overridenativeactivity.OverrideNativeActivity";
	TaskList tasks;
    AfterTask after_task;
	MaaAdbControllerType control_type = 0;

    auto device_size = scanning_devices();
    if (device_size == 0) {
        mpause();
        return -1;
    }
    bool proced = proc_argv(argc, argv, debug, adb, adb_address, client_type, tasks, after_task, control_type);
    if (!proced) {
        std::cout << "Failed to parse argv" << std::endl;
        mpause();
        return -1;
    }
    bool identified = app_package_and_activity(client_type, package, activity);
    if (!identified) {
        std::cout << "Failed to identify the client type" << std::endl;
        mpause();
        return -1;
    }
    if (tasks.empty()) {
        std::cout << "Task empty" << std::endl;
        mpause();
        return -1;
    }

    bool matched = false;
    MaaSize kIndex = 0;
    if (!adb.empty() && !adb_address.empty()) {
        matched = match_adb_address(adb_address, kIndex, device_size);
    }
    if (!matched) {
        kIndex = get_device_index(device_size);
        adb = MaaToolKitGetDeviceAdbPath(kIndex);
        adb_address = MaaToolKitGetDeviceAdbSerial(kIndex);
        save_config(adb, adb_address, client_type, tasks, control_type);
    }

    const auto cur_dir = std::filesystem::path(argv[0]).parent_path();
    std::string debug_dir = (cur_dir / "debug").string();
    std::string resource_dir = (cur_dir / "resource").string();
    std::string adb_config = MaaToolKitGetDeviceAdbConfig(kIndex);
    std::string agent_dir = (cur_dir / "MaaAgentBinary").string();

    MaaSetGlobalOption(MaaGlobalOption_Logging, (void*)debug_dir.c_str(), debug_dir.size());
    MaaSetGlobalOption(MaaGlobalOption_DebugMode, (void*)&debug, sizeof(bool));

    auto maa_handle = MaaCreate(nullptr, nullptr);
    auto resource_handle = MaaResourceCreate(nullptr, nullptr);
    auto controller_handle = MaaAdbControllerCreateV2(adb.c_str(), adb_address.c_str(), control_type,
                                                      adb_config.c_str(), agent_dir.c_str(), nullptr, nullptr);

    MaaBindResource(maa_handle, resource_handle);
    MaaBindController(maa_handle, controller_handle);
    int height = 720;
    MaaControllerSetOption(controller_handle, MaaCtrlOption_ScreenshotTargetShortSide, reinterpret_cast<void*>(&height), sizeof(int));
    MaaControllerSetOption(controller_handle, MaaCtrlOption_DefaultAppPackageEntry, (void*)activity.c_str(), activity.size());
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
        std::cout << "Failed to init Maa instance, a connection error or resource file corruption occurred, please refer to the log." << std::endl;
        mpause();
        return -1;
    }

    CustomActionRegistrar registerar;
    register_custom_action(maa_handle, registerar);

    MaaTaskId task_id = 0;
    for (const auto& task : tasks) {
        if (!task.enabled)
        {
            continue;
        }
        std::cout << task.type << " Start" << std::endl;
        task_id = MaaPostTask(maa_handle, task.type.c_str(), task.param.to_string().c_str());
        std::cout << task.type << " Running..." << std::endl;
        auto end_status = MaaWaitTask(maa_handle, task_id);
        std::cout << task.type << " End" << std::endl 
                  << task.type << " Result: " << TaskStatus(end_status) << std::endl;
    }

    if (after_task.enabled && !after_task.type.empty()) {
        std::cout << "EndTask Start" << std::endl;
        task_id = MaaPostTask(maa_handle, after_task.type.c_str(), after_task.param.to_string().c_str());
        std::cout << "EndTask Running..." << std::endl;
        auto end_status = MaaWaitTask(maa_handle, task_id);
        std::cout << "EndTask End" << std::endl 
                  << "EndTask Result: " << TaskStatus(end_status) << std::endl;
    }

    std::cout << std::endl << "All Tasks Over" << std::endl;

    destroy();

	return 0;
}

void print_help()
{
	std::cout <<
		R"(MAA BH3 CLI, 
Github: https://github.com/MaaAssistantArknights/MAABH3

Usage: MAABH3_CLI.exe [adb_path] [adb_address] [task_name]...

Modify config.json to configure tasks.

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
    std::cout << "Scanning Finished"
              << std::endl
              << std::endl;
    return device_size;
}

bool app_package_and_activity(int client_type, std::string& package, std::string& activity)
{
    switch (client_type)
    {
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

void print_device_list(const MaaSize& device_size) {
    for (MaaSize i = 0; i < device_size; i++) {
        std::cout << "  " << i << ". " << MaaToolKitGetDeviceName(i) << " (" << MaaToolKitGetDeviceAdbSerial(i) << ")\n";
    }
}

MaaSize get_device_index(const MaaSize& device_size)
{
    MaaSize index;
    while (true) {
        std::cout << std::endl
                  << "Please Select a Device to Connect:"
                  << std::endl
                  << std::endl;
        print_device_list(device_size);
        std::cout << std::endl
                  << "Please Enter the Device Number:" 
                  << std::endl;
        std::cin >> index;
        if (index > device_size) {
            std::cout << std::endl
                      << "Unknown Device Number: " << index 
                      << std::endl
                      << std::endl;
            continue;
        }
        std::cout << std::endl;
        break;
    }
    return index;
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

json::value end_to_do_param() {
    json::value param;
    auto& diff = param["diff_task"];
    auto& close_bh3 = diff["CloseBH3"]["enabled"];
    auto& close_bh3_doc = diff["CloseBH3"]["doc"];

    close_bh3 = false;
    close_bh3_doc = "关闭崩坏3；默认false";

    return param;
}

bool proc_argv(int argc, char** argv, bool& debug, std::string& adb, std::string& adb_address, int& client_type,
               TaskList& tasks, AfterTask& after_task, MaaAdbControllerType& ctrl_type)
{
    int touch = 3;
    int key = 1;
    int screencap = 3;

    tasks.clear();

    if (auto config_opt = json::open("config.json")) {
        auto& confing = *config_opt;

        debug = confing.get("debug", false);
        adb = confing["adb"].as_string();
        adb_address = confing["adb_address"].as_string();
        client_type = confing["client_type"].as_integer();

        int index = 1;
        for (auto& task : confing["tasks"].as_array()) {
            Task task_obj;
            if (task.is_string()) {
                task_obj.type = task.as_string();
                task_obj.name = "MyTask" + std::to_string(index++);
            }
            else {
                task_obj.enabled = task.get("enabled", true);
                if (!task_obj.enabled) {
                    continue;
                }
                task_obj.type = task["type"].as_string();
                task_obj.name = task["name"].as_string();
                task_obj.param = task["param"];
            }
            tasks.emplace_back(task_obj);
        }

        after_task.enabled = confing["tasks_completion_after"].get("enabled", false);
        after_task.type = confing["tasks_completion_after"]["type"].as_string();
        after_task.param = confing["tasks_completion_after"]["param"];

        touch = confing.get("touch", touch);
        key = confing.get("key", key);
        screencap = confing.get("screencap", screencap);

        ctrl_type = touch << 0 | key << 8 | screencap << 16;
    }
    else {
        std::cout << std::endl
            << "Please select client type: "
            << std::endl
            << std::endl
            << "  1. Official(CN)\n"
               "  2. Bilibili\n"
               "  3. Vivo\n" 
            << std::endl
            << "Please enter the client type number: "
            << std::endl;
        std::string client_type_tmp;
        std::getline(std::cin, client_type_tmp);
        client_type = std::stoi(client_type_tmp);
        if (client_type < 1 || client_type > 3) {
            std::cout << "Unknown client type: " << client_type << std::endl;
            return false;
        }
        std::cout << std::endl
            << std::endl
            << "Please select tasks: "
            << std::endl
            << std::endl
            << "  1. Dorm\n"
               "  2. MaterialEvent\n"
               "  3. Armada\n"
               "  4. Shop\n"
               "  5. Awards\n"
               "  6. UniversalMirage\n"
            << std::endl
            << "Please enter the task numbers to be executed: "
            << std::endl;
        std::vector<int> task_ids;
        std::string line;
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
            tasks.emplace_back(std::move(task_obj));
        }

        ctrl_type = touch << 0 | key << 8 | screencap << 16;
    }

    if (argc >= 3) {
        adb = argv[1];
        adb_address = argv[2];

        std::vector<std::string> task_names;
        for (int i = 3; i < argc; ++i) {
            task_names.emplace_back(argv[i]);
        }
        auto all_tasks = std::move(tasks);
        tasks.clear();
        for (auto& task_name : task_names) {
            for (auto& task : all_tasks) {
                if (task.name == task_name) {
                    task.enabled = true;
                    tasks.emplace_back(task);
                    break;
                }
            }
        }
    }

    return true;
}

void save_config(const std::string& adb, const std::string& adb_address, const int& client_type, const TaskList& tasks,
    MaaAdbControllerType ctrl_type)
{
    json::value config;
    config["debug"] = false;
    config["adb"] = adb;
    config["adb_Doc"] = "adb.exe 所在路径，相对绝对均可";
    config["adb_address"] = adb_address;
    config["adb_address_Doc"] = "adb 连接地址，例如 127.0.0.1:7555";
    config["client_type"] = client_type;
    config["client_type_Doc"] = "客户端类型：1: 官服, 2: Bilibili服, 3: Vivo服";

    json::value tasks_array;
    for (auto& task : tasks) {
        json::value task_obj;
        task_obj["enabled"] = task.enabled;
        task_obj["type"] = task.type;
        task_obj["name"] = task.name;
        task_obj["param"] = task.param;
        tasks_array.emplace(std::move(task_obj));
    }

    config["tasks"] = std::move(tasks_array);
    config["tasks_Doc"] = "要执行的任务 Homeland, MaterialEvent, Armada, Shop, Awards, UniversalMirage";

    json::value after_task;
    after_task["enabled"] = false;
    after_task["type"] = "EndToDo";
    after_task["param"] = end_to_do_param();

    config["tasks_completion_after"] = std::move(after_task);
    config["tasks_completion_after_Doc"] = "任务完成后执行的操作";

    config["touch"] = (ctrl_type & MaaAdbControllerType_Touch_Mask) >> 0;
    config["touch_Doc"] = "点击方式：1: Adb, 2: MiniTouch, 3: MaaTouch";
    // config["key"] = key;
    // config["key_Doc"] = "按键方式：1: Adb, 2: MaaTouch";
    config["screencap"] = (ctrl_type & MaaAdbControllerType_Screencap_Mask) >> 16;
    config["screencap_Doc"] = "截图方式：1: 自动测速, 2: RawByNetcat, 3: RawWithGzip, 4: Encode, 5: EncodeToFile, 6: "
        "MinicapDirect, 7: MinicapStream";
    config["version"] = "v0.1.0";

    std::ofstream ofs("config.json", std::ios::out);
    ofs << config;
    ofs.close();
}

std::string TaskStatus(MaaStatus status) {
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
        task_status = "Unkown";
        break;
    }
    return task_status;
}

void mpause()
{
    std::ignore = getchar();
}