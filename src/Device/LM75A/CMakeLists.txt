introsatlib_add_static_component(IntroSatLib_LM75A LM75A LM75A.cpp)
target_link_libraries(
	IntroSatLib_LM75A
	PUBLIC IntroSatLib::Device IntroSatLib::Base
)
