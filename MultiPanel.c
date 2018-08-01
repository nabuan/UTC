

//------------------------------------------------------------------------------
// Example to demonstrate how to use threads with multiple UIR Panels
//------------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
double d; 
int panel;
int panelHandle;
int mainPanelHandle; 

#ifdef __WIN32__
#include <windows.h>
#endif /* __WIN32 */
#include <utility.h>

#include <cvirte.h>
#include <cvi2009compat.h>
#include <userint.h>
#include <toolbox.h>

#include "MultiPanel.h"
#include "ChartPanel.h"

#include "UTC v1.h"         


//----------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Local variables
//----------------------------------------------------------------------------


static char errorBuf[CMT_MAX_MESSAGE_BUF_SIZE];
static CmtThreadPoolHandle poolHandle = 0;

//----------------------------------------------------------------------------
// Function prototypes
//----------------------------------------------------------------------------
extern int CVICALLBACK ChartPanelMain (void * data);
extern int CVICALLBACK SNAP2 (void);
extern  int ExitFunction ();
 extern int panel;
extern void StopCamera (void);  

// Static
static void ShutdownApplication (void);
static int ScheduleNewFunction (void);
static int RefreshFunctionListBox (void);
static void DimMainPanelCtrls (void);
static void CVICALLBACK QuitUICallback (void *returnValue);
static void CVICALLBACK NotifyThreadFunctionExecState (CmtThreadPoolHandle poolHandle, 
						 							   CmtThreadFunctionID functionID,
													   unsigned int event, 
													   int value,
													   void *callbackData);
static int PoolCreated (void);
static double t1;
			static double t2;
			

//----------------------------------------------------------------------------
// WinMain
//----------------------------------------------------------------------------
int MulitMain(int argc, char*argv[])
{
    int error = 0;
    
    if (InitCVIRTE (0, argv, 0) == 0)    /* Initialize CVI libraries */
        return -1;    /* out of memory */
        
    // Load and display panel    
    if ((mainPanelHandle = LoadPanel (0, "MultiPanel.uir", MAINPANEL)) < 0)
        return -1;
	
	if ((panel = LoadPanelEx (0, "ChartPanel.uir", PANEL, __CVIUserHInst)) < 0)
        return panel;
        
    DisplayPanel (mainPanelHandle);
    
    // Set sleep policy for main thread
    SetSleepPolicy(VAL_SLEEP_MORE);
    
    // Wait for user input  
    RunUserInterface ();
    
    //DiscardPanel (mainPanelHandle);  
    
Error:
	ShutdownApplication ();
    return error;
}



//----------------------------------------------------------------------------
// ScheduleFunctionCallback
//----------------------------------------------------------------------------
int CVICALLBACK ScheduleFunctionCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
        	ScheduleNewFunction ();
            break;
    }

    return 0;
}


//----------------------------------------------------------------------------
// TerminateThreadCallback
//----------------------------------------------------------------------------
int CVICALLBACK ExitFunctionCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
  
		switch (event) {
		case EVENT_COMMIT:
	ExitFunction();
		}
	return 0;
}

 int ExitFunction(void)
{
int index= -1;
    int fnID;
	unsigned int threadID;
    int error = 0;

    
	 
	 if(mainPanelHandle>0) {
             GetCtrlIndex (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, &index); 
	 }
            
            if (index < 0) {
                MessagePopup("Terminate Thread...", 
                             "There are no threads running");
            
				QuitUserInterface (0); 
		
            
			}   
			
             else  { 
			StopCamera();  
		//	printf("stopcamera");
			GetValueFromIndex (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, index,
							   &fnID);
            CmtGetThreadPoolFunctionAttribute (poolHandle, fnID,
            								   ATTR_TP_FUNCTION_THREAD_ID, 
            								   &threadID);
		  
			// Tell the thread to execute QuitUserInterface.
			PostDeferredCallToThread (QuitUICallback, 0, threadID);
           
             }
  
    
Error:
	if (error < 0) {
		CmtGetErrorMessage (error, errorBuf);
		MessagePopup ("Error:", errorBuf);
	}
	return 0;
	
}

//----------------------------------------------------------------------------
// ThreadListBoxChange
//----------------------------------------------------------------------------
int CVICALLBACK FunctionListBoxChange (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_VAL_CHANGED:
            RefreshFunctionListBox();
            break;
    }
    return 0;
}


