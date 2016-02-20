find_package(PythonInterp 2.7 REQUIRED)

if(PYTHONINTERP_FOUND)
    set(CXXTEST_USE_PYTHON true)
    set(CXXTEST_TESTGEN_ARGS --xunit-printer --have-eh)
    set(CXXTEST_PYTHON_INTERPRETER "${PYTHON_EXECUTABLE}")
endif()

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIRS})
    enable_testing()
endif(CXXTEST_FOUND)
