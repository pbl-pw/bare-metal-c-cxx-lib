add_subdirectory(picolibc/picocrt/machine/arm picocrt EXCLUDE_FROM_ALL)
target_link_libraries(picocrt PUBLIC libc)
target_link_libraries(picocrt-semihost PUBLIC libc)
