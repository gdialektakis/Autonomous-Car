#pragma config(Sensor, S1, sonarSensor, sensorSONAR)
//This line of code has to be before any comments and at line 1 of any code!

/* Date: 14/11/2017
   Writers: George Dialektakis - Theodoros Apostolopoulos
   Project: Autonomous Car
   Phase: Milestone 2
   Platform: RobotC
   Technology: NXT Lego Mindstorms
   Description: An Autonomous Car designed to avoid any obstacle on its way by checking if it's possible to overtake it from the right or the left side. If it doesn't find a path to ovecome the obstacle,
                   it chooses to go back and continue its movement in free space.
*/


/*   motorA --> movement (rear wheels)
     motorB --> sonarSensor rotation
     motorC --> steering (front wheels)
     S1--> Ultrasonic Sensor
*/


//global variables
float R = 2.7;   //wheel radius
int obst_distance = 40;  //distance away from an obstacle to stop at

//functions
int move(int distance, bool direction);   //function for moving forwards or backwards for the given distance based on the direction argument
int check();   //function for checking the left and right side of the car to find a path in order to overtake an obstacle
void rotateSonar(int degrees, bool direction);   //rotates the gear that the Ultrasonic Sensor is placed on, for the given degrees and in the given direction
void rotateWheels(int degrees, bool direction);  //rotates the front wheels for steering
void guide(int degrees);  //make the car look at the direction of the argument degrees



//main function
task main(){

int result  = 0;  //Here we store the returned value of the function check()
int obstmet = 0;  //Here we store the returned value of the function move()

   while(true){   //runs forever

     obstmet = move(100,true);

     if(obstmet == -1){   //if the car met an obstacle
       result = check();  //check each side for available space to overtake the obstacle

       if(result == 1){  //overtake obstacle from the right
         guide(90);
         rotateSonar(45,true);   //calibrate Sensor to look forwards
         move(40,true);
         guide(-90);
         move(40,true);
       }else if(result == 2){   //overtake obstacle from the left
         guide(-90);
         rotateSonar(45,false);     //calibrate Sensor to look forwards
         move(40,true);
         guide(90);
         move(40,true);
       }else{
         move(20,false);  //go back
         guide(180);     //move to the opposite direction
         move(20,true);
       }
     }
   }
//end of program
}



int move(int distance, bool direction){  //direction--> true = forwards and false = backwards
  int result = 0;  //return 0 if we traversed the given distance or -1 if an obstacle was met
  nMotorEncoder[motorA] = 0;  //reset counter
  nMotorEncoderTarget[motorA] = (distance*360)/(2*PI*R);  //for the specified 'distance'. This is a target in degrees to rotate.

  if(direction){
    motor[motorA] =  50;  //forwards
  }
  else{
    rotateSonar(180,true);  //rotate sensor so that it looks backwards
    motor[motorA] = -50;    //backwards
  }
  while(nMotorRunState[motorA] != runStateIdle && SensorValue[S1] > obst_distance) //as long as we haven't traversed the given distance and we haven't met an obstacle, keep moving
  {// while Motor A is still running (hasn't yet reached its target)
  }

  if(SensorValue[S1] <= obst_distance){
    result = -1;   //car stoppped because it met an obstacle
    writeDebugStreamLine("Sensor final value: %d", SensorValue[S1]);
  }

  if(!direction){
    rotateSonar(180,false);  //calibrate sensor to starting point (0 degrees)
  }
  motor[motorA] = 0;
  return result;
}


void rotateSonar(int degrees, bool direction){  //direction true = left rotation and false = right rotation

  nMotorEncoder[motorB] = 0;   //reset counter
  nMotorEncoderTarget[motorB] = 56 * degrees;  // 1 rotation of Ultrasonic sensor equals 56 rotations of motorB
  if(direction){
    motor[motorB] =  100;  //right rotation
  }else{
    motor[motorB] = -100;  //left rotation
  }

  while(nMotorRunState[motorB] != runStateIdle)
  {// while Motor B is still running (hasn't yet reached its target):
  }

}


void rotateWheels(int degrees, bool direction){  //direction true = right rotation and false = left rotation

  nMotorEncoder[motorC] = 0;  //reset counter
  nMotorEncoderTarget[motorC] = (3/2) * degrees;  // 3/2 is the ratio of the two gears responsible for turning the front wheels

  if(direction){
    motor[motorC] =  50;  //right rotation
  }else{
    motor[motorC] = -50;  //left rotation
  }

  while(nMotorRunState[motorC] != runStateIdle)
  {// while Motor C is still running (hasn't yet reached its target):
  }

}


int check(){

 rotateSonar(45,false);  //checking on the right side
 writeDebugStreamLine("Value right is: %d", SensorValue[S1]);
 if(SensorValue[S1] > 150){            //if the sensor doesn't <see> an obstacle within 1.5 meters on its right
   return 1;
 }else{
   rotateSonar(90,true);  //checking on the left side
   writeDebugStreamLine("Value left is: %d", SensorValue[S1]);
   if(SensorValue[S1] > 150){         //if the sensor doesn't <see> an obstacle within 1.5 meters on its left
     return 2;
   }else{                   //sensor didn't find any available path to overtake the obstacle
     rotateSonar(45,false);  //calibrate to the starting point
     writeDebugStreamLine("Value front is: %d", SensorValue[S1]);
     return 0;
   }
 }

}


void guide(int degrees){
  int position = 0;  //multiples of 45 degrees
  bool direction = true;   //true for turning right and false for left

  if(degrees == 45){
    position = 1;
    direction = true;
  }else if(degrees == 90){
    position = 2;
    direction = true;
  }else if(degrees == 135){
    position = 3;
    direction = true;
  }else if(degrees == -45){
    position = 1;
    direction = false;
  }else if(degrees == -90){
    position = 2;
    direction = false;
  }else if(degrees == -135){
    position = 3;
    direction = false;
  }else if(degrees == 180){
    position = 4;
    direction = true;
  }

  rotateWheels(70,direction);
  move(7*position,true);     //7cm distance with 70 degrees angle for the front wheels is enough to make the car look at 45 degrees
  rotateWheels(70,!direction);  //calibrate wheels to 0 degrees
}
