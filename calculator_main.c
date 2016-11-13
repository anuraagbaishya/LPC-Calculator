#include "LPC17xx.h"
#include "STRING_MANIPULATE.h"

//------LCD CONTROL--------
#define RS_CTRL  0x08000000  
#define EN_CTRL  0x10000000  
#define DT_CTRL  0x07800000  
//-------------------------

//---------FN DEFINITION---------
void scan(void)					;
void lcd_init(void)				;
void wr_cn(void)				;
void clr_disp(void)				;
void delay_lcd(unsigned int)	;
void lcd_com(void)				;						   
void wr_dn(void)				;
void lcd_data(void)				;
void clear_ports(void)			;
void lcd_puts(unsigned char *)	;
//-------------------------------

//-------------------GLOBAL DFN----------------------
unsigned char col, row, flag, sw					;
unsigned long int i, var1, temp, temp3, k			; 
unsigned long int temp1=0, temp2=0 					;
unsigned char tempz[16], tempx[16], blanks[16]		;
unsigned int mode_set = 0							;	
//---------------------------------------------------
 

//----------------------------------------------------ASCII STRINGS----------------------------------------------------------
unsigned char seven_code[4][4]={{0x31,0x32,0x33,0x2B}, {0x34,0x35,0x36,0x2D}, {0x37,0x38,0x39,0x2A} , {0x4D,0x30,0x3D,0x2F}};
unsigned char modify_btn[4][4]={{0x41,0x42,0x43,0x25}, {0x44,0x45,0x46,0x5E}, {0x68,0x6f,0x62,0x2E} , {0x4D,0x73,0x63,0x74}};
unsigned char modify_btn2[4][4]={{0x6C,0x21,0x30,0x30},{0x30,0x30,0x30,0x30}, {0x30,0x30,0x30,0x30},  {0x30,0x30,0x30,0x30}};
//---------------------------------------------------------------------------------------------------------------------------

 
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate(); 
	LPC_GPIO2->FIODIRL=0x3C00;
    lcd_init();				
	delay_lcd(3200);
	temp1 = 0x80;			
	lcd_com();
	delay_lcd(800);
	for(i = 0; i < 16; i++)
		blanks[i] = ' ';
	while(1)
	{
		for(row = 0; row<4; row++)
		{
			if(row == 0)
				temp = 0x00000400;
			else if(row == 1)
				temp = 0x00000800;
			else if(row == 2)
				temp = 0x00001000;
			else if(row == 3)
				temp = 0x00002000;
			LPC_GPIO2->FIOPIN = temp;
			flag= 0;
			scan();
			if(flag == 1 && mode_set == 0x02)
			{
				tempz[k++] = modify_btn2[row][col];
				lcd_puts(tempz);
				for(i = 0 ; i < 50000 ; i++);
				clear_ports();
				temp1 = 0x80;
				lcd_com();
				delay_lcd(800);
				mode_set = 0x00;
				break;				
			}
			if(flag == 1 && mode_set == 0X01)
			{
				if(modify_btn[row][col] == 0x4D)
				{
					for(i = 0; i < 5000; i++);
					mode_set = 0x02;
					break;
				}
				tempz[k++] = modify_btn[row][col];
				lcd_puts(tempz);
				for(i = 0 ; i < 50000 ; i++);
				clear_ports();
				temp1 = 0x80;
				lcd_com();
				delay_lcd(800);
				mode_set = 0x00;
				break;
			}
			if(flag == 1 && mode_set == 0x00)
			{
				if(seven_code[row][col] == 0x4D)
				{
					for(i = 0 ; i < 50000 ;i++);
					mode_set = 0x01;
					break;
				}
				tempz[k++]=seven_code[row][col];
				if(seven_code[row][col] == '=')
				{
					sw = 1;
					temp1 = 0xC0;
					lcd_com();
					delay_lcd(800);
					evaluate(tempz , tempx);
					i = 0;
					while(tempx[i] != '\0')
					{
						tempz[i] = tempx[i];
						i++;
					}
					k = i;
					while(i<16)
					{	
						tempz[i] = '\0';
						i++;
					}
					
				}
				else{
					temp1 = 0x80;
					lcd_com();
					delay_lcd(800);
				}
				lcd_puts(tempz);
				if(sw == 1){
					temp1 = 0x80;
					lcd_com();
					delay_lcd(800);
					lcd_puts(blanks);
					temp1 = 0x80;
					lcd_com();
					delay_lcd(800);
					sw = 0;
				}	
				for(i=0;i<50000;i++);
				clear_ports();
				break;	 
			}
		}
	}
}

 
void lcd_init()
{
	
	LPC_PINCON->PINSEL1 &= 0xFC003FFF;
 	LPC_GPIO0->FIODIR |= DT_CTRL;	
	LPC_GPIO0->FIODIR |= RS_CTRL;	
	LPC_GPIO0->FIODIR |= EN_CTRL;	
    clear_ports();
	delay_lcd(3200);
	temp2 = (0x30<<19);
	wr_cn();	   
	delay_lcd(30000); 
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);	 
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);
	temp2 = (0x20<<19);
	wr_cn();
	delay_lcd(30000);
	temp1 = 0x28;
	lcd_com();
	delay_lcd(30000);
	temp1 = 0x0C;		
	lcd_com();
	delay_lcd(800);
	temp1 = 0x06;
	lcd_com();
	delay_lcd(800);
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);
    return;
}
 
void scan(void)
{
	temp3=LPC_GPIO1->FIOPIN;
	temp3=temp3&0x07800000;
	if(temp3!=0x0)
	{
		flag=1;
		if(temp3==0x00800000)
			col=0;
		else if(temp3==0x01000000)
			col=1;
		else if(temp3==0x02000000)
			col=2;
		else if(temp3==0x04000000)
			col=3;
	}
}

void lcd_com(void)
{
	temp2 = temp1 & 0xf0;
	temp2 = temp2 << 19;
	wr_cn();
	temp2 = temp1 & 0x0f; 
	temp2 = temp2 << 23; 
	wr_cn();
	delay_lcd(1000);
    return;
}

void wr_cn(void)                        
{ 	 
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;		
    LPC_GPIO0->FIOCLR = RS_CTRL;		
	LPC_GPIO0->FIOSET = EN_CTRL;   	
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;		 		
    return;
}
 
void lcd_data(void)
{             
   temp2 = temp1 & 0xf0;
   temp2 = temp2 << 19;
   wr_dn();
   temp2= temp1 & 0x0f;	
   temp2= temp2 << 23;
   wr_dn();
   delay_lcd(1000);	
   return;
} 

void wr_dn(void)
{  	  
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;	   
	LPC_GPIO0->FIOSET = RS_CTRL;
	LPC_GPIO0->FIOSET = EN_CTRL;   
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
    return;
}

void delay_lcd(unsigned int r1)
{
  	unsigned int r;
  	for(r=0;r<r1;r++);
    return;
}

void clr_disp(void)
{
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
    return;
}

void clear_ports(void)
{
	LPC_GPIO0->FIOCLR = DT_CTRL;
	LPC_GPIO0->FIOCLR = RS_CTRL;
	LPC_GPIO0->FIOCLR = EN_CTRL;    
    return;
}

void lcd_puts(unsigned char *buf1)
{
    unsigned int i=0;
    while(buf1[i]!='\0')
    {
        temp1 = buf1[i];
     	lcd_data();
		i++;
        if(i==16)
		{
           	temp1 = 0xc0;
			lcd_com();
		}
         
    }
    return;
}
