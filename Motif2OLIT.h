/* defines for compatibility between Motif and OLIT */

#ifndef Motif2OLIT
#define Motif2OLIT

#ifdef OLIT

#define XmNbackground XtNbackground
#define XmNforeground XtNforeground
#define XmNheight XtNheight
#define XmString String
#define XmNlabelString XtNstring
#define XmNmessageString XtNstring
#define XmNmaximum XtNsliderMax
#define XmNvalue XtNsliderValue
#define XmNsensitive XtNsensitive
#define XmNiconPixmap XtNiconPixmap

#define XmNactivateCallback XtNselect
#define XmNexposeCallback XtNexposeCallback
#define XmNresizeCallback XtNresizeCallback
#define XmNvalueChangedCallback XtNsliderMoved
#define XmNfocusCallback XtNpopupCallback

#define XmScaleCallbackStruct OlSliderVerify
#define XmDrawingAreaCallbackStruct OlDrawAreaCallbackStruct

#define xmLabelWidgetClass staticTextWidgetClass

#define BUTTON_PRESS OL_SELECTKEY

#else

#define BUTTON_PRESS "ArmAndActivate"

#endif  /* OLIT */

#endif  /* Motif2OLIT */