//----------------------------------------------------------------------------
// ThreadPriorityCallback
//----------------------------------------------------------------------------
int CVICALLBACK ThreadPriorityCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
#ifdef _NI_mswin_
    int threadPriority, fnID;
    HANDLE threadHandle;

    switch (event) {
        case EVENT_COMMIT:
        	// Get the thread handle of the thread running the selected function.
            GetCtrlVal (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, &fnID);
        	CmtGetThreadPoolFunctionAttribute (poolHandle, fnID,
											   ATTR_TP_FUNCTION_THREAD_HANDLE,
											   &threadHandle);
													   
			GetCtrlVal (mainPanelHandle, control, &threadPriority);
			if (!SetThreadPriority (threadHandle, threadPriority))
                MessagePopup ("Warning", "SetThreadPriority failed");
            break;
    }
    
Error:
#endif /* _NI_mswin_ */
    return 0;
}

//----------------------------------------------------------------------------
// MainPriorityCallback
//----------------------------------------------------------------------------
int CVICALLBACK MainPriorityCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
#ifdef _NI_mswin
    int threadPriority;
    
    switch (event) {
        case EVENT_COMMIT:
            GetCtrlVal (panel, control, &threadPriority);
            if (!SetThreadPriority(GetCurrentThread(), threadPriority))
                MessagePopup ("Warning", "SetThreadPriority failed");
            break;
    }
#endif /* _NI_mswin_ */
    return 0;
}

//----------------------------------------------------------------------------
// CreatePoolCallback
//----------------------------------------------------------------------------
int CVICALLBACK CreatePoolCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int maxNumThrds;
    int error = 0;
	
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal (mainPanelHandle, MAINPANEL_NUMTHREADSINPOOL, &maxNumThrds);
    		errChk (CmtNewThreadPool (maxNumThrds, &poolHandle));
			SetCtrlAttribute (mainPanelHandle, MAINPANEL_SCHEDULEFN, ATTR_DIMMED,
							  0);
			SetCtrlAttribute (mainPanelHandle, MAINPANEL_NUMTHREADSINPOOL,
							  ATTR_MIN_VALUE, maxNumThrds);
			SetCtrlAttribute (mainPanelHandle, MAINPANEL_CREATEPOOL, ATTR_DIMMED,
							  1);
			SetCtrlAttribute (mainPanelHandle, MAINPANEL_CHOOSECAMERA, ATTR_DIMMED,
							  0);
			break;
	}

Error:
	if (error < 0) {
		CmtGetErrorMessage (error, errorBuf);
	//	MessagePopup ("Error:", errorBuf);
	}
	return 0;
}

//----------------------------------------------------------------------------
// NumThrdsCallback
//----------------------------------------------------------------------------
int CVICALLBACK NumThrdsCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int maxNumThrds;
    int error = 0;
	
	switch (event) {
		case EVENT_VAL_CHANGED:
			if (PoolCreated ()) {
				GetCtrlVal (mainPanelHandle, control, &maxNumThrds);
				
				// Change the minimum value allowed in the numeric to prohibit 
				// downsizing the pool.
				SetCtrlAttribute (mainPanelHandle, control, ATTR_MIN_VALUE, 
								  maxNumThrds);
				errChk (CmtSetThreadPoolAttribute (poolHandle, ATTR_TP_MAX_NUM_THREADS,
												   maxNumThrds));
			}
			break;
	}

Error:
	if (error < 0) {
		CmtGetErrorMessage (error, errorBuf);
		MessagePopup ("Error:", errorBuf);
	}
	return 0;
}

//----------------------------------------------------------------------------
// NotifyThreadBegin
//----------------------------------------------------------------------------
static void CVICALLBACK NotifyThreadFunctionExecState (CmtThreadPoolHandle poolHandle, 
													   CmtThreadFunctionID functionID,
													   unsigned int event, 
													   int value,
													   void *callbackData)
{
	static char buf[256];
	int index;
	unsigned int threadID;
	
	switch (event) {
		case EVENT_TP_THREAD_FUNCTION_BEGIN:
			// Insert the function information in the list box.
			CmtGetThreadPoolFunctionAttribute (poolHandle, functionID,
											   ATTR_TP_FUNCTION_THREAD_ID,
											   &threadID);
			sprintf (buf, "%d\033p65l0x%x", functionID, threadID);
			InsertListItem (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, -1, buf,
							functionID);
			RefreshFunctionListBox ();
			break;
			
		case EVENT_TP_THREAD_FUNCTION_END:
			// Remove the function information from the list box.
			GetIndexFromValue (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, &index,
							   functionID);
			DeleteListItem (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, 
							index, 1);
			RefreshFunctionListBox ();
			
			
			CmtReleaseThreadPoolFunctionID (poolHandle, functionID);
			
			break;
			
		default:
			break;
	}

}

