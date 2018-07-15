// si570.c
//
// version: 1.1
// date: 20/5/2008
// (c) 2008, Gerrrit Polder, PA3BYA
// http://www.agri-vision.nl
//
// control si570 using Edimax Linux based router
// based on Silicon Labs AN334
//
// compile with: gcc -lm -o si570 si570.c

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
//#include <linux/i2c.h>
#include <math.h>
#include <linux/i2c-dev.h>

//these must be floating point number especially 2^28 so that 
//there is enough memory to use them in the calculation 
#define POW_2_16              65536.0 
#define POW_2_24           16777216.0 
#define POW_2_28          268435456.0 
#define FOUT_START_UP	 56.320       //MHz 
#define FXTAL_DEVICE	 114.27902015
#define I2CPORT			"/dev/i2c-0"
#define DEBUG

// Si57x's fdco range -- DO NOT EDIT 
const float FDCO_MAX = 5670; //MHz 
const float FDCO_MIN = 4850; //MHz 
const unsigned char HS_DIV[6] = {11, 9, 7, 6, 5, 4};

const float fout0 = FOUT_START_UP; 

unsigned char n1; 
unsigned char initial_n1; 
unsigned char hsdiv; 
unsigned char initial_hsdiv;
unsigned long frac_bits; 
double rfreq; 
double fxtal; 
unsigned long initial_rfreq_long; 
unsigned long final_rfreq_long; 
int f;

//----------------------------------------------------------------------------- 
// Function PROTOTYPES 
//-----------------------------------------------------------------------------

void get_registers(unsigned char*);
int RunFreqProg(double);
unsigned char SetBits(unsigned char original, unsigned char reset_mask, 
                      unsigned char new_val); 
void usage(char*);


//----------------------------------------------------------------------------- 
// Main Routine 
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	int address = 0x55;      /* i2c bus address */
	int i;
	double frequency;
	unsigned char buf[7];
    int rflag = 0;
    int qflag = 0;
    int iflag = 0;
    int sflag = 0;
    char *svalue = NULL;
    int index;
    int c;
    int showusage;
     
    opterr = 1;

	if (argc < 2)
		showusage=1;
		
    while ((c = getopt (argc, argv, "rqs:ih")) != -1)
         switch (c)
           {
           case 'r':
             rflag = 1;
             break;
           case 'q':
             qflag = 1;
             break;
           case 's':
             sflag=1;
             svalue = optarg;
             break;
           case 'i':
             iflag = 1;
             break;
           case 'h':
             showusage=1;
             break;
/*            case '?': */
/*              if (optopt == 'c') */
/*                fprintf (stderr, "Option -%c requires an argument.\n", optopt); */
/*              else if (isprint (optopt)) */
/*                fprintf (stderr, "Unknown option `-%c'.\n", optopt); */
/*              else */
/*                fprintf (stderr, */
/*                         "Unknown option character `\\x%x'.\n", */
/*                         optopt); */
/*              return 1; */
           default:
             abort ();
    }
    
    if (showusage)
    	usage(argv[0]);
    	
	f = open(I2CPORT,O_RDWR);
	if (f>0) {
		ioctl(f,I2C_SLAVE,address);
    
    	if (rflag) {
			buf[0]=135;
			buf[1]=0x01;
			if (-1 == (write(f,buf,2))) { 
				printf("write error.\n");
			}    	
			buf[0]=7;
			if (-1 == (write(f,buf,1))) { 
				printf("write error.\n");
			}
			if (-1 == (read(f,buf,6))) { 
				printf("read error.\n");
			}
			get_registers(buf);
			if (fabs(fxtal - FXTAL_DEVICE) > 1e-7) {
				printf("Error: xtal frequency of device (%.12g) different from coded FXTAL_DEVICE (%.12g)\n", fxtal,FXTAL_DEVICE);
				exit(1);
			}
				
    	}
    	
		if (iflag) {
			buf[0]=7;
			if (-1 == (write(f,buf,1))) { 
				printf("write error.\n");
			}
			if (-1 == (read(f,buf,6))) { 
				printf("read error.\n");
			}
			get_registers(buf);
			//display stuff
			int counter;
			printf("reg7-12: ");
			for(counter=0;counter<6;counter++)
				printf("%02x ",buf[counter]);
			printf("\n");

			printf("hsdiv: %x  n1: %x  ",initial_hsdiv,initial_n1);
        	printf("rfreq: %f  fxtal: %f\n",rfreq,FXTAL_DEVICE);
        	printf("fout: %f\n",(FXTAL_DEVICE*rfreq)/(initial_hsdiv*initial_n1));

		}
		
		if (sflag) {
    		frequency = atof(svalue); /* frequency is the first number after the program name */
			if (qflag)
				frequency = frequency * 4;
        	if (RunFreqProg(frequency) == -1) {
				printf("Error: unable to set frequency: %f [Mhz]\n", frequency);
        	}	
        
		}
		
	} else {
		printf("Error. unable to open i2c port: %s\n", I2CPORT);
		exit(1);
	}
	
		
	return close(f);
}

