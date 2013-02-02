import Mapper.*;
import Mapper.Db.*;

final Device dev = new Device("polytopes", 9000);
Device.Signal observation;
Device.Signal observationTerminal;
Device.Signal action;

final int DIM_OBSERVATIONS = 144+48;
final int DIM_ACTIONS = 144;

int nextAction = 0;

void setup()
{
  size( 50, 50 );
  frameRate( 24 );
  colorMode(RGB, 255);
  initMapper();

//  observation.update( new float[] { 1 } );
}

void draw() {
  // Wait for agent to take action.
  dev.poll(100);
//  while (dev.poll(1) == 0);

  // Background.
  background(128,128,128);
  
  float[] obs = new float[DIM_OBSERVATIONS];
  for (int i=0; i<obs.length; i++) obs[i] = nextAction;
  observation.update( obs );
}

void initMapper() {
  
  action = dev.add_input("/node/1/action", DIM_ACTIONS, 'i', "", new Double(0.0), null, 
    new InputListener() {
      public void onInput(int[] x) {
        System.out.println("Receiving action: "+Arrays.toString(x));
        nextAction = x[0];
      }
    }
  );

  System.out.println("Input signal name: "+action.name());

  observation = dev.add_output("/node/1/observation", DIM_OBSERVATIONS, 'f', "", new Double(0.0), new Double(1.0));
  observationTerminal = dev.add_output("/node/1/observation_terminal", 1, 'i', "", new Double(0.0), new Double(1.0));
  
  System.out.println("Output signal index: "+observation.index());
  System.out.println("Zeroeth output signal name: "+dev.get_output_by_index(0).name());

  System.out.println("Waiting for ready...");
  while (!dev.ready ()) {
    dev.poll(100);
  }
  System.out.println("Device is ready.");

  System.out.println("Device name: "+dev.name());
  System.out.println("Device port: "+dev.port());
  System.out.println("Device ordinal: "+dev.ordinal());
  System.out.println("Device interface: "+dev.iface());
  System.out.println("Device ip4: "+dev.ip4());
}

