#pragma config(Sensor, S1, sonarSensor, sensorSONAR)
//This line of code has to be before any comments and at line 1 of any code!

/* Date: 14/12/2017
   Writers: George Dialektakis - Theodoros Apostolopoulos
   Project: Autonomous Car
   Phase: Milestone 3
   Platform: RobotC
   Technology: NXT Lego Mindstorms
   Description: An autonomous car that drives forwards looking for a suitable space to park on its right or left side. When the space is found,
                the car performs a series of steps to park autonomously.
*/


/*    motorA --> movement (rear wheels)
      motorB --> Ultrasonic sensor rotation
      motorC --> steering (front wheels)
      S1--> Ultrasonic Sensor
*/


//global variables
float R = 2.7;   //wheel radius (cm)
int car_length = 21; //cm
int car_width =  17; //cm

float motorB_ratio = 3.2;  //gears ratio  (necessarily float, we couldn't do otherwise!)
float motorC_ratio = 1.5;


//functions
int  move(int distance, bool direction, int obst_distance, bool rotateSensor);
void rotateSonar(int degrees, bool direction);
void rotateWheels(int degrees, bool direction);
void guide(int degrees, bool heading, int obst_distance);
int  find_parking(bool side);
void park(bool side);
int  check();



//main function
task main(){


  if(find_parking(true) == 1){
    // we have found a suitable parking space on the right side
    park(true); //let's park
  }
  //The end!
}



/*function for moving forwards or backwards
 for the given distance based on the direction argument*/
int move(int distance, bool direction, int obst_distance, bool rotateSensor){   //direction--> true = forwards and false = backwards
                                                                                //if rotateSensor is false, we must not rotate the sensor even if we are moving backwards
  int result = 0;   //return 0 if we traversed the given distance or -1 if an obstacle was met
  nMotorEncoder[motorA] = 0;   //reset counter
  nMotorEncoderTarget[motorA] = (distance*360)/(2*PI*R);   //for the specified 'distance'. This is a target in degrees to rotate.

  if(direction){
    motor[motorA] =  40;  //forwards
  }
  else{
    if(rotateSensor){
       rotateSonar(180,true);  //rotate sensor so that it looks backwards
    }
    motor[motorA] = -40;    //backwards
  }
  while(nMotorRunState[motorA] != runStateIdle && SensorValue[S1] > obst_distance) //as long as we haven't traversed the given distance and we haven't met an obstacle, keep moving
  {// while Motor A is still running (hasn't yet reached its target)
  }

  motor[motorA] = 0;   //if we met an obstacle stop moving any further

  if(SensorValue[S1] <= obst_distance){
    result = -1;   //car stoppped because it met an obstacle
  }

  if(!direction && rotateSensor){
    rotateSonar(180,false);  //calibrate sensor to starting point (0 degrees)
  }

  return result;
}



/*Function rotating the gear that the Ultrasonic Sensor is placed on,
for the given degrees and in the given direction*/
void rotateSonar(int degrees, bool direction){  //direction true = right rotation and false = left rotation

  nMotorEncoder[motorB] = 0;   //reset counter
  nMotorEncoderTarget[motorB] = motorB_ratio * degrees;
  if(direction){
    motor[motorB] =  100;  //right rotation
  }else{
    motor[motorB] = -100;  //left rotation
  }

  while(nMotorRunState[motorB] != runStateIdle)
  {// while Motor B is still running (hasn't yet reached its target):
  }

}



//rotates the front wheels for steering
void rotateWheels(int degrees, bool direction){  //direction true = right rotation and false = left rotation

  nMotorEncoder[motorC] = 0;  //reset counter
  nMotorEncoderTarget[motorC] = motorC_ratio * degrees;

  if(direction){
    motor[motorC] =  80;  //right rotation
  }else{
    motor[motorC] = -80;  //left rotation
  }

  while(nMotorRunState[motorC] != runStateIdle)
  {// while Motor C is still running (hasn't yet reached its target):
  }

}



 /* function for checking the left and right side of the car
  to find a path in order to overtake an obstacle */
