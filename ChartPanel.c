

//----------------------------------------------------------------------------
// Thread portion of this demo.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
double d ;
int panel;
int panelHandle; 
int ImgNum=0 ; 
int mainPanelHandle; 
char  CamName;    
#include "UTC v1.h"   
#include <cvidef.h>
#if defined (_NI_mswin32_) && _NI_mswin32_
#include <windows.h>
#include <utility.h>
#include "NIIMAQdx.h"
#include "nivision.h"
#elif defined (_NI_linux_) && _NI_linux_
#include <pthread.h>
#endif
#include <cvirte.h> 
#include <userint.h>
#include <ansi_c.h>
#include "ChartPanel.h"
#include "MultiPanel.h" 
static PointFloat *rect;
static ROI* roi;
static Image *ExtrctdImage;
static int camNameIndex;
static char SaveImageDirectory[300];

static IMAQdxSession session = 0;
static IMAQdxAttributeInformation* attributeArray = NULL;
static Image* image = NULL;

// prototypes
void DisplayError(IMAQdxError error);
IMAQdxError FormatAttributeNameControl(void);
extern int CVICALLBACK SNAP2 (int SnapInstance); 
extern void StopCamera (void);
extern CameraAttributesControl(int AttrVal);

  
// constants
const char* DefaultAttributeRoot = "CameraAttributes";
const IMAQdxAttributeVisibility DefaultAttributeVisibility = IMAQdxAttributeVisibilitySimple;


//----------------------------------------------------------------------------
// ChartPanelMain
//----------------------------------------------------------------------------
int CVICALLBACK ChartPanelMain (void * data)
{
   
	//int panel, intValue; 
    char buffer[256];

    
    if ((panel = LoadPanelEx (0, "ChartPanel.uir", PANEL, __CVIUserHInst)) < 0)
        return panel;
        
    // Offset panel so user sees it better    
    /*GetPanelAttribute (panel, ATTR_TOP,  &intValue);    
    SetPanelAttribute (panel, ATTR_TOP,  intValue +(20*((int)(intptr_t)data%10)));    
    GetPanelAttribute (panel, ATTR_LEFT, &intValue);    
    SetPanelAttribute (panel, ATTR_LEFT, intValue +(20*((int)(intptr_t)data%10)));   */ 
        
    // Set sleep policy for thread
    SetSleepPolicy (VAL_SLEEP_MORE);
    
    // Change the Panel Title
    GetPanelAttribute (panel, ATTR_TITLE, buffer);
#if defined (_NI_mswin32_) && _NI_mswin32_
    sprintf (buffer, "%s (Thread ID=0x%x)", buffer, GetCurrentThreadId ());
#elif defined (_NI_linux_) && _NI_linux_
    sprintf (buffer, "%s (Thread ID=0x%x)", buffer, pthread_self ());
#endif
    SetPanelAttribute (panel, ATTR_TITLE, buffer);
	
	
	
	
	
/////////////////////////////////initialize camera//////////////////////////
			//	char			camName[64];
    		IMAQdxError     error = IMAQdxErrorSuccess;
	
	
			GetCtrlIndex (mainPanelHandle, MAINPANEL_CHOOSECAMERA, &camNameIndex);
			
		
					if(camNameIndex==0)
				error = IMAQdxOpenCamera ("cam1", IMAQdxCameraControlModeController, &session); 
			  	 	if(camNameIndex==1)  
			 	error = IMAQdxOpenCamera ("cam2", IMAQdxCameraControlModeController, &session);  
		
		
			// Open a session to the selected camera
			
		/*	if (error) 
			{
				break;
			}*/	
	
			// Create an image
			image = imaqCreateImage (IMAQ_IMAGE_U8, 0);
			
			// Configure and start the acquisiton
			error = IMAQdxConfigureGrab (session);
		/*	if (error)
			{
				break;
			}   */
			
			// Format attribute name control
		//	error = FormatAttributeNameControl();
			/*if (error)
			{
				break;
			} */

	
			SetCtrlAttribute (panel, PANEL_TIMER, ATTR_ENABLED, TRUE);
			QuickAttrSetup (0, 0, 1,0, 0, 0);
	
	
	
	
////////////////////////////////////////////////////////
	
	
	
	
	
	
	
	
	
	
	
	
	
    
    // Run UIR in thread
   // DisplayPanel (panel);
    RunUserInterface ();
    
    // Discard thread UIR
    DiscardPanel (panel);
    
    return 0;
}


