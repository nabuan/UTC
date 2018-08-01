
/*---------------------------------------------------------------------------*/ 
/*	Tweeztrol 2013.c									   						 */
/*													  						 */
/*	Main data collection program for high power tweezers.					 */
/*													  						 */
/*	Controls Newport IR lasers, ThorLabs xyz piezo stage, Melles Griot piezo */
/*	and	Melles Griot SpotOn lateral effect detectors.  						 */  
/*																			 */
/*  Notes:  Appropriate drivers and header files must be loaded.			 */
/*  		Thorlabs piezo runs through a PCI card using easyio library. 	 */
/*	 		Newport lasers are controlled through GPIB card and library. 	 */
/*			DoSpot library must be recompiled (externally) to match CVI.	 */
/*			Melles Griot controls exposed though function panel that must be */
/*			created and insterted into library (paths are required in 		 */
/*			compiler options as well).  									 */
/*													  						 */
/*---------------------------------------------------------------------------*/ 



/*  Standard Libraries  */
int panelHandle; 
int ImgNum ;   

#include <analysis.h>
#include <NIDAQmx.h>
#include <ansi_c.h>
#include <cvirte.h>
#include "c:\Users\Nabs\Desktop\UTC-Digital_2016_1\UTC v1.h"
#include <formatio.h>
#include <userint.h> 
#include <utility.h>
#include "MultiPanel.h" 
#include "PanelEvsT.h"  
extern int mainPanelHandle=0; 
extern int ExtTimePanelHandle =0;

extern int CVICALLBACK SNAP2 (int SnapInstance);    
static CmtThreadFunctionID functionId;
extern  int ExitFunction (void); 
extern CameraAttributesControl(int AttrVal);
int CameraAttributesValZero(void); 

extern int CVICALLBACK  MulitMain(int argc, char*argv[])  ; 

/*  Definitions & Declarations  */
#include "UTC v1.h"
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else  

/*  Instrument Libraries  */ 
#include <gpib.h>

/* Image Acquisition Variables */
static int SnapStrRetrnOn = 0;
static int SnapFFEOn = 0;
static int SnapFFROn= 0;
static int EnableSnapShots=0;
static int StrRtrnSnapItr;
static int FFESnapItr; 
static int FFRSnapItr; 
static int ImgIndexSave;

static int PopupExtTimeGraph=0;
static int forcefeedbackFlag=0; 

static float64 num_of_countsScalar;
static uInt64 SampChan;
static int32 WriteStat;
static int32 Dumm2;
static float64 stageDigiRead[5000];
static char Enable[300];
static TaskHandle dgWriteHandle;
static int32 Dummyone;

static uInt8  DirectVal[1];
static TaskHandle Direction;
static float64 stread1;
static int32 numread2;
static float64 stgread[1];
static TaskHandle StageReadHandle;
static float64 newsetstagepstn[1];
static int32 ffstat;
static uInt64 avgattr;
static char FileSaveName2[300];
static char FileSaveName[300];
static char **PastDir;
static int AutoSave;
static char fileName[300];
static char Drivename[300];
static char DirName[300];
static char currentDir[300];


//#include <easyio.h> 
//#include <stdio.h>
//#include <stdlib.h>


/*  Generic panel variables  */

static int confirm;
static int shutdown;
int panelcalHandle;

/*variables to read from detector*/


static float tempdata1x[1], tempdata1y[1];
static float tempdata2x[1], tempdata2y[1];
static float temppower1[1], temppower2[1];
static float Extension[1];
static float tempreadpstn[1];
static float readpiezopstn[1];
static double piezoposition[1];
static double elapsedtime0[1];  
static double offsetcorrection;

static float ForceAvg;
static float xavg;  
static float yavg;

static int CanSwitch=0;    



/* DAQmx for (Main, ReadDataAvg, Monitor) variables  */
static TaskHandle	collectHandle=0;
int 				avg=800000;		// Max memory allocation for PCI card (~10 sec of averaging). 
static int32      	error=0, Error=0;
static int32       	numRead=0;
static uInt32      	numChannels=7;
static float64  	*rawdata=NULL;
static char        	errBuff[2048]={'\0'};

/*  Piezo (Collect & Calibrate) variables  */
static int zeroconfirm;
static CAObjHandle confighandle;
static CAObjHandle piezohandle;
static long returnval;

static float piezovoltage = 0;
static long currentloop;

/*  Laser control variables  */
static int lasercontroller;
static int laserconfirm;
static int laser1switch = 0;
static int laser2switch = 0;
static int laser1current = 0;
static int laser2current = 0;
static int stringlength = 0;
static char laserstring[30];

/*  Microscope objective variables  */
static float objectivex = 0;
static float objectivey = 0;
static float objectivez = 0;

/*  Collect variable  */
static int stepsnumber;
static int stepsnumbertemp;
static int stepsnumberRel;
static double stepsincrement = 0;
static float Forcelevel=0;
static int averagepoints = 0;
static int rownumber=0;
static int totrownumber;
static int collectFlag=0;
static int ProcSys;   

/* Piezo stage position (Main, ReadDataAvg, Collect) variables */
static TaskHandle  stageHandle=0;
static double stageoffset = 0;
static double setstagepstn = 0;
static double currentsetstagepstn = 0;  
static int ZeroOffset = 0;
static int ZeroOffsetLED = 0;
static int ZeroOffsetMoveFlag = 0; 
static double offsetstepdelay =0;



static int stagezeroswitch= 1;
//static int avg = 0;
static int i, j, k;
static double newpiezoposition = 0;
static double offset = 0;
static double stepsdelay = 0;
 
static double zerotime;
static double elapsedtime1;  
static double elapsedtime2;
static double elapsedtimeFFext;    
static double elapsedtimeFFrel;    
static double elapsedtime = 0;
static int abortdata;
static int AbortCheck; 
static int rewindindicator;
static float forcedata[300000][9];
static int forcefeedback = 0;
static float I;
static float N;

static double testforce = 0;
static double TestForceRel=0;
static double targetforce = 0;
static double forcethreshold = 0;
static double stepresolution = 0;
static double remainder = 0;
static int pauseiterations = 0;
static double pausedelay = 0;
static int pauseptsavg = 0;

static int forcefeedbackRel = 0;     
static double targetforceRel = 0;
static double testforceRel = 0;
static double forcethresholdRel = 0;
static double stepresolutionRel = 0;
static int pauseiterationsRel = 0; 
static double pausedelayRel = 0;  
static int pauseptsavgRel = 0;
static int Relforcefeedbackflag=0;
int flag2=0 ;
int stepsnumber2;
int kk=0;


static int totalrows = 0;

/* Calibrate variables  */
static double samplingrate;
static double viscosity; 
static double sphereradius;
static double amplitude;
static double frequency;
static double viscousforce;

/*  Save variables  */
static int returnstatus;
static int overwriteconfirm;
static char filestorage[300];
static int savestatus=0;
static int saveconfirm;

/* Save Details variables*/
static char ProteinName[300];
static char Comments[300];
static double Conc;
static double stockconc;
static double stockvol;
static double finalvol;
static char details[8000];
static char details1[4000];  
static char DetailsArray[5][300];
static int SaveDetailsStatus;
static char DetailsStorage[300];
static char SaveName[300];  
static char AutoSaveName[300];  
static int Savedetails;
static double FinalConc; 



//Digital Aquisition
int8 enableval[1]; 
double currentpiezoposition = 0;
double diffpiezoposition=0;
	 


float64 freq,duty;
int fh1;



 //PFM

static float64 num_of_counts;
static float64 countpos;
static char MonitorPos[300];
static TaskHandle QuadMonitor;
static TaskHandle EnableTask; 


/*  Function prototypes - Datadisplayroutine to graph data. */
int DisplayStoreData (int avg,int imgindextosave); 
int ReadData (int avg); 
int Extend(int stepsnumber);
int Return(int stepsnumber);
int ForceFeedback(int pauseiterations, int FfeFlag);	

