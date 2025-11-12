add_executable(Client
    src/main.cpp
    src/client_app.cpp
    src/render.cpp
    src/input.cpp
    src/audio.cpp
    src/network.cpp
)

target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Client PRIVATE common glfw glad stb tinyobjloader miniaudio enet imgui)
