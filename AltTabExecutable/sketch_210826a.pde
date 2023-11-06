import processing.serial.*;
Serial myPort;
String val;
String valRecent;
import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.KeyEvent;
import java.awt.*;
import java.awt.event.*;
import java.io.IOException;

Robot robot;
Boolean altTab = false;

void setup(){
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
  try { 
    //Runtime.getRuntime().exec("notepad");
    robot = new Robot();
  } catch (Exception e) {
    //e.printStackTrace();
    //exit();
    // do nothing  
  }
}

void serialEvent(Serial myPort){
 if (myPort.available() > 0) {
  val = myPort.readStringUntil('\n'); 
  val = val.trim();
  println(val);
  println(val.equals("MOTION"));
 }
 if (!(valRecent == val)){
   //println(val);
   valRecent = val;
 }
 if (val.equals("MOTION")){
   altTab = true;
   altTab();
 }
 delay(1);
}

void draw(){}

void altTab(){
  try{
    robot.keyPress(KeyEvent.VK_SPACE);
    delay(10);
    robot.keyRelease(KeyEvent.VK_SPACE);
    robot.keyPress(KeyEvent.VK_ESCAPE);
    delay(10);
    robot.keyRelease(KeyEvent.VK_ESCAPE);
    robot.keyPress(KeyEvent.VK_ALT);
    robot.keyPress(KeyEvent.VK_TAB);
    delay(10);
    robot.keyRelease(KeyEvent.VK_ALT);
    robot.keyRelease(KeyEvent.VK_TAB); 
  } catch (Exception e) {
    e.printStackTrace();
    print(e);
  }
}
