#pragma once
#include <Arduino.h>
#include <Servo.h>

/*	
	ServoSmooth - biblioteka do plynnej kontroli serwomechanizmow
	- Jest dodatkiem do standardowej biblioteki Servo
	- Ustawienie maksymalnej predkosci serwomechanizmu
	- Ustawienie przyspieszenia (rozpedzania i hamowania) serwomechanizmu
	- Przy uzyciu ESC i regulatora predkosci uzyskujemy "plynne rozruchy" silnika
	- Ustawienie pozycji docelowej serwa pod katem (0-180) i dlugosci impulsu (500-2400)
	- Automatyczne odłączanie (detach) po osiagnieciu celu
	Czytaj dokumentacje tutaj: https://alexgyver.ru/servosmooth/	
	
	v1.1 - automatyczne odłączanie (detach) po osiagnieciu celu
	v1.2 - wlacz/wylacz automatyczne odłączanie serwa
	v1.3 - oddzielna metoda do ustawiania i odczytywania biezacej pozycji. Dodano wariant metody attach
	v1.4 - poprawiona kompatybilnosc
	v1.5 - poprawione getCurrent i getCurrentDeg
	v1.6 - nieznacznie zoptymalizowano inicjalizacje
	v1.7 - naprawiono problem z niska predkoscia/przyspieszeniem, zoptymalizowano kod
	v1.8 - poprawiona stabilnosc
	v1.9 - dodano ustawienie maks. kata serwa
	v1.10 - naprawiono problem, gdy biezaca pozycja jest taka sama jak pozycja docelowa
	v2.0 - uproszczono algorytm
	v2.1 - dodano zmiane kierunku
	v2.2 - drobna poprawka
	
	2019 by AlexGyver
*/

#define SS_SERVO_PERIOD 10		// okres dzialania tick(), ms
#define SS_DEADZONE 5			// martwa strefa
#define NORMAL 0
#define REVERSE 1

class ServoSmooth {
public:
	ServoSmooth(int maxAngle = 180);			// konstruktor z przekazaniem maksymalnego kata
	void write(uint16_t angle);					// analogiczna metoda z biblioteki Servo
	void writeMicroseconds(uint16_t angle);		// analogiczna metoda z biblioteki Servo
	void attach();								// podlącza do wybranego pinu przy attach(pin)
	void attach(uint8_t pin, int target = 0);	// analogiczna metoda z biblioteki Servo
	void attach(uint8_t pin, int min, int max, int target = 0);	// analogiczna metoda z biblioteki Servo. min domyslnie 500, max 2400. target - pozycja (w stopniach lub mikrosekundach, do ktorej serwo obróci się podczas podłączania)
	void detach();								// analogiczna metoda z biblioteki Servo
	void start();								// attach + umożliwia dzialanie tick
	void stop();								// detach + uniemozliwia dzialanie tick
	
	boolean tick();								// metoda zarzadzajaca serwą, powinna być wywoływana jak najczęściej.
	// Zwraca true, gdy osiagnieta jest pozycja docelowa.
	// Posiada wbudowany timer o okresie SS_SERVO_PERIOD
	
	boolean tickManual();						// metoda zarzadzajaca serwą, bez wbudowanego timera.
	// Zwraca true, gdy osiagnieta jest pozycja docelowa
	
	void setSpeed(int speed);					// ustawienie maksymalnej predkosci (jednostki arbitralne, 0 - 200)
	void setAccel(float accel);					// ustawienie przyspieszenia (0.05 - 1). Przy wartosci 1 przyspieszenie jest maksymalne
	void setTarget(int target);					// ustawienie pozycji docelowej w mikrosekundach (500 - 2400)
	void setTargetDeg(int target);				// ustawienie pozycji docelowej w stopniach (0-maks. kat). Zalezy od min i max
	void setAutoDetach(boolean set);			// wlacz/wylacz automatyczne odłączanie (detach) po osiagnieciu kata. Domyślnie włączone
	void setCurrent(int target);				// ustawienie bieżącej pozycji w mikrosekundach (500 - 2400)
	void setCurrentDeg(int target);				// ustawienie bieżącej pozycji w stopniach (0-maks. kat). Zalezy od min i max
	void setMaxAngle(int maxAngle);				// ustawienie maks. kata serwa
	int getCurrent();							// pobranie biezacej pozycji w mikrosekundach (500 - 2400)
	int getCurrentDeg();						// pobranie biezacej pozycji w stopniach (0-maks. kat). Zalezy od min i max
	int getTarget();							// pobranie pozycji docelowej w mikrosekundach (500 - 2400)
	int getTargetDeg();							// pobranie pozycji docelowej w stopniach (0-maks. kat). Zalezy od min i max

	void setDirection(bool _dir);				// zmiana kierunku obrotu
	
	Servo _servo;		
	
private:
	void writeUs(int val);
	int _maxAngle = 180;
	int _servoCurrentPos = 0;
	int _servoTargetPos = 0;
	float _newPos = 0;
	int _min = 500;
	int _max = 2400;
	uint32_t _prevServoTime = 0;		
	int8_t _pin;
	int _servoMaxSpeed = 50;		
	int _newSpeed = 0;		
	float _k = 0.1;
	boolean _tickFlag = true;
	boolean _servoState = true;
	boolean _autoDetach = true;
	byte _timeoutCounter = 0;	
	bool _dir = 0;
	bool _attached = false;
};
