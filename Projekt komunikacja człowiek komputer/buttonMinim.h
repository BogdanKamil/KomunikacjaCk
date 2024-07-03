// мини-класс для работы с кнопкой, версия 1.0

/*Ten kod przedstawia mini-klasę buttonMinim do obsługi przycisków w Arduino.
Klasa ta pozwala na wykrywanie różnych stanów przycisku, takich jak naciśnięcie, kliknięcie, trzymanie oraz trzymanie po dłuższym czasie.
Oto szczegółowe wyjaśnienie poszczególnych elementów kodu:
Jest to struktura bitowa zawierająca flagi stanu przycisku:
-holdedFlag: Flaga sygnalizująca, że przycisk był trzymany przez dłuższy czas.
-btnFlag: Flaga sygnalizująca aktualny stan przycisku (czy jest wciśnięty).
-pressF: Flaga sygnalizująca, że przycisk został właśnie naciśnięty.
-clickF: Flaga sygnalizująca, że przycisk został kliknięty (szybkie naciśnięcie i zwolnienie).
-holdF: Flaga sygnalizująca, że przycisk jest trzymany przez dłuższy czas.*/
#pragma pack(push,1)
typedef struct {
  bool holdedFlag: 1;
  bool btnFlag: 1;
  bool pressF: 1;
  bool clickF: 1;
  bool holdF: 1;
} buttonMinimFlags;
#pragma pack(pop)

/*flags: Instancja struktury buttonMinimFlags do przechowywania stanów przycisku.
_btnTimer: Zmienna do przechowywania czasu, w którym przycisk był ostatnio wciśnięty.
_pin: Numer pinu, do którego podłączony jest przycisk.*/
class buttonMinim {
  public:
    buttonMinim(uint8_t pin);//Konstruktor- Ustawia pin przycisku jako wejście z podciąganiem do stanu wysokiego (INPUT_PULLUP) i zapisuje numer pinu.
    boolean pressed();
    boolean clicked();
    boolean holding();
    boolean holded();
  private:
    buttonMinimFlags flags;
    void tick();
    uint32_t _btnTimer;
    byte _pin;
};

buttonMinim::buttonMinim(uint8_t pin) {
  pinMode(pin, INPUT_PULLUP);
  _pin = pin;
}
/*Metoda tick() aktualizuje stany przycisku na podstawie bieżącego odczytu stanu pinu:
Jeśli przycisk jest wciśnięty (stan niski), a nie był wcześniej wciśnięty, oraz minęło więcej niż 90 ms od ostatniego wciśnięcia, to flaga btnFlag jest ustawiana na true, a czas millis() jest zapisywany do _btnTimer.
Jeśli przycisk jest zwolniony (stan wysoki) i był wciśnięty, oraz minęło mniej niż 350 ms, to flaga btnFlag jest resetowana, a flaga clickF jest ustawiana na true.
Jeśli przycisk jest wciśnięty przez więcej niż 500 ms, to flaga holdF jest ustawiana na true, co oznacza długie trzymanie przycisku.
*/
void buttonMinim::tick() {
  boolean btnState = digitalRead(_pin);
  if (!btnState && !flags.btnFlag && ((uint32_t)millis() - _btnTimer > 90)) {
    flags.btnFlag = true;
    _btnTimer = millis();
    flags.pressF = true;
    flags.holdedFlag = true;
  }
  if (btnState && flags.btnFlag && ((uint32_t)millis() - _btnTimer < 350)) {
    flags.btnFlag = false;
    _btnTimer = millis();
    flags.clickF = true;
    flags.holdF = false;
  }
  if (flags.btnFlag && ((uint32_t)millis() - _btnTimer > 500)) {
    if (!btnState) {
      flags.holdF = true;
    } else {
      flags.btnFlag = false;
      flags.holdF = false;
      _btnTimer = millis();
    }
  }
}
/*Metody publiczne:
-pressed(): Sprawdza, czy przycisk został właśnie naciśnięty.
-clicked(): Sprawdza, czy przycisk został kliknięty (szybkie naciśnięcie i zwolnienie).
-holding(): Sprawdza, czy przycisk jest trzymany przez dłuższy czas.
-holded(): Sprawdza, czy przycisk był trzymany przez dłuższy czas.*/
boolean buttonMinim::pressed() {
  buttonMinim::tick();
  if (flags.pressF) {
    flags.pressF = false;
    return true;
  }
  else return false;
}

boolean buttonMinim::clicked() {
  buttonMinim::tick();
  if (flags.clickF) {
    flags.clickF = false;
    return true;
  }
  else return false;
}

boolean buttonMinim::holding() {
  buttonMinim::tick();
  if (flags.holdF) {
    return true;
  }
  else return false;
}

boolean buttonMinim::holded() {
  buttonMinim::tick();
  if (flags.holdF && flags.holdedFlag) {
    flags.holdedFlag = false;
    return true;
  }
  else return false;
}
