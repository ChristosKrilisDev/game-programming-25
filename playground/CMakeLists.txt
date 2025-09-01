file(GLOB file_src_list "*.c" "*.cpp")

foreach (file_src ${file_src_list})
    get_filename_component(targetname ${file_src} NAME)
    add_executable(${targetname} ${file_src}
            core/Types/types.h
            core/Physics/physics.cpp)

    target_include_directories(${targetname} PRIVATE incude/SDL)
    target_link_libraries(${targetname} PRIVATE SDL3::SDL3)
endforeach ()