add_library(common STATIC
    src/clock.cpp
    src/math.cpp
    src/ecs.cpp
    src/protocol.cpp
    src/serialization.cpp
    src/random.cpp
    src/systems.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(common PUBLIC enet)