int ReadPiezo(void); 
int Save(void);  
//int Save (void);					   
/*---------------------------------------------------------------------------*/
/* Main - Loads & runs main instrpanel (through which all other functions    */
/* are called).  Also loads and initializes SpotOn detectors and Melles   	 */
/* Griot piezoelectric Active X controls.  The SpotOn system presumes that   */
/* a PCI SpotOn detector pair are loaded.  The Active X controls require     */
/* a stage that has been configured under the file 'xaxisconfig', using the  */
/* stage name 'xaxis'.                                                       */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	/* Initialize CVI libraries - only necessary for external compiler  */
	if (InitCVIRTE(0,argv,0)==0)
		/* Out of memory  */
		return(-1);
	/* Check for location of uir panels and obtain handles.  */	
	if ((panelHandle = LoadPanel(0, "UTC v1.uir", instrpanel)) < 0)
		return(-1);
	

DAQmxCreateTask("readdata",&collectHandle);
DAQmxCreateAIVoltageChan(collectHandle,"Dev1/ai6","x1",DAQmx_Val_Cfg_Default,-5.0,5.0,DAQmx_Val_Volts,NULL);
DAQmxCreateAIVoltageChan(collectHandle,"Dev1/ai1","y1",DAQmx_Val_Cfg_Default,-5.0,5.0,DAQmx_Val_Volts,NULL);  
DAQmxCreateAIVoltageChan(collectHandle,"Dev1/ai2","p1",DAQmx_Val_Cfg_Default,0,10.0,DAQmx_Val_Volts,NULL);  
DAQmxCreateAIVoltageChan(collectHandle,"Dev1/ai3","x2",DAQmx_Val_Cfg_Default,-5.0,5.0,DAQmx_Val_Volts,NULL);  
DAQmxCreateAIVoltageChan(collectHandle,"Dev1/ai4","y2",DAQmx_Val_Cfg_Default,-5.0,5.0,DAQmx_Val_Volts,NULL);  
DAQmxCreateAIVoltageChan(collectHandle,"Dev1/ai5","p2",DAQmx_Val_Cfg_Default,0,10.0,DAQmx_Val_Volts,NULL);

DAQmxCreateTask("readstage",&StageReadHandle); 


//DAQmxCreateAIVoltageChan (StageReadHandle, "Dev1/ai7", "stageread", DAQmx_Val_Diff, -10.0, 10.0, DAQmx_Val_Volts, NULL);

DAQmxCreateTask ("Direction1", &Direction);
DAQmxCreateTask (Enable, &EnableTask);
DAQmxCreateDOChan (Direction, "Dev1/port0/line1", "port0Line1", DAQmx_Val_ChanPerLine);
DAQmxCreateDOChan (EnableTask, "Dev1/port0/line3", "port0Line3", DAQmx_Val_ChanPerLine); 
	
	
DAQmxErrChk (DAQmxGetTaskAttribute(collectHandle,DAQmx_Task_NumChans,&numChannels));
	if( (rawdata=(float64*)malloc(avg*6*sizeof(float64)))==NULL ) {
		MessagePopup("Error","Not enough memory");
		goto Error;
	}
	
	

	DAQmxCreateTask("",&stageHandle);

	DAQmxCfgSampClkTiming (collectHandle, "OnboardClock", 1000000/6, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps,avg);
	

	stageoffset=1;  
stagezeroswitch= 1;

			DAQmxCreateTask (MonitorPos, &QuadMonitor);
		
			
		   
			DAQmxStartTask (Direction);
			DAQmxStartTask (EnableTask); 
			DirectVal[0]=0;
			enableval[0]=0;
	DAQmxWriteDigitalLines (Direction, 1, 1, 10.0, DAQmx_Val_GroupByChannel, DirectVal, &Dummyone, 0);
	DAQmxWriteDigitalLines (EnableTask, 1, 1, 10.0, DAQmx_Val_GroupByChannel,enableval, &Dummyone, 0);
	enableval[0]=1;     
	DAQmxWriteDigitalLines (EnableTask, 1, 1, 10.0, DAQmx_Val_GroupByChannel,enableval, &Dummyone, 0);
	
	currentpiezoposition = 0;
	DAQmxCreateCILinEncoderChan (QuadMonitor, "Dev1/ctr0", MonitorPos, DAQmx_Val_X2, 1, 0, DAQmx_Val_ALowBHigh, DAQmx_Val_Meters, (0.095367)*1e-9, 0, "");
	
	DAQmxStartTask (QuadMonitor);  
	DAQmxErrChk (DAQmxStartTask(collectHandle));   
	
	DAQmxCreateTask("",&dgWriteHandle);
	DAQmxCreateCOPulseChanFreq (dgWriteHandle, "Dev1/ctr1", "", DAQmx_Val_Hz, DAQmx_Val_Low, 0.0, 4000000, 0.5);
	WriteStat = DAQmxCfgImplicitTiming (dgWriteHandle, DAQmx_Val_FiniteSamps, 4200);    
	  
	 

	/* Display and run panel.  */ 	
	DisplayPanel(panelHandle);
	
	RunUserInterface();
	stageoffset=1;

 
	DAQmxStopTask(collectHandle);
	DAQmxStopTask(Direction); 
	DAQmxStopTask (QuadMonitor);    
	DAQmxStopTask(stageHandle);
	DAQmxClearTask(collectHandle);
	DAQmxClearTask(stageHandle); 
	DAQmxClearTask(dgWriteHandle);
	DAQmxClearTask (QuadMonitor);   
	
	Error:
	SetWaitCursor(0);
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	if( rawdata )
		free(rawdata);
	if( DAQmxFailed(error) )
		MessagePopup("DAQmx Error",errBuff);
	
	return (0);
	

	
}


/*---------------------------------------------------------------------------*/
/* Monitor - function tracks beam power and position, graphing updates.		 */
/* Information is displayed in the 'beam position' and 'power' charts.       */
/* Begins upon program startup and collects data at 4/10 second intervals,   */
/* until Collect is called or program exits.								 */
/* Note that the xchannel of detector #2 is inverted.						 */
/* This data is not saved - is used to optimize system.  					 */
/*---------------------------------------------------------------------------*/
int Monitor (int panel, int control, int event, void *callbackData, int eventData1, int eventData2) 
{	



SetCtrlVal (panelHandle, instrpanel_ExtensionDisplay,yavg);
SetCtrlVal (panelHandle, instrpanel_ForceDisplay,xavg);
SetCtrlVal (panelHandle, instrpanel_cellpiezoposition, currentpiezoposition);  






ReadData(200);  
	
	return (0);
} 








