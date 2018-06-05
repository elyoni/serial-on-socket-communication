#include <Servo.h>

String input_string = "";
const String id = "{entity: [{ waterPlant: { get: [bool get01(void),bool get02(void)], set: [void set01(void), void set02(void)]}}]}";

enum Entity {Water_Plant, Sand_Watch};
enum StateMode {Work, Delay};
enum ControlMode {Automatic, Remote};


class Msg{
    private:
        Entity entity;
        //Serial serialConnection;
        String msg;
        void readSerial(void);
        Entity getEntity(void);
        void eraseMsg(void);
    public:
        void iter(void); // Use read serial 
};

//********************** Water Plant functions *********************
class WaterPlant{
    private: 
        Servo servo;
        int heightState;
        StateMode stateMode;
        ControlMode controlMode;
        void set_height(int percentage);
        int heightDelta;
        unsigned long delayStartPoint;
        int _delayDuration;

        void automation(void);
        void automationWork(void);
        bool plantDelay(int delayDuration);
    public:
        void iter(void);
        void height100(void);
        void fullHeight(void);
        void middleHeight(void);
        void fullDown(void);
        void init(int pin);
};


void WaterPlant::init(int pin){
    stateMode = Work;
    controlMode = Automatic;
    //_delayDuration = 1000; //60000*30; = 30 min
    heightState = 99; //Configure the stating point
    heightDelta = 5;
    servo.attach(pin);
    servo.write(75);
}

void WaterPlant::iter(void){
    switch(controlMode){
        case Automatic:
            automation();
            break;
        case Remote:
            break;
    }
}

void WaterPlant::automation(void){
switch(stateMode){
    case Work:
        automationWork();
        break;
    case Delay:
        if (plantDelay(5000)){
            stateMode = Work;
        }
        break;
    }
}

void WaterPlant::automationWork(void){
    set_height(heightState);
    heightState = heightState - heightDelta;
    if (heightState < 0)
        heightState = 0;
    stateMode = Delay;
}

bool WaterPlant::plantDelay(int delayDuration){
    if (delayStartPoint == 0){
        delayStartPoint = millis();
    }
    if ((millis() - delayStartPoint) > delayDuration ){
        Serial.print("Moving, millis:");
        Serial.print(millis());
        Serial.print(" delaystartpoint:");
        Serial.println(delayStartPoint);
        delayStartPoint = 0;
        return true;
    }else{
        return false;
    }
}

void WaterPlant::set_height(int percentage){
    // degree =  110 - percentage*7/20. 110 is the min and 75 is max
    servo.write(91 - percentage*7/20);
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
    Serial.println("Starting");
    sandWatch.init(9);
    waterPlant.init(10);
    input_string.reserve(200);
}

void loop(){
    waterPlant.iter();
    /*
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
    }*/
}
