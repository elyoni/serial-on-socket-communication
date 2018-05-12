#include <Servo.h>

String input_string = "";
const String id = "{entity: [{ waterPlant: { get: [bool get01(void),bool get02(void)], set: [void set01(void), void set02(void)]}}]}";

enum WaterPlantState {FullHeight, MiddleHeight, FullDown};
enum StateMode {Work, Delay}
enum ControlMode {Automatic, Remote};

//********************** Water Plant functions *********************
class WaterPlant{
    private: 
        Servo servo;
        WaterPlantState heightState;
        StateMode stateMode 
        ControlMode controlMode;
        void set_height(int percentage);
        int delayStartPoint;
        int delayDuration;

        void automation(void);
        void fullHeight(void);
        void middleHeight(void);
        void fullDown(void);
        void nextState(void);
    public:
        void init(int pin);
        void delay(void);
};


void WaterPlant::iter(void){
    switch(controlMode){
        case Automatic:
            automation()
            break;
        case Remote:
            break;
    }
}

void WaterPlant::nextState(void){
    
}

void WaterPlant::automation(void){
switch(heightState){
    case FullHeight:
        fullHeight();
        break;
    case MiddleHeight:
        middleHeight();
        break;
    case FullDown;
        fullDown();
        break;
    case Delay:
        break;
    }
}


void WaterPlant::delay(void){
    //TODO Need to add the delay aritmetic
    if ((millis() - delayStartPoint) > delayDuration ){
        //TODO move to the next state, maybe state++, I need to check what happend when overflow
    }
}
void WaterPlant::init(int pin){
    controlMode = Automatic;
    delayDuration = 60000*30;
    heightState = FullHeight;
    servo.attach(pin);
    servo.write(75);
}

void WaterPlant::set_height(int percentage){
    // degree =  110 - percentage*7/20. 110 is the min and 75 is max
    servo.write(110 - percentage*7/20);
}

void WaterPlant::fullHeight(void){
    switch(stateMode){
        case Work: 
            set_height(110);
            delay(500);
            set_height(100);
            delayStartPoint = millis()
            stateMode = Delay;
            break;
        case Delay:
            if (delay()){
                stateMode = Work;
            }
            break;
    }
}
void WaterPlant::middleHeight(void){
    switch(stateMode){
        case Work: 
            set_height(50);
            delayStartPoint = millis()
            stateMode = Delay;
            break;
        case Delay:
            if (delay()){
                stateMode = Work;
            }
            break;
    }
}

void WaterPlant::fullDown(void){
    switch(stateMode){
        case Work: 
            set_height(0);
            delayStartPoint = millis()
            stateMode = Delay;
            break;
        case Delay:
            if (delay()){
                stateMode = Work;
            }
            break;
    }
}

//********************** Sand watch functions *********************
class SandWatch{
    private: 
        int current_position;
        Servo sand_servo;
    public:
        SandWatch();
        void init(int sandWatch_pin);
        void flip(void);
};


SandWatch::SandWatch(){
    current_position = 0;
}

void SandWatch::init(int sandWatch_pin){
    sand_servo.attach(sandWatch_pin);  // attaches the servo on pin 9 to the servo object
}

void SandWatch::flip(void){
    if (current_position == 0){
        for (; current_position <= 180 ; current_position++){
            sand_servo.write(current_position);
            Serial.print(current_position);
            delay(1);
        }
    }else if (current_position == 180){
        for (; current_position >= 0 ; current_position--){
            sand_servo.write(current_position);
            delay(1);
        }
    }else{
        current_position = 0;
        sand_servo.write(current_position);
    }
    Serial.print("Current Position:");
    Serial.println(current_position);
}
//********************** Main functions *********************


void clean_serial_buff(void){
    if (Serial.available()){
        Serial.read(); 
    }
}


boolean new_serial_data(void){
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar == '\n') {
            return true;
        }
        input_string += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
    }
    return false;
}

WaterPlant waterPlant;
SandWatch sandWatch;

void setup(){
    Serial.begin(115200);
    sandWatch.init(9);
    waterPlant.init(10);
    input_string.reserve(200);
}

void loop(){
    if(new_serial_data()){
        Serial.println("********************************");
        Serial.println(input_string);
        Serial.println("********************************");
        if(input_string == "?id"){
            clean_serial_buff();
            Serial.println("this is my id");
            Serial.println(id);
        }else if(input_string == "up"){ 
            waterPlant.fullHeight();
        }else if(input_string == "down"){ 
            waterPlant.fullDown();
        }else if(input_string == "middle"){ 
            waterPlant.middleHeight();
        }else if(input_string == "flip"){
            sandWatch.flip();
        }else{
            Serial.print(input_string);
            Serial.println("What?");
        }
        input_string = "";
    }
}
