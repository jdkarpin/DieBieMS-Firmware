#include "modDisplay.h"

modDisplayInfoType modDisplayCurrentState;
uint32_t modDisplayLastRefresh;
uint32_t modDisplayStartupDelay;

extern modDisplayDataTypedef modDisplayData;

void modDisplayInit(void) {
	modDisplayStartupDelay = HAL_GetTick();
	libGraphicsInit(SSD1306_LCDWIDTH,SSD1306_LCDHEIGHT);
	while(!modDelayTick1ms(&modDisplayStartupDelay,STARTUPDELAY)) {};
	driverSWSSD1306Init(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
			
	modDisplayCurrentState = DISP_MODE_OFF;												//  Default content is nothing
	modDisplayLastRefresh = HAL_GetTick();
};

void modDisplayShowInfo(modDisplayInfoType newState, modDisplayDataTypedef modDisplayData) {
	static modDisplayDataTypedef modDisplayDataLast;
	
	if((modDisplayCurrentState != newState) || memcmp(&modDisplayDataLast,&modDisplayData,sizeof(modDisplayDataTypedef))) {											// Different state than last state?
		memcpy(&modDisplayDataLast,&modDisplayData,sizeof(modDisplayDataTypedef));
		switch(newState) {
			case DISP_MODE_OFF:
				driverSWSSD1306ClearDisplay();
				break;
			case DISP_MODE_SPLASH:
				driverSWSSD1306ClearDisplay();
				driverSWSSD1306FillBuffer(libLogos[LOGO_DIEBIE],SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH/8);
				libGraphicsSetTextSize(2);
				libGraphicsSetTextColor_0(WHITE);
				libGraphicsSetCursor(10,SSD1306_LCDHEIGHT/2+10);

				libGraphicsWrite('V');
				libGraphicsWrite('0');
				libGraphicsWrite('.');
				libGraphicsWrite('4');
				break;
			case DISP_MODE_LOAD:
				driverSWSSD1306ClearDisplay();
				driverSWSSD1306FillBuffer(libLogos[LOGO_LOAD],SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH/8);
				libGraphicsFillRect(7,7,(uint16_t)(modDisplayData.StateOfCharge/100*105),50,WHITE);
				break;
			case DISP_MODE_CHARGE:
				driverSWSSD1306ClearDisplay();
				driverSWSSD1306FillBuffer(libLogos[LOGO_CHARGING],SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH/8);  
				break;
			case DISP_MODE_POWEROFF:
				driverSWSSD1306ClearDisplay();
				libGraphicsSetTextSize(1);
				libGraphicsSetTextColor_0(WHITE);
				libGraphicsSetCursor(0,0);
			
				libGraphicsWrite('B');  
				libGraphicsWrite('y');  
				libGraphicsWrite('e');  
				break;
			case DISP_MODE_ERROR:
				driverSWSSD1306ClearDisplay();
				driverSWSSD1306FillBuffer(libLogos[LOGO_ERROR],SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH/8);  // Error logo uses half of the screen
			
				libGraphicsSetTextSize(1);
				libGraphicsSetTextColor_0(WHITE);
				libGraphicsSetCursor(68,0);																						// Display text on the other side of the screen
			
				libGraphicsWrite('E');  
				libGraphicsWrite('R');  
				libGraphicsWrite('R');  
				libGraphicsWrite('O');  
				libGraphicsWrite('R');  
				break;
			case DISP_MODE_EXTERNAL:
				driverSWSSD1306ClearDisplay();
				libGraphicsSetTextSize(1);
				libGraphicsSetTextColor_0(WHITE);
				libGraphicsSetCursor(0,0);
			
				libGraphicsWrite('E');  
				libGraphicsWrite('x');  
				libGraphicsWrite('t');  
				break;
			case DISP_MODE_BATTERY_DEAD:
				driverSWSSD1306ClearDisplay();
				driverSWSSD1306FillBuffer(libLogos[LOGO_BATTERY_DEAD],SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH/8);  
				break;
			case DISP_MODE_BALANCING:
				driverSWSSD1306ClearDisplay();
				driverSWSSD1306FillBuffer(libLogos[LOGO_CHARGING],SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH/8);
				libGraphicsSetTextSize(0);
				libGraphicsSetTextColor_0(INVERSE);
				libGraphicsSetCursor(35,0);
			
				libGraphicsWrite('B');  
				libGraphicsWrite('A');  
				libGraphicsWrite('L'); 
				libGraphicsWrite('A');  
				libGraphicsWrite('N');  
				libGraphicsWrite('C'); 
				libGraphicsWrite('I');  
				libGraphicsWrite('N');  
				libGraphicsWrite('G'); 
				break;
			case DISP_MODE_CHARGED:
				driverSWSSD1306ClearDisplay();
				driverSWSSD1306FillBuffer(libLogos[LOGO_BATTERY_FULL],SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH/8); 
			default:
				break;
		};
	}
	
	modDisplayCurrentState = newState;
};

void modDisplayTask(void) {
	driverSWSSD1306DisplayAsyncEfficient();
};
