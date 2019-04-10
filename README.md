git clone 
install cmake
qtbase5-dev
libqt5serialport5-dev

sudo raspi-config
	-enable i2c: Intercaface -> I2c
		

cd /home/pi/estimulator/controlador_qt/
mkdir build
cd build
../cmake
make -j4
./controlador