int check(){

 rotateSonar(45,true);  //checking on the right side
 writeDebugStreamLine("Value right is: %d", SensorValue[S1]);
 if(SensorValue[S1] > 100){            //if the sensor doesn't <see> an obstacle within 1 meters on its right
   return 1;
 }else{
   rotateSonar(90,false);  //checking on the left side
   writeDebugStreamLine("Value left is: %d", SensorValue[S1]);
   if(SensorValue[S1] > 100){         //if the sensor doesn't <see> an obstacle within 1 meters on its left
     return 2;
   }else{                   //sensor didn't find any available path to overtake the obstacle
     rotateSonar(45,true);  //calibrate to the starting point
     writeDebugStreamLine("Value front is: %d", SensorValue[S1]);
     return 0;
   }
 }

}



//Function which makes the car look at the direction of the argument degrees
void guide(int degrees, bool heading, int obst_distance){    //heading = if we are moving forwards or backwards

  int position = 0;  //multiples of 45 degrees
  bool direction = true;   //true for turning right and false for left

  if(!heading){    //if we are heading backwards invert sign of degrees
    degrees = -degrees;
  }

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

  rotateWheels(55,direction);
  move(9*position,heading,obst_distance,false);    //9 cm distance with 55 degrees angle for the front wheels is enough to make the car look at 45 degrees
  rotateWheels(55,!direction);   //calibrate wheels to 0 degrees
}



// function that moves the car in a straight line and finds a suitable space to park
int find_parking(bool side){

  int distance_covered = 0;
  rotateSonar(90,side); //looking at the right or left side of the car

  while(true){ //run until you find a suitable parking space
    motor[motorA] = 40;

    while(SensorValue[S1] < 2*car_width){
      //just continue going forwards
    }

    nMotorEncoder[motorA] = 0;   //reset counter
    while(SensorValue[S1] >= 2*car_width){ //we found an empty space
    }
    motor[motorA] = 0;
    wait1Msec(500);
    distance_covered = nMotorEncoder[motorA];

    writeDebugStreamLine("Distance(degrees): %d", distance_covered);
    distance_covered = 2*PI*R*distance_covered/360;  //convert distance from degrees to cm
    writeDebugStreamLine("Distance(cm): %d", distance_covered);

    if(distance_covered >= 1.5*car_length){ //space must be at least 1.5 times the car's length long
      PlaySound(soundBeepBeep);  //we found it
      rotateSonar(90,!side);  //calibrate sensor's position to 0 degrees
      wait1Msec(4000);
      return 1;  //exit function
    }else{
      PlaySound(soundException);
    }
  }

}



//a function that parks the car either on its right or on its left side depending on the argument <side>, true = right and false = left
void park(bool side){

  if(side){
    //we are parking to our right
    rotateSonar(180,true);
    guide(-90,false,12);
    guide(90,false,12);
    rotateSonar(180,false);    //calibrate sensor to 0 degrees
    rotateWheels(10,true);
    move(5,true,12,true);
    rotateWheels(10,false);
    //we have parked
    PlaySound(soundBeepBeep);  //play sound so that we know we have parked
    wait1Msec(2000);
    //now it's time to leave
    rotateWheels(10,true);
    move(5,false,12,true);    //go a little back
    rotateWheels(10,true);
    rotateSonar(45,false);   //check on your left before you leave
    //leaving the parking space
    guide(-90,true,20);
    rotateSonar(45,true);    //calibrate sensor to 0 degrees
    move(60,true,30,true);

  }else{

    //we are parking to our left
    rotateSonar(180,false);
    guide(90,false,12);
    guide(-90,false,12);
    rotateSonar(180,true);     //calibrate sensor to 0 degrees
    rotateWheels(10,false);
    move(5,true,12,true);
    rotateWheels(10,true);
    //we have parked
    PlaySound(soundBeepBeep);  //play sound so that we know we have parked
    wait1Msec(2000);
    //now it's time to leave
    rotateWheels(10,false);
    move(5,false,12,true);     //go a little back
    rotateWheels(10,true);
    rotateSonar(45,true);     //check on your right before you leave
    //leaving the parking space
    guide(90,true,20);
    rotateSonar(45,false);    //calibrate sensor to 0 degrees
    move(60,true,30,true);
  }

}
