#include <Servo.h>

String input_string = "";
const String id = "{entity: [{ water_plant: { get: [bool get01(void),bool get02(void)], set: [void set01(void), void set02(void)]}}]}";



//********************** Water Plant functions *********************
class WaterPlant{
    private: 
        int water_plant_pin;
            
    public:
        void init(int pin);
        void set_height(int percentage);
        void full_height(void);
        void full_down(void);
};
void WaterPlant::init(int pin){
    water_plant_pin = pin;
    pinMode(water_plant_pin, OUTPUT);
}

void WaterPlant::set_height(int percentage){
   analogWrite(water_plant_pin, percentage);
}

void WaterPlant::full_height(void){
   analogWrite(water_plant_pin, 220);
}

void WaterPlant::full_down(void){
   analogWrite(water_plant_pin, 170);
}

//********************** Sand watch functions *********************
class SandWatch{
    private: 
        int current_position;
        int sand_watch_pin;
        Servo sand_servo;
    public:
        SandWatch();
        void init(int sand_watch_pin);
        void flip(void);
};


SandWatch::SandWatch(){
    current_position = 0;
}

void SandWatch::init(int sand_watch_pin){
    sand_servo.attach(sand_watch_pin);  // attaches the servo on pin 9 to the servo object
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

WaterPlant water_plant;
SandWatch sand_watch;

void setup(){
    Serial.begin(115200);
    sand_watch.init(9);
    water_plant.init(10);
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
            water_plant.full_height();
        }else if(input_string == "down"){ 
            water_plant.full_down();
        }else if(input_string == "flip"){
            sand_watch.flip();
        }else{
            Serial.print(input_string);
            Serial.println("What?");
        }
        input_string = "";
    }
}
