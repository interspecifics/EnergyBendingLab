//CV to midi module
//Recibe señales de voltage de entre 0v y 5v
//Mapea las señales a formato midi entre 0 y 127
//Tiene 3 inputs CV1, CV2, gateIn
//CV1 controla el pitch bend
//CV2 modulación y velocidad
//GateIn produce la nota midi

// detalles de la libreria: http://arduinomidilib.fortyseveneffects.com/a00015.html
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

const int NUM_READINGS = 10;
const int NUM_SENSORS = 3;
const int GATE_THRESHOLD = 100;

// PINS
int inputPins[NUM_SENSORS] = {5, 6, 7};

// Sensor reading variables
int readings[NUM_SENSORS][NUM_READINGS];
int readingIndex[NUM_SENSORS];
int readingSum[NUM_SENSORS];
int readingAverage[NUM_SENSORS];

// midi
int lastNoteOn = 0;
bool bNoteOn = 0;

void setup() {
  //envia a todos los canales
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // init readings
  for (int i = 0; i < NUM_SENSORS; i++) {
    readingIndex[i] = 0;
    readingSum[i] = 0;
    readingAverage[i] = 0;
    for (int j = 0; j < NUM_READINGS; j++) {
      readings[i][j] = analogRead(inputPins[i]);
      readingSum[i] += readings[i][j];
      delay(1);
    }
    readingAverage[i] = readingSum[i] / NUM_READINGS;
  }
}


void loop() {
  // smooth values
  for (int i = 0; i < NUM_SENSORS; i++) {
    readingSum[i] -= readings[i][readingIndex[i]];
    readings[i][readingIndex[i]] = analogRead(inputPins[i]);
    readingSum[i] += readings[i][readingIndex[i]];
    readingIndex[i] += 1;
    if (readingIndex[i] >= NUM_READINGS)
      readingIndex[i] = 0;
  }

  // calculate the averages
  for (int i = 0; i < NUM_SENSORS; i++) {
    readingAverage[i] = readingSum[i] / NUM_READINGS;
  }

  // print values to serial
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("\t Sensor[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.print(readingAverage[i]);
  }

  //convertir el valor de input a nota
  int note = map(readingAverage[0], 0, 1023, 0, 127);
  int velocity = map(readingAverage[1], 0, 1023, 0, 127);
  int GateVal = map(readingAverage[2], 0, 1023, 0, 127);

  /// ????
  if ((GateVal >= GATE_THRESHOLD) && (!bNoteOn)) {
    MIDI.sendNoteOn(note, velocity, MIDI_CHANNEL_OMNI);
    lastNoteOn = note;
    bNoteOn = !bNoteOn;
  }
  else if ((GateVal < GATE_THRESHOLD) && (bNoteOn)) {
    note = lastNoteOn;
    MIDI.sendNoteOff(note, 0, MIDI_CHANNEL_OMNI);
    bNoteOn = !bNoteOn;
  }

  delay(10);
}


