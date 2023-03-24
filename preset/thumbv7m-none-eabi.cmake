set(EMLIB_ARCH "armv7m")
set(EMLIB_ABI "eabi")
set(EMLIB_ARCH_FLAGS "-march=armv7m -mfloat-abi=soft -mfpu=none")
set(preset_hash "armv7m-soft-none")

include("cmake/arm.cmake")
