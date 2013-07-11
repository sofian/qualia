
import Mapper.*;
import Mapper.Db.*;

/**
 * Vector field convolution world for Qualia interacting agents
 * communicating using libmapper. Stephen Sinclair 2012
 *
 * Based on vector field convolution idea by Joseph Malloch.
 *
 * CPU convolution engine based on Convolution processing example by
 * Daniel Shiffman.
 */

import processing.opengl.*;

PImage img, imgA, imgB;
int w = 80;

float[][][] matrix = {
                    { { 1, 1, 0 },
                      { 1, 0, 0 },
                      { 1, 1, 0 } },

                    { { 1, 1, 1 },
                      { 1, 0, 1 },
                      { 0, 0, 0 } },

                    { { 0, 1, 1 },
                      { 0, 0, 1 },
                      { 0, 1, 1 } },

                    { { 0, 0, 0 },
                      { 1, 0, 1 },
                      { 1, 1, 1 } }
};

Mapper.Device dev = new Device("vector", 9000);
final int maxAgents = 9;
Mapper.Device.Signal sig_obs[] = new Mapper.Device.Signal[maxAgents];
Mapper.Device.Signal sig_act[] = new Mapper.Device.Signal[maxAgents];

int[][] agentPositions = new int[maxAgents][];
float[][] agentVelocities = new float[maxAgents][2];

void setup() {
  size(200, 200);
  frameRate(30);
  imgA = createImage(200,200,ARGB);
  imgB = createImage(200,200,ARGB);
  img = imgA;
  imgA.loadPixels();
  for (int i=0; i<imgA.pixels.length; i++)
    imgA.pixels[i] = color(0,0,0,0);
  imgA.updatePixels();
  imgB.loadPixels();
  for (int i=0; i<imgB.pixels.length; i++)
    imgB.pixels[i] = color(0,0,0,0);
  imgB.updatePixels();
  
  for (int i=0; i<maxAgents; i++)
  {
    sig_obs[i] = dev.add_output("node/"+(i+1)+"/cardinal", 5, 'f', null, 0d, 1d);
    sig_act[i] = dev.add_input("node/"+(i+1)+"/force", 1, 'i', null, 0d, 1d,
      new PositionListener(i));
  }
}

void updatePositions()
{
  for (int i=0; i<maxAgents; i++)
  {
    if (agentPositions[i]!=null) {
      int x = agentPositions[i][0];
      int y = agentPositions[i][1];
      img.pixels[x + img.width*y] = color(255,255,255,255);
    }
  }
}

class PositionListener extends InputListener
{
  final int i;
  PositionListener(int j) { i=j; }
  public void onInput(int [] v) {
    int [] pos = agentPositions[i];
    float [] vel = agentVelocities[i];
    if (pos==null) {
      pos = new int[2];
      pos[0] = img.width/2;
    }

    // move to the right
    // (replace with physics based on action in 'v')
    vel[0] += (v[0]*2-1)/10.0f;
    pos[0] = int(pos[0]+vel[0]);
    if (pos[0] >= img.width) {
      pos[0] = img.width-1;
      vel[0] = 0;
    }
    if (pos[0] < 0) {
      pos[0] = 0;
      vel[0] = 0;
    }
    pos[1] = 15*(i-5)+100;

    agentVelocities[i] = vel;
    agentPositions[i] = pos;
  }
};

public void updateObservations()
{
    float obs[] = new float[5];
    for (int i=0; i<maxAgents; i++)
    {
      int [] pos = agentPositions[i];
      if (pos != null) {
        int x = constrain(pos[0], 1, img.width-2);
        int y = constrain(pos[1], 1, img.height-2);
        obs[0] = red  (img.pixels[    x + img.width * (y-1)]) / 255.0f;
        obs[1] = green(img.pixels[(x-1) + img.width *     y]) / 255.0f;
        obs[2] = blue (img.pixels[    x + img.width * (y+1)]) / 255.0f;
        obs[3] = alpha(img.pixels[(x+1) + img.width *     y]) / 255.0f;

        obs[4] = 0; // reward

        sig_obs[i].update(obs);
      }
    }
}

void draw() {
  dev.poll(0);
  
  updatePositions();

  int xstart = 0;
  int ystart = 0;
  int xend = img.width;
  int yend = img.height;

  int matrixsize = 3;
  img.loadPixels();

  updateObservations();

  PImage timg = imgA;
  if (img == imgA) timg = imgB;
  timg.loadPixels();

  // Begin our loop for every pixel
  for (int x = xstart; x < xend; x++) {
    for (int y = ystart; y < yend; y++ ) {
      color c = convolution(x,y,matrix,matrixsize,img);
      int loc = x + y*img.width;
      timg.pixels[loc] = c;
    }
  }
  timg.updatePixels();

  background(timg);
  img = timg;
}

color convolution(int x, int y, float[][][] matrix,int matrixsize, PImage img)
{
  float rtotal = 0.0;
  float gtotal = 0.0;
  float btotal = 0.0;
  float atotal = 0.0;
  int offset = matrixsize / 2;
  for (int i = 0; i < matrixsize; i++){
    for (int j= 0; j < matrixsize; j++){
      // What pixel are we testing
      int xloc = x+i-offset;
      int yloc = y+j-offset;

      if (xloc >= 0 && xloc < img.width
        && yloc >= 0 && yloc < img.height)
      {
        int loc = xloc + img.width*yloc;

        // Calculate the convolution
        float gain = 0.95;
        rtotal = max(rtotal,red(  img.pixels[loc]) * matrix[0][i][j] * gain);
        gtotal = max(gtotal,green(img.pixels[loc]) * matrix[1][i][j] * gain);
        btotal = max(btotal,blue( img.pixels[loc]) * matrix[2][i][j] * gain);
        atotal = max(atotal,alpha(img.pixels[loc]) * matrix[3][i][j] * gain);
      }
    }
  }
  // Make sure RGB is within range
  rtotal = constrain(rtotal,0,255);
  gtotal = constrain(gtotal,0,255);
  btotal = constrain(btotal,0,255);
  atotal = constrain(atotal,0,255);
  // Return the resulting color
  return color(rtotal,gtotal,btotal,atotal);
}

