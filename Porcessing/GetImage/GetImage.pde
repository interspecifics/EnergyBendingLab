import codeanticode.syphon.*;

PImage img;
SyphonClient client;

boolean record = false; 


public void setup() {
  size(720, 480, P3D);
  
  // Create syhpon client to receive frames 
  // from the first available running server: 
  client = new SyphonClient(this);
  
  background(0);
}

public void draw() {  
  if (client.available()) {
    // The first time getImage() is called with 
    // a null argument, it will initialize the PImage
    // object with the correct size.
    img = client.getImage(img); // load the pixels array with the updated image info (slow)
    //img = client.getImage(img, false); // does not load the pixels array (faster)
    image(img, 0, 0, width, height);
  }
  
   if (record == true) {

   saveFrame("timelapse-######.png");
   delay(10000);
    }
 
 if (record == false) {
    record = false; // Stop recording to the file
  }
}




void keyPressed() {
  if (key == 'R' || key == 'r') { // Press R to save the file
    record = true;
  }
}

