import oscP5.*;
import netP5.*;

// make osc object
OscP5 oscP5;

void setup(){
  
  size(400,400);

  oscP5 = new OscP5(this,);

}


void draw(){
  background(255);
  
}

void oscEvent(OscMessage theOscMessage) {
  
  /*String address = theOscMessage.addrPattern();
  if( address.equals("/mouse"){
    float val = theOscMessage.get(0).floatValue();
    println("value: " + val);
  }*/
  
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
  
}
