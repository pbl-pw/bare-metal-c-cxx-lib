set(EMLIB_ARCH "armv8m.base")
set(EMLIB_ABI "eabi")
set(EMLIB_ARCH_FLAGS "-march=armv8m.base -mfloat-abi=soft -mfpu=none")
set(preset_hash "armv8m.base-soft-none")

include("cmake/arm.cmake")
