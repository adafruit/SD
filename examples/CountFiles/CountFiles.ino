#include <SD.h>

void setup() {
 Serial.begin(9600);
 //This is pin modes for Uno, please use 46 or your
 //board spesific pin here for test run.
 pinMode(10, OUTPUT);
 SD.begin(10);
 
 File root = SD.open("/");
 Serial.print("Files: ");
 Serial.println(root.count(1));
 Serial.print("Folders: ");
 Serial.println(root.count(2));
 Serial.print("Hidden Files: ");
 Serial.println(root.count(3));
 Serial.print("All files/folders: ");
 Serial.println(root.count(0));
 root.close();
}

void loop() {
  
}
