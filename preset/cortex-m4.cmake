if(NOT EMLIB_ARCH_FLAGS)
	set(EMLIB_ARCH_FLAGS "-mfpu=fpv4-sp-d16")
endif(NOT EMLIB_ARCH_FLAGS)
include(preset/thumbv7em-none-eabihf.cmake)
