include(ExternalProject)

set(target_triple armv7m-none-eabi)
set(runtimes_flags "-march=armv7m+nofp -mfloat-abi=soft")
set(runtimes_cxx_flags "${runtimes_flags} ${runtimes_cxx_flags}")

set(cmake_asm_args
-DCMAKE_ASM_COMPILER_TARGET=${target_triple}
-DCMAKE_ASM_FLAGS=${runtimes_flags}
)
set(cmake_c_args
-DCMAKE_C_COMPILER_TARGET=${target_triple}
-DCMAKE_C_FLAGS=${runtimes_flags}
)
set(cmake_cxx_args
-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
-DCMAKE_CXX_COMPILER_TARGET=${target_triple}
-DCMAKE_CXX_FLAGS=${runtimes_cxx_flags}
)

ExternalProject_Add(compiler-rt
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../llvm-project/runtimes"
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
	STEP_TARGETS build
	USES_TERMINAL_CONFIGURE TRUE
	USES_TERMINAL_BUILD TRUE
	USES_TERMINAL_INSTALL TRUE
	USES_TERMINAL_TEST TRUE
	LIST_SEPARATOR ,
	# Always run the build command so that incremental builds are correct.
	BUILD_ALWAYS TRUE
	CONFIGURE_HANDLED_BY_BUILD TRUE
)

