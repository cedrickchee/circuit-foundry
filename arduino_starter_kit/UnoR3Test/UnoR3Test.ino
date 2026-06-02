const unsigned long LED_ON_MS = 1000;
const unsigned long LED_OFF_MS = 500;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for boards with native USB; harmless on Arduino UNO R3.
  }
  Serial.println("Arduino UNO R3 test sketch started");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED on");
  delay(LED_ON_MS);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED off");
  delay(LED_OFF_MS);
}
