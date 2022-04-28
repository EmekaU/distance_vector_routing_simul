add_executable(${PROJECT_NAME} router.c helper.c)
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/usr/local/opt/openssl/lib/pkgconfig")
pkg_check_modules(KAFKA REQUIRED IMPORTED_TARGET rdkafka)

target_include_directories(${PROJECT_NAME}
        PUBLIC  ${dvRouter_SOURCE_DIR}/external/json_lib
        PUBLIC  ${dvRouter_SOURCE_DIR}/kafkaService/include)
target_link_directories(${PROJECT_NAME}
        PRIVATE ${dvRouter_SOURCE_DIR}/kafkaService/
        PRIVATE ${dvRouter_SOURCE_DIR}/external/json_lib/) # specify library location for linking process
target_link_libraries(${PROJECT_NAME}
        kafkaService
        PkgConfig::KAFKA
        json-c)