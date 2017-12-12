project(ptt)

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBRARIES OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(ptt CAction.cpp CDependency.cpp CDomain.cpp CLearner.cpp common.cpp CParameter.cpp CPlan.cpp CPredicate.cpp CProblem.cpp CTypes.cpp lex.yy.cc pddlconvertor.tab.cpp main.cpp)

