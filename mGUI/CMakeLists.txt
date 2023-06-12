add_executable (mGUI "mgui.cpp" "mgui.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mGUI PROPERTY CXX_STANDARD 20)
endif()
