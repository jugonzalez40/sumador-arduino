//CONTROLS (MULTIPLEX)
#define analogC0 A4
#define analogC1 A5

/////switch
//analogs
int switch8 = A3;
int switch7 = A2;
int switch6 = A1;
int switch5 = A0;
//digitals
#define switch4 9
#define switch3 10
#define switch2 11
#define switch1 12

//OUTPUTS
#define segA 2
#define segB 3
#define segC 4
#define segD 5
#define segE 6
#define segF 7
#define segG 8

//MULTIPLEX ARRAY
bool multiplexController = 0;

//
bool dipSwitch[8] = {};

void setup() {
  Serial.begin(9600);           //  setup serial
  for (int i = 0; i < 7; i++) {
    pinMode(i + 2, OUTPUT);
  }
  pinMode(analogC0, OUTPUT);
  pinMode(analogC1, OUTPUT);
}

void loop() {
  //1. Corre la multiplexación
  runMultiplex();
  //2. Lee las entradas del dip switch
  renderInputs();
  //3. Construye las salidas del segmento 
  int segment = buildSegment();
  //4. Muestra la suma
  displaySegment(segment);
}

void runMultiplex(){
  if(!multiplexController) {
    analogWrite(analogC0, 255);
    analogWrite(analogC1, 0);
  } else {
    analogWrite(analogC1, 255);
    analogWrite(analogC0, 0);
  }
  multiplexController = !multiplexController;
}


void renderInputs() {
  //
  dipSwitch[0] = digitalRead(switch1);
  dipSwitch[1] = digitalRead(switch2);
  dipSwitch[2] = digitalRead(switch3);
  dipSwitch[3] = digitalRead(switch4);
  //
  dipSwitch[4] = analogToDigital(analogRead(switch5));
  dipSwitch[5] = analogToDigital(analogRead(switch6));
  dipSwitch[6] = analogToDigital(analogRead(switch7));
  dipSwitch[7] = analogToDigital(analogRead(switch8));
  //printMeLikeKing(dipSwitch);
}

byte getDigit(int maxSwitch) {
  byte digit = 0;
  for (int i = 0; i < 4; i++) {
    digit |= dipSwitch[(maxSwitch - 1) - i] << i;
  }
  return digit;
}

int buildSegment() {
  byte sum = getDigit(4) + getDigit(8);
  String sumChar = String(sum); 
  int _display = 0;
  if(!multiplexController) {
    if(sumChar.length() < 2) {
      _display = 0;
    } else {
      _display = sumChar.substring(1,0).toInt();
    }
  } else {
    if(sumChar.length() < 2) {
      _display = sumChar.substring(1,0).toInt();
    } else {
      Serial.println(sumChar.substring(1,2));
      Serial.println(sumChar);
    _display = sumChar.substring(1,2).toInt();
    }
  }

  Serial.println("==================");
  return _display;
}


void displaySegment(int _number){
  bool _A = bitRead(_number, 0);
  bool _B = bitRead(_number, 1);
  bool _C = bitRead(_number, 2);
  bool _D = bitRead(_number, 3);
  runSegmentA(_A, _B, _C, _D);
  runSegmentB(_A, _B, _C, _D);
  runSegmentC(_A, _B, _C, _D);
  runSegmentD(_A, _B, _C, _D);
  runSegmentE(_A, _B, _C, _D);
  runSegmentF(_A, _B, _C, _D);
  runSegmentG(_A, _B, _C, _D);
}


////////////////////Funciones Privadas
void runSegmentA(bool _A, bool _B, bool _C, bool _D) {
  bool condition = !(!_B && !_D && _XOR(_C, _A));
  (condition) ? digitalWrite(segA, LOW) : digitalWrite(segA, HIGH);
}

void runSegmentB(bool _A, bool _B, bool _C, bool _D) {
  bool condition = !(_C && _XOR(_B, _A));
  (condition) ? digitalWrite(segB, LOW) : digitalWrite(segB, HIGH);
}

void runSegmentC(bool _A, bool _B, bool _C, bool _D) {
  bool condition = !(!_D && !_C && _B && !_A);
  (condition) ? digitalWrite(segC, LOW) : digitalWrite(segC, HIGH);
}

void runSegmentD(bool _A, bool _B, bool _C, bool _D) {
  bool condition = !(!(!_C && !_A) && !(_B && !_C) && !(_B && !_A) && !(_C && !_B && _A));
  (condition) ? digitalWrite(segD, LOW) : digitalWrite(segD, HIGH);
}

void runSegmentE(bool _A, bool _B, bool _C, bool _D) {
  bool condition = !(!(!_C && !_A) && !(_B && !_A));
  (condition) ? digitalWrite(segE, LOW) : digitalWrite(segE, HIGH);
}

void runSegmentF(bool _A, bool _B, bool _C, bool _D) {
  bool condition = !(!_C && !(!_B && !_A) && !(_C && !_B) && !(_C && !_A));
  //bool condition = (_C || (!_B && _A) || (!_B && !_A) || (!_B && _C));
  (condition) ? digitalWrite(segF, LOW) : digitalWrite(segF, HIGH);
}

void runSegmentG(bool _A, bool _B, bool _C, bool _D) {
  bool con2 = (!_D && !_C && !_B && _A);
  bool condition = (_D || (_B && !_A) || _XOR(_C, _A) || (!_D && _C && !_B && _A)  );
  con2 ? digitalWrite(segG, HIGH) : (condition? digitalWrite(segG, LOW) : digitalWrite(segG, HIGH));
}

bool _XOR(bool A, bool B) {
  return ((A && !B ) || (!A && B));
}

int analogToDigital(int value) {
  return (value < 1000) ? 0 : 1;
}