int CVICALLBACK Collect (int panel, int control, int event, 
						void *callbackData, int eventData1, int eventData2)  
{
	
	
switch (event)
	{
		case EVENT_COMMIT:
		
		/* Determine if previous data is to be saved.  */
		 
	if (savestatus == 1)
			{
				saveconfirm = ConfirmPopup ("Confirm",
					  				"Current data set is not saved.  Save it now?");
			
				/* Call Save if a 'yes' response is received.  */		  				
				if (saveconfirm == 1)
				{
					
					Save ();
				
				}
			}
		
			DAQmxStopTask(dgWriteHandle);       
		
		/* Suspend the timer.  */
		SuspendTimerCallbacks ();
		rownumber=0;
		
			/* Image Acquisition controls  */
		GetCtrlVal (panelHandle, instrpanel_ENABLESNAP, &EnableSnapShots);
			if(EnableSnapShots)
			{
		GetCtrlVal (panelHandle, instrpanel_SNAPSTRRTRNCHECK, &SnapStrRetrnOn);
		GetCtrlVal (panelHandle, instrpanel_SNAPSFFECHECK, &SnapFFEOn); 
		GetCtrlVal (panelHandle, instrpanel_SNAPSFFRCHECK, &SnapFFROn); 
		
		
	   	GetCtrlVal (panelHandle, instrpanel_SNAPSTRETCHRETURN, &StrRtrnSnapItr); 
		GetCtrlVal (panelHandle, instrpanel_SNAPFFE, &FFESnapItr); 
		GetCtrlVal (panelHandle, instrpanel_SNAPFFR, &FFRSnapItr);
		
		
			}
		
		/* Light the Data Collection indicator.  */
		
		SetCtrlVal (panelHandle, instrpanel_ZeroOffsetLED, ZeroOffsetLED); 
		SetCtrlVal (panelHandle, instrpanel_ConstForceIternation, 0);
	

		/* Read the number of steps, step increment and delay.  */
		GetCtrlVal (panelHandle, instrpanel_stepsnumber, &stepsnumber);
		GetCtrlVal (panelHandle, instrpanel_stepsincrement, &stepsincrement);
		GetCtrlVal (panelHandle, instrpanel_stepsdelay, &stepsdelay);
		GetCtrlVal (panelHandle, instrpanel_averagepoints, &averagepoints);
		GetCtrlVal (panelHandle, instrpanel_AbortCheck, &AbortCheck); 
		GetCtrlVal (panelHandle, instrpanel_PROCSYS, &ProcSys);  
	
		
	
		
		/* Get constant position (
		pause) variables. */
		GetCtrlVal (panelHandle, instrpanel_pauseiterations, &pauseiterations);			
		GetCtrlVal (panelHandle, instrpanel_pausedelay, &pausedelay);				
		GetCtrlVal (panelHandle, instrpanel_pauseptsavg, &pauseptsavg);
		
		GetCtrlVal (panelHandle, instrpanel_pauseiterationsRel, &pauseiterationsRel);			
		GetCtrlVal (panelHandle, instrpanel_pausedelayRel, &pausedelayRel);				
		GetCtrlVal (panelHandle, instrpanel_pauseptsavgRel, &pauseptsavgRel);	
	
	
		/* Determine if force feedback is on. */
		GetCtrlVal (panelHandle, instrpanel_forcefeedback, &forcefeedback);
		GetCtrlVal (panelHandle, instrpanel_forcefeedbackRel, &forcefeedbackRel); 
		
		/* Get feedback collection variables.  */
		GetCtrlVal (panelHandle, instrpanel_targetforce, &targetforce);			
		GetCtrlVal (panelHandle, instrpanel_forcethreshold, &forcethreshold);				
		GetCtrlVal (panelHandle, instrpanel_stepresolution, &stepresolution);
		
		GetCtrlVal (panelHandle, instrpanel_targetforceRel, &targetforceRel);			
		GetCtrlVal (panelHandle, instrpanel_forcethresholdRel, &forcethresholdRel);				
		GetCtrlVal (panelHandle, instrpanel_stepresolutionRel, &stepresolutionRel);
		
		
		
		/* Check for abort command. */
		GetCtrlVal (panelHandle, instrpanel_abortdata, &abortdata);
		
		 //Popup Ext vs time during forcefeedback
		GetCtrlVal (panelHandle, instrpanel_POPUPEXTGRPH, &PopupExtTimeGraph); 
		
		
		//if(forcefeedbackRel == 1 || forcefeedback == 1) {    
		if(PopupExtTimeGraph==1 && (forcefeedback==1 || forcefeedbackRel==1))
		{
		
		if(ExtTimePanelHandle==0){
		ExtTimePanelHandle = LoadPanelEx (0, "PanelEvsT.uir", PANELXVT, __CVIUserHInst);
        }
		else
		{	
		SetActivePanel(ExtTimePanelHandle);	
		}
		
		DisplayPanel (ExtTimePanelHandle);
		}
	//}
	
		/* Resets abort button of inadvertently on when program started.  */
		if (abortdata == 1)
		{
			SetCtrlVal (panelHandle, instrpanel_abortdata, 0);
		}
			
		/* Establish initial position and voltage for piezo.  */ 	
		/*MG17Drivers_IPiezosGetCurrentPosition (piezohandle, NULL, "xaxis",
											   &piezoposition, &returnval);
		offset=piezoposition;
		
											   
		/* Multiply numbersteps and stepsize to check position limits (20 microns).  */
		ReadPiezo();
		if (piezoposition[0] + stepsnumber*stepsincrement/1000 > 25)
		{
			MessagePopup ("Warning - Piezo Limit",
					   		"The piezo can not travel further than 20 microns.");
		}
		
		/* Collect data.  */
		else
		{
			/* For 'Force Extension Data' - delete previous graph.  */
			DeleteGraphPlot (panelHandle, instrpanel_forceextensiongraph, -1,
					 			VAL_IMMEDIATE_DRAW);
			
		if(ExtTimePanelHandle)   {
		DeleteGraphPlot (ExtTimePanelHandle, PANELXVT_GRAPH, -1, VAL_IMMEDIATE_DRAW);
	
		
        }

			/* Finds starting time. */
			
			stepsnumberRel=stepsnumber;
			
			if(ZeroOffsetMoveFlag == 1)
			{
			GetCtrlVal (panelHandle, instrpanel_ZeroOffset, &ZeroOffset);
			stepsnumber= stepsnumber-ZeroOffset;
			rownumber=ZeroOffset+1 ; 
			}
			
			
			
			
			zerotime = Timer();
			//Execution of the run
			
			   
			
			Extend(stepsnumber);
			stepsnumberRel = TruncateRealNumber((piezoposition[0]
											-forcedata[0][1])*1000/stepsincrement);
		   stepsnumberRel=stepsnumberRel+1;
			
			Return (stepsnumberRel);  
			
			 
			
			if (Relforcefeedbackflag==1)
			Return (stepsnumberRel);
			
			totrownumber=rownumber; 
			
			if(ZeroOffsetMoveFlag==1)
			{
			stepsnumber=ZeroOffset;
			offset=offset-ZeroOffset*stepsincrement/1000; 
			ZeroOffset=0;
			rownumber=0;
			printf("ZeroOffset\t%d\tstepsnumber\t%d\t\t\n",ZeroOffset,stepsnumber);
			Extend(stepsnumber); 
			}
				

			
		

			}
		
		
		/* If data was aborted, rest the number of rows and restore abort indicator.  */
		if (abortdata == 1)
		{	
			SetCtrlVal (panelHandle, instrpanel_abortdata, 0);
		}

 		/* Resets the end of forcedata array, effectively eliminating previous data*/
		for (j = 0; j <= 7; j++)
		{
			forcedata[totrownumber][j] = '\0';
		}		
	/* Restore rewind indicator. */
		CameraAttributesControl(1);
		CameraAttributesValZero();
		rewindindicator = 0;
		Relforcefeedbackflag=0;
		flag2=0;
		/* Dim the indicator light.  */
		SetCtrlVal (panelHandle, instrpanel_acquire, 0);
		/* Resume the timer to monitor data */
		ResumeTimerCallbacks ();
				
	
		
	}
	
		
		
	
		
	
	return (0);

}



int Extend (int stepsnumber)

