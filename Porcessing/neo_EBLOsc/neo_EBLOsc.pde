/*
neo_EBLOsc

Control de ON/OFF por canal
Valores Enteros o normalizados (0.0 - 1.0)
mecanosaurio@interspecific.cc
*/


import processing.serial.*;
import oscP5.*;
import netP5.*;

final static int OSC_IN_PORT = 8444;

Serial mSerial;
OscP5 mOscP5;

Sensor ss[] = new Sensor[6];
boolean[] isSending = new boolean[6];
boolean[] isNorm = new boolean[6];
long lastOscMillis;


void setup() {
  size(600, 700, P2D);
  for (int i=0; i<ss.length; ++i) {
    ss[i] = new Sensor(new PVector(20, 20+i*110), new PVector(560, 100), "Sensor"+i);
    isSending[i] = true;
    isNorm[i] = true;  
  }
  lastOscMillis = millis();
  mOscP5 = new OscP5(this, OSC_IN_PORT);
  mSerial = new Serial(this, "COM3", 57600);
}


void draw() {
  background(192);

  // read input
  while (mSerial.available () > 2) {
    if (mSerial.read() == 0xAB) {
      int upperByte = mSerial.read();
      int lowerByte = mSerial.read();
      short pinNumber = (short)((upperByte>>0x04)&0x07);
      short value = (short)(((upperByte<<8)&0x0F00) | (lowerByte&0xFF));
      ss[pinNumber].addValue(value);
    }
  }

  // write osc
  if (millis()-lastOscMillis > 100) {
    /*for (Sensor s:ss) {
      s.sendOsc(true);
    }*/
    for (int k=0; k<ss.length; k++){
      if (isSending[k]) ss[k].sendOsc(isNorm[k]);
    }
  }

  // draw
  for (Sensor s:ss) {
    s.draw();
  }
  
  // buttons
  for (int k=0; k<ss.length;k++){
    //-- buttons
    strokeWeight(1);
    color green = color(20,255,30);
   
    if (isSending[k]) fill(green, 64);
    else noFill();
    rect(20, 20+k*110, 87, 25);
 
    if (isNorm[k]) fill(green, 127);
    else noFill();
    rect(110, 20+k*110, 25, 25);
 }
  
}




void mousePressed(){
  if (mouseButton==LEFT){
    for(int k=0; k<6; k++){
      if ((mouseX > 20) && (mouseX < 107) && (mouseY > 20+k*110) && (mouseY < 45+k*110)){
        isSending[k] = !isSending[k];
      }
      if ((mouseX > 110) && (mouseX < 135) && (mouseY > 20+k*110) && (mouseY < 45+k*110)){
        isNorm[k] = !isNorm[k];
      }
    }
  }
}
