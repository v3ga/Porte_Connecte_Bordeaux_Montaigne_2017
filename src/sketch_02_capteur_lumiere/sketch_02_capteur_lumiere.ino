// ------------------------------------
void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

// ------------------------------------
void loop()
{
  int luminosite = analogRead(A0);
  Serial.println(luminosite);
  if (luminosite>600)
  {
    digitalWrite(13,HIGH);
  }
  else
  {
    digitalWrite(13,LOW);
  }
}

