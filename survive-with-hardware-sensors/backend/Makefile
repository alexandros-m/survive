all: main_ultrasonic main_pressure check_for_plates

main_pressure: main_pressure.cpp
	g++ main_pressure.cpp -o main_pressure `pkg-config --cflags --libs opencv` -lpigpio -lrt

main_ultrasonic: main_ultrasonic.cpp
	g++ main_ultrasonic.cpp -o main_ultrasonic `pkg-config --cflags --libs opencv` -lpigpio -lrt

check_for_plates: check_for_plates.cpp
	g++ check_for_plates.cpp -o check_for_plates `pkg-config --cflags --libs opencv`