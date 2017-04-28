// ----------------------------------------------------------------------------
// includes
// ----------------------------------------------------------------------------
#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

// ----------------------------------------------------------------------------
// variables
// ----------------------------------------------------------------------------
// LEDs
int nLEDs = 144;
int dataPin  = 6;//vert
int clockPin = 5;//bleu
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// Bouton Gauche
int switchStateBoutonGauche = 0;
int prevSwitchStateBoutonGauche = 0;
unsigned long timeStartBoutonGauche = 0;
unsigned long timeBoutonGauche = 0;
boolean ledsGaucheOn = false;

// Index des Leds pour la gauche
int ledsIndexGaucheDebut = 0;
int ledsIndexGaucheFin = nLEDs / 2;
int ledsIndexGaucheChenille = ledsIndexGaucheDebut;


// Bouton Droit
int switchStateBoutonDroite = 0;
int prevSwitchStateBoutonDroite = 0;
unsigned long timeStartBoutonDroite = 0;
unsigned long timeBoutonDroite = 0;
boolean ledsDroiteOn = false;

// Index des Leds pour la droite
int ledsIndexDroiteDebut = nLEDs / 2 + 1;
int ledsIndexDroiteFin = nLEDs - 1;
int ledsIndexDroiteChenille = ledsIndexDroiteDebut;



// Clignotement (en millisecondes)
int periodeClignotement  = 50;
int periodeChenille  = 10;
int mode = 0; // 0 = clignote , 1 = chenille

// ----------------------------------------------------------------------------
// setup
// ----------------------------------------------------------------------------
void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  Serial.begin(9600);
  strip.begin();
}

// ----------------------------------------------------------------------------
// loop
// ----------------------------------------------------------------------------
void loop()
{
  prevSwitchStateBoutonGauche = switchStateBoutonGauche;
  prevSwitchStateBoutonDroite = switchStateBoutonDroite;

  // Lit l'état des boutons
  // LOW = relâché
  // HIGH = appuyé
  switchStateBoutonGauche = digitalRead(2);
  switchStateBoutonDroite = digitalRead(8);

  // Détecte l'appui sur les boutons
  if (prevSwitchStateBoutonGauche == LOW && switchStateBoutonGauche == HIGH)
  {
    timeStartBoutonGauche = millis();
    ledsGaucheOn = true;
    ledsIndexGaucheChenille = ledsIndexGaucheDebut;
    Serial.println("APPUI BOUTON GAUCHE");
  }

  if (prevSwitchStateBoutonDroite == LOW && switchStateBoutonDroite == HIGH)
  {
    timeStartBoutonDroite = millis();
    ledsDroiteOn = true;
    ledsIndexDroiteChenille = ledsIndexDroiteDebut;
    Serial.println("APPUI BOUTON DROIT");
  }

  // --------------------------------
  // Bouton gauche
  // --------------------------------
  if (switchStateBoutonGauche == LOW)
  {
    // Au repos quand le bouton n'est pas appuyé (0,0,0) = éteinte
    setColorLeds(strip.Color(  0,   0, 0), ledsIndexGaucheDebut, ledsIndexGaucheFin);
  }
  else
  {
    timeBoutonGauche = millis() - timeStartBoutonGauche;

    // Clignotement
    if ( mode == 0)
    {
      if (timeBoutonGauche >= periodeClignotement)
      {
        ledsGaucheOn = !ledsGaucheOn;
        timeStartBoutonGauche = millis();
      }
      if (ledsGaucheOn)
        setColorLeds(strip.Color(  0,   0, 127), ledsIndexGaucheDebut, ledsIndexGaucheFin); // clignote on
      else
        setColorLeds(strip.Color(  0,   0, 0), ledsIndexGaucheDebut, ledsIndexGaucheFin); // clignote off
    }

    // Chenille
    else if (mode == 1)
    {
      if (timeBoutonGauche > periodeChenille)
      {
        timeStartBoutonGauche = millis();
        ledsIndexGaucheChenille++;
        if (ledsIndexGaucheChenille >= ledsIndexGaucheFin)
        {
          ledsIndexGaucheChenille = ledsIndexGaucheDebut;
          setColorLeds(strip.Color(  0,   0, 0), ledsIndexGaucheDebut, ledsIndexGaucheFin);
        }
      }
      setColorLeds(strip.Color(  0,   0, 127), ledsIndexGaucheDebut, ledsIndexGaucheChenille);
    }

  }

  // --------------------------------
  // Bouton droite
  // --------------------------------
  if (switchStateBoutonDroite == LOW)
  {
    setColorLeds(strip.Color(  0,   0, 0), ledsIndexDroiteDebut, ledsIndexDroiteFin);
  }
  else
  {
    timeBoutonDroite = millis() - timeStartBoutonDroite;

    // Clignotement
    if (mode == 0)
    {

      if (timeBoutonDroite >= periodeClignotement)
      {
        ledsDroiteOn = !ledsDroiteOn;
        timeStartBoutonDroite = millis();
      }
      if (ledsDroiteOn)
        setColorLeds(strip.Color(  0,   0, 127), ledsIndexDroiteDebut, ledsIndexDroiteFin);  // clignote on
      else
        setColorLeds(strip.Color(  0,   0, 0), ledsIndexDroiteDebut, ledsIndexDroiteFin);  // clignote off
    }
    // Chenille
    else if (mode == 1)
    {
      if (timeBoutonDroite > periodeChenille)
      {
        timeStartBoutonDroite = millis();
        ledsIndexDroiteChenille++;
        if (ledsIndexDroiteChenille >= ledsIndexDroiteFin)
        {
          ledsIndexDroiteChenille = ledsIndexDroiteDebut;
          setColorLeds(strip.Color(  0,   0, 0), ledsIndexDroiteDebut, ledsIndexDroiteFin);
        }
      }
      setColorLeds(strip.Color(  0,   0, 127), ledsIndexDroiteDebut, ledsIndexDroiteChenille);
    }
  }
}
// ----------------------------------------------------------------------------
// setColorLeds
// ----------------------------------------------------------------------------
void setColorLeds(uint32_t c, int indexStart, int indexEnd)
{
  for (int i = indexStart; i < indexEnd; i++)
  {
    strip.setPixelColor(i, c);
  }
  strip.show();
}
