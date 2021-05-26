int incomingByte = 0; // for incoming serial data
int clockCount = 0;
bool started = false;

#define DACPin 0

void setup() {
  SerialUSB.begin(115200); // opens serial port, sets data rate to 9600 bps
  Serial1.begin(31250);
  pinMode(0, OUTPUT);
  analogWriteResolution(10);
}

void loop() {
  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial1.read();

    if (incomingByte == 248) {
      SerialUSB.println("Clock!");
      clockCount++;
      SerialUSB.println(clockCount);
    }

    else if (incomingByte == 250) {
      SerialUSB.println("Start!");
      started = true;
    }

    else if (incomingByte == 252) {
      SerialUSB.println("Stop!");
      started = false;
    }

    if (clockCount == 12 and started) {
      analogWrite(DACPin, 0);
      delayMicroseconds(2500);
      analogWrite(DACPin, 1024/3.3);
    }

    clockCount = clockCount % 12;

    
  }
}
