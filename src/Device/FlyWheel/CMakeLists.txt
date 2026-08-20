introsatlib_add_static_component(
	IntroSatLib_FlyWheel
	FlyWheel
	BaseFlyWheel.cpp
	CoilFlyWheel.cpp
	MotorFlyWheel.cpp
)
target_link_libraries(IntroSatLib_FlyWheel PUBLIC IntroSatLib::Device)
