import oscP5.*;
import netP5.*;

private static NetAddress oscOutAddress;
private static OscMessage mMessage;

public class Sensor {
  private static final short AVGSIZE = 20;
  private static final short DISPLAYSIZE = 600;
  private static final short RAWSIZE = 3000;

  private final static int OSC_OUT_PERIOD = 100;
  private final static int OSC_OUT_PORT = 8666;
  private final static String OSC_OUT_HOST = "localhost";
  private final static String OSC_OUT_PATTERN = "/EBL/";

  private short rawValues[] = new short[RAWSIZE+1];

  // begin/end indices for different things
  private short rawEnd;
  private short maxValue, minValue;
  private String name;
  private PVector location, dimension;

  public Sensor(PVector _location, PVector _dimension, String _name) {
    oscOutAddress = new NetAddress(OSC_OUT_HOST, OSC_OUT_PORT);
    mMessage = new OscMessage(OSC_OUT_PATTERN);

    // set location/dimension of graphs and name of sensor
    location = _location;
    dimension = _dimension;
    name = _name;

    // initial end indices
    //   "end" points to one after the last value in range
    rawEnd = (short)((rawValues.length)-1);
    
    // init values
    for (int i=0; i<(rawValues.length); ++i) {
      rawValues[i] = 0;
    }    
    minValue = (short)0x7fff;
    maxValue = 1;
  }


  public short getMin() {
    return minValue;
  }
  public short getMax() {
    return maxValue;
  }
  public String getName() {
    return name;
  }

  public short getRawValue() {
    // last written value is at end-1
    // make sure index is positive
    int getFromIndex = (rawEnd > 0)?(rawEnd-1):((rawValues.length)-1);
    return rawValues[getFromIndex];
  }
  


  public void addValue(short val) {
    // write value to raw array and update end index
    rawValues[rawEnd] = val;
    rawEnd = (short)((rawEnd+1)%(rawValues.length));
    minValue = val<minValue?val:minValue;
    maxValue = val>maxValue?val:maxValue;
  }


  public void sendOsc(boolean isNorm) {
    String mAddrPatt = OSC_OUT_PATTERN+getName()+"/";
    // raw
    mMessage.clear();
    mMessage.setAddrPattern(mAddrPatt+"raw");
    if (!isNorm) mMessage.add(getRawValue());
    else mMessage.add(map(getRawValue(), minValue, maxValue, 0, 1));
    OscP5.flush(mMessage, oscOutAddress);
  }




  public void draw() {
    pushMatrix();
      translate(location.x, location.y);
      drawGraph(rawValues, (short)(rawValues.length), rawEnd, dimension.x, dimension.y);
      // sensor title
      fill(255);
      textSize(16);
      text(name, 10, 16);
      // info string
      pushMatrix();
        translate(0, dimension.y*3/4);
        fill(255);
        textSize(16);
        String ss = "raw: "+getRawValue()+" min: "+minValue+" max: "+maxValue;
        text(ss, 10, 16);
      popMatrix();
    popMatrix();
  }


  
  void drawGraph(short values[], short sizeOfValues, short lastIndex, float gwidth, float gheight) {
    // create the graph
    PShape ps = createShape();
    ps.beginShape();
    ps.noFill();
    ps.stroke(20,255,30);
    ps.strokeWeight(3);
    for (int x=1, i=(int)(lastIndex-gwidth); x<gwidth; ++x, ++i) {
      int yIndex = i;
      while (yIndex<0) {
        yIndex += sizeOfValues;
      }
      yIndex = yIndex%sizeOfValues;
      short y0 = (short)map(values[yIndex], 1023, 0, 0, gheight);
      ps.vertex(x, y0);
    }
    ps.endShape();

    // then draw it
    fill(10);
    noStroke();
    rect(0, 0, gwidth, gheight);
    shape(ps, 0, 0);
  }
}
