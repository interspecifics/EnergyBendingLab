//CV to midi module
//Recive señales de voltage de entre 0v y 5v
//Mapea las señales a formato midi entre 0 y 127
//Tiene 3 inputs CV1, CV2, gateIn
//CV1 controla el pitch bend
//CV2 modulación
//GateIn produce la nota midi

#include <MIDI.h>
// detalles de la libreria: http://arduinomidilib.fortyseveneffects.com/a00015.html

const int NUM_READINGS = 10;

int readings1[NUM_READINGS];
int readings2[NUM_READINGS];
int readings3[NUM_READINGS];

int MyindexCV1 = 0; // the index of the current reading
int MyindexCV2 = 0;
int MyindexGate = 0;

int total = 0; // the running total
int average = 0;// the average


// CV INPUTS
int CV1 = 5;
int CV2 = 6;
int GateIn = 7;

// CV VARIABLE
int CV1val;
int CV2val;
int GateInVal;
int CV1reading;
int CV2reading;
int GateInreading;

//midi variables
int note;
int modulation;
int pitchBend;
int velocity;


void setup() {
  //envia a todos los canales
  MIDI.begin(MIDI_CHANNEL_OMNI);

  int sum = 0;

  //revisar valores del CV1 input
  for (int thisReading = 0; thisReading < NUM_READINGS; thisReading++)
  { readings1[thisReading] = 0;
    sum = sum + analogRead(CV1);//hacer lectura del sensor;
    delay(1);
  }

  //revisar valores del CV2 input
  for (int thisReading = 0; thisReading < NUM_READINGS; thisReading++)
  { readings2[thisReading] = 0;
    sum = sum + analogRead(CV2); //hacer lectura dle sensor2
    delay(1);
  }

  //revisar valores GateIn
  for (int thisReading = 0; thisReading < NUM_READINGS; thisReading++)
{ readings3[thisReading] = 0;
sum = sum + analogRead(GateIn);
delay(1);
}
}



void loop() {

  CV1reading = analogRead(CV1);
  CV2reading = analogRead(CV2);
  GateInreading = analogRead(GateIn);
  CV1val = map(CV1reading, 0, 1023, 0, 127);
  CV2val = map(CV2reading, 0, 1023, 0, 127);
  GateInVal = map(GateInreading, 0, 1023, 0, 127);


  //smothing values CV1
  total = total - readings1[MyIndexCV1];
  readings1[MyIndexCV1] = analogRead(CV1);
  total = total + readings1[MyIndexCV1];
  MyIndexCV1 = MyIndexCV1 + 1;

  if (MyindexCV1 >= NUM_READINGS)
  MyindexCV1 = 0;

 //smothing values CV2

 total = total - readings2[MyIndexCV2];
 readings2[MyIndexCV2] = analogRead(CV2);
 total = total + readings2[MyIndexCV2];
 MyIndexCV2 = MyIndexCV2 + 1;

  if (MyindexCV2 >= NUM_READINGS)
  MyindexCV2 = 0;


 //smothing values GateIn

 total = total - readings3[MyIndexGate];
 readings3[MyIndexGate] = analogRead(GateIn);
 total = total + readings3[MyIndexGate];
 MyIndexGate = MyIndexGate + 1;

  if (MyindexGate >= NUM_READINGS)
  MyindexGate = 0;

                 //convertir el valor mapeado a nota
                 readings = note;
                 //enviar la nota que se genero con la velocida de CV2
                 MIDI.sendNoteOn(note, 0, channel);
                 delay(200);
                 MIDI.sendNoteOff(note, 100, channel);

                 const int value = inPitchValue * MIDI_PITCHBEND_MAX * Settings::Toto;
                 sendPitchBend(value, inChannel);


}





