
/*First working bit of code to sequencially operate the LDASM in both traverse and rotational modes, using
 * the encoder rings on qwarterwave plates. This has shown  a 100% success operating over several days,
 * > 1000 operation on both rotational plate showing better than 2 micron accuracy.
 * 
 */




// the sensor communicates using SPI, so include the library:
#include <SPI.h>

 int chipSelectPin1=44;//address pin for quad decoder 
 int chipSelectPin2=42;
 long encoder2Value;
 long encoder1Value;
 long currentTime = 0;
 long nextTime = 0;
 long targetval = 15000;
 long encodermax= 0;
 int vern =15;
 long result;
 int x = 0; //variable for servo address
 int flag = 0; //variable for position flag
 int decodaddr = 0;//which Qwave encoder to read
  

void setup() {
  
  //Outputs
  pinMode(32, OUTPUT);//M1A1 ambient in 
  pinMode(35, OUTPUT);//M1B2 waveplate1 in
  pinMode(36, OUTPUT);//M2A1 waveplate1rot cw
  pinMode(37, OUTPUT);//M2A2 waveplate1rot ccw
  pinMode(38, OUTPUT);//M2B1 waveplate2 out
  pinMode(39, OUTPUT);//M2B2 waveplate2 in
  pinMode(40, OUTPUT);//M3A1 waveplate2rot cw
  pinMode(41, OUTPUT);//M3A2 waveplate2rot ccw
  pinMode(42, OUTPUT);//Enable Line for counter 1
  pinMode(43, OUTPUT);//Enable Line for Ethernet Card
  pinMode(44, OUTPUT);//Enable Line for Counter 2
  pinMode(46, OUTPUT);//Enable Reset
  
  //Inputs
  pinMode(24, INPUT);//Flag Load Home
  pinMode(25, INPUT);//Flag Load Ex
  pinMode(26, INPUT);//Qwave Plate 1 Home
  pinMode(27, INPUT);//Qwave Plate 1 Ex
  pinMode(28, INPUT);//Qwave Plate 1 Rot
  pinMode(29, INPUT);//Qwave Plate 2 Home
  pinMode(30, INPUT);//Qwave Plate 2 Ex
  pinMode(31, INPUT);//Qwave Plate 2 Rot



 
  //Analog PWM to motor drivers
 analogWrite(7,0 );

 //SPI initialization
 SPI.begin();
   
 Serial.begin(115200);
 Serial.println("LDASM Position Test");
  //pinMode(chipSelectPin1, OUTPUT);
  //pinMode(chipSelectPin2, OUTPUT);
 digitalWrite(chipSelectPin1, HIGH);
  //LS7366_Init();// this is coded into the ccw ref below
 delay(3000);
 //reset to ref all positions
 warmloadout();
 Qwaveplate1out();
 Qwaveplate2out(); 
 Qwaveplate1rotccw();
 Qwaveplate2rotccw();
 
}
//------------------------------------------------------------
void loop() 
{
 
 /*warmloadin();
 delay(2000);
 warmloadout();
 //delay(2000);
 Qwaveplate1in();
  delay(2000);
 Qwaveplate1out();
  delay(2000);
 Qwaveplate2in();
 */
 //delay(2000);
  Qwaveplate2rotcw();
  delay(2000);
  Qwaveplate2rotccw();
  delay(1000);
 
 //Qwaveplate2out();
   
  //Qwaveplate1rotccw();
  delay(2000);
  Qwaveplate1rotcw();
   
  delay(2000);
  Qwaveplate1rotccw();
  //delay(2000);
  //Serial.println(digitalRead (28));//qwaveplt1
  //Serial.println(digitalRead (31));//2
}
//-----------------------------------------------------------
void warmloadin()
{
 
 Serial.println("WarmLoad in");
 x = 32;//M1A1
 flag = 25;//Flag Load in

lateralmove();
}
//------------------------------------------------------
void warmloadout()
{

 Serial.println("WarmLoad Home");
 x = 33;//M1A2
 flag =24;//Flag Load Home
 lateralmove();

}
//------------------------------------------------- 
void Qwaveplate1in()
{
  
 Serial.println("QWP1 in");
 x = 35;//M1B1
 flag =26;//Flag QWP1 in
 lateralmove();
}
//---------------------------------------------------- 
void Qwaveplate1out()
{

 Serial.println("QWP1 Home");
 x = 34;//M1B2
 flag =27;//Flag QWP1 Home
 lateralmove();
}
//--------------------------------------------------------
void Qwaveplate2in()
{
 
 Serial.println("QWP2 in");
 x = 39;//M2B1
 flag =30;//Flag QWP2 in
 lateralmove();
}
//-----------------------------------------------------
void Qwaveplate2out()
{
 
 Serial.println("QWP2 Home");
 x = 38;//M2B2
 flag =29;//Flag QWP2 Home
 lateralmove();
}
//----------------------------------------------------
void Qwaveplate1rotcw()
{
 Serial.println("QWP1 rotating CW");
 x = 36;
 flag = 28;
 decodaddr = 42;
 if(digitalRead (flag) == HIGH)
    {
    //qwaveplaterot();
    qwaveplatepos();
    }
    else Qwaveplate1rotccw();
    //qwaveplatepos();
 Serial.println("finishedQWP1 rotating CW");
 }