{
	
	//SuspendTimerCallbacks ();
	
  
	rewindindicator = 0;
	  
	SetCtrlAttribute (panelHandle,instrpanel_acquire,ATTR_ON_COLOR,VAL_CYAN);   
	SetCtrlVal (panelHandle, instrpanel_acquire, 1);
	DirectVal[0]=1;
	DAQmxWriteDigitalLines (Direction, 1, 1, 10.0, DAQmx_Val_GroupByChannel, DirectVal, &Dummyone, 0);
	SampChan=42*stepsincrement ;
	DAQmxSetTimingAttribute (dgWriteHandle, DAQmx_SampQuant_SampPerChan, SampChan);
	for (i = 0; i <stepsnumber; i++)
			  
			
			{
				
				
				if(i==0)
				{
					ReadData(averagepoints) ;
				DisplayStoreData(averagepoints,0); 
				}
				
				GetCtrlVal (panelHandle, instrpanel_abortdata, &abortdata);
				if (abortdata == 1)
				{
					stepsnumberRel=i-1;
					break;	
				}
			
				/* Moves stage.  */
			
				else
				{
				    
					DAQmxStartTask(dgWriteHandle);
			
				}

				
				Delay(stepsdelay/1000);
						
				/* Collect , store and `display data. */
			
				ReadData(averagepoints) ;
				
				if(SnapStrRetrnOn==1 && i%StrRtrnSnapItr==0)
				{
				ImgIndexSave=SNAP2 (10000);
				DisplayStoreData(averagepoints,ImgIndexSave);  
				
				}
				else
				DisplayStoreData(averagepoints,0); 
				
			//	printf("write=%0.4f\tread=%0.4f\n",setstagepstn,readpiezopstn[0]);  
			
				/* Place data in appropriate row of collection array.  */
				 if(ProcSys==1)  
				ProcessSystemEvents ();	
			
				/* If feedback is on - break loop to hold at the desired force.*/
			 if (forcefeedback == 1 && abortdata==0)
			{
					/* Break when force is within threshold of desired force.*/
					/* Rest stepsnumber for later routine. */
				if (((forcedata[rownumber-1][2]+forcedata[rownumber-1][5])/2) >= (targetforce - forcethreshold))
				{
						stepsnumber = i;
						ForceFeedback(pauseiterations,1);
					break;
					
					}
				}
			
				 
		DAQmxStopTask(dgWriteHandle);
			
			}	
		  
		  

		if (pauseiterations > 0 && abortdata == 0 && forcefeedback == 0)
			{
			/*	Constant Position Loop (Time Measurement). */
			SetCtrlAttribute (panelHandle,instrpanel_acquire,ATTR_ON_COLOR,VAL_YELLOW);
			SetCtrlVal (panelHandle, instrpanel_acquire, 1);  
				
				elapsedtime1=Timer() ;
				for (j = 0; j <= 7; j++)
				{
				forcedata[rownumber][j] = 0;
				}
				rownumber++ ;
			
			
					/* Loop to collect data at constant position.  */
					for (i = 0; i < pauseiterations; i++)	  
					{
						/* Check screen for command updates.  */
						
				
						/* Allow loop termination if abort option is selected.  */
						/* Breaks out of collection loop.  */
					GetCtrlVal (panelHandle, instrpanel_abortdata, &abortdata);
						if (abortdata == 1)
						{
							stepsnumber=i;
							break;
						}
						  
					if(i%AbortCheck==0) 
						ProcessSystemEvents (); 
						  
						 
						/* Delay between data collection. */
						Delay(pausedelay/1000);

 						/* Collect and display data - allows different averaging for pause data. */
						avg = pauseptsavg;
						ReadData(avg) ;
				//	printf("write=%0.4f\tread=%0.4f\n",setstagepstn,readpiezopstn[0]);      
						DisplayStoreData(avg,0);
						
						
						
						elapsedtime2= Timer()- elapsedtime1;
							
						SetCtrlVal (panelHandle, instrpanel_elapsedtime, elapsedtime2);   
						SetCtrlVal (panelHandle, instrpanel_ConstForceIternation, i);   
						
				}
			
				
				
				
		}
	
	   		
	   	   	
	   return(0);

}




int Return (int stepsnumberRel)

{

	DirectVal[0]=0;
	DAQmxWriteDigitalLines (Direction, 1, 1, 10.0, DAQmx_Val_GroupByChannel, DirectVal, &Dummyone, 0);
	SampChan=42*stepsincrement ;
	DAQmxSetTimingAttribute (dgWriteHandle, DAQmx_SampQuant_SampPerChan, SampChan);
	
	SetCtrlVal (panelHandle, instrpanel_acquire, 0);
	SetCtrlAttribute (panelHandle,instrpanel_acquire,ATTR_ON_COLOR,VAL_RED);   
	SetCtrlVal (panelHandle, instrpanel_acquire, 1); 
	/*SetCtrlVal (panelHandle, instrpanel_acquire, 1); */  

//SuspendTimerCallbacks ();
savestatus=1;
	/* Indicate relaxation data (to be graphed with open circles.*/
	rewindindicator = 1;
	
	 
	for (j = 0; j <= 8; j++)
	{
	forcedata[rownumber][j] = 0;
	}
	rownumber++ ;
for (int i = stepsnumberRel-1; i >0; i--)
		
   {

			if(i==stepsnumberRel-1)
				{
					ReadData(averagepoints) ;
				DisplayStoreData(averagepoints,0); 
				}


				
				/* Advance piezo to new position.  */
			
				DAQmxStartTask(dgWriteHandle); 						
				Delay(stepsdelay/1000);
				ReadData(averagepoints) ; 
				
					
				if(SnapStrRetrnOn==1 && i%StrRtrnSnapItr==0)
				 {
					 	ImgIndexSave=SNAP2 (20000);  
				DisplayStoreData(averagepoints,ImgIndexSave);  
				}
				
				else
				{
				DisplayStoreData(averagepoints,0); 
				} 
				
				if(ProcSys==1)
					ProcessSystemEvents (); 	
											  
				TestForceRel = ( (forcedata[rownumber-1][2]+forcedata[rownumber-1][5])/2);
		
			 
				
				if(forcefeedbackRel == 1)
				{
				if (Relforcefeedbackflag==0  &&  TestForceRel <= (targetforceRel+forcethresholdRel) )
					{
				 	   
					Relforcefeedbackflag=1;   
					ForceFeedback(pauseiterationsRel,0);
					break;
					
					
					
					}
				}
				
				
DAQmxStopTask(dgWriteHandle);     
				
}
		

return (0);    

}

int ForceFeedback (int pauseiterations, int FfeFlag)

