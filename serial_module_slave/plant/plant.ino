#include <Servo.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> jsonDoc;
JsonObject root = jsonDoc.to<JsonObject>();
JsonArray entity = root.createNestedArray("entity");


String input_string = "";

const String id = "{entity: [{ waterPlant: { get: [bool get01(void),bool get02(void)], set: [void set01(void), void set02(void)]}}]}";

enum Entity {Water_Plant, Sand_Watch};
enum StateMode {Work, Delay};
enum ControlMode {Automatic, Remote};


class NonBlockDelay{
    private:
        unsigned long delayEndPoint;
    public:
        bool Delay(int delayDuration);

};

bool NonBlockDelay::Delay(int delayDuration){
    //Non blocking command, When the time has pass it will return true
    if (delayEndPoint == 0){
        delayEndPoint = millis() + delayDuration;
    }
    if (millis() > delayEndPoint ){
        delayEndPoint = 0;
        return true;
    }else{
        return false;
    }
}

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

class MotorMove{
    //min  and max are the move range of the motor,
    //the class will translate it to percentage
    private:
        int step_delta_;
        int max_;
        int min_;
        int height_state_;
        Servo servo_;
    public:
        MotorMove(int min, int max) : min_(min), max_(max) {}
        void init(int pin);
        //Need to move here the setHeight
};

void MotorMove::init(int pin){
    servo_.attach(pin);
}


//********************** Water Plant functions *********************
class WaterPlant{
    private: 
        Entity entity_name_;
        int height_state_;  //TODO need to remove it from the class
        ControlMode control_mode_;
        int heightDelta;//TODO need to remove it from the class
        NonBlockDelay delay;

        void automation(void);
        void automationWork(void);
        void remote(void); //Not yet implemented
        void buildRoot(void);
    public:
        WaterPlant();
        void setHeight(int percentage);
        void iter(void);
        void init(int pin);
};

WaterPlant::WaterPlant(){
    entity_name_ = Water_Plant;
    control_mode_ = Automatic;
}

void buildRoot(void){
    root["name"] = "water_plant";
    JsonArray get = root.createNestedArray("get");
    JsonArray set = root.createNestedArray("set");
    get.add("setHeight(int, float)");

}

void WaterPlant::init(int pin){
    // I configure the init function to use the servo class,
    // if I use the servo class in the constractor it would wont work
    //_delayDuration = 1000; //60000*30; = 30 min
    height_state_ = 99; //Configure the stating point
    heightDelta = 5;
}

void WaterPlant::iter(void){
    switch(control_mode_){
        case Automatic:
            automation();
            break;
        case Remote:
            remote();
            break;
    }
}

void WaterPlant::automation(void){
    if (delay.Delay(1000)){
        automationWork();
    }
}

void WaterPlant::automationWork(void){
    setHeight(height_state_);
    height_state_ = height_state_ - heightDelta;
    if (height_state_ < 0)
        height_state_ = 0;
}


void WaterPlant::remote(void){
    if (delay.Delay(1000)){
        automationWork();
    }
}

void WaterPlant::setHeight(int percentage){
    // degree =  110 - percentage*7/20. 110 is the min and 75 is max
    //servo.write(91 - percentage*7/20);
    
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
Msg msg;

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
