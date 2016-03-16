String inputString = "";
//4 levels of resolution per pixel
//or 2 bits
//a Long has 4 bytes allowing for
//1 panel of data per long
//Left to Right, per panel
//e.g. p0_0 (0,0)...(3,3), p1_0 (0,0)...(3,3)
const int panelSize = 16;
unsigned long panel[18];
int activePanel = 0;
void setup() {
  //seed random with input from analog pin to ensure
  //random number generation each sketch
  randomSeed(analogRead(0));
  
  // initialize serial:
  Serial.begin(9600);
  
  //Serial.println(String(testShift>>2, BIN));


  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  
  ///*debug only
  encode();
  //debug only */
  
  /*int a = 1;
  int b = 1;
  int c = 3;
  String thisString = String((c<<4)|(b<<2) | a, BIN);
  
  Serial.println(" ");
  Serial.print(panel[0]);Serial.print("=");
  Serial.println(String(panel[0], BIN));
  */
  int t = 0;
  encodePanelPixel(0,15,1);
  Serial.println(decodePanelPixel(0,15)/3.0);
}
void encodePanelPixel(int indexPanel, int indexPixel, int pixelVal){
  bitWrite(panel[indexPanel], indexPixel*2, bitRead(pixelVal, 0));
  bitWrite(panel[indexPanel], indexPixel*2+1, bitRead(pixelVal, 1));
}
int decodePanelPixel(int indexPanel, int indexPixel){
  int pixelVal = (bitRead(panel[indexPanel]>>indexPixel*2,0)==1)?bit(0):0;//position 0
  pixelVal += (bitRead(panel[indexPanel]>>indexPixel*2,1)==1)?bit(1):0;//position 1
  return pixelVal;
}
/*Debug example */
void encode(){
  int lastVal = 0;
  int totalVal = 0;
  for (int j=0; j < 18; j++){
    for (int i=0; i< panelSize; i++){
      totalVal = (random(4)<<i*2)|totalVal;
    }
    panel[j] = totalVal;
  }
}
void loop() {
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      if (inputString.indexOf("panel:") > -1){
        inputString.replace("panel:", "");
        activePanel = inputString.toInt();
        Serial.println(activePanel);
        inputString = "";
      }else{
        panel[activePanel] = inputString.toInt();
        Serial.println(decodePanelPixel(0,100));
        updatePanel();
      }
    }
  }
}
void updatePanel(){

}


