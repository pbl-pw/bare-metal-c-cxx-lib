add_subdirectory(${picolibc_download_SOURCE_DIR}/picocrt/machine/arm picocrt EXCLUDE_FROM_ALL)
target_link_libraries(picocrt PUBLIC libc)
target_link_libraries(picocrt-semihost PUBLIC libc)