{
	
	forcefeedbackFlag=1; 
    DAQmxStopTask(dgWriteHandle);  	
	SetCtrlAttribute (panelHandle,instrpanel_acquire,ATTR_ON_COLOR,VAL_YELLOW);
	SetCtrlVal (panelHandle, instrpanel_acquire, 1);  
	SuspendTimerCallbacks ();
				for (j = 0; j <= 8; j++)
				{
				forcedata[rownumber][j] = 0;
				}	
				rownumber++ ;  
		 	/* Loop to collect data at constant force.  */
					elapsedtime1= Timer()  ;
					for (i = 0; i < pauseiterations; i++)
					{				 		
				 	
					  
					 SetCtrlVal (panelHandle, instrpanel_ConstForceIternation, i);
						/* Check screen for command updates.  */
						if(i%AbortCheck==0) 
						ProcessSystemEvents (); 
						  
					
						/* Get feedback collection variables.  */
						if(Relforcefeedbackflag==0)
						{
						GetCtrlVal (panelHandle, instrpanel_targetforce, &targetforce);			
						GetCtrlVal (panelHandle, instrpanel_forcethreshold, &forcethreshold);				
						GetCtrlVal (panelHandle, instrpanel_stepresolution, &stepresolution);
						avg = pauseptsavg;
						}
						else
						{
						GetCtrlVal (panelHandle, instrpanel_targetforceRel, &targetforce);			
						GetCtrlVal (panelHandle, instrpanel_forcethresholdRel, &forcethreshold);				
						GetCtrlVal (panelHandle, instrpanel_stepresolutionRel, &stepresolution);
						avg = pauseptsavgRel;
						pausedelay=pausedelayRel;
						}	
			
	SampChan=42*stepresolution;
	DAQmxSetTimingAttribute (dgWriteHandle, DAQmx_SampQuant_SampPerChan, SampChan);
						
						/* Allow loop termination if abort option is selected.  */
						/* Breaks out of collection loop.  */
						GetCtrlVal (panelHandle, instrpanel_abortdata, &abortdata);
						if (abortdata == 1)
						{
							break;
						}
				
						if (i == 0)
						{
							testforce = (forcedata[rownumber-2][2]
											+forcedata[rownumber-2][5])/2;
						}
						else
						{
							testforce = (forcedata[rownumber-1][2]
											+forcedata[rownumber-1][5])/2;							
						}
						
						/* Safety - check position against piezo limits. */
						/* Upper limit - 20.0 microns total travel (sum of control + hardware). */
						/* Lower limit - 0.0 microns total or < 0.0 microns control input. */
						/* Break from loop if limits exceeded & return to start through abort.*/
						/* Allows data to be saved without loss of program control. */
						if ((piezoposition[0] + stepresolution/1000) >= 25.0)
						{
						 	MessagePopup ("Piezo Control Error", 
						 				"Total output cannot exceed 25.0 microns!");
							abortdata = 1;
						 	break;
						}
						else if ((piezoposition[0]  - stepresolution/1000) <= 0.0)
						{
						 	MessagePopup ("Piezo Control Error", 
						 				"Controller output cannot be less than 0.0 microns!");
						 	abortdata = 1;
						 	break;
						}						
						
					
						if (testforce >= (targetforce + forcethreshold)) 
						{	
						
							DirectVal[0]=0;
							DAQmxWriteDigitalLines (Direction, 1, 1, 10.0, DAQmx_Val_GroupByChannel, DirectVal, &Dummyone, 0);
							DAQmxStartTask(dgWriteHandle);
												
				
						//	printf("decreasing; testforce=%0.1f\n\n",testforce);
						   	 
						}
						if (testforce < (targetforce - forcethreshold))   
						{	
							
							DirectVal[0]=1;
							DAQmxWriteDigitalLines (Direction, 1, 1, 10.0, DAQmx_Val_GroupByChannel, DirectVal, &Dummyone, 0);
							DAQmxStartTask(dgWriteHandle);  
												
					
							//printf("increasing; testforce=%0.1f\n\n",testforce) ;    
						}
	
						  
						Delay(pausedelay/1000);						
						
 					
						ReadData(avg) ; 
						
					if(SnapFFEOn==1 && i%FFESnapItr==0 && FfeFlag==1)
						{
				ImgIndexSave=SNAP2 (30000);
					DisplayStoreData(pauseptsavg,ImgIndexSave);  
						}
						
					else if(SnapFFROn==1 && i%FFRSnapItr==0 && FfeFlag==0)
					{
				ImgIndexSave=SNAP2 (40000);
				DisplayStoreData(pauseptsavg,ImgIndexSave); 
					}
					else
						DisplayStoreData(pauseptsavg,0);
						
						
					
						elapsedtime2= Timer()- elapsedtime1;
							
						SetCtrlVal (panelHandle, instrpanel_elapsedtime, elapsedtime2);   
						SetCtrlVal (panelHandle, instrpanel_ConstForceIternation, i);  
					
					DAQmxStopTask(dgWriteHandle);  			
				
					}
					/* Reset stepnumber to return stage to start (only if feedback is on). */
					/* Check current stage position and measured force (in microns). */
					/* Stepsnumber is reset in event of position drift due to feedback. */
				   
				if(Relforcefeedbackflag==0)
						elapsedtimeFFext= elapsedtime2;
				else
						elapsedtimeFFrel= elapsedtime2;
				
					
				
				stepsnumberRel = TruncateRealNumber((piezoposition[0]
											-forcedata[0][1])*1000/stepsincrement);
				remainder = (forcedata[0][1] + stepsincrement*stepsnumberRel/1000);
				setstagepstn =  remainder/2;
					  stepsnumberRel=stepsnumberRel+1;
				
         		forcefeedbackFlag=0; 
				   
				DAQmxStopTask(dgWriteHandle);  
				return(0);

}
						
						

int ReadData (int avgpnts)


{
	


	DAQmxStopTask(collectHandle);      
	
	avgattr=avgpnts;
	
	DAQmxSetTimingAttribute (collectHandle, DAQmx_SampQuant_SampPerChan, avgattr);
   
	  //clock_t begin, end; 
//float	DaqStart = Timer();
	  DAQmxStartTask(collectHandle); 
 
	DAQmxReadAnalogF64(collectHandle,avgpnts,1,DAQmx_Val_GroupByChannel,rawdata,avgpnts*6,&numRead,NULL);
//float	DaqEnd = Timer()-DaqStart;

/*	for (int i=0;i<avg*7;i++)
			printf("%f\t,%f\t,%f\t,%f\t,%f\t,%f\t,%f\t\n",rawdata[i],rawdata[i+avg],rawdata[i+2*avg],rawdata[i+3*avg],rawdata[i+4*avg],rawdata[i+5*avg],rawdata[i+6*avg]);   */ 
			
		   					

	/* Initialize averaging arrays.  */
	tempdata1x[0]  		= 0;
	tempdata1y[0]		= 0;
	tempdata2x[0]		= 0;
	tempdata2y[0]		= 0;
	temppower1[0] 		= 0;
	temppower2[0]		= 0;
	readpiezopstn[0]	= 0;
	

	for (j = 0; j < avgpnts; j++)
	{
		/* Average detector data.  */
		
		
			tempdata1x[0] 		= tempdata1x[0] + 1000*(rawdata[j])/avgpnts;
			tempdata1y[0] 		= tempdata1y[0] + 1000*(rawdata[avgpnts+j])/avgpnts;
			temppower1[0]		= temppower1[0] + (0.2)*(rawdata[avgpnts*2+j])/avgpnts;   
			tempdata2x[0]		= tempdata2x[0]  -	1000*(rawdata[avgpnts*3+j])/avgpnts; 
			tempdata2y[0] 		= tempdata2y[0] + 1000*(rawdata[avgpnts*4+j])/avgpnts; 
			temppower2[0]		= temppower2[0] + (0.2)*(rawdata[avgpnts*5+j])/avgpnts; 
		
	}
	DAQmxStopTask(collectHandle);   
	
	//CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, ThreadFunction, NULL, NULL);
	 
	ReadPiezo(); 
	yavg=(tempdata1y[0]+tempdata2y[0])/2 ;
	xavg= (tempdata1x[0]+tempdata2x[0])/2; 
	//currentpiezoposition= num_of_counts*1e6;
//piezoposition[0] = num_of_counts*1e6 + 12.5; 
	
	/*Update 'Power Meters'.  */
	SetCtrlVal (panelHandle, instrpanel_detector1power, temppower1[0]);
    SetCtrlVal (panelHandle, instrpanel_detector2power, temppower2[0]);
																	   
	
	//SetCtrlVal (panelHandle, instrpanel_ForceDisplay, ForceAvg);
	
	
	/* Delete graph and replot.  */
    DeleteGraphPlot (panelHandle,instrpanel_beampositiongraph,-1,VAL_IMMEDIATE_DRAW);
	PlotXY (panelHandle, instrpanel_beampositiongraph, tempdata1x, tempdata1y, 1, VAL_FLOAT, VAL_FLOAT, VAL_SCATTER, VAL_SOLID_CIRCLE, VAL_SOLID, 1, 0x00066CC);
	PlotXY (panelHandle,instrpanel_beampositiongraph,tempdata2x,tempdata2y,1,VAL_FLOAT,VAL_FLOAT,
	VAL_SCATTER,VAL_SOLID_CIRCLE,VAL_SOLID,1,0x006600);
	  
	
							 
																									   
		
	//	printf("digital=%0.6f nm\n",num_of_counts*1e9);
		
		 //	DAQmxStopTask (QuadMonitor);  

	
	
	return (0);
	
}	
			

														   

int DisplayStoreData (int avg, int imgindextosave)

