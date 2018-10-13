/**
* Compile the following code using mikroC compiler, burn to your device, connect the TSOP reciever to PORTC's 2nd pin
* and change the codes as per your remote. Works only for Sony devices. For other change the headers interval in scan remote.
*/

unsigned int scanRemote(void);

void doStuff(unsigned char value) {
  // glow leds
  // control rc vechiles
  // launch rockets
}

void main()
{
 unsigned short i;
 unsigned int data2=0;

 // The IR Reciever was connected to PortC -> Bit 2
 TRISC.B2=1;
 CCP1CON=0x04;
 T1CON=0x08;

 while(1)
 {
   data2=scanRemote();
   switch(data2)
   {
      // The following values 0x8877 etc. are 
      // different for different remotes and different button.
      // Check http://lirc.com for your remote    
      case 0x8877:{doStuff(0); break;}
      case 0x08F7:{doStuff(1); break;}
      case 0x28D7:{doStuff(2); break;}
      case 0x18E7:{doStuff(3); break;}
      case 0x38C7:{doStuff(4); break;}
      case 0xA857:{doStuff(5); break;}
      //default :{display(0xEE); break;}
   }
 }
}

unsigned int scanRemote(void)
{
    unsigned char digit=0;
    unsigned int data2=0;
    CCPR1H=0;
    CCPR1L=0;
    TMR1H=0;
    TMR1L=0;
    CCP1IF_bit=0;
    while(CCP1IF_bit==0);
    TMR1ON_bit=1;
    CCP1IF_bit=0;
    while(CCP1IF_bit==0);
    TMR1ON_bit=0;
    if((CCPR1H==0x83)||(CCPR1H==0x84))
    {
      TMR1H=0;
      TMR1L=0;
      TMR1ON_bit=1;
      while(digit<32)
      {
          CCP1IF_bit=0;
          while(CCP1IF_bit==0);
          TMR1ON_bit=0;
          TMR1H=0;
          TMR1L=0;
          if((CCPR1H==0x15)||(CCPR1H==0x16))
          {
            data2=data2<<1;
            data2++;
            TMR1ON_bit=1;
          }
          else if((CCPR1H==0x0A)||(CCPR1H==0x0B))
          {
            data2=data2<<1;
            TMR1ON_bit=1;
          }
          else
            break;
          digit++;
      }
      TMR1ON_bit=0;
    }
    return data2;
}
