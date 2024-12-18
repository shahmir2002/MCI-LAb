////////////////////////////////////////////////////
//Ultrasonic Pins Definition, here we specify pin usage for our tivaC Board

const int echo = 9, Trig = 10;  //Pin connections for HC-SR04
//const int LED1 = 11;          //Specify Pin for LED1


////////////////////////////////////////////////////
//Time Update Variables

long duration, cm;                          // To measure pulse width and distance in cm
long distance = 0;                          // To return distance of object
int numReadings = 0;                        // Counter for the number of readings
long totalDist = 0;                         // Cumulative sum of distances
long avgDist ;   
unsigned long LastUpdateMicrosecs = 0;    
unsigned long LastUpdateMillisecs = 0;
unsigned long CurrentMicrosecs = 0;
unsigned long MicrosecsSinceLastUpdate = 0;
float SecondsSinceLastUpdate = 0;


////////////////////////////////////////////////////
//Initializing Pins Function

void setup()
{
  Serial.begin(115200);     //Initialize Serial COM Port with 115200 buad rate
  pinMode(Trig, OUTPUT);    //Trigger is output since it will send 8 pulses at 40KHz from HC-SR04
  pinMode(echo, INPUT);     //Echo is input since it will detect the reflected wave from HC-SR04
  //pinMode(LED1, OUTPUT);    //Pin for LED1 should output HIGH(on) or LOW(off)
}


////////////////////////////////////////////////////
//Main Loop that will keep on running, it is similar to int main()
void loop() {
    // Reset the cumulative variables before taking new readings
    totalDist = 0;
    numReadings = 0;

    // Collect 10 readings and accumulate the total distance
    for (int i = 0; i < 10; i++) {
        totalDist+=Update_Ultra_Sonic(); // Take a single reading
        numReadings++;
        delay(100);           // Delay between readings (100ms)
    }
    
    // Calculate the average distance
    avgDist = totalDist / numReadings;

    // Display the averaged distance on Serial Monitor
    Serial.print("Average Distance = ");
    Serial.print(avgDist);
    Serial.println(" cm");

    delay(1000); // Delay before starting the next measurement cycle
}
////////////////////////////////////////////////////
//Will update ultrasonic sensors through Serial port

long Update_Ultra_Sonic()
{    //Setting Trig Signal HIGH for 10us to produce burst of 8 pulses at 40KHz
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    duration = pulseIn(echo, HIGH);   // Reads duration (microseconds) for which Echo pin reads HIGH till wave is reflected

    
  //digitalRead(anypin/switch) //Use this function to read the state of any Pin/Switch i.e. SW1 and SW2
  
    cm = microsecondsToCentimeters(duration); // convert the time into a distance, refer to function below
    
  //Sending through serial port to print on Monitor
    Serial.print("Distance=");
    Serial.print("\t");
    Serial.print(cm);
    Serial.print("\n");

    return cm;
}

////////////////////////////////////////////////////
//Conversion of microsecond to centimeter
long microsecondsToCentimeters(long microseconds)
{
   distance = 0.0343*microseconds;
   

return distance / 2;
}
