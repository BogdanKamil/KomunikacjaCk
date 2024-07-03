#include <Servo.h>
#include "ServoSmooth.h"

// Konstruktor klasy ServoSmooth inicjalizuje maksymalny kąt serwomechanizmu (_maxAngle) na podstawie przekazanego parametru.
ServoSmooth::ServoSmooth(int maxAngle) {
	_maxAngle = maxAngle;
}

/* Metody writeUs, write oraz writeMicroseconds służą do ustawiania położenia serwomechanizmu. writeUs przekształca wartość val na mikrosekundy 
i korzysta z _dir do odwrócenia kierunku, jeśli jest ustawiony. write i writeMicroseconds przekształcają wartość kątową na przedział _min do _max i wywołują writeUs.*/


void ServoSmooth::writeUs(int val) {
	_servo.writeMicroseconds(_dir ? (_max - val + _min) : val);
}
void ServoSmooth::write(uint16_t angle) {
	writeUs(map(angle, 0, _maxAngle, _min, _max));
}

void ServoSmooth::writeMicroseconds(uint16_t angle) {
	writeUs(angle);
}

/*Metoda attach() przyłącza serwomechanizm do podanego pinu _pin. Metody attach(pin, target) oraz attach(pin, min, max, target) ustawiają pin, minimalny
 i maksymalny kąt oraz docelową pozycję serwomechanizmu. detach() odłącza serwomechanizm.*/
void ServoSmooth::attach() {
	_servo.attach(_pin);
}

void ServoSmooth::attach(uint8_t pin, int target) {
	_pin = pin;
	_servo.attach(_pin);
	if (target <= _maxAngle) target = map(target, 0, _maxAngle, _min, _max);	
	writeUs(target);
	_servoTargetPos = target;
	_servoCurrentPos = target;
	_newPos = target;
}

void ServoSmooth::attach(uint8_t pin, int min, int max, int target) {
	attach(pin, target);
	_min = min;
	_max = max;
}

void ServoSmooth::detach() {
	_servo.detach();
}

// Metoda start() przyłącza serwomechanizm i ustawia flagi _tickFlag i _servoState na true. 
void ServoSmooth::start() {
	_servo.attach(_pin);	
	_tickFlag = true;
	_servoState = true;
}

// Metoda stop() odłącza serwomechanizm i ustawia flagi _tickFlag i _servoState na false.
void ServoSmooth::stop() {
	_servo.detach();
	_tickFlag = false;
	_servoState = false;
}

/*Metody setSpeed, setAccel, setTarget, setTargetDeg, setCurrent oraz setCurrentDeg służą do ustawiania prędkości, przyspieszenia,
 aktualnej i docelowej pozycji serwomechanizmu, zarówno w kątach, jak i w mikrosekundach.*/
void ServoSmooth::setSpeed(int speed) {
	_servoMaxSpeed = speed;
}

void ServoSmooth::setAccel(float accel) {
	_k = accel;
}

void ServoSmooth::setTarget(int target) {
	_servoTargetPos = target;
}

void ServoSmooth::setTargetDeg(int target) {
	target = constrain(target, 0, _maxAngle);
	_servoTargetPos = map(target, 0, _maxAngle, _min, _max);
}

void ServoSmooth::setCurrent(int target) {
	_servoCurrentPos = target;
	_newPos = _servoCurrentPos;
}

void ServoSmooth::setCurrentDeg(int target) {
	target = constrain(target, 0, _maxAngle);
	_servoCurrentPos = map(target, 0, _maxAngle, _min, _max);
	_newPos = _servoCurrentPos;
}

//Metody getCurrent, getCurrentDeg, getTarget oraz getTargetDeg zwracają aktualną i docelową pozycję serwomechanizmu, zarówno w mikrosekundach, jak i w kątach.
int ServoSmooth::getCurrent() {
	return (int)_newPos;
}
int ServoSmooth::getCurrentDeg() {
	return (map((int)_newPos, _min, _max, 0, _maxAngle));
}

int ServoSmooth::getTarget() {
	return _servoTargetPos;
}
int ServoSmooth::getTargetDeg() {
	return (map(_servoTargetPos, _min, _max, 0, _maxAngle));
}

//Metody setAutoDetach, setMaxAngle oraz setDirection służą odpowiednio do ustawienia automatycznego odłączania serwomechanizmu, maksymalnego kąta oraz kierunku obrotu.
void ServoSmooth::setAutoDetach(boolean set) {
	_autoDetach = set;
}

void ServoSmooth::setMaxAngle(int maxAngle) {
	_maxAngle = maxAngle;
}

void ServoSmooth::setDirection(bool dir) {
	_dir = dir;
}

/*Metody tickManual oraz tick odpowiadają za płynne przemieszczanie się serwomechanizmu do docelowej pozycji. tickManual oblicza nową prędkość,
 aktualizuje pozycję i wywołuje writeUs do wysłania nowej pozycji do serwomechanizmu. tick wywołuje tickManual z określoną częstotliwością, ustawioną w SS_SERVO_PERIOD.*/
boolean ServoSmooth::tickManual() {
	if (_tickFlag) {
    _newSpeed = _servoTargetPos - _servoCurrentPos;                           // Obliczanie prędkości
    if (_servoState) {
        _newSpeed = constrain(_newSpeed, -_servoMaxSpeed, _servoMaxSpeed);    // Ograniczenie do maksymalnej prędkości
        _servoCurrentPos += _newSpeed;                                        // Ustalanie nowej pozycji
        _newPos += (float)(_servoCurrentPos - _newPos) * _k;                  // Filtracja pozycji
        // _newPos = _servoCurrentPos;                                        // Bez filtracji
        _newPos = constrain(_newPos, _min, _max);                             // Ograniczenie pozycji
        writeUs((int)_newPos);                                                // Wysłanie do serwa
    }
}
if (abs(_servoTargetPos - (int)_newPos) < SS_DEADZONE) {
    if (_autoDetach && _servoState) {            
        _servoCurrentPos = _servoTargetPos;
        _servoState = false;
        _servo.detach();
    }            
    return !_servoState;    // Dotarliśmy do celu
	} else {
		if (_autoDetach && !_servoState) {
			_servoState = true;
			_servo.attach(_pin);
		}
		_timeoutCounter = 0;		
	}
	return false;
}

boolean ServoSmooth::tick() {
	if (millis() - _prevServoTime >= SS_SERVO_PERIOD) {
		_prevServoTime = millis();
		ServoSmooth::tickManual();
	}
	return !_servoState;
}