//----------------------------------------------------------------------------- 
// Functions 
//-----------------------------------------------------------------------------
void usage(char* command) {
      	printf("\nUsage: %s [-rqsih] [frequency]\n\n", command);
      	printf("Control si570 VCXO on I2C bus\n\n");
      	printf("Options:\n\n");
      	printf("\t-r\treset si570 to initial frequency\n");
      	printf("\t-q\tquadrature mode for SDR (multiply frequency with 4)\n");
      	printf("\t-s\tset frequency [MHz]\n");
      	printf("\t-i\tprint information from si570 registers\n");
      	printf("\t-h\tprint this information\n\n");
}

void get_registers(unsigned char* reg) {
	
	// HS_DIV conversion 
	initial_hsdiv = ((reg[0] & 0xE0) >> 5) + 4; // get reg 7 bits 5, 6, 7 
	// initial_hsdiv's value could be verified here to ensure that it is one 
	// of the valid HS_DIV values from the datasheet. 
	// initial_n1 conversion 
	initial_n1 = (( reg[0] & 0x1F ) << 2 ) + // get reg 7 bits 0 to 4 
	(( reg[1] & 0xC0 ) >> 6 );  // add with reg 8 bits 7 and 8 
	if(initial_n1 == 0) 
	{ 
		initial_n1 = 1; 
	} 
	else if(initial_n1 & 1 != 0) 
	{ 
		// add one to an odd number 
		initial_n1 = initial_n1 + 1; 
	} 
	
	frac_bits = (( reg[2] & 0xF ) * POW_2_24 ); 
	frac_bits = frac_bits + (reg[3] * POW_2_16); 
	frac_bits = frac_bits + (reg[4] * 256); 
	frac_bits = frac_bits + reg[5]; 
	
	
	rfreq = frac_bits; 
    rfreq = rfreq / POW_2_28;
    rfreq = rfreq + ( (( reg[1] & 0x3F ) << 4 ) + (( reg[2] & 0xF0 ) >> 4 ) );
	fxtal = (fout0 * initial_n1 * initial_hsdiv) / rfreq; //MHz 
}

