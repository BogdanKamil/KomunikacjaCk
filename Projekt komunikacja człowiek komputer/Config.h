//╞══════════════════════════════╣ USTAWIENIA ⚒ ╠══════════════════════════════════╡

#define NUM_SHOTS     5     // liczba szklanek (również liczba diod LED i przycisków!)
#define TIMEOUT_OFF   5     // czas do wyłączenia (zatrzymuje drgania napędu), minuty
#define SWITCH_LEVEL  0     // przyciski 1 - wysoki sygnal przy zamknięciu, 0 - niski
#define PARKING_POS   0     // pozycja parkowania w stopniach
#define TIME_50ML     5000  // czas napełnienia 50 ml
#define STBY_LIGHT    20    // jasność podświetlenia w trybie gotowości. 255 -> maksimum, 0 -> wyłączone
#define RAINBOW_FLOW  1     // 1 -> dynamiczne podświetlenie nalewanych szklanek, 0 -> statyczne
#define STATUS_LED    0     // jasność diody stanu. 255 -> maksimum, 0 -> nie podłączono.

#define STEPS_PER_REVOLUTION     2037.88642  // liczba kroków na obrót silnika
#define MICROSTEPS               2           // wartość mikrokroku ustawiona na sterowniku silnika
#define INVERT_STEPPER           0           // odwróć kierunek obrotu silnika krokowego
#define STEPPER_ENDSTOP_INVERT   0           // 1 - wysoki sygnal przy zamknięciu, 0 - niski
#define STEPPER_POWERSAFE        1           // tryb oszczędzania energii silnika krokowego. 1 -> włączony, 0 -> wyłączony
#define STEPER_BACKLASH          3           // kompensacja luzu silnika w stopniach. 0 wyłącza tę funkcję
#define STEPPER_SPEED            10          // prędkość obrotowa silnika w rpm
#define STEPPER_HOMING_SPEED     5           // prędkość obrotu silnika podczas poruszania się do końca (jeśli jest) w rpm

#define DEBUG_UART 0                         // debug

//╞═════════════════════╡Pozycje serwa nad środkami szklanek╞══════════════════════╡

// numer szklanki               1   2   3   4    5    6
//                            ^   ^   ^   ^    ^    ^
#define SHOT_POSITIONS       30, 60, 90, 120, 150, 180

//╞═════════════════════════════╡PINY Arduino Nano╞═══════════════════════════════╡

#if defined(ARDUINO_AVR_NANO)
#define PUMP_POWER      3   // sygnal sterujący sterownika pompy (zwykle oznaczony jako IN na sterownikach silników lub GATE/BASE, jeśli używany jest tranzystor)
#define STEPPER_STEP    5   // sygnal STEP sterownika
#define STEPPER_DIR     4   // sygnal DIR sterownika
#define STEPPER_EN      2   // sygnal EN sterownika
//#define STEPPER_ENDSTOP A7  // sygnal z końcowego przełącznika dla silnika. W przypadku nie używania końcowca zakomentuj.
#define LED_PIN         6   // sygnal DIO/DI pierwszej diody LED (dla pierwszej szklanki)
#define BTN_PIN         7   // sygnal z głównego przycisku do nalewania i zmiany trybów
#define ENC_SW          8   // sygnal z centralnego przycisku na enkoderze do przepompowywania i ustawiania różnych objętości. Oznaczone jako SW lub BTN
#define ENC_DT          9   // sygnal DT lub A z enkodera
#define ENC_CLK         10  // sygnal CLK lub B z enkodera
#define DISP_DIO        11  // sygnal DIO na wyświetlaczu TM1637
#define DISP_CLK        12  // sygnal CLK na wyświetlaczu TM1637
#define SW_PINS         A0, A1, A2,
