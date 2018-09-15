#include <ArduinoSTL.h>


class Entity{
    protected:
        String _name;
    public:
        void set_name(String);
        void DoSomthing(void);
};

void Entity::set_name(String name){
    _name = name;
}

class EntityDemo : public Entity{
    EntityDemo(String name);
    void DoSomthing(void);
};

EntityDemo::EntityDemo(String name){
   _name = name;
}

void EntityDemo::DoSomthing(void){
    
}




class SerialServer{
    private:
        String _input_string;
        boolean check_serial_data(void);
        void clean_input_string(void);
        void clean_serial_buff(void);
    public:
        SerialServer();
        void iter(void);
        void add_new_entity(EntityDemo entity);

};
SerialServer::SerialServer(){
    _input_string = "";
    Serial.begin(115200);
}
void SerialServer::clean_input_string(void){
    _input_string = "";
}

boolean SerialServer::check_serial_data(void){
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar == '\n') {
            return true;
        }
        _input_string += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
    }
    return false;
}

void SerialServer::clean_serial_buff(void){
    if (Serial.available()){
        Serial.read(); 
    }
}

void SerialServer::iter(void){
    check_serial_data();
}



SerialServer serial_server;


void setup(){
}

void loop(){
    
}
