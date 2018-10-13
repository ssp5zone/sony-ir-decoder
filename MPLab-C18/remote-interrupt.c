/**
* An interrupt based IR decoder.
*
* The advantage of using an interrupt is you dont have to worry about contiously reading and decoding the input.
* That will automatically happen whenever needed.  
*
* Change the values per your device's buttons, Compile with MPLAB-C18 compiler, burn to your device,
* connect the TSOP reciever at PortC -> Pin 2
*/

#pragma config OSC = HS, OSCS = OFF
#pragma config PWRT = OFF, BOR = ON, BORV = 42
#pragma config WDT = OFF
#pragma config DEBUG = OFF, LVP = OFF, STVR = OFF

#include <P18F458.h>

void Delay(unsigned int);
void remote(void);

unsigned char changed = 0;
unsigned int new_data = 0x0;
unsigned int current_data = 0x0;
unsigned char flag = 0;
unsigned char digit = 0;
unsigned char header = 0;
unsigned char count = 0;


// Mark this as an an Assembly routine lable: chk_isr 
#pragma interrupt chk_isr
void chk_isr(void)
{
	if(PIR1bits.CCP1IF==1)
		remote();
}

// The following is burnt at the interrupt vector table (IVT)
#pragma code Interrupted = 0x0008
void Interrupted(void)
{

	// Since the IVT space is small we keep our interrupt service routine elsewhere
	// and directly give an assembly reference to it. Only 1 line added in IVT block.
	_asm
	GOTO chk_isr
	_endasm
}
#pragma code

void main(void)
{
  
  // The IR Reciever was connected to PortC -> Bit 2
  TRISCbits.TRISC2 = 1;
  
  // configure timer
  CCP1CON = 0X04;
  T3CON = 0X0;
  T1CON = 0X0;

  // Enable interrupts
  PIE1bits.CCP1IE=1;
  INTCONbits.PEIE=1;
  INTCONbits.GIE=1;
  PIR1bits.CCP1IF=0;
		
  while(1)
  {	
  	// current_data holds the decoded remote code
  	doStuff(current_data);
  }
}
	
void remote(void)
{
 	
	if(flag==0)
	{
		TMR1H=0;
		TMR1L=0;
		T1CONbits.TMR1ON=1;
		CCPR1H=0;
		CCPR1L=0;
		flag=1;
	}
	
	else
	{
		T1CONbits.TMR1ON=0;
		if((CCPR1H==0X1C)||(CCPR1H==0X1D))  
		{
			header=1;
			TMR1H=0;
			TMR1L=0;
			CCPR1H=0;
			CCPR1L=0;
			T1CONbits.TMR1ON=1;
			
		}
		else if((header==1)&&((CCPR1H==0X10)||(CCPR1H==0X11)))
		{	
			new_data=new_data<<1;
			TMR1H=0;
			TMR1L=0;
			T1CONbits.TMR1ON=1;
			CCPR1H=0;
			CCPR1L=0;
			new_data++;
			digit++;
			
		}
	    else if((header==1)&&((CCPR1H==0X0B)||(CCPR1H==0X0A)))
		{	
			new_data=new_data<<1;
			TMR1H=0;
			TMR1L=0;
			T1CONbits.TMR1ON=1;
			CCPR1H=0;
			CCPR1L=0;
			digit++;
			
		}
		else
		{
				header=0;
				TMR1H=0;
				TMR1L=0;
				T1CONbits.TMR1ON=1;
				CCPR1H=0;
				CCPR1L=0;
				digit=0;
				new_data=0x0;			
		}
		if(digit==11)
		{	
			flag=0;
			digit=0;
			if(current_data!=new_data)
			{
				current_data=new_data;
				changed=1;
			}
			new_data=0x0;
			header=0;
		}
	}
	PIR1bits.CCP1IF=0;		
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

void doStuff(unsigned char remoteCode) {
	switch(remoteCode)
	{
		// The following values 0x178 etc. are 
      	// different for different remotes and different button.
      	// Check http://lirc.com for your remote    
		case (0x178):
					{
						// led on
						break;
					}
		case (0x578):
					{
						// led off
						break;
					}
		case (0x168):
					{
						// move vechile forward
						break;
					}
		case (0x538):
					{
						// launch missile
						break;
					}
		case (0x668):
					{
						// activate Terminator mode
						break;
					}
	}
}
