add_executable(${PROJECT_NAME} router.c helper.c)

target_include_directories(${PROJECT_NAME}
        PUBLIC kafkaService/include
        PUBLIC external/json_lib)
target_link_directories(${PROJECT_NAME}
        PRIVATE ${dvRouter_SOURCE_DIR}/kafkaService/
        PRIVATE ${dvRouter_SOURCE_DIR}/external/json_lib) # specify library location for linking process
target_link_libraries(${PROJECT_NAME}
        kafkaService
        json-c)
target_link_libraries(${PROJECT_NAME} kafkaService)