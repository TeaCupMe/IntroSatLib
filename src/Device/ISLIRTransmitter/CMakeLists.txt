introsatlib_add_static_component(
	IntroSatLib_ISLIRTransmitter
	ISLIRTransmitter
	ISLIRTransmitter.cpp
)
target_link_libraries(
	IntroSatLib_ISLIRTransmitter
	PUBLIC IntroSatLib::Device IntroSatLib::ISLIRProtocol
)
