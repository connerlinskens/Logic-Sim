add_library(PUGIXML STATIC ${CMAKE_CURRENT_LIST_DIR}/../pugixml.cpp)
set(PUGIXML_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/..)
message("Found TARGET PUGIXML at ${CMAKE_CURRENT_LIST_DIR}")
