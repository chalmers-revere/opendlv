
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  # TODO: Not tested.
  set(COMPILER_FLAGS "-fstack-protector-all -fPIC -ggdb -pedantic-errors -Wall -Weffc++ -Werror -Wextra -Wshadow -Wswitch-default")
  set(C_COMPILER_FLAGS "-std=c99")
  set(CXX_COMPILER_FLAGS "-std=c++14")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(COMPILER_FLAGS "-fstack-protector-all -fPIC -ggdb -Wall -Weffc++ -Werror -Wextra -Wshadow -pedantic-errors -Wswitch-default")
  set(C_COMPILER_FLAGS "-std=c99")
  set(CXX_COMPILER_FLAGS "-std=c++14")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  # TODO: Not tested.
  set(COMPILER_FLAGS "/MP /Gy /GR- /Zi /W4 /WX /wd\"4100\" /wd\"4127\" /wd\"4244\" /wd\"4512\" /wd\"4701\" /wd\"4702\" /wd\"4996\"")
endif()

set(CMAKE_C_FLAGS "${COMPILER_FLAGS} ${C_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${CXX_COMPILER_FLAGS}")
