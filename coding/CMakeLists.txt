cmake_minimum_required(VERSION 3.16)

project(cpp_refresh LANGUAGES CXX)

file(GLOB APP_SOURCES CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB LIB_SOURCES CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/lib/*.cpp"
)

add_library(cpp_refresh_lib ${LIB_SOURCES})

target_include_directories(cpp_refresh_lib
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(cpp_refresh_lib PUBLIC cxx_std_23)

foreach(app_source IN LISTS APP_SOURCES)
  get_filename_component(app_name "${app_source}" NAME_WE)

  add_executable(${app_name} "${app_source}")
  target_link_libraries(${app_name} PRIVATE cpp_refresh_lib)
endforeach()
