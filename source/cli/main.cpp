#include <MaaFramework/MaaAPI.h>
#include <meojson/json.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

struct Task
{
	std::string name;
	std::string type;
	json::value param = json::object();
};


using TaskList = std::vector<Task>;

void print_help();
bool proc_argv(int argc, char** argv, std::string& adb, std::string& adb_address, TaskList& tasks,
	MaaAdbControllerType& ctrl_type);
void save_config(const std::string& adb, const std::string& adb_address, const TaskList& tasks,
	MaaAdbControllerType ctrl_type);
std::string read_adb_config(const std::filesystem::path& cur_dir);
void pause();

int main(int argc, char** argv)
{
	print_help();

	std::string adb = "adb";
	std::string adb_address = "127.0.0.1:7555";
	TaskList tasks;
	MaaAdbControllerType control_type = 0;

    bool proced = proc_argv(argc, argv, adb, adb_address, tasks, control_type);
    if (!proced) {
        std::cout << "Failed to parse argv" << std::endl;
        pause();
        return -1;
    }
    if (tasks.empty()) {
        std::cout << "Task empty" << std::endl;
        pause();
        return -1;
    }

    const auto cur_dir = std::filesystem::path(argv[0]).parent_path();
    std::string debug_dir = (cur_dir / "debug").string();
    std::string resource_dir = (cur_dir / "resource").string();
    std::string adb_config = read_adb_config(cur_dir);

    MaaSetGlobalOption(MaaGlobalOption_Logging, (void*)debug_dir.c_str(), debug_dir.size());

    auto maa_handle = MaaCreate(nullptr, nullptr);
    auto resource_handle = MaaResourceCreate(nullptr, nullptr);
    auto controller_handle = MaaAdbControllerCreate(adb.c_str(), adb_address.c_str(), control_type, adb_config.c_str(), nullptr, nullptr);

    MaaBindResource(maa_handle, resource_handle);
    MaaBindController(maa_handle, controller_handle);
    int height = 720;
    MaaControllerSetOption(controller_handle, MaaCtrlOption_ScreenshotTargetShortSide, reinterpret_cast<void*>(&height), sizeof(int));

    auto resource_id = MaaResourcePostResource(resource_handle, resource_dir.c_str());
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
        std::cout << "Failed to init Maa instance, a connection error or resource file corruption occurred, please refer to the log." << std::endl;
        pause();
        return -1;
    }

    MaaTaskId task_id = 0;
    for (const auto& task : tasks) {
        task_id = MaaPostTask(maa_handle, task.type.c_str(), task.param.to_string().c_str());
    }
    MaaWaitTask(maa_handle, task_id);

    destroy();

	return 0;
}

void print_help()
{
	std::cout <<
		R"(MAA BH3 CLI, 
Github: https://github.com/dongwlin/MAABH3

Usage: MAABH3.exe [adb_path] [adb_address] [task_name]...

Modify config.json to configure tasks.

Welcome to come and create a GUI for us! :)
)" << std::endl;
}

bool proc_argv(int argc, char** argv, std::string& adb, std::string& adb_address, TaskList& tasks,
    MaaAdbControllerType& ctrl_type)
{
    int touch = 1;
    int key = 1;
    int screencap = 3;

    tasks.clear();

    if (auto config_opt = json::open("config.json")) {
        auto& confing = *config_opt;

        adb = confing["adb"].as_string();
        adb_address = confing["adb_address"].as_string();

        int index = 1;
        for (auto& task : confing["tasks"].as_array()) {
            Task task_obj;
            if (task.is_string()) {
                task_obj.type = task.as_string();
                task_obj.name = "MyTask" + std::to_string(index++);
            }
            else {
                task_obj.type = task["type"].as_string();
                task_obj.name = task["name"].as_string();
                task_obj.param = task["param"];
            }
            tasks.emplace_back(task_obj);
        }
        touch = confing.get("touch", touch);
        key = confing.get("key", key);
        screencap = confing.get("screencap", screencap);

        ctrl_type = touch << 0 | key << 8 | screencap << 16;
    }
    else {
        std::cout << std::endl
            << std::endl
            << "Please enter the adb path: " << std::endl;
        std::getline(std::cin, adb);
        std::cout << std::endl
            << std::endl
            << "Please enter the adb address: " << std::endl;
        std::getline(std::cin, adb_address);
        std::cout << std::endl
            << std::endl
            << "Please select a task: " << std::endl
            << std::endl
            << "1. Homeland\n"
               "2. Awards\n"
            << std::endl
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

        // tasks.emplace_back(Task { .name = "MyTask0", .type = "Start1999" });

        int index = 1;
        for (auto id : task_ids) {
            Task task_obj;
            task_obj.name = "MyTask" + std::to_string(index++);

            switch (id) {
            case 1:
                task_obj.type = "Homeland";
                break;
            case 2:
                task_obj.type = "Awards";
                break;

            default:
                std::cout << "Unknown task: " << id << std::endl;
                return false;
            }
            tasks.emplace_back(std::move(task_obj));
        }

        ctrl_type = touch << 0 | key << 8 | screencap << 16;
        save_config(adb, adb_address, tasks, ctrl_type);
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
                    tasks.emplace_back(task);
                    break;
                }
            }
        }
    }

    return true;
}

void save_config(const std::string& adb, const std::string& adb_address, const TaskList& tasks,
    MaaAdbControllerType ctrl_type)
{
    json::value config;
    config["adb"] = adb;
    config["adb_Doc"] = "adb.exe 所在路径，相对绝对均可";
    config["adb_address"] = adb_address;
    config["adb_address_Doc"] = "adb 连接地址，例如 127.0.0.1:7555";

    json::value tasks_array;
    for (auto& task : tasks) {
        json::value task_obj;
        task_obj["type"] = task.type;
        task_obj["name"] = task.name;
        task_obj["param"] = task.param;
        tasks_array.emplace(std::move(task_obj));
    }
    config["tasks"] = std::move(tasks_array);
    config["tasks_Doc"] = "要执行的任务 StartBH3, Homeland, Awards";

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

std::string read_adb_config(const std::filesystem::path& cur_dir)
{
    std::ifstream ifs(cur_dir / "resource" / "controller_config.json", std::ios::in);
    if (!ifs.is_open()) {
        std::cout << "Can't open controller_config.json" << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

void pause()
{
    std::ignore = getchar();
}