cmake_minimum_required(VERSION 3.22)

project(hedgehog LANGUAGES C)

include(FetchContent)
FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.6.1 
)
FetchContent_MakeAvailable(Corrosion)

corrosion_import_crate(MANIFEST_PATH core_parser/Cargo.toml)

add_executable(browser src/main.c)

target_link_libraries(browser PRIVATE core_parser)
