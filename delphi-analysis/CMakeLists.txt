set (CMAKE_CXX_STANDARD 17)
add_library(delphi-analysis STATIC src/phdst_analysis.cpp src/skelana_analysis.cpp)
target_include_directories(delphi-analysis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)