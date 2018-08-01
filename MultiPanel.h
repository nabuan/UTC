/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAINPANEL                        1
#define  MAINPANEL_CREATEPOOL             2       /* control type: command, callback function: CreatePoolCallback */
#define  MAINPANEL_NUMTHREADSINPOOL       3       /* control type: numeric, callback function: NumThrdsCallback */
#define  MAINPANEL_MAINPRIORITY           4       /* control type: ring, callback function: MainPriorityCallback */
#define  MAINPANEL_CMAERAPANELEXIT        5       /* control type: command, callback function: CameraPanelExit */
#define  MAINPANEL_SCHEDULEFN             6       /* control type: command, callback function: ScheduleFunctionCallback */
#define  MAINPANEL_FUNCTIONLISTBOX        7       /* control type: listBox, callback function: FunctionListBoxChange */
#define  MAINPANEL_THREADPRIORITY         8       /* control type: ring, callback function: ThreadPriorityCallback */
#define  MAINPANEL_SNAPINST               9       /* control type: command, callback function: InstantSnapCallBack */
#define  MAINPANEL_EXITFUNCTION           10      /* control type: command, callback function: ExitFunctionCallback */
#define  MAINPANEL_QUICKATTRSETUP         11      /* control type: command, callback function: QuickAttrSetup */
#define  MAINPANEL_DECORATION             12      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_4           13      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_5           14      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_3           15      /* control type: deco, callback function: (none) */
#define  MAINPANEL_CAMERAGAINVAL          16      /* control type: ring, callback function: CameraGainVal */
#define  MAINPANEL_GAINMODE               17      /* control type: ring, callback function: GainMode */
#define  MAINPANEL_CHOOSECAMERA           18      /* control type: ring, callback function: (none) */
#define  MAINPANEL_Camera                 19      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_Mutithreading          20      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CameraGainVal(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CameraPanelExit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CreatePoolCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitFunctionCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FunctionListBoxChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GainMode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK InstantSnapCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPriorityCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NumThrdsCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuickAttrSetup(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ScheduleFunctionCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ThreadPriorityCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