{			
	
	SuspendTimerCallbacks ();


	/*Update 'Power Meters'.  */
	SetCtrlVal (panelHandle, instrpanel_detector1power, temppower1[0]);
    SetCtrlVal (panelHandle, instrpanel_detector2power, temppower2[0]);
	SetCtrlVal (panelHandle, instrpanel_ExtensionDisplay,piezoposition[0]);
	SetCtrlVal (panelHandle, instrpanel_ForceDisplay,xavg);  
	SetCtrlVal (panelHandle, instrpanel_cellpiezoposition,currentpiezoposition);  
	elapsedtime = Timer() - zerotime;
				

	/* Draw data points onto large graph - solid circles for extension, */
	/* open circles for relaxation.                                     */


	if (rewindindicator == 0)
	{
		/* For 'Force Extension Data' - add data point to graph.  */
		PlotXY (panelHandle, instrpanel_forceextensiongraph,piezoposition,tempdata1x,1, VAL_DOUBLE, VAL_FLOAT, VAL_SCATTER,VAL_SOLID_CIRCLE, VAL_SOLID, 1, 0x00066CC);
		PlotXY (panelHandle, instrpanel_forceextensiongraph, piezoposition,
				tempdata2x, 1, VAL_DOUBLE, VAL_FLOAT, VAL_SCATTER,
				VAL_SOLID_CIRCLE, VAL_SOLID, 1, 0x006600);
		
		
		
	}
	else
	{
		/* For 'Force Extension Data' - add data point to graph.  */
		PlotXY (panelHandle, instrpanel_forceextensiongraph,piezoposition,
				tempdata1x, 1, VAL_DOUBLE, VAL_FLOAT, VAL_SCATTER,
				VAL_EMPTY_CIRCLE, VAL_SOLID, 1, 0x00066CC);
		PlotXY (panelHandle, instrpanel_forceextensiongraph, piezoposition, tempdata2x, 1, VAL_DOUBLE, VAL_FLOAT, VAL_SCATTER, VAL_EMPTY_CIRCLE, VAL_SOLID, 1, 0x0006600);
	}
	
	 elapsedtime0[0]=elapsedtime;
	  
	//	if(forcefeedbackRel == 1 || forcefeedback == 1) {    
	if(PopupExtTimeGraph==1 && forcefeedbackFlag==1)
	{
	PlotXY (ExtTimePanelHandle, PANELXVT_GRAPH, elapsedtime0, piezoposition, 1, VAL_DOUBLE, VAL_DOUBLE, VAL_SCATTER, VAL_SOLID_CIRCLE, VAL_SOLID, 1, 0x808000L);
//XYGraphPopup ("Ext vs Time", &elapsedtime, piezoposition, 1, VAL_DOUBLE, VAL_DOUBLE);
	}
	
			
				forcedata[rownumber][0] = elapsedtime;
				forcedata[rownumber][1] = piezoposition[0];
				forcedata[rownumber][2] = tempdata1x[0];
 				forcedata[rownumber][3] = tempdata1y[0];
				forcedata[rownumber][4] = temppower1[0];
				forcedata[rownumber][5] = tempdata2x[0];
 				forcedata[rownumber][6] = tempdata2y[0];
				forcedata[rownumber][7] = temppower2[0];
				forcedata[rownumber][8] = imgindextosave;
				
				
				rownumber++ ; 


	return (0);
}
						
						
						



/*---------------------------------------------------------------------------*/
/* Lasercommit - function commands Newport laser controller through GPIB     */
/* protocol.  Device is restored to local control after update.  Works       */
/* through the functions Readlasercontrols and Updatelasers.                 */
/*---------------------------------------------------------------------------*/







