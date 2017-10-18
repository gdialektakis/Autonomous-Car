#pragma config(Sensor, S1, sonarSensor, sensorSONAR)
/* Date: 18/10/2017
   Editors: George Dialektakis - Theodoros Apostolopoulos
   Project: Autonomous Car
   Phase: Milestone 1
   Description: A car that is designed to drive itself forwards on a straight line until it meets an obstacle.Then it stops, 
   it moves back a little, makes a 180 degrees turn and continues straight for 1.5 meters or until it meets another obstacle.

*/

/*   motorA --> movement (rear wheels)
     motorB --> sonarSensor rotation
     motorC --> turning (front wheels)
     S1--> Ultrasonic Sensor

*/

task main()
{

   float R = 2.7;  //rear wheel radius
   int x = 30;     //distance to cover backwards when meeting an obstacle
   int distance = 30;   // Create variable 'distance' and initialize it to 30(cm).

   motor[motorA] = 80;
   while(SensorValue[S1] > distance)   // While the Sonar Sensor readings are less than the specified, 'distance':
   {
      writeDebugStreamLine("Sensor value is: %d", SensorValue[S1]);  //printing the readings of the ultraSonic sensor
      wait1Msec(50);
   }

   motor[motorA] = 0;    //when it meets an obstacle, it stops moving
   wait1Msec(500);

   nMotorEncoder[motorA] = 0;           //then it moves backwards
	 nMotorEncoderTarget[motorA] = (x*360)/(2*3.14*R);  //for the specified distance 'x'
   motor[motorA] = -80;

   while(nMotorRunState[motorA] != runStateIdle)
	// while Motor A is still running (hasn't yet reached its target):
 {
  // do not continue
 }

 nMotorEncoder[motorC] = 0;
 nMotorEncoderTarget[motorC] = 80 ;   //turning right
 motor[motorC] = 40;

 while(nMotorRunState[motorC] != runStateIdle)
 // while Motor C is still running (hasn't yet reached their target):
{
  // do not continue
}

motor[motorA] = 50;
wait1Msec(2200);


nMotorEncoder[motorC] = 0;
nMotorEncoderTarget[motorC] = 80 ;
motor[motorC] = -40;


while(nMotorRunState[motorC] != runStateIdle)
// while Motor C is still running (hasn't yet reached their target):
{
  // do not continue
}

	 nMotorEncoder[motorA] = 0;
	 nMotorEncoderTarget[motorA] = (150*360)/(2*3.14*R); //moves back for 1.5 meters
   motor[motorA] = 80;

   while(nMotorRunState[motorA] != runStateIdle  && SensorValue[S1] > distance )
	// while Motor A is still running (hasn't yet reached its target):
 {
  // do not continue
 }

/

}
