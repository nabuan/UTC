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

#define  calibpanel                       1
#define  calibpanel_calibrationgraph      2       /* control type: graph, callback function: (none) */
#define  calibpanel_calibratestart        3       /* control type: command, callback function: Startcalibrate */
#define  calibpanel_exitcalibration       4       /* control type: command, callback function: Exitcalibrate */
#define  calibpanel_sphereradius          5       /* control type: numeric, callback function: (none) */
#define  calibpanel_viscosity             6       /* control type: numeric, callback function: (none) */
#define  calibpanel_viscousforce          7       /* control type: numeric, callback function: (none) */
#define  calibpanel_samplingrate          8       /* control type: numeric, callback function: (none) */
#define  calibpanel_frequency             9       /* control type: numeric, callback function: (none) */
#define  calibpanel_amplitude             10      /* control type: numeric, callback function: (none) */
#define  calibpanel_DECORATION_5          11      /* control type: deco, callback function: (none) */
#define  calibpanel_DECORATION_4          12      /* control type: deco, callback function: (none) */
#define  calibpanel_TEXTMSG_7             13      /* control type: textMsg, callback function: (none) */
#define  calibpanel_TEXTMSG_6             14      /* control type: textMsg, callback function: (none) */
#define  calibpanel_TEXTMSG_5             15      /* control type: textMsg, callback function: (none) */
#define  calibpanel_TEXTMSG_4             16      /* control type: textMsg, callback function: (none) */
#define  calibpanel_TEXTMSG_3             17      /* control type: textMsg, callback function: (none) */

