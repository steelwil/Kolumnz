; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKolumnzView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tetris.h"
LastPage=0

ClassCount=10
Class1=CControlKey
Class2=CCustomize
Class3=CEnterHighScore
Class4=CHighSoreDisplay
Class5=CMainFrame
Class6=CSetup
Class7=CKolumnzApp
Class8=CAboutDlg
Class9=CKolumnzDoc
Class10=CKolumnzView

ResourceCount=6
Resource1=IDD_SETUP_DIALOG
Resource2=IDD_OPTIONS_DIALOG
Resource3=IDD_ENTERHIGHSCORE
Resource4=IDD_HIGHSCORETABLE
Resource5=IDR_MAINFRAME
Resource6=IDD_ABOUTBOX

[CLS:CControlKey]
Type=0
BaseClass=CHotKeyCtrl
HeaderFile=ControlKey.h
ImplementationFile=ControlKey.cpp
LastObject=CControlKey

[CLS:CCustomize]
Type=0
BaseClass=CDialog
HeaderFile=Customize.h
ImplementationFile=Customize.cpp
Filter=D
VirtualFilter=dWC
LastObject=CCustomize

[CLS:CEnterHighScore]
Type=0
BaseClass=CDialog
HeaderFile=EnterHighScore.h
ImplementationFile=EnterHighScore.cpp
LastObject=1

[CLS:CHighSoreDisplay]
Type=0
BaseClass=CDialog
HeaderFile=HighSoreDisplay.h
ImplementationFile=HighSoreDisplay.cpp
LastObject=CHighSoreDisplay

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=W
VirtualFilter=fWC

[CLS:CSetup]
Type=0
BaseClass=CDialog
HeaderFile=Setup.h
ImplementationFile=Setup.cpp
LastObject=CSetup
Filter=D
VirtualFilter=dWC

[CLS:CKolumnzApp]
Type=0
BaseClass=CWinApp
HeaderFile=Kolumnz.h
ImplementationFile=Kolumnz.cpp
LastObject=CKolumnzApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Kolumnz.cpp
ImplementationFile=Kolumnz.cpp
LastObject=CAboutDlg

[CLS:CKolumnzDoc]
Type=0
BaseClass=CDocument
HeaderFile=KolumnzDoc.h
ImplementationFile=KolumnzDoc.cpp
Filter=N
VirtualFilter=DC
LastObject=CKolumnzDoc

[CLS:CKolumnzView]
Type=0
BaseClass=CView
HeaderFile=KolumnzView.h
ImplementationFile=KolumnzView.cpp
Filter=W
VirtualFilter=VWC
LastObject=CKolumnzView

[DLG:IDD_OPTIONS_DIALOG]
Type=1
Class=CCustomize
ControlCount=34
Control1=IDC_TITLE_EDIT,edit,1350631552
Control2=IDC_PLAYERS_RADIO,button,1342373897
Control3=IDC_RADIO8,button,1342242825
Control4=IDC_PREVEIW_CHECK,button,1342373891
Control5=IDC_PENALIZE_CHECK,button,1342242819
Control6=IDC_SOUND_CHECK,button,1342242819
Control7=IDC_ROWS_EDIT,edit,1350633600
Control8=IDC_ROWS_SPIN,msctls_updown32,1342177330
Control9=IDC_LEVEL_EDIT,edit,1350633600
Control10=IDC_LEVEL_SPIN,msctls_updown32,1342177330
Control11=IDC_BASE_EDIT,edit,1350633600
Control12=IDC_BASE_SPIN,msctls_updown32,1342177330
Control13=IDC_HEIGHT_EDIT,edit,1350633600
Control14=IDC_HEIGHT_SPIN,msctls_updown32,1342177330
Control15=IDC_LENGTH_EDIT,edit,1350633600
Control16=IDC_LENGTH_SPIN,msctls_updown32,1342177330
Control17=IDC_COLORS_EDIT,edit,1350633600
Control18=IDC_COLORS_SPIN,msctls_updown32,1342177330
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342242823
Control21=IDC_DEFAULTS_BUTTON,button,1342242816
Control22=IDC_KEYBOARD_BUTTON,button,1342242816
Control23=IDCANCEL,button,1342242816
Control24=IDOK,button,1342242817
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,button,1342177287
Control30=IDC_STATIC,button,1342177287
Control31=IDC_STATIC,button,1342177287
Control32=IDC_STATIC,button,1342177287
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352

