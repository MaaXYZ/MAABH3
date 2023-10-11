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
    CustomAction/Combat.h 
    CustomAction/Combat.cpp 
    CustomAction/CustomActionRegistrar.h 
    CustomAction/CustomActionRegistrar.cpp 
    CustomAction/CustomAction.h 
    CustomAction/CustomAction.cpp 
    CustomAction/CustomActionRegistry.h
)
target_link_libraries(MAABH3_CLI PRIVATE MaaFramework MaaToolKit)

add_dependencies(MAABH3_CLI AssetsResource)

install(TARGETS MAABH3_CLI RUNTIME DESTINATION .)
