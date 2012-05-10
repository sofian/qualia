
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

PImage img1, img2, imgA, imgB, imgC, imgD;
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
final int maxAgents = 20;
Mapper.Device.Signal sig_obs[] = new Mapper.Device.Signal[maxAgents];
Mapper.Device.Signal sig_act[] = new Mapper.Device.Signal[maxAgents];

int[][] agentPositions = new int[maxAgents][];

color[] colors = {color(255,0,0), color(0,255,0),
                  color(0,0,255), color(255,255,0)};

float[][] agentVelocities = new float[maxAgents][2];
boolean[] agentCharges = new boolean[maxAgents];

void setup() {
  size(200, 200, OPENGL);
  frameRate(30);
  imgA = createImage(200,200,ARGB);
  imgB = createImage(200,200,ARGB);
  imgC = createImage(200,200,ARGB);
  imgD = createImage(200,200,ARGB);
  img1 = imgA;
  img2 = imgB;
  imgA.loadPixels();
  for (int i=0; i<imgA.pixels.length; i++)
    imgA.pixels[i] = color(0,0,0,0);
  imgA.updatePixels();
  imgB.loadPixels();
  for (int i=0; i<imgB.pixels.length; i++)
    imgB.pixels[i] = color(0,0,0,0);
  imgB.updatePixels();
  imgC.loadPixels();
  for (int i=0; i<imgC.pixels.length; i++)
    imgC.pixels[i] = color(0,0,0,0);
  imgC.updatePixels();
  imgD.loadPixels();
  for (int i=0; i<imgD.pixels.length; i++)
    imgD.pixels[i] = color(0,0,0,0);
  imgD.updatePixels();
  
  for (int i=0; i<maxAgents; i++)
  {
    sig_obs[i] = dev.add_output("node/"+(i+1)+"/observation", 4, 'f', null, 0d, 1d);
    sig_act[i] = dev.add_input("node/"+(i+1)+"/position", 2, 'i', null, 0d, 200d,
      new ActionListener(i));
  }
}

void updatePositions()
{
  for (int i=0; i<maxAgents; i++)
  {
    if (agentPositions[i]!=null) {
      int x = agentPositions[i][0];
      int y = agentPositions[i][1];
      img1.pixels[x + img1.width*y] = ( agentCharges[i] ? color(255,255,255,255) : color(0,0,0,0) );
      img2.pixels[x + img2.width*y] = ( agentCharges[i] ? color(255,255,255,255) : color(0,0,0,0) );
    }
  }
}

class ActionListener extends InputListener
{
  final int i;
  ActionListener(int j) { i=j; }
  public void onInput(int [] v) {
    int [] pos = agentPositions[i];
    if (pos==null)
      agentPositions[i] = new int[2];
    agentPositions[i][0] = constrain(v[0], 0, img1.width-1);
    agentPositions[i][1] = constrain(v[1], 0, img1.height-1);;
  }
};

public void updateObservations()
{
    float obs[] = new float[5];
    for (int i=0; i<maxAgents; i++)
    {
      int [] pos = agentPositions[i];
      if (pos != null) {

        float r = 0;
        float [] o = observe(pos);
        for (int j=0; j<4; j++) {
          obs[i] = o[i];
          r += obs[i];
        }
        r /= 4;
        //obs[4] = (i % 2 == 0 ? r : -r); // reward
        obs[4] = r;

        sig_obs[i].update(obs);
      }
    }
}

float [] observe(int [] pos)
{
  float [] obs = new float [4];
  int x = constrain(pos[0], 1, img1.width-2);
  int y = constrain(pos[1], 1, img1.height-2);
  obs[0] = red  (img1.pixels[    x + img1.width * (y-1)]) / 255.0f;
  obs[1] = green(img1.pixels[(x-1) + img1.width *     y]) / 255.0f;
  obs[2] = red  (img2.pixels[    x + img2.width * (y+1)]) / 255.0f;
  obs[3] = green(img2.pixels[(x+1) + img2.width *     y]) / 255.0f;
  return obs;
}

void draw() {
  dev.poll(0);
  
  updatePositions();

  int xstart = 0;
  int ystart = 0;
  int xend = img1.width;
  int yend = img1.height;

  int matrixsize = 3;
  img1.loadPixels();
  img2.loadPixels();

  updateObservations();

  PImage timg1 = imgA, timg2 = imgC;
  if (img1 == imgA) {
    timg1 = imgB;
    timg2 = imgD;
  }
  timg1.loadPixels();
  timg2.loadPixels();

  // Begin our loop for every pixel
  for (int x = xstart; x < xend; x++) {
    for (int y = ystart; y < yend; y++ ) {
      int loc = x + y*img1.width;
      color c = convolution(x,y,matrix,matrixsize,0,img1);
      timg1.pixels[loc] = c;
      c = convolution(x,y,matrix,matrixsize,1,img2);
      timg2.pixels[loc] = c;
    }
  }
  timg1.updatePixels();
  timg2.updatePixels();

  background(0);

  loadPixels();
  for (int i=0; i < pixels.length; i++)
    pixels[i] =
      color((red(timg1.pixels[i]) + green(timg1.pixels[i])
            + red(timg2.pixels[i]) + green(timg2.pixels[i]))
            / 4);
  updatePixels();

  int [] pos;
  int i;
  for (i=0; i < maxAgents; i++)
  {
    pos = agentPositions[i];
    if (pos != null) {
      if (agentCharges[i])
        fill(255);
      else
        fill(0);
      ellipse(pos[0], pos[1], 15, 15);
    }
  }
  
  img1 = timg1;
  img2 = timg2;
}

color convolution(int x, int y, float[][][] matrix,int matrixsize, int n, PImage img)
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
        rtotal = max(rtotal,red(  img.pixels[loc]) * matrix[0+n*2][i][j] * gain);
        gtotal = max(gtotal,green(img.pixels[loc]) * matrix[1+n*2][i][j] * gain);
      }
    }
  }
  // Make sure RGB is within range
  rtotal = constrain(rtotal,0,255);
  gtotal = constrain(gtotal,0,255);
  // Return the resulting color
  return color(rtotal,gtotal,0,0);
}

