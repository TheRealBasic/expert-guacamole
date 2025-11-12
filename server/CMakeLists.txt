add_executable(Server
    src/main.cpp
    src/server_app.cpp
    src/network.cpp
    src/game_loop.cpp
)

target_include_directories(Server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Server PRIVATE common enet)
