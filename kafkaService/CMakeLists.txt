include_directories(${dvRouter_SOURCE_DIR}/kafkaService/include)
add_library(kafkaService producer.c)
find_package(PkgConfig REQUIRED)

# Resolve Mac issue linking Openssl
set(ENV{PKG_CONFIG_PATH} "/usr/local/opt/openssl/lib/pkgconfig")
pkg_check_modules(KAFKA REQUIRED IMPORTED_TARGET rdkafka)
target_link_directories(kafkaService PRIVATE ${dvRouter_SOURCE_DIR}/external/json_lib/)
target_link_libraries(kafkaService PRIVATE PkgConfig::KAFKA PRIVATE json-c)
