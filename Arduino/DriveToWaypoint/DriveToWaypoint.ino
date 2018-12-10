
int speedL;
int speedR;

void setup() {
  Serial.begin(9600);

}

void loop() {
int current_angle = 0;

int target_angle = 90;

int degrees_to_target = target_angle - current_angle;
Serial.print(degrees_to_target);
Serial.println(" degrees to target");
  
}
