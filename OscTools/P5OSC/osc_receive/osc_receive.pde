import oscP5.*;
import netP5.*;

// make osc object
OscP5 oscP5;
float px = 0;
float py = 0;

void setup(){
  
  size(640,480);
  
  // listen port
  oscP5 = new OscP5(this,9339);

}


void draw(){
  background(255);
  ellipse(px,py,10,10);
  
}

void oscEvent(OscMessage theOscMessage) {
  
  /*String address = theOscMessage.addrPattern();
  if( address.equals("/pose/position") ){
    float x = theOscMessage.get(0).floatValue();
    float y = theOscMessage.get(1).floatValue();
    px = x;
    py = y;
  }*/
  
  /*String address = theOscMessage.addrPattern();
  if( address.equals("/cv/blobs")){
    // x
    int id = theOscMessage.get(0).intValue();
    float x = theOscMessage.get(3).floatValue();
    float y = theOscMessage.get(4).floatValue();
    if(id==0) {
      px= x*width;
      py= y*width;
    }
    //println("value: " + val);
  }*/
  
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
  
}