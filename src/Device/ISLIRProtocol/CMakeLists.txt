introsatlib_add_interface_component(
	IntroSatLib_ISLIRProtocol
	ISLIRProtocol
)
target_link_libraries(
	IntroSatLib_ISLIRProtocol
	INTERFACE IntroSatLib::Core
)
