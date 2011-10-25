#
# Zillians
# Copyright (C) 2011-2012 Zillians.com
# For more information see http://www.zillians.com
#

include_directories(${PROJECT_ROOT_DIR}/include/)

add_executable(thorpp
    cg/UnicodeUtil.cpp
    cg/Main.cpp
    cg/CppDispatch.cpp
    cg/JavaDispatch.cpp
    cg/PythonDispatch.cpp
    )
    
target_link_libraries(thorpp
    ${Boost_LIBRARIES}
    )