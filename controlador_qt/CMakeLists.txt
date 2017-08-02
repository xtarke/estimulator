cmake_minimum_required(VERSION 2.8.11)
project(controlador)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
# find_package(QtSerialPort REQUIRED)
# find_package(OpenCV REQUIRED)

set(controlador_SRC
  src/main.cpp
  src/mainwindow.cpp
  src/i2cmodule.cpp
)

# Create code from a list of Qt designer ui files.
#set(CMAKE_AUTOUIC ON) # use this if you have CMake 3.x instead of the following
qt5_wrap_ui(controlador_SRC src/mainwindow.ui)

# Tell CMake to create the helloworld executable
add_executable(controlador ${controlador_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(controlador ${OpenCV_LIBS} Qt5::Widgets Qt5Test)

# Install the executable
install(TARGETS controlador DESTINATION bin)

# Add QT modules
qt5_use_modules(controlador Core SerialPort)
