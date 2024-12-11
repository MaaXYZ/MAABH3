#include "tasks.h"

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

    if (std::cin.rdbuf()->in_avail() > 0) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
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