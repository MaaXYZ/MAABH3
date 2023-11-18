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
    main.h 
    CustomAction/CombatAction.h 
    CustomAction/CombatAction.cpp 
    CustomAction/CustomActionRegistrar.h 
    CustomAction/CustomActionRegistrar.cpp 
    CustomAction/CustomAction.h 
    CustomAction/CustomAction.cpp
    CustomAction/CustomActionRegistry.h
    CustomAction/RandomPointGenerator.h
    CustomAction/RandomPointGenerator.cpp
    CustomRecognizer/CustomRecognizer.cpp
    CustomRecognizer/CustomRecognizer.h
    CustomRecognizer/CustomRecognizerRegistrar.cpp
    CustomRecognizer/CustomRecognizerRegistrar.h
    CustomRecognizer/CustomRecognizerRegistry.h
    CustomRecognizer/CombatRecognizer.h
    CustomRecognizer/CombatRecognizer.cpp
    Config/Config.h
    Config/Config.cpp
    Config/ControlConfig.h
    Config/ControlConfig.cpp
)
target_link_libraries(MAABH3_CLI PRIVATE MaaFramework MaaToolKit)

add_dependencies(MAABH3_CLI AssetsResource)

install(TARGETS MAABH3_CLI RUNTIME DESTINATION .)
