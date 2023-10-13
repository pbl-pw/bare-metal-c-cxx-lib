if(NOT EMLIB_ARCH_FLAGS)
	set(EMLIB_ARCH_FLAGS "-mfpu=fpv5-sp-d16")
endif(NOT EMLIB_ARCH_FLAGS)
include(preset/thumbv8m.main-none-eabi.cmake)
