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

#define  PANEL                            1
#define  PANEL_QUIT                       2       /* control type: command, callback function: ChartPanelQuit */
#define  PANEL_TIMER                      3       /* control type: timer, callback function: ChartPanelTimerCallback */

#define  POPUP                            2
#define  POPUP_OK                         2       /* control type: command, callback function: (none) */
#define  POPUP_INFO                       3       /* control type: textBox, callback function: (none) */
#define  POPUP_NUMERICSLIDE               4       /* control type: scale, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ChartPanelQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ChartPanelTimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
