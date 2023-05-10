#define ledR 25
#define ledB 26
#define btR 32
#define btB 33

int btnRedCounter = 0;
int btnRedState = 0;
int lastBtnRedState = 0;

int btnBlueCounter = 0;
int btnBlueState = 0;
int lastBtnBlueState = 0;

void setup() {
  pinMode(ledR, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(btR, INPUT);
  pinMode(btB, INPUT);
  Serial.begin(115200);

}

void loop() {

  //=================================
  btnRedState = digitalRead(btR);
  if (btnRedState != lastBtnRedState) {
    if (btnRedState == 1) {
      btnRedCounter++;
      Serial.print("countBTNRed: "); Serial.println(btnRedCounter);
    }
    lastBtnRedState = btnRedState;
  }

  if (btnRedCounter % 2 == 0) digitalWrite(ledR, 0);
  else digitalWrite(ledR, 1);

  //==================================
  btnBlueState = digitalRead(btB);
  if (btnBlueState != lastBtnBlueState) {
    if (btnBlueState == 1) {
      btnBlueCounter++;
      Serial.print("countBTNBlue: "); Serial.println(btnBlueCounter);
    }
    lastBtnBlueState = btnBlueState;
  }

  if (btnBlueCounter % 2 == 0) digitalWrite(ledB, 0);
  else digitalWrite(ledB, 1);
}
