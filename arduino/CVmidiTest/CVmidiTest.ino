//Mapeo sensor1 99 - 130
//sensor 2 96 - 122
// sensor 3 102 - 128
// sensor 4 100 - 133

#include <MIDI.h>
int CV1 = A1;
int CV2 = A2;
int CV3 = A3;

int led = 8;
int led1 = 10;
int led2 = 11;


const int channel = 1;
int CV1val;
int CV2val;
int CV3val;

int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;


void setup() {
  // put your setup code here, to run once:
  MIDI.begin();
  pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);


}

void loop() {

  //CONTROLAR LOS LEDS CON CADA UNO DE LOS INPUTS PARA VISUALIZAR LA SENAL
 digitalWrite(led, HIGH);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);

  int note;

  CV1val = map(CV1, 0, 1023, 99, 130);
  CV2val = map(CV2, 0, 1023, 99, 122);
  CV3val = map(CV3, 0, 1023, 102, 128);


  // put your main code here, to run repeatedly:
  sensorValue1 = analogRead(CV1);
  sensorValue2 = analogRead(CV2);
  CV1val = note;
  //enviar la nota que se genero con la velocida de CV2
  MIDI.sendNoteOn(sensorValue1, sensorValue2, channel);

 
  //const int value = inPitchValue * MIDI_PITCHBEND_MAX * Settings::Toto;
  MIDI.sendPitchBend(CV3val, channel);
  MIDI.send(AfterTouchPoly, sensorValue1, CV2val, channel);
  delay(100);
  MIDI.sendNoteOff(sensorValue1, sensorValue2, channel);

}
