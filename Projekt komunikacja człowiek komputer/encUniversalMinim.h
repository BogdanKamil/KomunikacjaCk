// мmini-klasa do pracy z enkoderem
/*Konstruktor: encMinim(uint8_t clk, uint8_t dt, uint8_t sw, boolean dir, boolean type): Inicjalizuje enkoder, ustawiając odpowiednie piny i tryby.
Metody:
tick(): Aktualizuje stan enkodera i przycisku.
isClick(): Sprawdza, czy przycisk został kliknięty.
isTurn(): Sprawdza, czy enkoder został obrócony.
isRight(): Sprawdza, czy enkoder został obrócony w prawo.
isLeft(): Sprawdza, czy enkoder został obrócony w lewo.
isRightH(): Sprawdza, czy enkoder został obrócony w prawo z przyciśniętym przyciskiem.
isLeftH(): Sprawdza, czy enkoder został obrócony w lewo z przyciśniętym przyciskiem.*/
class encMinim
{
  public:
    encMinim(uint8_t clk, uint8_t dt, uint8_t sw, boolean dir, boolean type);
    void tick();
    boolean isClick();
    boolean isTurn();
    boolean isRight();
    boolean isLeft();
    boolean isRightH();
    boolean isLeftH();
/*_clk, _dt, _sw: Piny enkodera i przycisku.
_type: Typ enkodera (pełno-krokowy lub pół-krokowy).
_state, _lastState: Aktualny i ostatni stan pinu clk.
_turnFlag, _swState, _swFlag, _turnState: Flagi stanu obrotu i przycisku.
_encState: Stan enkodera (0: nic, 1: lewo, 2: prawo, 3: prawo z przyciskiem, 4: lewo z przyciskiem, 5: kliknięcie).
_debTimer: Timer do debouncingu przycisku.*/
  private:
    byte _clk, _dt, _sw;
    boolean _type = false;
    boolean _state, _lastState, _turnFlag, _swState, _swFlag, _turnState;
    byte _encState;
    uint32_t _debTimer;
    // 0 - nic, 1 - w lewo, 2 - w prawo, 3 - w prawo Wciśnięty, 4 - w lewo Wciśnięty
};
//Konstruktor inicjalizuje piny clk, dt i sw, ustawia je jako wejścia z podciąganiem (INPUT_PULLUP), oraz zapisuje początkowy stan pinu clk.
encMinim::encMinim(uint8_t clk, uint8_t dt, uint8_t sw, boolean dir, boolean type) {
  if (dir) {
    _clk = clk;
    _dt = dt;
  } else {
    _clk = dt;
    _dt = clk;
  }
  _sw = sw;
  _type = type;
  pinMode (_clk, INPUT_PULLUP);
  pinMode (_dt, INPUT_PULLUP);
  pinMode (_sw, INPUT_PULLUP);
  _lastState = digitalRead(_clk);
}
/*Metoda tick() aktualizuje stany enkodera i przycisku:
Sprawdza zmiany stanu pinu clk i na ich podstawie określa kierunek obrotu.
Ustawia odpowiednie stany _encState na podstawie kierunku obrotu i stanu przycisku.
Obsługuje debouncing dla przycisku enkodera.
Każda z tych metod sprawdza odpowiedni stan _encState i resetuje go po odczytaniu, aby uniknąć wielokrotnego zgłaszania tego samego zdarzenia.*/
void encMinim::tick() {
  _encState = 0;
  _state = digitalRead(_clk);
  _swState = digitalRead(_sw);

  if (_state != _lastState) {
    _turnState = true;
    _turnFlag = !_turnFlag;
    if (_turnFlag || !_type) {
      if (digitalRead(_dt) != _lastState) {
        if (_swState) _encState = 1;
        else _encState = 3;
      } else {
        if (_swState) _encState = 2;
        else _encState = 4;
      }
    }
    _lastState = _state;
  }

  if (!_swState && !_swFlag && millis() - _debTimer > 80) {
    _debTimer = millis();
    _swFlag = true;
    _turnState = false;
  }
  if (_swState && _swFlag && millis() - _debTimer > 80) {
    _debTimer = millis();
    _swFlag = false;
    if (!_turnState) _encState = 5;
  }
}
boolean encMinim::isTurn() {
  if (_encState > 0 && _encState < 5) {
    return true;
  } else return false;
}
boolean encMinim::isRight() {
  if (_encState == 1) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isLeft() {
  if (_encState == 2) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isRightH() {
  if (_encState == 3) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isLeftH() {
  if (_encState == 4) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isClick() {
  if (_encState == 5) {
    _encState = 0;
    return true;
  } else return false;
}