//------------------------------------------------------
void Qwaveplate1rotccw()//sends qwaveplate back to ref
{

 Serial.println("QWP1 rotating CCW");
 x = 37;
    //Serial.println(x);
 flag = 28;
 decodaddr = 42;
 qwaveplateref();

}


//-----------------------------------------------------------------------------------
void Qwaveplate2rotcw()//rotates to arb position
{
 Serial.println("QWP2 rotating CW");
 x = 40;
 flag = 31;
 decodaddr = 44;
 if(digitalRead (flag) == HIGH)
    {
    //qwaveplaterot();
    qwaveplatepos();
    }
    else Qwaveplate2rotccw();
Serial.println("finishedQWP2 rotating CW");
    //qwaveplatepos();
}
//-----------------------------------------------------------
void Qwaveplate2rotccw()//Sends Qwaveplate2 back to ref
{
 Serial.println("QWP2 rotating CCW");
 //delay(1000);
 x = 41;
 flag = 31;
 decodaddr = 44;
 qwaveplateref();  

}
//-------------------------------------------------------------------------------------------
void lateralmove()
{
 digitalWrite(x, HIGH); 
    
 while(digitalRead (flag) == LOW)
     {
      analogWrite(7,50 );     
     }   
     
 analogWrite(7,0 ); 
 digitalWrite(x, LOW );  
  
}
//-------------------------------------------------------------
void qwaveplaterot()
{
 digitalWrite(x, HIGH);
 analogWrite(7, 50);
    
 delay(5000);
    
 analogWrite(7,0 );
 //Serial.println("QWP2stopped");
 EncoderValue();
 //encoder1Value = result;
 Serial.println("position");
 Serial.print(result);
 Serial.print("\r\n");
 delay(2000);
 digitalWrite(x, LOW );
}
//--------------------------------------------------------------
//Rotates the plate back to reference point at the absolute position of flag, this will zero out the counter
void qwaveplateref()
{
 Serial.println("finding reference");
 digitalWrite(x, HIGH);
    
 //delay(5000);
 //Serial.println(decodaddr);
 while(digitalRead (flag) == LOW)     
      {
      analogWrite(7, 50);
      }
 analogWrite(7,0 );
 delay(100);
 LS7366_Init();//reset counter
 Serial.println("Counter Reset");
   
 digitalWrite(x, LOW );

 EncoderValue();

 Serial.println(result);
 delay(2000);


}
//----------------------------------------------------------------------
void qwaveplatepos()
{
 EncoderValue();
 Serial.print("Encoder = ");
 Serial.println(result);
        

     
 currentTime = millis();//take current time(ms) since program started 
       
 digitalWrite(x, HIGH);
 //while (result < targetval)
 while(result < targetval)
      {
        analogWrite(7, 50);
        EncoderValue();
        Serial.println(result);
       // if((currentTime + 15000) < millis() or encoder1Value > 33500)//sets limit in time and distance
        if(millis() > (currentTime + 15000) or result > 33000)//sets limit in time and distance
               {
                Serial.println("Time out");
                delay(1000);
                analogWrite(7,0 );
                digitalWrite(x, LOW);
          
                break;
           
               }
      }
 analogWrite(7,0 );
 digitalWrite(x, LOW);
 Serial.print("stopped");
 delay(100);
}



// LS7366 Initialization and configuration
//*************************************************
void LS7366_Init(void)
//*************************************************
{
   
    
   
   //SPI.setClockDivider(SPI_CLOCK_DIV16);      // SPI at 1Mhz (on 16Mhz clock)
   delay(10);
   
   digitalWrite(decodaddr,LOW);
   //SPI.transfer(0x88); 
   SPI.transfer(0xE0);//resets counter
   
   SPI.transfer(0x22); //note Z ref not swapped in polarity
   //SPI.transfer(0x83);//set to x4 quad and sync index
   //0x03?
   digitalWrite(decodaddr,HIGH); 
   delay(100);
  
   
}//end func

//*****************************************************  
//long getEncoderValue(int encoder)
//long getEncoderValue()
 void EncoderValue()
//*****************************************************
{
    unsigned int count1Value, count2Value, count3Value, count4Value;
    //long result;
    
    //selectEncoder(encoder);
    digitalWrite(decodaddr,LOW);
    //digitalWrite(5,LOW);
    SPI.transfer(0x63); // Request count
    count1Value = SPI.transfer(0x00); // Read highest order byte
    count2Value = SPI.transfer(0x00);
    count3Value = SPI.transfer(0x00);
    count4Value = SPI.transfer(0x00); // Read lowest order byte
    //digitalWrite(5,HIGH);
    digitalWrite(decodaddr,HIGH);

    //deselectEncoder(encoder);
    
    result= ((long)count1Value<<24) + ((long)count2Value<<16) + ((long)count3Value<<8) + (long)count4Value;
    
    //return result;
}//end func
//----------------------------------------------------------


//-------------------------------------------------------
