#include <LiquidCrystal.h>    //Library for LCD Display
#include <Keypad.h>           //Library for Keypad

//Initializing LCD Pins with object "lcd"
LiquidCrystal lcd(27, 28, 29, 37, 36, 35);  //RS,EN,D4,D5,D6,D7 at PE1,PE2,PE3,PC4,PC5,PC6

//Built-in LED Indicators for answers
const int RED_led = 30; //At PF1
const int BLUE_led = 40; //At PF2

//////////////////////////////////////////////////////////////////////////
// Defining the Keymap layout
const byte ROWS = 4;      // Four rows
const byte COLS = 3;      // Three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

//Connect keypad ROW0, ROW1, ROW2 and ROW3 to these TivaC pins PB0, PB1, PE4, PE5
byte rowPins[ROWS] = { 3, 4, 5, 6 };
// Connect keypad COL0, COL1 and COL2 to these TivaC pins PB4, PA5, PA6
byte colPins[COLS] = { 7, 8, 9};

//Create the Keypad object
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#define pin_length 6              //Passcode/pin length+1
char Data[pin_length];            //Creating array of size of the passcode
char Master[pin_length] = "11111";    //Enter you student ID here
byte data_count = 0, master_count = 0;
bool Pass_is_good;            //flag to detect if password is correct
char customKey;               //stores the character entered by user on keypad
//////////////////////////////////////////////////////////////////////////

void setup()
{
    pinMode(27,OUTPUT);//Pins 8-14 are enabled as output for LCD
    pinMode(28,OUTPUT);
    pinMode(29,OUTPUT);
    pinMode(37,OUTPUT);
    pinMode(36,OUTPUT);
    pinMode(35,OUTPUT);

    pinMode(RED_led, OUTPUT);
    pinMode(BLUE_led, OUTPUT);
  
    //Initialize data to be printed on LCD whenever program starts
    lcd.begin(16, 2);               //Initializing LCD
    lcd.print("Entering");          //Prints in the first row
    lcd.setCursor(0,1);             //Set curor in second row, first column
    lcd.print("Safe Lock");         //Prints in the second row, first column
    delay(5000);                    //Left the data written on LCD for 5 seconds
    lcd.clear();                    //Clears everything on the LCD
}


void loop()
{
    safeLock();       //Function to execute Safe Lock logic, to be used in task02
    calculator();  //Function to execute calculator logic, to be used in task03
}

void safeLock(){
  lcd.setCursor(0,0);        //Set the cursor to initial position
  lcd.print("Enter Pin:");   //Prints in the first row of LCD

  customKey = customKeypad.getKey();  //Whenever key is pressed, the char is stored in customKey
  if (customKey){
    Data[data_count] = customKey;   //Store the character at the i'th index in Data array
    lcd.setCursor(data_count,1);    //Set cursor to second row and i'th column
    lcd.print(Data[data_count]);    //Prints character from keypad at the cursor position
    data_count++;                   //Increment the index for next iteration
  }

  //When the passcode length is reached, check whether it's correct or not
  if(data_count == pin_length-1){
    lcd.clear();      //clears all data from LCD to display results of passcode

    if(!strcmp(Data, Master)){
      lcd.print("Valid");
      digitalWrite(BLUE_led, HIGH);
      delay(3000);
      digitalWrite(BLUE_led, LOW);
      //If passcode is correct print "Valid" on LCD and turn on BLUE_led for 3 seconds below    
      }
    else{
      //If passcode is incorrect print "Invalid" on LCD and turn on RED_led for 3 seconds 
      lcd.print("Invalid");
      digitalWrite(RED_led, HIGH);
      delay(3000);  
      digitalWrite(RED_led, LOW);
      }
    
    lcd.clear();
    clearData();  
  }
}

//Functions clear all the data from Data array that we created to store keypad characters
void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}


void calculator(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calculator");
    delay(2000); // Display "Calculator" for 2 seconds

    // Variables to store input numbers and operation
    float num1, num2, result;
    char operation;

    // Input first number
    lcd.clear();
    lcd.print("First Number: ");
    num1 = getNumberInput();
    
    // Input operation
    lcd.clear();
    lcd.print("Operation: ");
    operation = getOperationInput();
    
    // Input second number
    lcd.clear();
    lcd.print("Second Number: ");
    num2 = getNumberInput();
    
    // Perform calculation
    if (operation == '*') {
        result = num1 * num2;
    } else if (operation == '#') {
        if (num2 == 0) {
            lcd.clear();
            lcd.print("Invalid");
            delay(2000);
            return; // Return to step 1
        }
        result = num1 / num2;
    } else {
        lcd.clear();
        lcd.print("Invalid");
        delay(2000);
        return; // Return to step 1
    }

    // Display result
    lcd.clear();
    lcd.print("Result: ");
    lcd.setCursor(0, 1);
    lcd.print(result);
    delay(5000); // Display result for 5 seconds
}

// Function to get a valid floating-point number input
float getNumberInput() {
    String input = "";
    while (true) {
        char key = customKeypad.getKey();
        if (key) {
            if (key == '#') {
                float num = input.toFloat();
                if (num != 0 || input == "0") {
                    return num;
                } else {
                    lcd.clear();
                    lcd.print("Invalid");
                    delay(2000);
                    return 0; // Return to step 1
                }
            } else {
                input += key;
                lcd.print(key);
            }
        }
    }
}

// Function to get a valid operation input
char getOperationInput() {
    while (true) {
        char key = customKeypad.getKey();
        if (key == '*' || key == '#') {
            return key;
        }
    }
}