[DLG:IDD_ENTERHIGHSCORE]
Type=1
Class=CEnterHighScore
ControlCount=18
Control1=IDC_NAME,edit,1350631424
Control2=IDC_QUOTE,edit,1350631424
Control3=IDC_STATIC,static,1342177280
Control4=IDC_STATIC,static,1342177280
Control5=IDC_STATIC,static,1342177281
Control6=IDC_STATIC,static,1342177281
Control7=IDC_STATIC,static,1342177283
Control8=IDC_STATIC,static,1342177283
Control9=IDC_STATIC,static,1342177283
Control10=IDC_STATIC,static,1342177283
Control11=IDC_STATIC,static,1342177283
Control12=IDC_STATIC,static,1342177283
Control13=IDC_STATIC,static,1342177283
Control14=IDC_STATIC,static,1342177283
Control15=1,button,1342242816
Control16=IDC_STATIC,static,1342177287
Control17=IDC_STATIC,static,1342177287
Control18=900,static,1342177281

[DLG:IDD_HIGHSCORETABLE]
Type=1
Class=CHighSoreDisplay
ControlCount=26
Control1=IDC_TITLE_STATIC,static,1342177793
Control2=IDC_STATIC,static,1342177287
Control3=IDC_STATIC,static,1342177287
Control4=IDC_OK_BUTTON,button,1342242816
Control5=IDC_STATIC,static,1342177283
Control6=IDC_STATIC,static,1342177283
Control7=IDC_LIST,listbox,1342193920
Control8=IDC_SCORE_LIST,listbox,1342193920
Control9=IDC_RANK_LIST,listbox,1342193920
Control10=IDC_ROWS_LIST,listbox,1342193920
Control11=IDC_DATE_LIST,listbox,1342193920
Control12=IDC_LEVEL_LIST,listbox,1342193920
Control13=IDC_QUOTE_LIST,listbox,1342193920
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342177288
Control22=IDC_STATIC,static,1342177288
Control23=IDC_STATIC,static,1342177288
Control24=IDC_STATIC,static,1342177288
Control25=IDC_STATIC,static,1342177288
Control26=IDC_STATIC,static,1342177288

[DLG:IDD_SETUP_DIALOG]
Type=1
Class=CSetup
ControlCount=20
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LEFT1_HOTKEY,msctls_hotkey32,1350631424
Control4=IDC_RIGHT1_HOTKEY,msctls_hotkey32,1350631424
Control5=IDC_DROP1_HOTKEY,msctls_hotkey32,1350631424
Control6=IDC_ROTATE1_HOTKEY,msctls_hotkey32,1350631424
Control7=IDC_LEFT2_HOTKEY,msctls_hotkey32,1350631424
Control8=IDC_RIGHT2_HOTKEY,msctls_hotkey32,1350631424
Control9=IDC_DROP2_HOTKEY,msctls_hotkey32,1350631424
Control10=IDC_ROTATE2_HOTKEY,msctls_hotkey32,1350631424
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_DEFAULTS_BUTTON,button,1342242816
Control18=IDC_DOWN1_HOTKEY,msctls_hotkey32,1350631424
Control19=IDC_DOWN2_HOTKEY,msctls_hotkey32,1350631424
Control20=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_GAME_PLAY
Command5=ID_GAME_PAUSEF3
Command6=ID_GAME_HIGHSCORES
Command7=ID_OPTIONS_CUSTOMIZE
Command8=ID_APP_ABOUT
Command9=ID_BUTTON32782
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_GAME_PLAY
Command8=ID_GAME_PAUSEF3
Command9=ID_GAME_HIGHSCORES
Command10=ID_VIEW_TOOLBAR
Command11=ID_VIEW_STATUS_BAR
Command12=ID_OPTIONS_CUSTOMIZE
Command13=ID_HELP_CONTENTS
Command14=ID_APP_ABOUT
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_HELP_CONTENTS
Command5=ID_GAME_PLAY
Command6=ID_GAME_PAUSEF3
Command7=ID_GAME_HIGHSCORES
Command8=ID_OPTIONS_CUSTOMIZE
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_GAME_PAUSEF3
CommandCount=11

