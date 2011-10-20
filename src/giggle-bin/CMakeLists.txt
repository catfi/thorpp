#
# Zillians
# Copyright (C) 2011-2012 Zillians.com
# For more information see http://www.zillians.com
#

include_directories(${PROJECT_ROOT_DIR}/include/)

add_executable(giggle
    cg/UnicodeUtil.cpp
    cg/GiggleMain.cpp
    cg/GiggleCppDispatch.cpp
    cg/GiggleJavaDispatch.cpp
    cg/GigglePythonDispatch.cpp
    )
    
target_link_libraries(giggle
    ${Boost_LIBRARIES}
    )