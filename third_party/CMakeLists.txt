add_library(glfw STATIC glfw/src_stub.cpp)
target_include_directories(glfw PUBLIC ${CMAKE_CURRENT_LIST_DIR}/glfw/include)

add_library(glad STATIC glad/src/glad_stub.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_LIST_DIR}/glad/include)

add_library(stb STATIC stb/stb_image_stub.c)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/stb)

add_library(tinyobjloader STATIC tinyobjloader/tinyobjloader_stub.cpp)
target_include_directories(tinyobjloader PUBLIC ${CMAKE_CURRENT_LIST_DIR}/tinyobjloader)

add_library(miniaudio STATIC miniaudio/miniaudio_stub.c)
target_include_directories(miniaudio PUBLIC ${CMAKE_CURRENT_LIST_DIR}/miniaudio)

add_library(enet STATIC enet/enet_stub.c)
target_include_directories(enet PUBLIC ${CMAKE_CURRENT_LIST_DIR}/enet/include)

add_library(imgui STATIC imgui/imgui_stub.cpp)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/imgui)
