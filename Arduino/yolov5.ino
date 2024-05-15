#include <hidboot.h>
#include <usbhub.h>
#include <Mouse.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

int v_x = 0;
int v_y = 0;
String received_code;
int index1;
int index2;
int index3;
int index4;

class MouseRptParser : public MouseReportParser
{
protected:
  void OnMouseMove  (MOUSEINFO *mi); 
  void OnLeftButtonUp (MOUSEINFO *mi);
  void OnLeftButtonDown (MOUSEINFO *mi);
  void OnRightButtonUp  (MOUSEINFO *mi);
  void OnRightButtonDown  (MOUSEINFO *mi);
  void OnMiddleButtonUp (MOUSEINFO *mi);
  void OnMiddleButtonDown (MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
  Mouse.move(mi->dX,mi->dY,0);
};
void MouseRptParser::OnLeftButtonUp (MOUSEINFO *mi)
{
    Mouse.release();
};
void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi)
{
    Mouse.press();
};
void MouseRptParser::OnRightButtonUp  (MOUSEINFO *mi)
{
    Mouse.release(MOUSE_RIGHT);
};
void MouseRptParser::OnRightButtonDown  (MOUSEINFO *mi)
{
    Mouse.press(MOUSE_RIGHT);
};
void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi)
{
    Mouse.release(MOUSE_MIDDLE);
};
void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi)
{
    Mouse.press(MOUSE_MIDDLE);
};

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

MouseRptParser                               Prs;

void setup()
{
    Serial.begin( 115200 );
    Mouse.begin();
#if !defined(_MIPSEL_)
    while (!Serial);
#endif
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay( 200 );
    HidMouse.SetReportParser(0, &Prs);


}

void loop()
{
  Usb.Task();
    while(Serial.available()){
    char c = Serial.read();
    if(c == '*'){
      index1 = received_code.indexOf(',');
      String x_direction = received_code.substring(0,index1);
      index2 = received_code.indexOf(',',index1+1);
      String x_value = received_code.substring(index1+1,index2+1);
      index3 = received_code.indexOf(',',index2+1);
      String y_direction = received_code.substring(index2+1,index3+1);
      index4 = received_code.indexOf(',',index3+1);
      String y_value = received_code.substring(index3+1);
      
      String x_d = String(x_direction);
      String y_d = String(y_direction);
      int x_v = x_value.toInt();
      int y_v = y_value.toInt();
      //Serial.print(x_d);
     //Serial.print(y_d);
      
      if(x_d == "p" && y_d=="p,"){
        while(x_v > 0 || y_v > 0){
          if(x_v > 0){
            Mouse.move(3,0,0);
            x_v -= 1;
          }
          if(y_v > 0){
            Mouse.move(0,3,0);
            y_v -= 1;
          }
        }
      }
      if(x_d == "p" && y_d=="n,"){
        while(x_v > 0 || y_v > 0){
          if(x_v > 0){
            Mouse.move(3,0,0);
            x_v -= 1;
          }
          if(y_v > 0){
            Mouse.move(0,-3,0);
            y_v -= 1;
          }
        }
      }
      if(x_d == "n" && y_d=="p,"){
        while(x_v > 0 || y_v > 0){
          if(x_v > 0){
            Mouse.move(-3,0,0);
            x_v -= 1;
          }
          if(y_v > 0){
            Mouse.move(0,3,0);
            y_v -= 1;
          }
        }
      }
      if(x_d == "n" && y_d=="n,"){
        while(x_v > 0 || y_v > 0){
          if(x_v > 0){
            Mouse.move(-3,0,0);
            x_v -= 1;
          }
          if(y_v > 0){
            Mouse.move(0,-3,0);
            y_v -= 1;
          }
        }
      }
      Mouse.click();
      received_code = "";
    }
    else{
      received_code += c;
    }
  }
}