#define  instrpanel                       2
#define  instrpanel_forceextensiongraph   2       /* control type: graph, callback function: (none) */
#define  instrpanel_collectdata           3       /* control type: command, callback function: Collect */
#define  instrpanel_detector2power        4       /* control type: scale, callback function: (none) */
#define  instrpanel_detector1power        5       /* control type: scale, callback function: (none) */
#define  instrpanel_savedata              6       /* control type: command, callback function: SaveButton */
#define  instrpanel_pauseptsavgRel        7       /* control type: numeric, callback function: (none) */
#define  instrpanel_pauseptsavg           8       /* control type: numeric, callback function: (none) */
#define  instrpanel_averagepoints         9       /* control type: numeric, callback function: (none) */
#define  instrpanel_stepsdelay            10      /* control type: numeric, callback function: (none) */
#define  instrpanel_stepsincrement        11      /* control type: numeric, callback function: (none) */
#define  instrpanel_pausedelayRel         12      /* control type: numeric, callback function: (none) */
#define  instrpanel_pausedelay            13      /* control type: numeric, callback function: (none) */
#define  instrpanel_pauseiterationsRel    14      /* control type: numeric, callback function: (none) */
#define  instrpanel_PiezoOffsetControl    15      /* control type: numeric, callback function: (none) */
#define  instrpanel_SNAPFFR               16      /* control type: numeric, callback function: (none) */
#define  instrpanel_SNAPFFE               17      /* control type: numeric, callback function: (none) */
#define  instrpanel_SNAPSTRETCHRETURN     18      /* control type: numeric, callback function: (none) */
#define  instrpanel_pauseiterations       19      /* control type: numeric, callback function: (none) */
#define  instrpanel_stepresolutionRel     20      /* control type: numeric, callback function: (none) */
#define  instrpanel_stepresolution        21      /* control type: numeric, callback function: (none) */
#define  instrpanel_forcethresholdRel     22      /* control type: numeric, callback function: (none) */
#define  instrpanel_forcethreshold        23      /* control type: numeric, callback function: (none) */
#define  instrpanel_targetforceRel        24      /* control type: numeric, callback function: (none) */
#define  instrpanel_AbortCheck            25      /* control type: numeric, callback function: (none) */
#define  instrpanel_targetforce           26      /* control type: numeric, callback function: (none) */
#define  instrpanel_OffsetStepDelay       27      /* control type: numeric, callback function: (none) */
#define  instrpanel_ZeroOffset            28      /* control type: numeric, callback function: (none) */
#define  instrpanel_stepsnumber           29      /* control type: numeric, callback function: (none) */
#define  instrpanel_calcconc              30      /* control type: command, callback function: calcconc */
#define  instrpanel_quit                  31      /* control type: command, callback function: Quit */
#define  instrpanel_STRETCHRETRNSNAP      32      /* control type: deco, callback function: (none) */
#define  instrpanel_DECORATION_3          33      /* control type: deco, callback function: (none) */
#define  instrpanel_DECORATION_6          34      /* control type: deco, callback function: (none) */
#define  instrpanel_DECORATION_7          35      /* control type: deco, callback function: (none) */
#define  instrpanel_DECORATION_2          36      /* control type: deco, callback function: (none) */
#define  instrpanel_abortdata             37      /* control type: textButton, callback function: (none) */
#define  instrpanel_forcefeedbackRel      38      /* control type: textButton, callback function: (none) */
#define  instrpanel_forcefeedback         39      /* control type: textButton, callback function: (none) */
#define  instrpanel_beampositiongraph     40      /* control type: graph, callback function: (none) */
#define  instrpanel_acquire               41      /* control type: LED, callback function: (none) */
#define  instrpanel_timermonitor          42      /* control type: timer, callback function: Monitor */
#define  instrpanel_TEXTMSG_21            43      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_24            44      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_19            45      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_6             46      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_5             47      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_23            48      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_3             49      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_7             50      /* control type: textMsg, callback function: (none) */
#define  instrpanel_ForceDisplay          51      /* control type: numeric, callback function: (none) */
#define  instrpanel_ExtensionDisplay      52      /* control type: numeric, callback function: (none) */
#define  instrpanel_OffsetDisplay         53      /* control type: numeric, callback function: (none) */
#define  instrpanel_elapsedtime           54      /* control type: numeric, callback function: (none) */
#define  instrpanel_ConstForceIternation  55      /* control type: numeric, callback function: (none) */
#define  instrpanel_FileSaveName          56      /* control type: string, callback function: (none) */
#define  instrpanel_ProteinName           57      /* control type: string, callback function: (none) */
#define  instrpanel_finalvol              58      /* control type: numeric, callback function: (none) */
#define  instrpanel_stockvol              59      /* control type: numeric, callback function: (none) */
#define  instrpanel_stockconc             60      /* control type: numeric, callback function: (none) */
#define  instrpanel_Concentration         61      /* control type: numeric, callback function: (none) */
#define  instrpanel_Comments              62      /* control type: textBox, callback function: (none) */
#define  instrpanel_Savedetails           63      /* control type: textButton, callback function: (none) */
#define  instrpanel_TEXTMSG_12            64      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_22            65      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_10            66      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_11            67      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_9             68      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_14            69      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_15            70      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_13            71      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_16            72      /* control type: textMsg, callback function: (none) */
#define  instrpanel_DECORATION_5          73      /* control type: deco, callback function: (none) */
#define  instrpanel_DECORATION_4          74      /* control type: deco, callback function: (none) */
#define  instrpanel_FinalConc             75      /* control type: numeric, callback function: (none) */
#define  instrpanel_DECORATION            76      /* control type: deco, callback function: (none) */
#define  instrpanel_TEXTMSG_17            77      /* control type: textMsg, callback function: (none) */
#define  instrpanel_TEXTMSG_18            78      /* control type: textMsg, callback function: (none) */
#define  instrpanel_GoBackZeroOffset      79      /* control type: command, callback function: GoBackZeroOffset */
#define  instrpanel_MoveZeroOffset        80      /* control type: command, callback function: MoveZeroOffset */
#define  instrpanel_ZeroOffsetLED         81      /* control type: LED, callback function: (none) */
#define  instrpanel_PROCSYS               82      /* control type: radioButton, callback function: (none) */
#define  instrpanel_SetStageOffset        83      /* control type: command, callback function: SetStageOffset */
#define  instrpanel_LOADCAMERA            84      /* control type: command, callback function: LoadCamera */
#define  instrpanel_ZeroStage             85      /* control type: command, callback function: ZeroStage */
#define  instrpanel_cellpiezoposition     86      /* control type: scale, callback function: (none) */
#define  instrpanel_SNAPSFFRCHECK         87      /* control type: radioButton, callback function: (none) */
#define  instrpanel_SNAPSFFECHECK         88      /* control type: radioButton, callback function: (none) */
#define  instrpanel_POPUPEXTGRPH          89      /* control type: radioButton, callback function: (none) */
#define  instrpanel_SNAPSTRRTRNCHECK      90      /* control type: radioButton, callback function: (none) */
#define  instrpanel_ENABLESNAP            91      /* control type: binary, callback function: EnableSnap */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK calcconc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Collect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK EnableSnap(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Exitcalibrate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GoBackZeroOffset(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadCamera(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Monitor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MoveZeroOffset(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetStageOffset(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Startcalibrate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ZeroStage(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