//----------------------------------------------------------------------------
// ChartPanelTimerCallback
//----------------------------------------------------------------------------
int CVICALLBACK ChartPanelTimerCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
 
	
//	//	static double lastTick = 0;
//	double newTick;
	//static unsigned int lastBufferNumber = - 1;
	unsigned int bufferNumber;
	IMAQdxError error = IMAQdxErrorSuccess;


   switch (event) {
        case EVENT_TIMER_TICK:
           
			error = IMAQdxGrab (session, image, TRUE, &bufferNumber);
			if (error)
			{
				break;
			}
			
		
			imaqDisplayImage (image, 0, TRUE);
			
		
			break;
	}
	
/*	if (error)
	{
		DisplayError(error);
		
		StopCamera();
		

     }   */
    
    return 0;
}


//----------------------------------------------------------------------------
// ChartPanelGoFunction
//----------------------------------------------------------------------------
int CVICALLBACK ChartPanelGoFunction(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int value;
    
    switch (event) {
        case EVENT_COMMIT:
            GetCtrlVal(panel, control, &value);
            SetCtrlAttribute (panel, PANEL_TIMER, ATTR_ENABLED, value);
            break;
    }    
    return 0;
}


//----------------------------------------------------------------------------
// ChartPanelTimerRateCallback
//----------------------------------------------------------------------------
int CVICALLBACK ChartPanelTimerRateCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int value;

    switch (event) {
        case EVENT_COMMIT:
            GetCtrlVal(panel, control, &value);
            SetCtrlAttribute (panel, PANEL_TIMER, ATTR_INTERVAL, ((double)value)/1000.0);

            break;
    }
    return 0;
}


//----------------------------------------------------------------------------
// ChartPanelPopupInfo
//----------------------------------------------------------------------------
int CVICALLBACK ChartPanelPopupInfo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int pnl=0, ctrl=0, quit=0;
    int popuphandle;

        if (event == EVENT_COMMIT) {
            if ((popuphandle = LoadPanelEx (0, "ChartPanel.uir", POPUP, __CVIUserHInst)) < 0)
                return 0;
            InstallPopup (popuphandle);
            while (!quit) {
                GetUserEvent (1, &pnl, &ctrl);
                if (ctrl == POPUP_OK) {
                    DiscardPanel (popuphandle);
                    quit=1;
                }
            }
        }
            
    return 0;
}


//----------------------------------------------------------------------------
// ChartPanelDisplayTaskBarCallback
//----------------------------------------------------------------------------
int CVICALLBACK ChartPanelDisplayTaskBarCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int intValue;
    char buffer[256];

    switch (event) {
        case EVENT_COMMIT:
            GetCtrlVal(panel, control, &intValue);
            GetPanelAttribute(panel, ATTR_TITLE, buffer);
            SetSystemAttribute (ATTR_TASKBAR_BUTTON_VISIBLE, intValue);
            SetSystemAttribute (ATTR_TASKBAR_BUTTON_TEXT, buffer);

            break;
    }
    return 0;
}


//----------------------------------------------------------------------------
// ChartPanelQuit
//----------------------------------------------------------------------------
int CVICALLBACK ChartPanelQuit (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
            QuitUserInterface(0);
            
            break;
    }
    return 0;
}




int CVICALLBACK SNAP2 (int SnapInstance) 
{
			 static int imageWrite;
			 static int ImgIndex;
			
			ImgIndex=SnapInstance+ImgNum;
			static char ImgFileName[400];
			sprintf(ImgFileName,"%s\\Img%d.jpg",SaveImageDirectory,ImgIndex);
		   // Rect rect = {0,200,200,200};
		//	imaqExtractTetragon (image, ExtrctdImage, roi, &rect, NULL);
	
			imageWrite = imaqWriteJPEGFile (image, ImgFileName, 750, NULL);
		//	printf("Image captured %d\n",ImgNum) ;
			ImgNum++;
			
	

	return ImgIndex;
	
}



void StopCamera (void)
{
	
	//printf("StopCamera");   
	// Stop the Imaq Loop
	SetCtrlAttribute (panel, PANEL_TIMER, ATTR_ENABLED, FALSE);
	SetCtrlAttribute (panelHandle, instrpanel_ENABLESNAP, ATTR_DIMMED, FALSE);  
	Delay(1);
			//SetCtrlAttribute (panelHandle, PANEL_STOP, ATTR_DIMMED, TRUE);
			//SetCtrlAttribute (panelHandle, PANEL_GRAB, ATTR_DIMMED, FALSE);
			//SetCtrlAttribute (panelHandle, PANEL_ATTRIBUTENAME, ATTR_DIMMED, TRUE);
			
			// Close the session
			if (session)
			{
				IMAQdxCloseCamera (session);
				session = 0;
				
			}
		

			if (image)
			{
				imaqDispose(image);
				image = NULL;
			}
		

}


