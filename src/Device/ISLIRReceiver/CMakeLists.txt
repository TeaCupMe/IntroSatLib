introsatlib_add_static_component(
	IntroSatLib_ISLIRReceiver
	ISLIRReceiver
	ISLIRReceiver.cpp
)
target_link_libraries(
	IntroSatLib_ISLIRReceiver
	PUBLIC IntroSatLib::Device IntroSatLib::ISLIRProtocol
)
