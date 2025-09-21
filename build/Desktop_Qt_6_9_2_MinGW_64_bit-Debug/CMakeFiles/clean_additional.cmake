# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GraphicViewExample_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GraphicViewExample_autogen.dir\\ParseCache.txt"
  "GraphicViewExample_autogen"
  )
endif()