/*---------------------------------------------------------------------------*/
/* Save - function saves current data set as seen on screen, including 		 */
/* stage position, beam positions and power. 								 */
/* Array is saved as text.													 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK Save (void)  
{
	
	
		GetCtrlVal (panelHandle, instrpanel_FileSaveName, FileSaveName);
		sprintf(FileSaveName2,"%s.txt",FileSaveName);  
		returnstatus = FileSelectPopup ("My Documents",FileSaveName2, "*.*", "Save Data", VAL_SAVE_BUTTON, 0, 0, 1, 1, filestorage);
		if (returnstatus == 0)   {
			MessagePopup ("Save Data", "No file selected - data not saved!");
	
		}
		else
		
		{	
			
		  	
			/* Save data as ascii text, separated by spaces.  */
			ArrayToFile (filestorage, forcedata, VAL_FLOAT,(9*(totrownumber)), (totrownumber),VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_SEP_BY_TAB,10, VAL_ASCII, VAL_TRUNCATE);
			savestatus = 0;	
		
			/* Reset status to indicate data is saved.  */
			
			/*Save Details*/
			
		GetCtrlVal (panelHandle,instrpanel_Savedetails, &Savedetails); 
			
		if(Savedetails==1)
			
		{
			
		savestatus = 0;	
		 
			
		
		/* Get constant position (pause) variables. */
		GetCtrlVal (panelHandle, instrpanel_pauseiterations, &pauseiterations);			
		GetCtrlVal (panelHandle, instrpanel_pausedelay, &pausedelay);				
		GetCtrlVal (panelHandle, instrpanel_pauseptsavg, &pauseptsavg);
		
		GetCtrlVal (panelHandle, instrpanel_pauseiterationsRel, &pauseiterationsRel);			
		GetCtrlVal (panelHandle, instrpanel_pausedelayRel, &pausedelayRel);				
		GetCtrlVal (panelHandle, instrpanel_pauseptsavgRel, &pauseptsavgRel);	
	
	
		/* Determine if force feedback is on. */
		GetCtrlVal (panelHandle, instrpanel_forcefeedback, &forcefeedback);
		GetCtrlVal (panelHandle, instrpanel_forcefeedbackRel, &forcefeedbackRel); 
		
		/* Get feedback collection variables.  */
		GetCtrlVal (panelHandle, instrpanel_targetforce, &targetforce);			
		GetCtrlVal (panelHandle, instrpanel_forcethreshold, &forcethreshold);				
		GetCtrlVal (panelHandle, instrpanel_stepresolution, &stepresolution);
		
		GetCtrlVal (panelHandle, instrpanel_targetforceRel, &targetforceRel);			
		GetCtrlVal (panelHandle, instrpanel_forcethresholdRel, &forcethresholdRel);				
		GetCtrlVal (panelHandle, instrpanel_stepresolutionRel, &stepresolutionRel);
		
			
			
			
			
			
		
		GetCtrlVal(panelHandle,instrpanel_Comments, Comments);
		GetCtrlVal(panelHandle,instrpanel_ProteinName, ProteinName); 
		GetCtrlVal (panelHandle,instrpanel_Concentration, &Conc);
		GetCtrlVal (panelHandle,instrpanel_stockconc, &stockconc);
		GetCtrlVal (panelHandle,instrpanel_stockvol, &stockvol);
		GetCtrlVal (panelHandle,instrpanel_finalvol, &finalvol);
		FinalConc = (stockconc*stockvol)/(1000* finalvol);  
	
		sprintf(SaveName,"Experimental Log of %s",filestorage); 
		
		
		
		if(forcefeedback==1 && forcefeedbackRel==0)  																																																																												    
		sprintf (details, "Date\t:\t%s\nTime\t:\t%s\n%s\n\n----Protein Info--------------------\n  Protein Name\t: %s\n  Concentration\t: %0.2f nM\n  Comments\t: %s\n\n----Protein Dilution----------------\n  Stock Conc\t\t: %0.f nM\n  Stock Vol\t\t: %0.1f ul\n  Final Vol\t\t: %0.f ml\n  Cal Final Conc\t: %0.1f nM\n\n---Stretch/Release Info------------\n  No. Of Steps\t: %i\n  StepIncrement\t: %0.1f nm\n  Step Delay\t: %0.1f ms\n  Points Avg\t: %i \n\n----Force Feedback-Extension Info-------------\n  Status\t\t: Active\n  Time Elapsed\t\t: %0.3f sec\n  Target Force\t\t: %0.1f mic\n  Force Threshold\t: %0.1f mic\n  Step Resolution\t: %0.1f mic\n  Iterations\t\t: %i \n  delay\t\t\t: %0.1f s\n  Points Avg\t\t: %i\n", DateStr(),TimeStr(),SaveName,ProteinName,Conc,Comments,stockconc,stockvol,finalvol,FinalConc,stepsnumber,stepsincrement,stepsdelay,averagepoints,elapsedtimeFFext,targetforce,forcethreshold,stepresolution,pauseiterations,pausedelay,pauseptsavg);
	
		else if(forcefeedback==0 && forcefeedbackRel==1)  																																																																												    
		sprintf (details, "Date\t:\t%s\nTime\t:\t%s\n%s\n\n----Protein Info--------------------\n  Protein Name\t: %s\n  Concentration\t: %0.2f nM\n  Comments\t: %s\n\n----Protein Dilution----------------\n  Stock Conc\t\t: %0.f nM\n  Stock Vol\t\t: %0.1f ul\n  Final Vol\t\t: %0.f ml\n  Cal Final Conc\t: %0.1f nM\n\n---Stretch/Release Info------------\n  No. Of Steps\t: %i\n  StepIncrement\t: %0.1f nm\n  Step Delay\t: %0.1f ms\n  Points Avg\t: %i \n\n----Force Feedback-Return Info-------------\n  Status\t\t: Active\n Time Elapsed\t\t: %0.3f sec\n  Target Force\t\t: %0.1f mic\n  Force Threshold\t: %0.1f mic\n  Step Resolution\t: %0.1f mic\n  Iterations\t\t: %i \n  delay\t\t\t: %0.1f s\n  Points Avg\t\t: %i\n", DateStr(),TimeStr(),SaveName,ProteinName,Conc,Comments,stockconc,stockvol,finalvol,FinalConc,stepsnumber,stepsincrement,stepsdelay,averagepoints,elapsedtimeFFrel,targetforceRel,forcethresholdRel,stepresolutionRel,pauseiterationsRel,pausedelayRel,pauseptsavgRel);
		
		else if(forcefeedback==1 && forcefeedbackRel==1)
		{
		sprintf (details, "Date\t:\t%s\nTime\t:\t%s\n%s\n\n----Protein Info--------------------\n  Protein Name\t: %s\n  Concentration\t: %0.2f nM\n  Comments\t: %s\n\n----Protein Dilution----------------\n  Stock Conc\t\t: %0.f nM\n  Stock Vol\t\t: %0.1f ul\n  Final Vol\t\t: %0.f ml\n  Cal Final Conc\t: %0.1f nM\n\n---Stretch/Release Info------------\n  No. Of Steps\t: %i\n  StepIncrement\t: %0.1f nm\n  Step Delay\t: %0.1f ms\n  Points Avg\t: %i \n\n----Force Feedback-Extension Info-------------\n  Status\t\t: Active\n  Time Elapsed\t\t: %0.3f sec\n  Target Force\t\t: %0.1f mic\n  Force Threshold\t: %0.1f mic\n  Step Resolution\t: %0.1f mic\n  Iterations\t\t: %i \n  delay\t\t\t: %0.1f s\n  Points Avg\t\t: %i\n", DateStr(),TimeStr(),SaveName,ProteinName,Conc,Comments,stockconc,stockvol,finalvol,FinalConc,stepsnumber,stepsincrement,stepsdelay,averagepoints,elapsedtimeFFext,targetforce,forcethreshold,stepresolution,pauseiterations,pausedelay,pauseptsavg);
		sprintf (details1, "\n----Force Feedback-Return Info-------------\n  Status\t\t: Active\n  Time Elapsed\t\t: %0.3f sec\n  Target Force\t\t: %0.1f mic\n  Force Threshold\t: %0.1f mic\n  Step Resolution\t: %0.1f mic\n  Iterations\t: %i \n  delay\t\t: %0.1f s\n  Points Avg\t: %i\n",elapsedtimeFFrel,targetforceRel,forcethresholdRel,stepresolutionRel,pauseiterationsRel,pausedelayRel,pauseptsavgRel); 
		strcat(details, details1);
		}

		else  
		sprintf (details, "Date\t:\t%s\nTime\t:\t%s\n%s\n\n----Protein Info--------------------\n  Protein Name\t:\t%s\n  Concentration\t:\t%0.2f nM\n  Comments\t:\t%s\n\n----Protein Dilution----------------\n  Stock Conc\t\t: %0.f nM\n  Stock Vol\t\t: %0.1f ul\n  Final Vol\t\t: %0.f ml\n  Cal Final Conc\t: %0.1f nM\n\n---Stretch/Release Info------------\n  No. Of Steps\t:%i\n  StepIncrement\t:%0.1f nm\n  Step Delay\t:%0.1f ms\n  Points Avg\t:%i \n\n----Force Feedback Info-------------\n  Status\t: Disabled ", DateStr(),TimeStr(),SaveName,ProteinName,Conc,Comments, stockconc,stockvol,finalvol,
		FinalConc,stepsnumber,stepsincrement,stepsdelay,averagepoints);


		int nc;
		if (( nc = FindPattern (filestorage, 0, -1, ".", 0, 1)) > 0) filestorage[nc] = 0;
		 
		sprintf(AutoSaveName,"%s,details.txt",filestorage); 
		fh1 = OpenFile (AutoSaveName, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_ASCII);
		WriteFile (fh1, details,strlen(details));
		CloseFile (fh1);
	
		}
			
			
			
	
			
		}	
	
	
	return (0);
}


/*---------------------------------------------------------------------------*/
/* Quit - function exits the application.  Does not save data!               */
/*---------------------------------------------------------------------------*/
int CVICALLBACK Quit (int panel, int control, int event, void *callbackData,
				      int eventData1, int eventData2)
{
	if (event == EVENT_COMMIT)
	{
		
		int numItems=0;;
		
		if(mainPanelHandle)
		GetNumListItems (mainPanelHandle, MAINPANEL_FUNCTIONLISTBOX, &numItems); 
		
		if(numItems!=0)
		{
		
		MessagePopup ("Camera in progress", "Yo! Stop the Camera First!");
		}
		else{
		confirm = ConfirmPopup ("Instrument Shutdown", 
								 "              Exit the program?"); 
		if (confirm == 1) 
		{
			
    	
    		/* Determine if previous data is to be saved.  */
			if (savestatus == 1)
			{
				saveconfirm = ConfirmPopup ("Confirm",
					  				"Current data set is not saved.  Save it now?");
			
				/* Call Save if a 'yes' response is received.  */		  				
				if (saveconfirm == 1)
				{
					Save ();
				}
			}
			/* To close user interface protocols.  */
    	
			ExitFunction();  
			QuitUserInterface (0); 
    	}
		}
    }
	return (0);
}



int CVICALLBACK calcconc (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
		GetCtrlVal (panelHandle,instrpanel_stockconc, &stockconc);
		GetCtrlVal (panelHandle,instrpanel_stockvol, &stockvol);
		GetCtrlVal (panelHandle,instrpanel_finalvol, &finalvol);
		
		FinalConc = (stockconc*stockvol)/(1000* finalvol);
		
		SetCtrlVal (panelHandle, instrpanel_FinalConc, FinalConc);
		
		

			break;
	}
	return 0;
}

