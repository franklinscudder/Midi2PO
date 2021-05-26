int incomingByte = 0; // for incoming serial data
int clockCount = 0;
bool started = false;
int mode = 12;
static int modes[] = {96, 48, 24, 12, 6, 4, 2, 1}; // PPQN = 0.25 0.5 1 2 4 6 12 24

#define DACPin 0
#define sw1Pin 8
#define sw2Pin 9
#define sw3Pin 10

#define LEDPin 1
#define PwrLEDPin 2

void setup() {
  SerialUSB.begin(115200);
  Serial1.begin(31250);
  pinMode(DACPin, OUTPUT);
  pinMode(sw1Pin, INPUT_PULLUP);
  pinMode(sw2Pin, INPUT_PULLUP);
  pinMode(sw3Pin, INPUT_PULLUP);
  pinMode(LEDPin, OUTPUT);
  pinMode(PwrLEDPin, OUTPUT);
  digitalWrite(PwrLEDPin, HIGH);
  analogWriteResolution(10);
}

int switchState() {
  // query DIP switch states, return mode index
  int state = 0;
  state += !digitalRead(sw1Pin);
  state += !digitalRead(sw2Pin) * 2;
  state += !digitalRead(sw3Pin) * 4;

  return state;
}

void loop() {

  if (not started){
    mode = modes[switchState()];
    SerialUSB.println("Mode: ");
    SerialUSB.println(mode);
  }

  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial1.read();

    // midi clock byte
    if (incomingByte == 248) {
      SerialUSB.println("Clock!");
      clockCount++;
      SerialUSB.println(clockCount);
    }

    // midi start or continue byte
    else if (incomingByte == 250 or incomingByte == 251) {
      SerialUSB.println("Start!");
      started = true;
    }

    // midi stop byte
    else if (incomingByte == 252) {
      SerialUSB.println("Stop!");
      started = false;
    }

    if (clockCount == mode and started) {
      analogWrite(DACPin, 0);
      delayMicroseconds(2500);
      analogWrite(DACPin, 1024/3.3);
    }

    clockCount = clockCount % mode; 
  }
}
