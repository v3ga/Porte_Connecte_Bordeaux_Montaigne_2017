// ------------------------------------
void setup()
{
  pinMode(13, OUTPUT);     
  Serial.begin(9600);
}

// ------------------------------------
void loop()
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  Serial.println("Blink!");
}