int CVICALLBACK MoveZeroOffset(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
//	SuspendTimerCallbacks ();
	GetCtrlVal (panelHandle, instrpanel_ZeroOffset, &ZeroOffset);
	GetCtrlVal (panelHandle, instrpanel_stepsnumber, &stepsnumber);
	GetCtrlVal (panelHandle, instrpanel_OffsetStepDelay, &offsetstepdelay);
/*	MG17Drivers_IPiezosGetCurrentPosition (piezohandle, NULL, "xaxis",
											   &offset, &returnval);*/

	
	
				 	if (ZeroOffset>stepsnumber)
		{
							MessagePopup ("Invalid Zero Offset",
					   		"Zero Offset should be lesser than number of steps");
							ZeroOffset=0;
							break;
		}
	
		
			 if(ZeroOffsetMoveFlag == 0 && ZeroOffset>0)
			 {
			ZeroOffsetLED=1;  	 
			
			GetCtrlVal (panelHandle, instrpanel_stepsincrement, &stepsincrement);
			 
			SetCtrlVal (panelHandle, instrpanel_ZeroOffsetLED, ZeroOffsetLED);  
			 
			for (i = 0; i <= ZeroOffset; i++)
			
				{
				
				
				GetCtrlVal (panelHandle, instrpanel_ZeroOffset, &ZeroOffset);   
				
				setstagepstn =  i*(stepsincrement)/2000;
				DAQmxWriteAnalogF64(stageHandle,1,1,1.0,DAQmx_Val_GroupByChannel,&setstagepstn,NULL,NULL); 
				
				Delay(offsetstepdelay/1000); 
			
				GetCtrlVal (panelHandle, instrpanel_ZeroOffset, &ZeroOffset);
				
				}
			    Delay(0.3) ;
				/*MG17Drivers_IPiezosGetCurrentPosition (piezohandle, NULL, "xaxis",
											   &piezoposition, &returnval);   */
				SetCtrlVal (panelHandle, instrpanel_OffsetDisplay,(piezoposition[0]-offset)); 
				ZeroOffsetMoveFlag = 1 ;
				currentsetstagepstn= setstagepstn;
				
			 }	
					
			
	}
	return 0;
}

int CVICALLBACK GoBackZeroOffset (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		//SuspendTimerCallbacks ();
		
		GetCtrlVal (panelHandle,instrpanel_ZeroOffset, &ZeroOffset);
		GetCtrlVal (panelHandle, instrpanel_OffsetStepDelay, &offsetstepdelay);     
			
			 if(ZeroOffsetMoveFlag == 1 && ZeroOffset>0)
			 {
			
			
			GetCtrlVal (panelHandle, instrpanel_stepsincrement, &stepsincrement);
			for (i = ZeroOffset; i >= 0; i--)
			
				{
				
				
				setstagepstn =  i*(stepsincrement)/2000;
			//	DAQmxWriteAnalogF64(stageHandle,1,1,1.0,DAQmx_Val_GroupByChannel,&setstagepstn,NULL,NULL); 
				
				 
				Delay(offsetstepdelay/1000);
				
				
				
				}
				Delay(0.3) ; 
				/*MG17Drivers_IPiezosGetCurrentPosition (piezohandle, NULL, "xaxis",
											   &piezoposition, &returnval);
				SetCtrlVal (panelHandle, instrpanel_OffsetDisplay,(piezoposition-offset));*/ 
			 }	
			ZeroOffset=0;
			ZeroOffsetLED=0;
			ZeroOffsetMoveFlag = 0 ;
			SetCtrlVal (panelHandle, instrpanel_ZeroOffsetLED, ZeroOffsetLED); 
			currentsetstagepstn=0;
		
			
	}
	return 0;
}

int CVICALLBACK ZeroStage (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
	enableval[0]=0;
	DAQmxWriteDigitalLines (EnableTask, 1, 1, 10.0, DAQmx_Val_GroupByChannel,enableval, &Dummyone, 0);
	enableval[0]=1;     
	DAQmxWriteDigitalLines (EnableTask, 1, 1, 10.0, DAQmx_Val_GroupByChannel,enableval, &Dummyone, 0);
	//DAQmxStopTask(dgWriteHandle);
	
	
			break;
	}
	return 0;
}

int CVICALLBACK SetStageOffset (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
		//ZeroStage (int panel, int control, int event,void *callbackData, int eventData1, int eventData2);	
			//DAQmxStopTask(dgWriteHandle);
			GetCtrlVal (panelHandle, instrpanel_PiezoOffsetControl, &stageoffset);	
		
		//CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, ThreadFunction, NULL, NULL);
	 	//DAQmxStopTask(dgWriteHandle); 
	   
		//enableval[0]=0;
//	DAQmxWriteDigitalLines (EnableTask, 1, 1, 10.0, DAQmx_Val_GroupByChannel,enableval, &Dummyone, 0);
//	enableval[0]=1;     
//	DAQmxWriteDigitalLines (EnableTask, 1, 1, 10.0, DAQmx_Val_GroupByChannel,enableval, &Dummyone, 0);
		for(int i=0;i<2;i++)
		{
		ReadPiezo();
	if(currentpiezoposition+12.5>=stageoffset)
	{
 diffpiezoposition= currentpiezoposition+12.5 - stageoffset;
DirectVal[0]=0;
}
else
	 {
DirectVal[0]=1;
	diffpiezoposition= - (currentpiezoposition+12.5) + stageoffset;
}
 

	 //printf("diff=%0.1f\ncurrent=%0.1f",diffpiezoposition,currentpiezoposition);     
	 DAQmxWriteDigitalLines (Direction, 1, 1, 10.0, DAQmx_Val_GroupByChannel, DirectVal, &Dummyone, 0);
	 //DAQmxSetChanAttribute (dgWriteHandle, "Dev1/ctr1", DAQmx_CO_Pulse_Freq, 400000.00, 1000); 
	SampChan=(diffpiezoposition)*(41.94304)*1000;
	
	if( SampChan>=1)
	{
	 DAQmxStopTask(dgWriteHandle); 
	DAQmxSetTimingAttribute (dgWriteHandle, DAQmx_SampQuant_SampPerChan, SampChan);

	DAQmxStartTask(dgWriteHandle);
	Delay(1);
	}
	}
	}  
	return 0;	  
}

int CVICALLBACK ReadPiezo(void)	   //Have to run data aquisition in a new thread to be able to access the UI after you start generating a pulse.
{
	 

	DAQmxReadCounterF64Ex (QuadMonitor, 2000, 1, DAQmx_Val_GroupByScanNumber, stageDigiRead, 2000, &Dumm2, 0);
//	DAQmxReadCounterScalarF64 (QuadMonitor, 10.0, &num_of_countsScalar, 0);
	//DAQmxReadCounterU32Ex (QuadMonitor, 1000, 1, DAQmx_Val_GroupByChannel, stageDigiRead, 1000,&Dumm2 , 0);
	//DAQmxReadCounterScalarF64 (QuadMonitor, 10.0, , 0);
	Mean (stageDigiRead,2000, &num_of_counts);
	currentpiezoposition= num_of_counts*1e6;
	piezoposition[0] = num_of_counts*1e6+12.5;
//	DAQmxStopTask (QuadMonitor);
	
//	printf("%0.4f\n%0.4f\n\n",piezoposition[0],num_of_countsScalar*1e6+12.5);
	return currentpiezoposition;
}

int CVICALLBACK SaveButton (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			  	Save ();   
			break;
	}
	return 0;
}

int CVICALLBACK LoadCamera (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	//	int mainPanelHandle=NULL;
	 
		
		if(mainPanelHandle==0){
		mainPanelHandle = LoadPanelEx (0, "MultiPanel.uir", MAINPANEL, __CVIUserHInst);
		MulitMain(0, 0) ;
        }
		else
		{	
		SetActivePanel(mainPanelHandle);	
		}
		
	
		break;
	}
	return 0;
}

int CameraAttributesValZero(void) 
{
SnapStrRetrnOn = 0;
SnapFFEOn = 0;
SnapFFROn= 0;
EnableSnapShots=0;
SetCtrlVal (panelHandle, instrpanel_ENABLESNAP, 0);
ImgNum=0 ;
return 0;
}


int CVICALLBACK ExtvsTimeExit (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
		DiscardPanel(ExtTimePanelHandle);
   		ExtTimePanelHandle=0; 

			break;
	}
	return 0;
}


