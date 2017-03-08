import gab.opencv.*;
import SimpleOpenNI.*;

SimpleOpenNI  context;
OpenCV opencv;
PImage cvImageForBlobs;
int cvWidth = 320;
int cvHeight= 240;

// ---------------------------------------------------------- SETUP


void setup()
{

  context = new SimpleOpenNI(this);
  context.setMirror(true);
  context.enableDepth();
  context.enableRGB();

  opencv = new OpenCV(this, 320, 240);
  cvImageForBlobs = new PImage(cvWidth, cvHeight);
  
  size(640,480);
}




// ----------------------------------------------------------- DRAW


void draw()
{
    background(0);
    
    context.update();

    PImage myImage = new PImage(cvWidth, cvHeight);
    myImage = getBinaryImageFromKinect();
    //opencv.copy(myImage);
    //opencv.blur(OpenCV.BLUR, 7);
    //opencv.threshold(10);
    //cvImageForBlobs = opencv.image();
    //blobs = opencv.blobs( 100, 320*240/3, 20, true );
   


    image(context.depthImage(),0,0,320,240);
    //image(myImage,320,240);
    //image( cvImageForBlobs, 320, 0 );
    //drawBlobs();
    println(frameRate);
}



// ----------------------------------------------------------- EVENTS


// ----------------------------------------------------------- METHODS

/*
Computes a binary image from the kinect considering all non-black pixels
 to be foreground (white).
 */

PImage getBinaryImageFromKinect()
{
  PImage tempImage = new PImage(320,240);
  tempImage.loadPixels();

  int[] depthValues = context.depthMap();

  for(int y = 0; y < 480; y+=2) {

    for(int x = 0; x < 640; x+=2) {

      int i = x + y * 640;
      int j = (x/2)+(y/2) *320;
      int currentDepthValue = depthValues[i];

      if(currentDepthValue > 0 && currentDepthValue < 2000)
        tempImage.pixels[j] = color(255,255,255);
      else tempImage.pixels[j] = color(0,0,0);
    }
  }

  tempImage.updatePixels();
  return tempImage;
}



/*void drawBlobs()
{
    int w = 320;
    int h = 240;
    noFill();

    pushMatrix();
    translate(w,0);
    
    for( int i=0; i<blobs.length; i++ ) {

        //Rectangle bounding_rect	= blobs[i].rectangle;
        float area = blobs[i].area;
        float circumference = blobs[i].length;
        //Point centroid = blobs[i].centroid;
        //Point[] points = blobs[i].points;

        // rectangle
        noFill();
        //stroke( blobs[i].isHole ? 128 : 64 );
         stroke(255,0,0);
         rect(  blobs[i].rectangle.x,  blobs[i].rectangle.y,  blobs[i].rectangle.width,  blobs[i].rectangle.height );


        // centroid
        stroke(0,0,255);
        line( blobs[i].centroid.x-5, blobs[i].centroid.y, blobs[i].centroid.x+5, blobs[i].centroid.y );
        line( blobs[i].centroid.x, blobs[i].centroid.y-5, blobs[i].centroid.x, blobs[i].centroid.y+5 );
        noStroke();
        fill(0,0,255);
        //text( area,blobs[i].centroid.x+5, blobs[i].centroid.y+5 );



    }
    popMatrix();
}
*/