//----------------------------------------------------------------------------
// ScheduleNewFunction
//----------------------------------------------------------------------------
static int ScheduleNewFunction (void) 
{
    static int threadCount = 0;
    int fnID;
    int error = 0;
    
    errChk (CmtScheduleThreadPoolFunctionAdv (poolHandle, ChartPanelMain, (void *)threadCount, THREAD_PRIORITY_HIGHEST, NotifyThreadFunctionExecState, (EVENT_TP_THREAD_FUNCTION_BEGIN | EVENT_TP_THREAD_FUNCTION_END),
											  0, CmtGetCurrentThreadID(), &fnID));

    // Add thread to list
    threadCount++;
    RefreshFunctionListBox ();

Error:
	if (error < 0) {
		CmtGetErrorMessage (error, errorBuf);
		MessagePopup ("Error:", errorBuf);
	}

    return error;
}


//----------------------------------------------------------------------------
// RefreshFunctionListBox
//----------------------------------------------------------------------------
static int RefreshFunctionListBox (void) 
{
    int numItems, index;
	int fnID;
#ifdef _NI_mswin_
	HANDLE threadHandle;
#endif /* _NI_mswin_ */
	int threadPriority;
	
	GetNumListItems (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, &numItems);
	if (numItems != 0) {
    	// Update the Thread Priority Control
    	GetCtrlIndex (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, &index);
    	GetValueFromIndex (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, index, &fnID);
#ifdef _NI_mswin_
		CmtGetThreadPoolFunctionAttribute (poolHandle, fnID,
										   ATTR_TP_FUNCTION_THREAD_HANDLE,
										   &threadHandle);
										   
		if ((threadPriority = GetThreadPriority (threadHandle)) == THREAD_PRIORITY_ERROR_RETURN)
            MessagePopup("Warning", "GetThreadPriority failed");
        else 
        	SetCtrlVal (mainPanelHandle, MAINPANEL_THREADPRIORITY, threadPriority);										   
#endif /* _NI_mswin_ */
    }
    
    DimMainPanelCtrls ();

    return 0;    
}

//----------------------------------------------------------------------------
// DimMainPanelCtrls
//----------------------------------------------------------------------------
static void DimMainPanelCtrls (void)
{
	int numItems;
	
	GetNumListItems (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, &numItems);
	
    SetCtrlAttribute (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, ATTR_DIMMED, 
    				  (numItems == 0));
    SetCtrlAttribute (mainPanelHandle, MAINPANEL_THREADPRIORITY, ATTR_DIMMED, 
    				  (numItems == 0));
    SetCtrlAttribute (mainPanelHandle, MAINPANEL_EXITFUNCTION, ATTR_DIMMED, 
    				  (numItems == 0));
	SetCtrlAttribute (panelHandle, instrpanel_ENABLESNAP, ATTR_DIMMED, FALSE); 
	
	SetCtrlAttribute (mainPanelHandle, MAINPANEL_GAINMODE, ATTR_DIMMED, 
    				  (numItems == 0));
	SetCtrlAttribute (mainPanelHandle, MAINPANEL_CAMERAGAINVAL, ATTR_DIMMED, 
    				  (numItems == 0));
	SetCtrlAttribute (mainPanelHandle, MAINPANEL_QUICKATTRSETUP, ATTR_DIMMED, 
    				  (numItems == 0));
	SetCtrlAttribute (mainPanelHandle, MAINPANEL_SCHEDULEFN, ATTR_DIMMED, 
    				  (numItems != 0));
		SetCtrlAttribute (mainPanelHandle, MAINPANEL_SNAPINST, ATTR_DIMMED, 
    				  (numItems == 0));
		SetCtrlAttribute (mainPanelHandle, MAINPANEL_CMAERAPANELEXIT, ATTR_DIMMED, 
    				  (numItems != 0));

		SetCtrlAttribute (	panelHandle, instrpanel_quit, ATTR_DIMMED, 
    				  (numItems != 0));
	  
}										

//----------------------------------------------------------------------------
// ShutdownApplication
//----------------------------------------------------------------------------
static void ShutdownApplication (void)
{
	if (PoolCreated ())
		CmtDiscardThreadPool (poolHandle);
}

//----------------------------------------------------------------------------
// QuitUICallback
//----------------------------------------------------------------------------
static void CVICALLBACK QuitUICallback (void *returnValue)
{

	QuitUserInterface ((int)(intptr_t)returnValue);
//	MessagePopup ("Safe to Exit", "Press OK to exit");
			 
	
} 

//----------------------------------------------------------------------------
// PoolCreated
//----------------------------------------------------------------------------
static int PoolCreated (void)
{
	return poolHandle != 0;
}



int CVICALLBACK CameraPanelExit (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
		ExitFunction();
		DiscardPanel(mainPanelHandle);
    	mainPanelHandle=0; 

			break;
	}
	return 0;
}