int CVICALLBACK QuickAttrSetup (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			IMAQdxSetAttribute (session, "CameraAttributes::AnalogControls::GainAuto", IMAQdxValueTypeString, "Continuous");  
			IMAQdxSetAttribute (session, "CameraAttributes::AnalogControls::GainRaw", IMAQdxValueTypeU32, 3);
			
			SetCtrlIndex (mainPanelHandle, MAINPANEL_GAINMODE, 2);
			SetCtrlVal(mainPanelHandle, MAINPANEL_CAMERAGAINVAL, 3);  
			
			break;
	}
	return 0;
}

int CVICALLBACK EnableSnap (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	int SaveImageDirSelStatus;
	switch (event)
	{
		case EVENT_COMMIT:
			
			if(session) {
			SaveImageDirSelStatus = DirSelectPopup ("", "Select Directory to Save Images", 1, 1, SaveImageDirectory);
			if(SaveImageDirSelStatus)
				
			{
			 CameraAttributesControl(0);
		
			}
			else
					SetCtrlVal (panelHandle, instrpanel_ENABLESNAP, 0);     
			}
			else
			{
			MessagePopup("Start Camera","There is no active camera session");
			SetCtrlVal (panelHandle, instrpanel_ENABLESNAP, 0);    
			}
			break;
	}
	return 0;
}


int CVICALLBACK CameraGainVal (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int RawGainVal;
			
			GetCtrlVal(mainPanelHandle, MAINPANEL_CAMERAGAINVAL, &RawGainVal);
			IMAQdxSetAttribute (session, "CameraAttributes::AnalogControls::GainRaw", IMAQdxValueTypeU32, RawGainVal);

			break;
	}
	return 0;
}

int CVICALLBACK GainMode (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	IMAQdxError error = IMAQdxErrorSuccess;

	char attributeValue ;
  
	static char GainModeVal; 
	int GainModeInd;

	switch (event)
	{
		case EVENT_COMMIT:
		
			GetCtrlIndex (mainPanelHandle, MAINPANEL_GAINMODE, &GainModeInd);
	
			
			switch(GainModeInd)
			{
					
				case (2):
				IMAQdxSetAttribute (session, "CameraAttributes::AnalogControls::GainAuto", IMAQdxValueTypeString, "Continuous");    
				case (0):
				IMAQdxSetAttribute (session, "CameraAttributes::AnalogControls::GainAuto", IMAQdxValueTypeString, "Off");    
				case (1):
				IMAQdxSetAttribute (session, "CameraAttributes::AnalogControls::GainAuto", IMAQdxValueTypeString, "Once");    
			
			}
		
		
			if (error)
			{
				//DisplayError(error);
			}
			
		
			break;
	}
	return 0;
}



extern CameraAttributesControl(int AttrVal) 
{
	SetCtrlAttribute (panelHandle, instrpanel_SNAPSTRETCHRETURN, ATTR_DIMMED, AttrVal);
	SetCtrlAttribute (panelHandle, instrpanel_SNAPFFE  , ATTR_DIMMED, AttrVal); 
	SetCtrlAttribute (panelHandle, instrpanel_SNAPFFR, ATTR_DIMMED, AttrVal);
	SetCtrlAttribute (panelHandle, instrpanel_SNAPSTRRTRNCHECK , ATTR_DIMMED, AttrVal);
	SetCtrlAttribute (panelHandle, instrpanel_SNAPSFFECHECK , ATTR_DIMMED, AttrVal);  
	SetCtrlAttribute (panelHandle, instrpanel_SNAPSFFRCHECK , ATTR_DIMMED, AttrVal); 
	
	
	return 0;
}	

int CVICALLBACK InstantSnapCallBack (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	char InstSnapSave[300];
	int imageWriteInst ;
	int SaveImageDirSelStatusInst;
	switch (event)
	{
		case EVENT_COMMIT:
			int  SaveImageDirSelStatusInst;
			static char SaveImageDirectoryInst[300];   
		SaveImageDirSelStatusInst = FileSelectPopupEx ("", "", "*.jpg*", "Save Snapped Image", VAL_SAVE_BUTTON, 0, 0, InstSnapSave);
			if(	SaveImageDirSelStatusInst)
				imageWriteInst = imaqWriteJPEGFile (image, InstSnapSave, 750, NULL); 

			break;
	}
	return 0;
}
