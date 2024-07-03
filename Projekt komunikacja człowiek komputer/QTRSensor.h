#include <Arduino.h>
#include <QTRSensors.h> // Biblioteka do obsługi czujników odbiciowych

// Ustawienia czujników odbiciowych
const unsigned char sensorCount = 2; // Liczba czujników
const unsigned char sensorPins[] = {2, 3}; // Piny, do których są podłączone czujniki
QTRSensorsRC sensors(sensorPins, sensorCount); // Obiekt czujników

void setup() {
  Serial.begin(9600); // Inicjalizacja komunikacji szeregowej

  sensors.calibrate(); // Kalibracja czujników
  delay(1000); // Oczekiwanie na zakończenie kalibracji
}

void loop() {
  unsigned int sensorValues[sensorCount];
  sensors.read(sensorValues); // Odczyt wartości z czujników

  // Sprawdzenie obecności pojemnika na czujnikach
  bool containerDetected = false;
  for (int i = 0; i < sensorCount; ++i) {
    if (sensorValues[i] > 500) { // Wartość odbicia oznaczająca wykrycie pojemnika
      containerDetected = true;
      break;
    }
  }

  if (containerDetected) {
    Serial.println("Pojemnik wykryty.");
    // Tutaj możesz dodać odpowiednie działania, np. rozpoczęcie nalewania płynu
  } else {
    Serial.println("Brak pojemnika.");
    // Tutaj możesz dodać odpowiednie działania, np. zatrzymanie procesu nalewania
  }

  delay(100); // Oczekiwanie przed kolejnym odczytem czujników
}
