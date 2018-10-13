/**
* A polling based IR decoder.
* Compile with MPLAB-C18 compiler, burn to your device, change the values per your device's buttons.
*/

#pragma config OSC = HS, OSCS = OFF
#pragma config PWRT = OFF, BOR = ON, BORV = 42
#pragma config WDT = OFF
#pragma config DEBUG = OFF, LVP = OFF, STVR = OFF

// my device, change your device name here
#include <P18F458.h>

void Delay(unsigned int);
unsigned int scanremote(void);

void main(void)
{
  unsigned int data=0;
  TRISB = 0;

  // The IR Reciever was connected to PortC -> Bit 2
  TRISCbits.TRISC2 = 1;
  CCP1CON = 0X04;
  T3CON = 0X0;
  T1CON = 0X0;
		
  while(1)
  {	
	data=scanremote();
	switch(data)
	{
      // The following values 0x178 etc. are 
      // different for different remotes and different button.
      // Check http://lirc.com for your remote    
		case (0x178):
					{
						doStuff(1);
						break;
					}
		case (0x578):
					{
						doStuff(2);
						break;
					}
		case (0x168):
					{
						doStuff(3);
						break;
					}
		case (0x538):
					{
						doStuff(4);
						break;
					}
		case (0x668):
					{
						doStuff(5);
						break;
					}
	}
  }
}
	
unsigned int scanremote(void)
{
 
  while(1)
  {
	unsigned char digit = 0;
	unsigned int data = 0x0;
	CCPR1H=0;
	CCPR1L=0;
	TMR1H=0;
	TMR1L=0;
	PIR1bits.CCP1IF=0;
	while(PIR1bits.CCP1IF==0);
	T1CONbits.TMR1ON=1;
	PIR1bits.CCP1IF=0;
	while(PIR1bits.CCP1IF==0);
	T1CONbits.TMR1ON=0;
	if(CCPR1H==0X1D||CCPR1H==0X1C)
	{
		TMR1H=0;
		TMR1L=0;
		PORTBbits.RB4=1;
		T1CONbits.TMR1ON=1;
		while(digit<11)
		{
			PIR1bits.CCP1IF=0;
			while(PIR1bits.CCP1IF==0);
			T1CONbits.TMR1ON=0;
			TMR1H=0;
		    TMR1L=0;
			if(CCPR1H==0X10||CCPR1H==0X11)
			{
				data=data<<1;
				data++;
				T1CONbits.TMR1ON=1;
				PORTBbits.RB5=1;
			}
			else if(CCPR1H==0X0B||CCPR1H==0X0A)
			{
				data=data<<1;
				T1CONbits.TMR1ON=1;
				PORTBbits.RB6=1;
			}
			else 
			{	
				break;
			}
			digit++;
		}
		T1CONbits.TMR1ON=0;
		return data;
	}
  }
}		

// There is no inbuilt delay() here so, writing our own.
// 165 was for crystal ocillator(XTAL) rated 48MHz
void Delay(unsigned int itime)
{
		unsigned int i;
		unsigned char j;
		for(i=0;i<itime;i++)
			for(j=0;j<165;j++); 
}

void doStuff(unsigned char value) {
  // glow leds
  // control rc vechiles
  // launch rockets
}
