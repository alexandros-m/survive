#include <pigpio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void turnOn(int bcm);
void turnOff(int bcm);
void takeAction();
int checkDistance();
int readbcm(int bcm);

const int red=21, yellow=20, green=16;
const int blinkLed=12, plate=4;

void gpioReset(){
	int i;
	for (i=1;i<28;i++){
		turnOff(i);
	}
	gpioTerminate();
}

void sighandler(int sig)
{
	gpioReset();
	exit(1);
}

int main(){

	if (gpioInitialise() < 0)
	{
		printf("GPIO initilisation failed\n");
	}

	gpioSetMode(red, PI_OUTPUT);
	gpioSetMode(yellow, PI_OUTPUT);
	gpioSetMode(green, PI_OUTPUT);
	gpioSetMode(blinkLed, PI_OUTPUT);
	gpioSetMode(plate, PI_INPUT);

	signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);

	while(1){
		takeAction();
    // long double distance;
    // distance = checkDistance();
    // printf("%lF\n", distance);
	}
	gpioTerminate();
}

void turnOn(int bcm){
  // Turns on a gpio
  // based on an int given
  // (bcm type).
	gpioWrite(bcm, 1);
}

void turnOff(int bcm){
  // Turns off a gpio
  // based on an int given
  // (bcm type).
	gpioWrite(bcm,0);
}

int readGPIO(int bcm){
	return gpioRead(bcm);
}

int checkVoltage(){
	return readGPIO(4);
}

void takeAction(){
	int threshold = 38;
	int weight;
	clock_t timeStart = clock();
	time_t t = time(NULL);
	char photoName[200];

	struct tm tm;
	struct timespec curtime;
	double millisec;

	while ((long double)(clock() - timeStart)/(long double)(CLOCKS_PER_SEC) < 10.0){
		turnOn(red);
		turnOff(yellow);
		turnOff(green);
		weight = readGPIO(plate);
		if (weight){
			turnOn(blinkLed);
			tm = *localtime(&t);
			clock_gettime(CLOCK_REALTIME, &curtime);
			millisec = curtime.tv_nsec/1000000;
			sprintf(photoName, "photos/%d-%02d-%02d_%02d:%02d%02d_%03d.jpg", tm.tm_year + 1900,
				tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, (int)millisec);
			VideoCapture cap(0); 
			Mat save_img;
			cap >> save_img;
			cout << "Took Photo" << endl;
			cout << "Location: " << photoName << endl;
			imwrite(photoName, save_img);
			sleep(0.1);
			turnOff(blinkLed);
		}
	}
	while ((long double)(clock() - timeStart)/(long double)(CLOCKS_PER_SEC) >= 10.0 && (long double)(clock() - timeStart)/(long double)(CLOCKS_PER_SEC) < 20.0){
    // green light
		turnOff(red);
		turnOff(yellow);
		turnOn(green);
	}
	while ((long double)(clock() - timeStart)/(long double)(CLOCKS_PER_SEC) >= 20.0 && (long double)(clock() - timeStart)/(long double)(CLOCKS_PER_SEC) < 23.0){
    // yellow light
		turnOff(red);
		turnOn(yellow);
		turnOff(green);
	}

	timeStart = clock();
}