int RunFreqProg(double currentFrequency) 
{ 
	int i;
	float ratio = 0;
	unsigned char counter;
	unsigned char reg137;
	unsigned char buf[7]; 
	unsigned char reg[6]; 
	unsigned int divider_max;
	unsigned int curr_div; 
	unsigned int whole;
	unsigned char validCombo; 
	float curr_n1; 
	float n1_tmp; 
		
	// find dividers (get the max and min divider range for the HS_DIV and N1 combo) 
	divider_max = floor(FDCO_MAX / currentFrequency); //floorf for SDCC 
	curr_div = ceil(FDCO_MIN / currentFrequency); //ceilf for SDCC 
	validCombo = 0; 
	while (curr_div <= divider_max) 
	{ 
		//check all the HS_DIV values with the next curr_div 
		for(counter=0; counter<6; counter++) 
		{ 
			// get the next possible n1 value 
			hsdiv = HS_DIV[counter]; 
			curr_n1 = (curr_div * 1.0) / (hsdiv * 1.0); 
			// determine if curr_n1 is an integer and an even number or one
			// then it will be a valid divider option for the new frequency 
			n1_tmp = floor(curr_n1); 
			n1_tmp = curr_n1 - n1_tmp; 
			if(n1_tmp == 0.0) 
			{ 
				//then curr_n1 is an integer 
				n1 = (unsigned char) curr_n1; 
				if( (n1 == 1) || ((n1 & 1) == 0) ) 
				{ 
					// then the calculated N1 is either 1 or an even number 
					validCombo = 1; 
				} 
			} 
			if(validCombo == 1) break; 
		} 
		if(validCombo == 1) break; 
		//increment curr_div to find the next divider 
		//since the current one was not valid 
		curr_div = curr_div + 1; 
	} 

#ifdef DEBUG	
	printf("RunFreqProg -> ");
	printf("validcombo: %d  n1: %x hsdiv: %x fxtal: %f\n",validCombo,n1,hsdiv,fxtal);
#endif

	// if(validCombo == 0) at this point then there's an error 
	// in the calculatio. Check if the provided fout0 and fout1 
	// are not valid frequencies 
	// (old method) new RFREQ calculation -- kept for comparison purposes 
	
	if (validCombo == 0)
		return -1;
	rfreq = (currentFrequency * n1 * hsdiv) / FXTAL_DEVICE; //using float 
	for(counter = 0; counter < 6; counter++) 
	{
		reg[counter] = 0; //clear registers 
	} 
	
	// new HS_DIV conversion 
	hsdiv = hsdiv - 4; 
	//reset this memory 
	reg[0] = 0; 
	//set the top 3 bits of reg 13 
	reg[0] = (hsdiv << 5); 
	// convert new N1 to the binary representation 
	if(n1 == 1) n1 = 0; 
	else if((n1 & 1) == 0) n1 = n1 - 1; //if n1 is even, subtract one 
	// set reg 7 bits 0 to 4 
	reg[0] = SetBits(reg[0], 0xE0, n1 >> 2); 
	// set reg 8 bits 6 and 7 
	reg[1] = (n1 & 3) << 6; 
	
	// (old method) 
	// convert new RFREQ to the binary representation 
	// separate the integer part 
	whole = floor(rfreq); 
	// get the binary representation of the fractional part 
	frac_bits = floor((rfreq - whole) * POW_2_28); 
	// set reg 12 to 10 making frac_bits smaller by 
	// shifting off the last 8 bits everytime 
	for(counter=5; counter >=3; counter--) 
	{ 
		reg[counter] = frac_bits & 0xFF; 
		frac_bits = frac_bits >> 8;
	} 
	// set the last 4 bits of the fractional portion in reg 9 
	reg[2] = SetBits(reg[2], 0xF0, (frac_bits & 0xF)); 
	// set the integer portion of RFREQ across reg 8 and 9 
	reg[2] = SetBits(reg[2], 0x0F, (whole & 0xF) << 4); 
	reg[1] = SetBits(reg[1], 0xC0, (whole >> 4) & 0x3F); 
	
	//debug
	get_registers(reg);
    // tot hier gaat het goed !!!!
	//debug
	
	
	// Load the new frequency 
	// get the current state of register 137
	buf[0]=137;
	if (-1 == (write(f,buf,1))) { 
			printf("write error.\n");
	}
	if (-1 == (read(f,buf,1))) { 
			printf("read error.\n");
	}
 	reg137 = buf[0];
 	
	// set the Freeze DCO bit in that register 
	buf[0]=137;
	buf[1]=reg137 | 0x10;
	if (-1 == (write(f,buf,2))) { 
			printf("write error.\n");
	}
	
	// load the new values into the device at registers 7 to 12; 
	buf[0]=7;
	for (i=1;i<7;i++) {
		buf[i]=reg[i-1];
	}
	if (-1 == (write(f,buf,7))) { 
			printf("write error.\n");
	}


	// get the current state of register 137
	buf[0]=137;
	if (-1 == (write(f,buf,1))) { 
			printf("write error.\n");
	}
	if (-1 == (read(f,buf,1))) { 
			printf("read error.\n");
	}
 	reg137 = buf[0]; 
	// clear the FZ_DCO bit in that register
	buf[0]=137;
	buf[1]= reg137 & 0xEF;
	if (-1 == (write(f,buf,2))) { 
			printf("write error.\n");
	}
	
	// set the NewFreq bit, bit will clear itself once the device is ready 
	buf[0]=135;
	buf[1]= 0x40;
	if (-1 == (write(f,buf,2))) { 
			printf("write error.\n");
	}
} 

unsigned char SetBits(unsigned char original, unsigned char reset_mask, unsigned 
char new_val) 
{ 
return (( original & reset_mask ) | new_val ); 
} 

