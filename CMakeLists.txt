cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(emlib VERSION 18.1.1)

include(FetchContent)

FetchContent_Populate(picolibc_download
	QUIET
	GIT_REPOSITORY "https://github.com/picolibc/picolibc.git"
	GIT_TAG "1.8.7"
	GIT_SHALLOW TRUE
	SUBBUILD_DIR download/picolibc/subbuild
	SOURCE_DIR download/picolibc/src
	BINARY_DIR download/picolibc/build
)
FetchContent_Populate(llvm_download
	QUIET
	GIT_REPOSITORY "https://github.com/llvm/llvm-project.git"
	GIT_TAG "llvmorg-18.1.8"
	GIT_SHALLOW TRUE
	SUBBUILD_DIR download/llvm/subbuild
	SOURCE_DIR download/llvm/src
	BINARY_DIR download/llvm/build
)

set(emlib_preset_postinclude cmake/empty) # guard prevent set outside

if(EMLIB_PRESET)
	include("preset/${EMLIB_PRESET}.cmake" OPTIONAL RESULT_VARIABLE has_preset)
	if(NOT has_preset)
		file(GLOB presets RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/preset" *.cmake)
		list(TRANSFORM presets REPLACE "(.*).cmake" "\\1")
		string(REPLACE ";" " " presets "${presets}")
		message(FATAL_ERROR "preset '${EMLIB_PRESET}' not defined, setting 'EMLIB_ARCH' and(maybe) 'EMLIB_ABI' and(maybe) 'EMLIB_ARCH_FLAGS', or set preset: ${presets}")
	else()
		message("preset '${EMLIB_PRESET}' loaded: EMLIB_ARCH => ${EMLIB_ARCH}, EMLIB_ABI => ${EMLIB_ABI}, EMLIB_ARCH_FLAGS => ${EMLIB_ARCH_FLAGS}, EMLIB_LLD_TARGET => ${EMLIB_LLD_TARGET}")
	endif(NOT has_preset)
else(EMLIB_PRESET) # NOT EMLIB_PRESET
	if(NOT EMLIB_ARCH)
		set(EMLIB_ARCH "armv7m") # arch name, must support by compiler-rt
		set(EMLIB_LLD_TARGET "armelf")
		set(emlib_preset_postinclude cmake/arm-post.cmake)
	endif(NOT EMLIB_ARCH)

	if(NOT EMLIB_ABI)
		set(EMLIB_ABI "eabi") # "abi, ex: eabi, eabihf
	endif(NOT EMLIB_ABI)

	if(NOT EMLIB_ARCH_FLAGS)
		set(EMLIB_ARCH_FLAGS "-march=armv7m -mfloat-abi=soft -mfpu=none") # detail flags, ex: -march=armv7m -mfloat-abi=soft -mfpu=none
	endif(NOT EMLIB_ARCH_FLAGS)
endif(EMLIB_PRESET)

if(NOT EMLIB_INSTALL_DIR)
	set(EMLIB_INSTALL_DIR "${CMAKE_BINARY_DIR}/install")
endif(NOT EMLIB_INSTALL_DIR)

set(target_triple "${EMLIB_ARCH}-none-${EMLIB_ABI}")
message("target tripple: ${target_triple} (with flags: ${EMLIB_ARCH_FLAGS})")

if(${EMLIB_ABI} MATCHES ".*hf$" AND ${EMLIB_ARCH} MATCHES "^arm")
	set(builtins_lib_path "${EMLIB_INSTALL_DIR}/lib/generic/libclang_rt.builtins-armhf.a") # llvm 18.1 patch
else()
	set(builtins_lib_path "${EMLIB_INSTALL_DIR}/lib/generic/libclang_rt.builtins-${EMLIB_ARCH}.a")
endif(${EMLIB_ABI} MATCHES ".*hf$" AND ${EMLIB_ARCH} MATCHES "^arm")
set(c_lib_path "${EMLIB_INSTALL_DIR}/lib/libc.a")
set(cxx_lib_path "${EMLIB_INSTALL_DIR}/lib/libc++.a")
set(cxxabi_lib_path "${EMLIB_INSTALL_DIR}/lib/libc++abi.a")

set(cmake_args
-Wno-dev
--fresh
-DCMAKE_BUILD_TYPE=MinSizeRel
-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
-DCMAKE_SYSTEM_NAME=Generic
-DCMAKE_SYSTEM_PROCESSOR=${EMLIB_ARCH}
-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
-DCMAKE_ASM_COMPILER=${CMAKE_C_COMPILER}
)
set(cmake_asm_args
-DCMAKE_ASM_COMPILER_TARGET=${target_triple}
"-DCMAKE_ASM_FLAGS=${EMLIB_ARCH_FLAGS} --no-default-config"
)
set(cmake_c_args
-DCMAKE_C_COMPILER_TARGET=${target_triple}
"-DCMAKE_C_FLAGS=${EMLIB_ARCH_FLAGS} --no-default-config"
)
set(cmake_cxx_args
-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
-DCMAKE_CXX_COMPILER_TARGET=${target_triple}
"-DCMAKE_CXX_FLAGS=${EMLIB_ARCH_FLAGS} --no-default-config -isystem${EMLIB_INSTALL_DIR}/include"
)

include(ExternalProject)

ExternalProject_Add(compiler-rt
	SOURCE_DIR "${llvm_download_SOURCE_DIR}/runtimes"
	PREFIX compiler-rt
	INSTALL_DIR "${EMLIB_INSTALL_DIR}"
	CMAKE_ARGS ${cmake_args} ${cmake_asm_args} ${cmake_c_args} ${cmake_cxx_args}
		-DLLVM_BINARY_DIR=OFF
		-DLLVM_ENABLE_RUNTIMES=compiler-rt
		-DLLVM_DEFAULT_TARGET_TRIPLE=${target_triple}
		-DLLVM_INCLUDE_TESTS=OFF
		-DCOMPILER_RT_BAREMETAL_BUILD=ON
		-DCOMPILER_RT_BUILD_BUILTINS=ON
		-DCOMPILER_RT_BUILD_LIBFUZZER=OFF
		-DCOMPILER_RT_BUILD_PROFILE=OFF
		-DCOMPILER_RT_BUILD_SANITIZERS=OFF
		-DCOMPILER_RT_BUILD_XRAY=OFF
		-DCOMPILER_RT_BUILD_ORC=OFF
		-DCOMPILER_RT_BUILD_MEMPROF=OFF
		-DCOMPILER_RT_BUILD_GWP_ASAN=OFF
		-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON
		-DCOMPILER_RT_INCLUDE_TESTS=OFF
	USES_TERMINAL_CONFIGURE TRUE
	LIST_SEPARATOR ,
	CONFIGURE_HANDLED_BY_BUILD TRUE
	EXCLUDE_FROM_ALL TRUE
	INSTALL_BYPRODUCTS ${builtins_lib_path}
)

ExternalProject_Add(picolibc
	SOURCE_DIR "${picolibc_download_SOURCE_DIR}"
	PREFIX picolibc
	INSTALL_DIR "${EMLIB_INSTALL_DIR}"
	CMAKE_ARGS ${cmake_args} ${cmake_asm_args} ${cmake_c_args}
		-DNEWLIB_GLOBAL_ERRNO=ON
		-DPICOLIBC_TLS=OFF
		-D_WANT_IO_C99_FORMATS=OFF
		-D_HAVE_SEMIHOST=0
		-D_NANO_FORMATTED_IO=1
		-D__OBSOLETE_MATH_FLOAT=0
		-D__OBSOLETE_MATH_DOUBLE=0
		-D_IO_FLOAT_EXACT=0
	USES_TERMINAL_CONFIGURE TRUE
	LIST_SEPARATOR ,
	CONFIGURE_HANDLED_BY_BUILD TRUE
	EXCLUDE_FROM_ALL TRUE
	INSTALL_BYPRODUCTS ${c_lib_path}
)

ExternalProject_Add(libcxx
	SOURCE_DIR "${llvm_download_SOURCE_DIR}/runtimes"
	PREFIX libcxx
	INSTALL_DIR "${EMLIB_INSTALL_DIR}"
	DEPENDS picolibc compiler-rt
	CMAKE_ARGS ${cmake_args} ${cmake_asm_args} ${cmake_c_args} ${cmake_cxx_args}
		-DLLVM_BINARY_DIR=OFF
		-DLLVM_ENABLE_RUNTIMES=libcxx,libcxxabi
		-DLLVM_DEFAULT_TARGET_TRIPLE=${target_triple}
		-DLLVM_INCLUDE_TESTS=OFF
		-DLIBCXX_CXX_ABI=libcxxabi
		-DLIBCXX_ENABLE_FILESYSTEM=OFF
		-DLIBCXX_ENABLE_SHARED=OFF
		-DLIBCXX_ENABLE_STATIC=ON
		-DLIBCXX_INCLUDE_BENCHMARKS=OFF
		-DLIBCXX_ENABLE_EXCEPTIONS=OFF
		-DLIBCXX_ENABLE_MONOTONIC_CLOCK=OFF
		-DLIBCXX_ENABLE_RANDOM_DEVICE=OFF
		-DLIBCXX_ENABLE_RTTI=OFF
		-DLIBCXX_ENABLE_THREADS=OFF
		-DLIBCXX_ENABLE_LOCALIZATION=OFF
		-DLIBCXX_USE_COMPILER_RT=ON
		-DLIBCXXABI_BAREMETAL=ON
		-DLIBCXXABI_ENABLE_ASSERTIONS=OFF
		-DLIBCXXABI_ENABLE_SHARED=OFF
		-DLIBCXXABI_ENABLE_STATIC=ON
		-DLIBCXXABI_USE_COMPILER_RT=ON
		-DLIBCXXABI_USE_LLVM_UNWINDER=OFF
		-DLIBCXXABI_ENABLE_EXCEPTIONS=OFF
		-DLIBCXXABI_ENABLE_THREADS=OFF
	USES_TERMINAL_CONFIGURE TRUE
	LIST_SEPARATOR ,
	CONFIGURE_HANDLED_BY_BUILD TRUE
	EXCLUDE_FROM_ALL TRUE
	INSTALL_BYPRODUCTS ${cxx_lib_path} ${cxxabi_lib_path}
)

string(REGEX REPLACE " +" ";" common_flags ${EMLIB_ARCH_FLAGS})
set(common_flags "--target=${target_triple}" ${common_flags} --no-default-config -nostdlib -nostdlibinc -ffunction-sections -fdata-sections)
if(EMLIB_LLD_TARGET)
	set(EMLIB_LLD_TARGET "-Wl,-m,${EMLIB_LLD_TARGET}")
endif(EMLIB_LLD_TARGET)

add_library(libc INTERFACE)
target_link_libraries(libc INTERFACE ${c_lib_path} ${builtins_lib_path})
target_include_directories(libc SYSTEM INTERFACE "${EMLIB_INSTALL_DIR}/include")
target_compile_options(libc INTERFACE ${common_flags})
target_link_options(libc INTERFACE ${common_flags} ${EMLIB_LLD_TARGET} "-Wl,-gc-sections")
add_dependencies(libc picolibc compiler-rt)

add_library(libc++ INTERFACE)
target_link_libraries(libc++ INTERFACE libc ${cxx_lib_path} ${cxxabi_lib_path})
target_include_directories(libc++ SYSTEM INTERFACE $<$<COMPILE_LANGUAGE:CXX>: ${EMLIB_INSTALL_DIR}/include/c++/v1>)
target_compile_options(libc++ INTERFACE $<$<COMPILE_LANGUAGE:CXX>: -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-c++-static-destructors -fno-use-cxa-atexit>)
target_link_options(libc++ INTERFACE "-fno-rtti-data")
add_dependencies(libc++ libcxx)

include("${emlib_preset_postinclude}" OPTIONAL)
