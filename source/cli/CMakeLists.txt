include_directories(${CMAKE_SOURCE_DIR}/deps/include)
if (WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/deps/lib)
else ()
    link_directories(${CMAKE_SOURCE_DIR}/deps/bin)
endif ()

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CONFIGURATION_TYPES "Release")

add_executable(MAABH3_CLI 
    main.cpp
    control.cpp
    device.cpp
    tasks.cpp
    CustomAction/CustomAction.cpp
    CustomAction/CustomActionRegistrar.cpp  
    CustomAction/CombatAction.cpp 
    CustomAction/RandomPointGenerator.cpp
    CustomRecognizer/CustomRecognizer.cpp
    CustomRecognizer/CustomRecognizerRegistrar.cpp
    CustomRecognizer/CombatRecognizer.cpp
    Config/Config.cpp
    Config/ControlConfig.cpp
    Config/DeviceConfig.cpp
    Config/TasksConfig.cpp
    Config/ConfigMgr.cpp
    Cache/AdbConfigCache.cpp
)
target_link_libraries(MAABH3_CLI PRIVATE MaaFramework MaaToolKit)

add_dependencies(MAABH3_CLI AssetsResource)

install(TARGETS MAABH3_CLI RUNTIME DESTINATION .)
