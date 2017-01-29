/***************************************************************
 * Name:      xScheduleMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    xLights ()
 * Created:   2016-12-30
 * Copyright: xLights (http://xlights.org)
 * License:
 **************************************************************/

#include "xScheduleMain.h"
#include <wx/msgdlg.h>
#include "PlayList/PlayList.h"
#include "MyTreeItemData.h"
#include <wx/config.h>
#include "ScheduleManager.h"
#include "Schedule.h"
#include "ScheduleOptions.h"
#include "OptionsDialog.h"
#include "WebServer.h"
#include <log4cpp/Category.hh>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/mimetype.h>
#include "PlayList/PlayListStep.h"
#include <wx/bitmap.h>
#include "../xLights/xLightsVersion.h"
#include <wx/protocol/http.h>
#include <wx/debugrpt.h>
#include "RunningSchedule.h"
#include "UserButton.h"
#include "OutputProcessingDialog.h"
#include <wx/clipbrd.h>
#include "../xLights/osxMacUtils.h"
#include "BackgroundPlaylistDialog.h"
#include "MatricesDialog.h"

#include "../include/xs_xyzzy.xpm"
#include "../include/xs_save.xpm"
#include "../include/xs_otlon.xpm"
#include "../include/xs_otloff.xpm"
#include "../include/xs_otlautoon.xpm"
#include "../include/xs_otlautooff.xpm"
#include "../include/xs_scheduled.xpm"
#include "../include/xs_queued.xpm"
#include "../include/xs_notscheduled.xpm"
#include "../include/xs_inactive.xpm"
#include "../include/xs_pllooped.xpm"
#include "../include/xs_plnotlooped.xpm"
#include "../include/xs_plsteplooped.xpm"
#include "../include/xs_plstepnotlooped.xpm"
#include "../include/xs_playing.xpm"
#include "../include/xs_idle.xpm"
#include "../include/xs_paused.xpm"
#include "../include/xs_random.xpm"
#include "../include/xs_notrandom.xpm"
#include "../include/xs_volume_down.xpm"
#include "../include/xs_volume_up.xpm"
#include "../include/xs_falcon.xpm"

#include "../include/xLights.xpm"
#include "../include/xLights-16.xpm"
#include "../include/xLights-32.xpm"
#include "../include/xLights-64.xpm"
#include "../include/xLights-128.xpm"

//(*InternalHeaders(xScheduleFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

ScheduleManager* xScheduleFrame::__schedule = nullptr;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(xScheduleFrame)
const long xScheduleFrame::ID_BITMAPBUTTON1 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON3 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON6 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON2 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON4 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON5 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON7 = wxNewId();
const long xScheduleFrame::ID_CUSTOM1 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON8 = wxNewId();
const long xScheduleFrame::ID_CUSTOM2 = wxNewId();
const long xScheduleFrame::ID_BITMAPBUTTON9 = wxNewId();
const long xScheduleFrame::ID_PANEL2 = wxNewId();
const long xScheduleFrame::ID_TREECTRL1 = wxNewId();
const long xScheduleFrame::ID_BUTTON1 = wxNewId();
const long xScheduleFrame::ID_BUTTON2 = wxNewId();
const long xScheduleFrame::ID_BUTTON3 = wxNewId();
const long xScheduleFrame::ID_BUTTON4 = wxNewId();
const long xScheduleFrame::ID_PANEL3 = wxNewId();
const long xScheduleFrame::ID_LISTVIEW1 = wxNewId();
const long xScheduleFrame::ID_PANEL5 = wxNewId();
const long xScheduleFrame::ID_SPLITTERWINDOW1 = wxNewId();
const long xScheduleFrame::ID_PANEL1 = wxNewId();
const long xScheduleFrame::ID_STATICTEXT1 = wxNewId();
const long xScheduleFrame::ID_STATICTEXT2 = wxNewId();
const long xScheduleFrame::ID_PANEL4 = wxNewId();
const long xScheduleFrame::ID_MNU_SHOWFOLDER = wxNewId();
const long xScheduleFrame::ID_MNU_SAVE = wxNewId();
const long xScheduleFrame::idMenuQuit = wxNewId();
const long xScheduleFrame::ID_MNU_MNUADDPLAYLIST = wxNewId();
const long xScheduleFrame::ID_MENUITEM1 = wxNewId();
const long xScheduleFrame::ID_MNU_BACKGROUND = wxNewId();
const long xScheduleFrame::ID_MNU_MATRICES = wxNewId();
const long xScheduleFrame::ID_MNU_OPTIONS = wxNewId();
const long xScheduleFrame::ID_MNU_VIEW_LOG = wxNewId();
const long xScheduleFrame::ID_MNU_CHECK_SCHEDULE = wxNewId();
const long xScheduleFrame::ID_MNU_WEBINTERFACE = wxNewId();
const long xScheduleFrame::ID_MNU_IMPORT = wxNewId();
const long xScheduleFrame::ID_MNU_MODENORMAL = wxNewId();
const long xScheduleFrame::ID_MNU_FPPMASTER = wxNewId();
const long xScheduleFrame::ID_MNU_FPPREMOTE = wxNewId();
const long xScheduleFrame::idMenuAbout = wxNewId();
const long xScheduleFrame::ID_STATUSBAR1 = wxNewId();
const long xScheduleFrame::ID_TIMER1 = wxNewId();
const long xScheduleFrame::ID_TIMER2 = wxNewId();
//*)

const long xScheduleFrame::ID_MNU_ADDPLAYLIST = wxNewId();
const long xScheduleFrame::ID_MNU_ADDADVPLAYLIST = wxNewId();
const long xScheduleFrame::ID_MNU_DUPLICATEPLAYLIST = wxNewId();
const long xScheduleFrame::ID_MNU_SCHEDULEPLAYLIST = wxNewId();
const long xScheduleFrame::ID_MNU_DELETE = wxNewId();
const long xScheduleFrame::ID_MNU_EDIT = wxNewId();
const long xScheduleFrame::ID_MNU_EDITADV = wxNewId();
const long xScheduleFrame::ID_BUTTON_USER = wxNewId();

class BrightnessControl : public wxControl
{
    int _value;

public:

    BrightnessControl(wxWindow *parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxStaticBitmapNameStr) : wxControl(parent, id, pos, size, style, validator, name)
    {
        _value = 100;
        Connect(wxEVT_PAINT, (wxObjectEventFunction)&BrightnessControl::OnPaint);
    }
    virtual ~BrightnessControl() {};
    virtual void SetValue(int value) {
        if (_value != value) { _value = value; Refresh(); }
    }
protected:
    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        int grey = (_value * 255) / 100;
        wxColor c(grey, grey, grey, 255);
        dc.SetBrush(wxBrush(c));
        int w, h;
        GetSize(&w, &h);
        dc.DrawRectangle(0, 0, w, h);
        if (_value < 60)
        {
            dc.SetTextForeground(*wxWHITE);
        }
        else
        {
            dc.SetTextForeground(*wxBLACK);
        }
        wxString text = wxString::Format("%i%%", _value);
        int tw, th;
        dc.GetTextExtent(text, &tw, &th);
        dc.DrawText(text, (32 - tw) / 2, (32 - th)/ 2);
        dc.SetBrush(wxNullBrush);
        dc.SetPen(wxNullPen);
    }

private:
    //wxDECLARE_DYNAMIC_CLASS(BrightnessControl);
    wxDECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(BrightnessControl, wxControl)
//(*EventTable(PerspectivesPanel)
//*)
END_EVENT_TABLE()

class VolumeDisplay : public wxControl
{
    int _value;

public:

    VolumeDisplay(wxWindow *parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxStaticBitmapNameStr) : wxControl(parent, id, pos, size, style, validator, name)
    {
        _value = 100;
        Connect(wxEVT_PAINT, (wxObjectEventFunction)&VolumeDisplay::OnPaint);
    }
    virtual ~VolumeDisplay() {};
    virtual void SetValue(int value) {
        if (_value != value) { _value = value; Refresh(); }
    }
protected:
    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        dc.SetBrush(wxBrush(*wxLIGHT_GREY));
        int w, h;
        GetSize(&w, &h);
        dc.DrawRectangle(0, 0, w, h);
        dc.SetBrush(wxBrush(*wxYELLOW));
        dc.DrawRectangle(0, h - ((h * _value) / 100), w, h);
        dc.SetTextForeground(*wxBLACK);
        wxString text = wxString::Format("%i%%", _value);
        int tw, th;
        dc.GetTextExtent(text, &tw, &th);
        dc.DrawText(text, (32 - tw) / 2, (32 - th) / 2);
        dc.SetBrush(wxNullBrush);
        dc.SetPen(wxNullPen);
    }

private:
    //wxDECLARE_DYNAMIC_CLASS(VolumeDisplay);
    wxDECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(VolumeDisplay, wxControl)
//(*EventTable(PerspectivesPanel)
//*)
END_EVENT_TABLE()

wxDEFINE_EVENT(EVT_FRAMEMS, wxCommandEvent);
wxDEFINE_EVENT(EVT_STATUSMSG, wxCommandEvent);
wxDEFINE_EVENT(EVT_RUNACTION, wxCommandEvent);
wxDEFINE_EVENT(EVT_SCHEDULECHANGED, wxCommandEvent);

BEGIN_EVENT_TABLE(xScheduleFrame,wxFrame)
    //(*EventTable(xScheduleFrame)
    //*)
    EVT_COMMAND(wxID_ANY, EVT_FRAMEMS, xScheduleFrame::RateNotification)
    EVT_COMMAND(wxID_ANY, EVT_STATUSMSG, xScheduleFrame::StatusMsgNotification)
    EVT_COMMAND(wxID_ANY, EVT_RUNACTION, xScheduleFrame::RunAction)
    EVT_COMMAND(wxID_ANY, EVT_SCHEDULECHANGED, xScheduleFrame::ScheduleChange)
    END_EVENT_TABLE()

xScheduleFrame::xScheduleFrame(wxWindow* parent, const std::string& showdir, const std::string& playlist, wxWindowID id)
{
    //MatrixMapper::Test();

    _statusSetAt = wxDateTime::Now();
    __schedule = nullptr;
    _webServer = nullptr;
    _timerOutputFrame = false;

    //(*Initialize(xScheduleFrame)
    wxMenuItem* MenuItem2;
    wxFlexGridSizer* FlexGridSizer3;
    wxMenuItem* MenuItem1;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer2;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxFlexGridSizer* FlexGridSizer6;
    wxMenu* Menu2;

    Create(parent, id, _("xLights Scheduler"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer1->AddGrowableCol(0);
    FlexGridSizer1->AddGrowableRow(1);
    Panel2 = new wxPanel(this, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    FlexGridSizer5 = new wxFlexGridSizer(0, 11, 0, 0);
    BitmapButton_OutputToLights = new wxBitmapButton(Panel2, ID_BITMAPBUTTON1, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
    FlexGridSizer5->Add(BitmapButton_OutputToLights, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_Playing = new wxBitmapButton(Panel2, ID_BITMAPBUTTON3, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
    FlexGridSizer5->Add(BitmapButton_Playing, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_IsScheduled = new wxBitmapButton(Panel2, ID_BITMAPBUTTON6, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON6"));
    FlexGridSizer5->Add(BitmapButton_IsScheduled, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_Random = new wxBitmapButton(Panel2, ID_BITMAPBUTTON2, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
    FlexGridSizer5->Add(BitmapButton_Random, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_PLLoop = new wxBitmapButton(Panel2, ID_BITMAPBUTTON4, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON4"));
    FlexGridSizer5->Add(BitmapButton_PLLoop, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_StepLoop = new wxBitmapButton(Panel2, ID_BITMAPBUTTON5, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON5"));
    FlexGridSizer5->Add(BitmapButton_StepLoop, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_Unsaved = new wxBitmapButton(Panel2, ID_BITMAPBUTTON7, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON7"));
    FlexGridSizer5->Add(BitmapButton_Unsaved, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Brightness = new BrightnessControl(Panel2,ID_CUSTOM1,wxDefaultPosition,wxSize(32,32),0,wxDefaultValidator,_T("ID_CUSTOM1"));
    FlexGridSizer5->Add(Brightness, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_VolumeDown = new wxBitmapButton(Panel2, ID_BITMAPBUTTON8, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON8"));
    FlexGridSizer5->Add(BitmapButton_VolumeDown, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Custom_Volume = new VolumeDisplay(Panel2,ID_CUSTOM2,wxDefaultPosition,wxSize(32,32),0,wxDefaultValidator,_T("ID_CUSTOM2"));
    FlexGridSizer5->Add(Custom_Volume, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BitmapButton_VolumeUp = new wxBitmapButton(Panel2, ID_BITMAPBUTTON9, wxNullBitmap, wxDefaultPosition, wxSize(32,32), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON9"));
    FlexGridSizer5->Add(BitmapButton_VolumeUp, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel2->SetSizer(FlexGridSizer5);
    FlexGridSizer5->Fit(Panel2);
    FlexGridSizer5->SetSizeHints(Panel2);
    FlexGridSizer1->Add(Panel2, 1, wxALL|wxEXPAND, 0);
    SplitterWindow1 = new wxSplitterWindow(this, ID_SPLITTERWINDOW1, wxDefaultPosition, wxSize(52,39), wxSP_3D, _T("ID_SPLITTERWINDOW1"));
    SplitterWindow1->SetMinSize(wxSize(10,10));
    SplitterWindow1->SetMinimumPaneSize(10);
    SplitterWindow1->SetSashGravity(0.5);
    Panel3 = new wxPanel(SplitterWindow1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer2->AddGrowableCol(0);
    FlexGridSizer2->AddGrowableRow(0);
    TreeCtrl_PlayListsSchedules = new wxTreeCtrl(Panel3, ID_TREECTRL1, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL1"));
    TreeCtrl_PlayListsSchedules->SetMinSize(wxSize(300,300));
    FlexGridSizer2->Add(TreeCtrl_PlayListsSchedules, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Button_Add = new wxButton(Panel3, ID_BUTTON1, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer1->Add(Button_Add, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button_Edit = new wxButton(Panel3, ID_BUTTON2, _("Edit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer1->Add(Button_Edit, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button_Delete = new wxButton(Panel3, ID_BUTTON3, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer1->Add(Button_Delete, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button_Schedule = new wxButton(Panel3, ID_BUTTON4, _("Schedule"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer1->Add(Button_Schedule, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel3->SetSizer(FlexGridSizer2);
    FlexGridSizer2->Fit(Panel3);
    FlexGridSizer2->SetSizeHints(Panel3);
    Panel5 = new wxPanel(SplitterWindow1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    FlexGridSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer3->AddGrowableCol(0);
    FlexGridSizer3->AddGrowableRow(0);
    ListView_Running = new wxListView(Panel5, ID_LISTVIEW1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_NO_SORT_HEADER, wxDefaultValidator, _T("ID_LISTVIEW1"));
    ListView_Running->SetMinSize(wxSize(300,300));
    FlexGridSizer3->Add(ListView_Running, 1, wxALL|wxEXPAND, 5);
    Panel5->SetSizer(FlexGridSizer3);
    FlexGridSizer3->Fit(Panel5);
    FlexGridSizer3->SetSizeHints(Panel5);
    SplitterWindow1->SplitVertically(Panel3, Panel5);
    FlexGridSizer1->Add(SplitterWindow1, 1, wxALL|wxEXPAND, 0);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    FlexGridSizer4 = new wxFlexGridSizer(0, 0, 0, 0);
    Panel1->SetSizer(FlexGridSizer4);
    FlexGridSizer4->Fit(Panel1);
    FlexGridSizer4->SetSizeHints(Panel1);
    FlexGridSizer1->Add(Panel1, 1, wxALL|wxEXPAND, 0);
    Panel4 = new wxPanel(this, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    FlexGridSizer6 = new wxFlexGridSizer(0, 2, 0, 0);
    FlexGridSizer6->AddGrowableCol(1);
    StaticText_ShowDir = new wxStaticText(Panel4, ID_STATICTEXT1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer6->Add(StaticText_ShowDir, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText_Time = new wxStaticText(Panel4, ID_STATICTEXT2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer6->Add(StaticText_Time, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    Panel4->SetSizer(FlexGridSizer6);
    FlexGridSizer6->Fit(Panel4);
    FlexGridSizer6->SetSizeHints(Panel4);
    FlexGridSizer1->Add(Panel4, 1, wxALL|wxEXPAND, 0);
    SetSizer(FlexGridSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem_ShowFolder = new wxMenuItem(Menu1, ID_MNU_SHOWFOLDER, _("Show &Folder"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_ShowFolder);
    MenuItem_Save = new wxMenuItem(Menu1, ID_MNU_SAVE, _("&Save"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem_Save);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu5 = new wxMenu();
    MenuItem_AddPlayList = new wxMenuItem(Menu5, ID_MNU_MNUADDPLAYLIST, _("&Add Playlist"), wxEmptyString, wxITEM_NORMAL);
    Menu5->Append(MenuItem_AddPlayList);
    Menu_OutputProcessing = new wxMenuItem(Menu5, ID_MENUITEM1, _("&Output Processing"), wxEmptyString, wxITEM_NORMAL);
    Menu5->Append(Menu_OutputProcessing);
    MenuItem_BackgroundPlaylist = new wxMenuItem(Menu5, ID_MNU_BACKGROUND, _("&Background Playlist"), wxEmptyString, wxITEM_CHECK);
    Menu5->Append(MenuItem_BackgroundPlaylist);
    MenuItem_Matrices = new wxMenuItem(Menu5, ID_MNU_MATRICES, _("&Matrices"), wxEmptyString, wxITEM_NORMAL);
    Menu5->Append(MenuItem_Matrices);
    MenuItem_Options = new wxMenuItem(Menu5, ID_MNU_OPTIONS, _("&Options"), wxEmptyString, wxITEM_NORMAL);
    Menu5->Append(MenuItem_Options);
    MenuBar1->Append(Menu5, _("&Edit"));
    Menu3 = new wxMenu();
    MenuItem_ViewLog = new wxMenuItem(Menu3, ID_MNU_VIEW_LOG, _("&View Log"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem_ViewLog);
    MenuItem_CheckSchedule = new wxMenuItem(Menu3, ID_MNU_CHECK_SCHEDULE, _("&Check Schedule"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem_CheckSchedule);
    MenuItem_WebInterface = new wxMenuItem(Menu3, ID_MNU_WEBINTERFACE, _("&Web Interface"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem_WebInterface);
    MenuItem_ImportxLights = new wxMenuItem(Menu3, ID_MNU_IMPORT, _("Import xLights Playlist"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem_ImportxLights);
    MenuBar1->Append(Menu3, _("&Tools"));
    Menu4 = new wxMenu();
    MenuItem_Standalone = new wxMenuItem(Menu4, ID_MNU_MODENORMAL, _("Standalone"), wxEmptyString, wxITEM_RADIO);
    Menu4->Append(MenuItem_Standalone);
    MenuItem_FPPMaster = new wxMenuItem(Menu4, ID_MNU_FPPMASTER, _("FPP Master"), wxEmptyString, wxITEM_RADIO);
    Menu4->Append(MenuItem_FPPMaster);
    MenuItem_FPPRemote = new wxMenuItem(Menu4, ID_MNU_FPPREMOTE, _("FPP Remote"), wxEmptyString, wxITEM_RADIO);
    Menu4->Append(MenuItem_FPPRemote);
    MenuBar1->Append(Menu4, _("&Modes"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    DirDialog1 = new wxDirDialog(this, _("Select show folder ..."), wxEmptyString, wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("wxDirDialog"));
    _timer.SetOwner(this, ID_TIMER1);
    _timer.Start(500000, false);
    _timerSchedule.SetOwner(this, ID_TIMER2);
    _timerSchedule.Start(50000, false);
    FileDialog1 = new wxFileDialog(this, _("Select file"), wxEmptyString, _("xlights_schedule.xml"), _("xlights_schedule.xml"), wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_OutputToLightsClick);
    Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_PlayingClick);
    Connect(ID_BITMAPBUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_IsScheduledClick);
    Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_RandomClick);
    Connect(ID_BITMAPBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_PLLoopClick);
    Connect(ID_BITMAPBUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_StepLoopClick);
    Connect(ID_BITMAPBUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_UnsavedClick);
    Connect(ID_BITMAPBUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_VolumeDownClick);
    Custom_Volume->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&xScheduleFrame::OnCustom_VolumeLeftDown,0,this);
    Connect(ID_BITMAPBUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnBitmapButton_VolumeUpClick);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_ITEM_ACTIVATED,(wxObjectEventFunction)&xScheduleFrame::OnTreeCtrl_PlayListsSchedulesItemActivated);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&xScheduleFrame::OnTreeCtrl_PlayListsSchedulesSelectionChanged);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_KEY_DOWN,(wxObjectEventFunction)&xScheduleFrame::OnTreeCtrl_PlayListsSchedulesKeyDown);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_ITEM_MENU,(wxObjectEventFunction)&xScheduleFrame::OnTreeCtrl_PlayListsSchedulesItemMenu);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnButton_AddClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnButton_EditClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnButton_DeleteClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&xScheduleFrame::OnButton_ScheduleClick);
    Connect(ID_LISTVIEW1,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&xScheduleFrame::OnListView_RunningItemActivated);
    Connect(ID_MNU_SHOWFOLDER,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_ShowFolderSelected);
    Connect(ID_MNU_SAVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_SaveSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnQuit);
    Connect(ID_MNU_MNUADDPLAYLIST,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_AddPlayListSelected);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenu_OutputProcessingSelected);
    Connect(ID_MNU_BACKGROUND,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_BackgroundPlaylistSelected);
    Connect(ID_MNU_MATRICES,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_MatricesSelected);
    Connect(ID_MNU_OPTIONS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_OptionsSelected);
    Connect(ID_MNU_VIEW_LOG,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_ViewLogSelected);
    Connect(ID_MNU_CHECK_SCHEDULE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_CheckScheduleSelected);
    Connect(ID_MNU_WEBINTERFACE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_WebInterfaceSelected);
    Connect(ID_MNU_IMPORT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_ImportxLightsSelected);
    Connect(ID_MNU_MODENORMAL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_StandaloneSelected);
    Connect(ID_MNU_FPPMASTER,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_FPPMasterSelected);
    Connect(ID_MNU_FPPREMOTE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnMenuItem_FPPRemoteSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&xScheduleFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&xScheduleFrame::On_timerTrigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&xScheduleFrame::On_timerScheduleTrigger);
    Connect(wxEVT_SIZE,(wxObjectEventFunction)&xScheduleFrame::OnResize);
    //*)

    Connect(wxID_ANY, EVT_FRAMEMS, (wxObjectEventFunction)&xScheduleFrame::RateNotification);
    Connect(wxID_ANY, EVT_STATUSMSG, (wxObjectEventFunction)&xScheduleFrame::StatusMsgNotification);
    Connect(wxID_ANY, EVT_RUNACTION, (wxObjectEventFunction)&xScheduleFrame::RunAction);
    Connect(wxID_ANY, EVT_SCHEDULECHANGED, (wxObjectEventFunction)&xScheduleFrame::ScheduleChange);
    Connect(wxID_ANY, wxEVT_CHAR_HOOK, (wxObjectEventFunction)&xScheduleFrame::OnKeyDown);

    wxIconBundle icons;
    icons.AddIcon(wxIcon(xlights_16_xpm));
    icons.AddIcon(wxIcon(xlights_32_xpm));
    icons.AddIcon(wxIcon(xlights_64_xpm));
    icons.AddIcon(wxIcon(xlights_128_xpm));
    icons.AddIcon(wxIcon(xlights_xpm));
    SetIcons(icons);

    int x, y, w, h;
    wxConfigBase* config = wxConfigBase::Get();
    x = config->ReadLong("xsWindowPosX", 50);
    y = config->ReadLong("xsWindowPosY", 50);
    w = config->ReadLong("xsWindowPosW", 800);
    h = config->ReadLong("xsWindowPosH", 600);

    // limit weirdness
    if (x < -100) x = 0;
    if (x > 2000) x = 400;
    if (y < -100) y = 0;
    if (y > 2000) y = 400;

    SetPosition(wxPoint(x, y));
    SetSize(w, h);

    ListView_Running->AppendColumn("Step");
    ListView_Running->AppendColumn("Duration");
    ListView_Running->AppendColumn("");

    _otlon = wxBitmap(xs_otlon);
    _otloff = wxBitmap(xs_otloff);
    _otlautooff = wxBitmap(xs_otlautooff);
    _otlautoon = wxBitmap(xs_otlautoon);
    _save = wxBitmap(xs_save);
    _scheduled = wxBitmap(xs_scheduled);
    _queued = wxBitmap(xs_queued);
    _notscheduled = wxBitmap(xs_notscheduled);
    _inactive = wxBitmap(xs_inactive);
    _pllooped = wxBitmap(xs_pllooped);
    _plnotlooped = wxBitmap(xs_plnotlooped);
    _plsteplooped = wxBitmap(xs_plsteplooped);
    _plstepnotlooped = wxBitmap(xs_plstepnotlooped);
    _playing = wxBitmap(xs_playing);
    _idle = wxBitmap(xs_idle);
    _paused = wxBitmap(xs_paused);
    _random = wxBitmap(xs_random);
    _notrandom = wxBitmap(xs_notrandom);
    _volumeup = wxBitmap(xs_volume_up);
    _volumedown = wxBitmap(xs_volume_down);
    _falconremote = wxBitmap(xs_falcon);

    Brightness->SetToolTip("Brightness");
    BitmapButton_VolumeUp->SetToolTip("Increase Volume");
    BitmapButton_VolumeDown->SetToolTip("Decrease Volume");
    Custom_Volume->SetToolTip("Volume");
    BitmapButton_VolumeDown->SetBitmap(_volumedown);
    BitmapButton_VolumeUp->SetBitmap(_volumeup);

    if (showdir == "")
    {
        LoadShowDir();
    }
    else
    {
        _showDir = showdir;
    }

    __schedule = nullptr;
    LoadSchedule();

    wxFrame::SendSizeEvent();
    size_t rate = 50;

    if (playlist != "")
    {
        auto p = __schedule->GetPlayList(playlist);
        __schedule->PlayPlayList(p, rate, true);
        UpdateUI();
    }

    _timer.Stop();
    _timer.Start(rate/2, false);
    _timerSchedule.Stop();
    _timerSchedule.Start(500, true);
    
    AddWindowsMenu();

    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::LoadSchedule()
{
    if (__schedule != nullptr)
    {
        delete __schedule;
        __schedule = nullptr;
    }
    __schedule = new ScheduleManager(_showDir);

    if (_webServer != nullptr)
    {
        delete _webServer;
        _webServer = nullptr;
    }
    _webServer = new WebServer(__schedule->GetOptions()->GetWebServerPort(), __schedule->GetOptions()->GetAPIOnly(), __schedule->GetOptions()->GetPassword(), __schedule->GetOptions()->GetPasswordTimeout());

    if (wxFile::Exists(_showDir + "/xlights_networks.xml"))
    {
        StaticText_ShowDir->SetLabel(_showDir);
        StaticText_ShowDir->SetForegroundColour(*wxBLACK);
    }
    else
    {
        StaticText_ShowDir->SetLabel(_showDir + " : Missing xlights_networks.xml");
        StaticText_ShowDir->SetForegroundColour(*wxRED);
    }

    UpdateTree();

    CreateButtons();
}

xScheduleFrame::~xScheduleFrame()
{
    int x, y;
    GetPosition(&x, &y);

    int w, h;
    GetSize(&w, &h);

    wxConfigBase* config = wxConfigBase::Get();
    config->Write("xsWindowPosX", x);
    config->Write("xsWindowPosY", y);
    config->Write("xsWindowPosW", w);
    config->Write("xsWindowPosH", h);

    //(*Destroy(xScheduleFrame)
    //*)

    delete _webServer;
    _webServer = nullptr;

    delete __schedule;
    __schedule = nullptr;
}

void xScheduleFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void xScheduleFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
    ValidateWindow();
}

bool xScheduleFrame::IsPlayList(wxTreeItemId id) const
{
    if (!id.IsOk()) return false;

    return (TreeCtrl_PlayListsSchedules->GetItemParent(id) == TreeCtrl_PlayListsSchedules->GetRootItem());
}

bool xScheduleFrame::IsSchedule(wxTreeItemId id) const
{
    if (!id.IsOk()) return false;

    return (TreeCtrl_PlayListsSchedules->GetItemParent(id) != TreeCtrl_PlayListsSchedules->GetRootItem() && TreeCtrl_PlayListsSchedules->GetItemParent(id) != nullptr);
}

void xScheduleFrame::OnTreeCtrl_PlayListsSchedulesItemMenu(wxTreeEvent& event)
{
    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->HitTest(event.GetPoint());
    TreeCtrl_PlayListsSchedules->SelectItem(treeitem);

    wxMenu mnu;
    mnu.Append(ID_MNU_ADDPLAYLIST, "Add Playlist");
    if (!__schedule->GetOptions()->IsAdvancedMode())
    {
        wxMenuItem* addadv = mnu.Append(ID_MNU_ADDADVPLAYLIST, "Add Advanced Playlist");
    }
    wxMenuItem* sched = mnu.Append(ID_MNU_SCHEDULEPLAYLIST, "Schedule");
    if (!IsPlayList(treeitem))
    {
        sched->Enable(false);
    }

    wxMenuItem* del = mnu.Append(ID_MNU_DELETE, "Delete");
    wxMenuItem* duplicate = mnu.Append(ID_MNU_DUPLICATEPLAYLIST, "Duplicate");
    wxMenuItem* edit = mnu.Append(ID_MNU_EDIT, "Edit");
    wxMenuItem* editadv = nullptr;
    if (!__schedule->GetOptions()->IsAdvancedMode())
    {
        if (IsPlayList(treeitem))
        {
            wxMenuItem* editadv = mnu.Append(ID_MNU_EDITADV, "Edit Advanced");
        }
    }

    if (!IsPlayList(treeitem) && !IsSchedule(treeitem))
    {
        del->Enable(false);
        edit->Enable(false);
    }

    if (!IsPlayList(treeitem))
    {
        duplicate->Enable(false);
    }

    mnu.Connect(wxEVT_MENU, (wxObjectEventFunction)&xScheduleFrame::OnTreeCtrlMenu, nullptr, this);
    PopupMenu(&mnu);
    ValidateWindow();
}

void xScheduleFrame::OnTreeCtrlMenu(wxCommandEvent &event)
{
    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();
    if (event.GetId() == ID_MNU_ADDPLAYLIST)
    {
        AddPlayList();
    }
    else if (event.GetId() == ID_MNU_ADDADVPLAYLIST)
    {
        AddPlayList(true);
    }
    else if (event.GetId() == ID_MNU_SCHEDULEPLAYLIST)
    {
        AddSchedule();
    }
    else if (event.GetId() == ID_MNU_DELETE)
    {
        DeleteSelectedItem();
    }
    else if (event.GetId() == ID_MNU_EDIT)
    {
        EditSelectedItem();
    }
    else if (event.GetId() == ID_MNU_EDITADV)
    {
        EditSelectedItem(true);
    }
    else if (event.GetId() == ID_MNU_DUPLICATEPLAYLIST)
    {
        PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();

        PlayList* newpl = new PlayList(*playlist, true);

        wxTreeItemId  newitem = TreeCtrl_PlayListsSchedules->AppendItem(TreeCtrl_PlayListsSchedules->GetRootItem(), playlist->GetName(), -1, -1, new MyTreeItemData(newpl));
        TreeCtrl_PlayListsSchedules->Expand(newitem);
        TreeCtrl_PlayListsSchedules->EnsureVisible(newitem);
        __schedule->AddPlayList(newpl);
    }
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::DeleteSelectedItem()
{
    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();
    if (treeitem.IsOk() && (IsPlayList(treeitem) || IsSchedule(treeitem)))
    {
        if (wxMessageBox("Are you sure?", "Are you sure?", wxYES_NO) == wxYES)
        {
            wxTreeItemId parent = TreeCtrl_PlayListsSchedules->GetItemParent(treeitem);
            if (IsPlayList(treeitem))
            {
                PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
                __schedule->RemovePlayList(playlist);
                delete playlist;

                TreeCtrl_PlayListsSchedules->Delete(treeitem);
            }
            else if (IsSchedule(treeitem))
            {
                PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(TreeCtrl_PlayListsSchedules->GetItemParent(treeitem)))->GetData();
                Schedule* schedule = (Schedule*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
                playlist->RemoveSchedule(schedule);
                delete schedule;
                TreeCtrl_PlayListsSchedules->Delete(treeitem);
            }
            TreeCtrl_PlayListsSchedules->SelectItem(parent);
        }
    }
}

void xScheduleFrame::OnTreeCtrl_PlayListsSchedulesSelectionChanged(wxTreeEvent& event)
{
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::DoPaste()
{
    wxTextDataObject clip;
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->GetData(clip);
        wxTheClipboard->Close();
    }
    std::string _copybuffer = clip.GetText().ToStdString();

    if (_copybuffer != "")
    {
        wxArrayString copy = wxSplit(_copybuffer, ',');

        if (copy[0] == "PL")
        {
            if (copy.Count() == 2)
            {
                PlayList* playlist = __schedule->GetPlayList(wxAtoi(copy[1]));
                if (playlist != nullptr)
                {
                    PlayList* newpl = new PlayList(*playlist, true);
                    __schedule->AddPlayList(newpl);
                    UpdateTree();
                }
            }
        }
        else if (copy[0] == "SC")
        {
            if (copy.Count() == 3)
            {
                PlayList* playlist = __schedule->GetPlayList(wxAtoi(copy[1]));
                if (playlist != nullptr)
                {
                    Schedule* schedule = playlist->GetSchedule(wxAtoi(copy[2]));
                    if (schedule != nullptr)
                    {
                        wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();
                        PlayList* to = nullptr;
                        if (IsSchedule(treeitem))
                        {
                            to = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(TreeCtrl_PlayListsSchedules->GetItemParent(treeitem)))->GetData();
                        }
                        else if (IsPlayList(treeitem))
                        {
                            to = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
                        }

                        if (to != nullptr)
                        {
                            to->AddSchedule(new Schedule(*schedule, true));
                            UpdateTree();
                        }
                    }
                }
            }
        }
    }
}

void xScheduleFrame::DoCopy()
{
    std::string copybuffer = "";
    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();
    if (IsPlayList(treeitem))
    {
        PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
        copybuffer = "PL," + wxString::Format(wxT("%i"), playlist->GetId());
    }
    else if (IsSchedule(treeitem))
    {
        PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(TreeCtrl_PlayListsSchedules->GetItemParent(treeitem)))->GetData();
        Schedule* schedule = (Schedule*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
        copybuffer = "SC," + wxString::Format(wxT("%i"), playlist->GetId()) + "," + wxString::Format(wxT("%i"), schedule->GetId());
    }

    if (wxTheClipboard->Open())
    {
        wxTheClipboard->Clear();
        wxTheClipboard->SetData(new wxTextDataObject(copybuffer));
        wxTheClipboard->Flush();
        wxTheClipboard->Close();
    }
}

void xScheduleFrame::OnTreeCtrl_PlayListsSchedulesKeyDown(wxTreeEvent& event)
{
}

void xScheduleFrame::OnMenuItem_SaveSelected(wxCommandEvent& event)
{
    __schedule->Save();
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::OnMenuItem_ShowFolderSelected(wxCommandEvent& event)
{
    DirDialog1->SetPath(_showDir);

    if (DirDialog1->ShowModal() == wxID_OK)
    {
        _showDir = DirDialog1->GetPath().ToStdString();
        SaveShowDir();
        LoadSchedule();
        _timerSchedule.Stop();
        _timerSchedule.Start(50, true);
    }
    ValidateWindow();
}

void xScheduleFrame::SaveShowDir() const
{
    wxConfigBase* config = wxConfigBase::Get();
    config->Write("SchedulerLastDir", wxString(_showDir));
}

void xScheduleFrame::LoadShowDir()
{
    // get the show directory
    wxConfigBase* config = wxConfigBase::Get();
    wxString showDir;
    if (!config->Read("SchedulerLastDir", &showDir))
    {
        if (!config->Read("LastDir", &showDir))
        {
            DirDialog1->SetPath(_showDir);

            if (DirDialog1->ShowModal() == wxID_OK)
            {
                _showDir = DirDialog1->GetPath().ToStdString();
                SaveShowDir();
            }
            else
            {
                _showDir = ".";
            }
        }
        else
        {
            _showDir = showDir.ToStdString();
            SaveShowDir();
        }
    }
    else
    {
        _showDir = showDir.ToStdString();
    }
}

void xScheduleFrame::UpdateTree() const
{
    TreeCtrl_PlayListsSchedules->Freeze();

    TreeCtrl_PlayListsSchedules->DeleteAllItems();

    wxTreeItemId root = TreeCtrl_PlayListsSchedules->AddRoot("Playlists");

    auto pls = __schedule->GetPlayLists();

    for (auto it = pls.begin(); it != pls.end(); ++it)
    {
        auto pl = TreeCtrl_PlayListsSchedules->AppendItem(root, (*it)->GetName(), -1, -1, new MyTreeItemData(*it));

        auto schedules = (*it)->GetSchedules();
        for (auto it2 = schedules.begin(); it2 != schedules.end(); ++it2)
        {
            TreeCtrl_PlayListsSchedules->AppendItem(pl, GetScheduleName(*it2, __schedule->GetRunningSchedules()), -1, -1, new MyTreeItemData(*it2));
        }
        TreeCtrl_PlayListsSchedules->Expand(pl);
    }
    TreeCtrl_PlayListsSchedules->Expand(root);

    TreeCtrl_PlayListsSchedules->Thaw();
    TreeCtrl_PlayListsSchedules->Refresh();
}

std::string xScheduleFrame::GetScheduleName(Schedule* schedule, const std::list<RunningSchedule*>& active) const
{
    for (auto it = active.begin(); it != active.end(); ++it)
    {
        if ((*it)->GetSchedule()->GetId() == schedule->GetId())
        {
            if (!(*it)->GetPlayList()->IsRunning())
            {
                return schedule->GetName() + " [Stopped]";
            }
        }
    }

    return schedule->GetName() + " [" + schedule->GetNextTriggerTime() + "]";
}

void xScheduleFrame::OnTreeCtrl_PlayListsSchedulesItemActivated(wxTreeEvent& event)
{
    EditSelectedItem();
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::On_timerTrigger(wxTimerEvent& event)
{
    static int last = -1;
    if (__schedule == nullptr) return;

    __schedule->Frame(_timerOutputFrame);

    if (last != wxDateTime::Now().GetSecond() && _timerOutputFrame)
    {
        last = wxDateTime::Now().GetSecond();
        wxCommandEvent event(EVT_SCHEDULECHANGED);
        wxPostEvent(this, event);
    }

    _timerOutputFrame = !_timerOutputFrame;
}

void xScheduleFrame::UpdateSchedule()
{
    if (__schedule == nullptr) return;

    TreeCtrl_PlayListsSchedules->Freeze();

    int rate = __schedule->CheckSchedule();

    // highlight the state of all schedule items in the tree
    wxTreeItemIdValue tid;
    auto root = TreeCtrl_PlayListsSchedules->GetRootItem();
    for (auto it = TreeCtrl_PlayListsSchedules->GetFirstChild(root, tid); it != nullptr; it = TreeCtrl_PlayListsSchedules->GetNextChild(root, tid))
    {
        //PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(it))->GetData();

        wxTreeItemIdValue tid2;
        for (auto it2 = TreeCtrl_PlayListsSchedules->GetFirstChild(it, tid2); it2 != nullptr; it2 = TreeCtrl_PlayListsSchedules->GetNextChild(it, tid2))
        {
            Schedule* schedule = (Schedule*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(it2))->GetData();

            TreeCtrl_PlayListsSchedules->SetItemText(it2, GetScheduleName(schedule, __schedule->GetRunningSchedules()));

            if (__schedule->IsScheduleActive(schedule))
            {
                RunningSchedule* rs = __schedule->GetRunningSchedule();
                if (rs != nullptr && rs->GetPlayList()->IsRunning() &&rs->GetSchedule()->GetId() == schedule->GetId())
                {
                    TreeCtrl_PlayListsSchedules->SetItemBackgroundColour(it2, wxColor(146, 244, 155));
                }
                else
                {
                    RunningSchedule* r = __schedule->GetRunningSchedule(schedule);
                    wxASSERT(r != nullptr);
                    if (r == nullptr || !r->GetPlayList()->IsRunning())
                    {
                        // stopped
                        TreeCtrl_PlayListsSchedules->SetItemBackgroundColour(it2, wxColor(0xe7, 0x74, 0x71));
                    }
                    else
                    {
                        // waiting
                        TreeCtrl_PlayListsSchedules->SetItemBackgroundColour(it2, wxColor(244, 241, 146));
                    }
                }
            }
            else
            {
                TreeCtrl_PlayListsSchedules->SetItemBackgroundColour(it2, *wxWHITE);
            }
        }
    }

    // if the desired rate is different than the current rate then restart timer with the desired rate
    if (_timer.GetInterval() != rate / 2)
    {
        _timer.Stop();
        _timer.Start(rate / 2);
    }

    // Ensure I am firing on the minute
    if (wxDateTime::Now().GetSecond() != 0)
    {
        _timerSchedule.Stop();
        _timerSchedule.Start((60 - wxDateTime::Now().GetSecond()) * 1000, false);
    }
    else if (_timerSchedule.GetInterval() != 60000)
    {
        _timerSchedule.Stop();
        _timerSchedule.Start(60000, false);
    }

    UpdateUI();
    ValidateWindow();

    TreeCtrl_PlayListsSchedules->Thaw();
    TreeCtrl_PlayListsSchedules->Refresh();
}

void xScheduleFrame::On_timerScheduleTrigger(wxTimerEvent& event)
{
    UpdateSchedule();
}

void xScheduleFrame::ValidateWindow()
{
    if (__schedule->GetOptions()->GetAPIOnly())
    {
        MenuItem_WebInterface->Enable(false);
    }
    else
    {
        MenuItem_WebInterface->Enable(true);
    }

    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();
    if (IsPlayList(treeitem) || IsSchedule(treeitem))
    {
        Button_Delete->Enable();
        Button_Edit->Enable();
    }
    else
    {
        Button_Delete->Enable(false);
        Button_Edit->Enable(false);
    }

    if (IsPlayList(treeitem))
    {
        Button_Schedule->Enable();
    }
    else
    {
        Button_Schedule->Disable();
    }

    if (__schedule->GetBackgroundPlayList() == nullptr)
    {
        MenuItem_BackgroundPlaylist->Check(false);
    }
    else
    {
        MenuItem_BackgroundPlaylist->Check();
    }
}

void xScheduleFrame::OnMenuItem_OptionsSelected(wxCommandEvent& event)
{
    OptionsDialog dlg(this, __schedule->GetOptions());

    int oldport = __schedule->GetOptions()->GetWebServerPort();

    if (dlg.ShowModal() == wxID_OK)
    {
        if (oldport != __schedule->GetOptions()->GetWebServerPort())
        {
            delete _webServer;
            _webServer = new WebServer(__schedule->GetOptions()->GetWebServerPort(), __schedule->GetOptions()->GetAPIOnly(),
                __schedule->GetOptions()->GetPassword(), __schedule->GetOptions()->GetPasswordTimeout());
        }
        else
        {
            _webServer->SetAPIOnly(__schedule->GetOptions()->GetAPIOnly());
            _webServer->SetPassword(__schedule->GetOptions()->GetPassword());
            _webServer->SetPasswordTimeout(__schedule->GetOptions()->GetPasswordTimeout());
        }

        __schedule->OptionsChanged();
        CreateButtons();
    }

    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::CreateButton(const std::string& label)
{
    wxButton* b = new wxButton(Panel1, ID_BUTTON_USER, label);
    FlexGridSizer4->Add(b, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
    Connect(ID_BUTTON_USER, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&xScheduleFrame::OnButton_UserClick);
}

void xScheduleFrame::CreateButtons()
{
    auto buttons = Panel1->GetChildren();
    for (auto it = buttons.begin(); it != buttons.end(); ++it)
    {
        Disconnect((*it)->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&xScheduleFrame::OnButton_UserClick);
        FlexGridSizer4->Detach(*it);
        delete *it;
    }

    auto bs = __schedule->GetOptions()->GetButtons();

    // create some default buttons
    if (bs.size() == 0)
    {
        __schedule->GetOptions()->AddButton("Play Selected", "Play selected playlist", "", '~');
        __schedule->GetOptions()->AddButton("Stop All", "Stop all now", "", '~');
        __schedule->GetOptions()->AddButton("Next Step", "Next step in current playlist", "", '~');
        __schedule->GetOptions()->AddButton("End Gracefully", "Jump to play once at end at end of current step and then stop", "", '~');
        __schedule->GetOptions()->AddButton("Add 10 Mins To Schedule", "Add to the current schedule n minutes", "10", '~');

        bs = __schedule->GetOptions()->GetButtons();
    }

    FlexGridSizer4->SetCols(5);
    int rows = bs.size() / 5;
    if (bs.size() % 5 != 0) rows++;
    FlexGridSizer4->SetRows(rows);

    for (auto it = bs.begin(); it != bs.end(); ++it)
    {
        CreateButton((*it)->GetLabel());
    }

    SendSizeEvent();

    UpdateUI();
}

void xScheduleFrame::RateNotification(wxCommandEvent& event)
{
    if (_timer.GetInterval() != event.GetInt() / 2)
    {
        _timer.Stop();
        _timer.Start(event.GetInt() / 2);
    }
}

void xScheduleFrame::StatusMsgNotification(wxCommandEvent& event)
{
    SetTempMessage(event.GetString().ToStdString());
}

void xScheduleFrame::RunAction(wxCommandEvent& event)
{
    wxArrayString a = wxSplit(event.GetString(), '|');

    if (a.Count() == 2)
    {
        size_t rate;
        std::string msg;
        __schedule->Action(a[0].ToStdString(), a[1].ToStdString(), nullptr, nullptr, rate, msg);
        if (msg != "")
        {
            SetTempMessage(msg);
        }
    }
}

void xScheduleFrame::OnButton_UserClick(wxCommandEvent& event)
{
    PlayList* playlist = nullptr;
    Schedule* schedule = nullptr;

    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();

    if (IsPlayList(treeitem))
    {
        playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
    }
    else if (IsSchedule(treeitem))
    {
        schedule = (Schedule*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
    }

    size_t rate = _timer.GetInterval();
    std::string msg = "";
    __schedule->Action(((wxButton*)event.GetEventObject())->GetLabel().ToStdString(), playlist, schedule, rate, msg);

    if (rate / 2 != _timer.GetInterval())
    {
        _timer.Stop();
        _timer.Start(rate / 2);
    }

    UpdateSchedule();
    UpdateUI();
}

void xScheduleFrame::SetTempMessage(const std::string& msg)
{
    _statusSetAt = wxDateTime::Now();
    StatusBar1->SetStatusText(msg);
}

void xScheduleFrame::OnMenuItem_ViewLogSelected(wxCommandEvent& event)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    wxString dir;
    wxString fileName = "xSchedule_l4cpp.log";
#ifdef __WXMSW__
    wxGetEnv("APPDATA", &dir);
    wxString filename = dir + "/" + fileName;
#endif
#ifdef __WXOSX_MAC__
    wxFileName home;
    home.AssignHomeDir();
    dir = home.GetFullPath();
    wxString filename = dir + "/Library/Logs/" + fileName;
#endif
#ifdef __LINUX__
    wxString filename = "/tmp/" + fileName;
#endif
    wxString fn = "";
    if (wxFile::Exists(filename))
    {
        fn = filename;
    }
    else if (wxFile::Exists(wxFileName(_showDir, fileName).GetFullPath()))
    {
        fn = wxFileName(_showDir, fileName).GetFullPath();
    }
    else if (wxFile::Exists(wxFileName(wxGetCwd(), fileName).GetFullPath()))
    {
        fn = wxFileName(wxGetCwd(), fileName).GetFullPath();
    }

    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension("txt");
    if (fn != "" && ft)
    {
        wxString command = ft->GetOpenCommand("foo.txt");
        command.Replace("foo.txt", fn);

        logger_base.debug("Viewing log file %s.", (const char *)fn.c_str());

        wxExecute(command);
        delete ft;
    }
    else
    {
        logger_base.warn("Unable to view log file %s.", (const char *)fn.c_str());
        wxMessageBox(_("Unable to show log file."), _("Error"));
    }
}

void xScheduleFrame::OnResize(wxSizeEvent& event)
{
    bool done = false;

    int pw, ph;
    Panel1->GetSize(&pw, &ph);

    int n = 20;
    while (!done && n > 0)
    {
        auto buttons = Panel1->GetChildren();
        FlexGridSizer4->SetRows(buttons.size() / n + (buttons.size() % n > 0 ? 1 : 0));
        FlexGridSizer4->SetCols(n);
        FlexGridSizer4->Fit(Panel1);
        FlexGridSizer4->SetSizeHints(Panel1);

        bool changed = false;

        int lastx = 0;
        int lasty = 0;
        for (auto it = buttons.begin(); it != buttons.end(); ++it)
        {
            int x, y, w, h;
            (*it)->GetPosition(&x, &y);
            (*it)->GetSize(&w, &h);

            if ((x < lastx && y == lasty) || x+w > pw - 10)
            {
                n--;
                changed = true;
                break;
            }

            lasty = y;
            lastx = x + w;
        }

        if (!changed)
        {
            break;
        }
    }

    Layout();
}

void xScheduleFrame::OnListView_RunningItemActivated(wxListEvent& event)
{
    int selected = ListView_Running->GetFirstSelected();

    PlayList* p = __schedule->GetRunningPlayList();

    if (selected > 0 && p != nullptr && p->GetRunningStep()->GetNameNoTime() != ListView_Running->GetItemText(selected, 0))
    {
        size_t rate;
        std::string msg;
        __schedule->Action("Jump to specified step in current playlist", ListView_Running->GetItemText(selected, 0).ToStdString(), p, nullptr, rate, msg);
    }
}

std::string FormatTime(size_t timems, bool ms = false)
{
    if (ms)
    {
        return wxString::Format(wxT("%i:%02i.%03i"), timems / 60000, (timems % 60000) / 1000, timems % 1000).ToStdString();
    }
    else
    {
        return wxString::Format(wxT("%i:%02i"), timems / 60000, (timems % 60000) / 1000).ToStdString();
    }
}

void xScheduleFrame::UpdateStatus()
{
    ListView_Running->Freeze();

    if (StatusBar1->GetStatusText() != "" && (wxDateTime::Now() - _statusSetAt).GetMilliseconds() >  5000)
    {
        StatusBar1->SetStatusText("");
    }

    static int lastcc = -1;
    static int lastid = -1;
    static int lastrunning = -1;
    PlayList* p = __schedule->GetRunningPlayList();

    if (p == nullptr)
    {
        wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();
        if (treeitem.IsOk() && IsPlayList(treeitem))
        {
            p = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
        }
    }

    if (p == nullptr)
    {
        ListView_Running->DeleteAllItems();
    }
    else
    {
        if (p->GetId() != lastid || p->GetChangeCount() != lastcc || (int)p->IsRunning() != lastrunning)
        {
            lastcc = p->GetChangeCount();
            lastid = p->GetId();
            lastrunning = (int)p->IsRunning();

            ListView_Running->DeleteAllItems();

            auto steps = p->GetSteps();

            int i = 0;
            for (auto it = steps.begin(); it != steps.end(); ++it)
            {
                ListView_Running->InsertItem(i, (*it)->GetNameNoTime());
                ListView_Running->SetItem(i, 1, FormatTime((*it)->GetLengthMS()));
                i++;
            }
        }

        PlayListStep* step = p->GetRunningStep();
        PlayListStep* next = nullptr;

        if (!p->IsRandom())
        {
            bool didloop;
            next = p->GetNextStep(didloop);
        }

        if (step != nullptr)
        {
            for (int i = 0; i < ListView_Running->GetItemCount(); i++)
            {
                if (ListView_Running->GetItemText(i, 0) == step->GetNameNoTime())
                {
                    ListView_Running->SetItem(i, 2, step->GetStatus());
                    ListView_Running->SetItemBackgroundColour(i, wxColor(146,244,155));
                }
                else
                {
                    if (next != nullptr && next->GetNameNoTime() == ListView_Running->GetItemText(i,0))
                    {
                        ListView_Running->SetItemBackgroundColour(i, wxColor(244,241,146));
                    }
                    else
                    {
                        ListView_Running->SetItemBackgroundColour(i, *wxWHITE);
                    }
                    ListView_Running->SetItem(i, 2, "");
                }
            }
        }

        ListView_Running->SetColumnWidth(0, wxLIST_AUTOSIZE);
        if (ListView_Running->GetColumnWidth(0) < 50)
            ListView_Running->SetColumnWidth(0, 50);
        ListView_Running->SetColumnWidth(1, wxLIST_AUTOSIZE);
        if (ListView_Running->GetColumnWidth(1) < 80)
            ListView_Running->SetColumnWidth(1, 80);
        ListView_Running->SetColumnWidth(2, wxLIST_AUTOSIZE);
        if (ListView_Running->GetColumnWidth(2) < 250)
            ListView_Running->SetColumnWidth(2, 250);
    }

    ListView_Running->Thaw();
    ListView_Running->Refresh();

    static int saved = -1;
    static int otl = -1;
    static int scheduled = -1;
    static int random = -1;
    static int plloop = -1;
    static int steploop = -1;
    static int playing = -1;

    if (__schedule->IsOutputToLights())
    {
        if (__schedule->GetManualOutputToLights() == -1)
        {
            if (otl != 2)
                BitmapButton_OutputToLights->SetBitmap(_otlautoon);
            otl = 2;
        }
        else
        {
            if (otl != 1)
                BitmapButton_OutputToLights->SetBitmap(_otlon);
            otl = 1;
        }
        if (BitmapButton_OutputToLights->GetToolTipText() != "Lights output ON.")
            BitmapButton_OutputToLights->SetToolTip("Lights output ON.");
    }
    else
    {
        if (__schedule->GetManualOutputToLights() == -1)
        {
            if (otl != 3)
                BitmapButton_OutputToLights->SetBitmap(_otlautooff);
            otl = 3;
        }
        else
        {
            if (otl != 0)
                BitmapButton_OutputToLights->SetBitmap(_otloff);
            otl = 0;
        }
        if (BitmapButton_OutputToLights->GetToolTipText() != "Lights output OFF.")
            BitmapButton_OutputToLights->SetToolTip("Lights output OFF.");
    }

    if (__schedule->IsDirty())
    {
        if (saved != 1)
            BitmapButton_Unsaved->SetBitmap(_save);
        if (BitmapButton_Unsaved->GetToolTipText() != "Unsaved changes.")
            BitmapButton_Unsaved->SetToolTip("Unsaved changes.");
        saved = 1;
    }
    else
    {
        if (saved != 0)
            BitmapButton_Unsaved->SetBitmap(_inactive);
        saved = 0;
    }

    if (p == nullptr || !p->IsRunning())
    {
        if (scheduled != 0)
            BitmapButton_IsScheduled->SetBitmap(_inactive);
        BitmapButton_IsScheduled->SetToolTip("");
        scheduled = 0;

        if (playing != 0)
            BitmapButton_Playing->SetBitmap(_idle);
        if (BitmapButton_Playing->GetToolTipText() != "Idle.")
            BitmapButton_Playing->SetToolTip("Idle.");
        playing = 0;

        if (plloop != 2)
            BitmapButton_PLLoop->SetBitmap(_inactive);
        plloop = 2;
        BitmapButton_PLLoop->SetToolTip("");

        if (steploop != 2)
            BitmapButton_StepLoop->SetBitmap(_inactive);
        steploop = 2;
        BitmapButton_StepLoop->SetToolTip("");

        if (random != 2)
            BitmapButton_Random->SetBitmap(_inactive);
        BitmapButton_Random->SetToolTip("");
        random = 2;
    }
    else
    {
        if (__schedule->IsCurrentPlayListScheduled())
        {
            if (scheduled != 1)
                BitmapButton_IsScheduled->SetBitmap(_scheduled);
            if (BitmapButton_IsScheduled->GetToolTipText() != "Scheduled playlist playing.")
                BitmapButton_IsScheduled->SetToolTip("Scheduled playlist playing.");
            scheduled = 1;
        }
        else if (__schedule->IsQueuedPlaylistRunning())
        {
            if (scheduled != 4)
                BitmapButton_IsScheduled->SetBitmap(_queued);
            if (BitmapButton_IsScheduled->GetToolTipText() != "Queued playlist playing.")
                BitmapButton_IsScheduled->SetToolTip("Queued playlist playing.");
            scheduled = 4;
        }
        else
        {
            if (scheduled != 2)
                BitmapButton_IsScheduled->SetBitmap(_notscheduled);
            if (BitmapButton_IsScheduled->GetToolTipText() != "Manually started playlist playing.")
                BitmapButton_IsScheduled->SetToolTip("Manually started playlist playing.");
            scheduled = 2;
        }

        if (p->IsPaused())
        {
            if (playing != 2)
                BitmapButton_Playing->SetBitmap(_paused);
            if (BitmapButton_Playing->GetToolTipText() != "Paused.")
                BitmapButton_Playing->SetToolTip("Paused.");
            playing = 2;
        }
        else
        {
            if (playing != 1)
                BitmapButton_Playing->SetBitmap(_playing);
            if (BitmapButton_Playing->GetToolTipText() != "Playing.")
                BitmapButton_Playing->SetToolTip("Playing.");
            playing = 1;
        }

        if (p->IsLooping())
        {
            if (plloop != 1)
                BitmapButton_PLLoop->SetBitmap(_pllooped);
            if (BitmapButton_PLLoop->GetToolTipText() != "Playlist looping.")
                BitmapButton_PLLoop->SetToolTip("Playlist looping.");
            plloop = 1;
        }
        else
        {
            if (plloop != 0)
                BitmapButton_PLLoop->SetBitmap(_plnotlooped);
            if (BitmapButton_PLLoop->GetToolTipText() != "Playlist not looping.")
                BitmapButton_PLLoop->SetToolTip("Playlist not looping.");
            plloop = 0;
        }

        if (p->IsStepLooping())
        {
            if (steploop != 1)
                BitmapButton_StepLoop->SetBitmap(_plsteplooped);
            if (BitmapButton_StepLoop->GetToolTipText() != "Playlist step looping.")
                BitmapButton_StepLoop->SetToolTip("Playlist step looping.");
            steploop = 1;
        }
        else
        {
            if (steploop != 0)
                BitmapButton_StepLoop->SetBitmap(_plstepnotlooped);
            if (BitmapButton_StepLoop->GetToolTipText() != "Playlist step not looping.")
                BitmapButton_StepLoop->SetToolTip("Playlist step not looping.");
            steploop = 0;
        }

        if (p->IsRandom())
        {
            if (random != 1)
                BitmapButton_Random->SetBitmap(_random);
            if (BitmapButton_Random->GetToolTipText() != "Randomly choosing steps.")
                BitmapButton_Random->SetToolTip("Randomly choosing steps.");
            random = 1;
        }
        else
        {
            if (random != 0)
                BitmapButton_Random->SetBitmap(_notrandom);
            if (BitmapButton_Random->GetToolTipText() != "Sequentially played steps.")
                BitmapButton_Random->SetToolTip("Sequentially played steps.");
            random = 0;
        }
    }

    // update each button based on current status
    auto buttons = Panel1->GetChildren();
    for (auto it = buttons.begin(); it != buttons.end(); ++it)
    {
        UserButton* b = __schedule->GetOptions()->GetButton((*it)->GetLabel().ToStdString());

        std::string command = b->GetCommand();
        std::string parameters = b->GetParameters();

        PlayList* playlist = nullptr;
        Schedule* schedule = nullptr;

        wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();

        if (IsPlayList(treeitem))
        {
            playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
        }
        else if (IsSchedule(treeitem))
        {
            schedule = (Schedule*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
        }

        Command* c = __schedule->GetCommand(command);
        std::string msg;
        if (c != nullptr && c->IsValid(parameters, playlist, schedule, __schedule, msg, __schedule->IsQueuedPlaylistRunning()))
        {
            (*it)->Enable();
        }
        else
        {
            (*it)->Enable(false);
        }
    }

    Custom_Volume->SetValue(__schedule->GetVolume());

    StaticText_Time->SetLabel(wxDateTime::Now().FormatTime());
}

void xScheduleFrame::OnBitmapButton_OutputToLightsClick(wxCommandEvent& event)
{
    __schedule->ManualOutputToLightsClick();
    UpdateUI();
}

void xScheduleFrame::OnBitmapButton_RandomClick(wxCommandEvent& event)
{
    std::string msg = "";
    __schedule->ToggleCurrentPlayListRandom(msg);
    UpdateUI();
}

void xScheduleFrame::OnBitmapButton_PlayingClick(wxCommandEvent& event)
{
    std::string msg = "";
    __schedule->ToggleCurrentPlayListPause(msg);
    UpdateUI();
}

void xScheduleFrame::OnBitmapButton_PLLoopClick(wxCommandEvent& event)
{
    std::string msg = "";
    __schedule->ToggleCurrentPlayListLoop(msg);
    UpdateUI();
}

void xScheduleFrame::OnBitmapButton_StepLoopClick(wxCommandEvent& event)
{
    std::string msg = "";
    __schedule->ToggleCurrentPlayListStepLoop(msg);
    UpdateUI();
}

void xScheduleFrame::OnBitmapButton_IsScheduledClick(wxCommandEvent& event)
{
    // do nothing
    UpdateUI();
}

void xScheduleFrame::OnBitmapButton_UnsavedClick(wxCommandEvent& event)
{
    __schedule->Save();
    UpdateUI();
}

void xScheduleFrame::CreateDebugReport(wxDebugReportCompress *report) {
    if (wxDebugReportPreviewStd().Show(*report)) {
        report->Process();
        SendReport("crashUpload", *report);
        wxMessageBox("Crash report saved to " + report->GetCompressedFileName());
    }
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    logger_base.crit("Exiting after creating debug report: " + report->GetCompressedFileName());
    delete report;
    exit(1);
}

void xScheduleFrame::SendReport(const wxString &loc, wxDebugReportCompress &report) {
    wxHTTP http;
    http.Connect("dankulp.com");

    const char *bound = "--------------------------b29a7c2fe47b9481";
    int i = wxGetUTCTimeMillis().GetLo();
    i &= 0xFFFFFFF;
    wxString fn = wxString::Format("xschedule-%s_%d_%s.zip", wxPlatformInfo::Get().GetOperatingSystemFamilyName().c_str(), i, xlights_version_string);
    const char *ct = "Content-Type: application/octet-stream\n";
    std::string cd = "Content-Disposition: form-data; name=\"userfile\"; filename=\"" + fn.ToStdString() + "\"\n\n";

    wxMemoryBuffer memBuff;
    memBuff.AppendData(bound, strlen(bound));
    memBuff.AppendData("\n", 1);
    memBuff.AppendData(ct, strlen(ct));
    memBuff.AppendData(cd.c_str(), strlen(cd.c_str()));


    wxFile f_in(report.GetCompressedFileName());
    wxFileOffset fLen = f_in.Length();
    void* tmp = memBuff.GetAppendBuf(fLen);
    size_t iRead = f_in.Read(tmp, fLen);
    memBuff.UngetAppendBuf(iRead);
    f_in.Close();

    memBuff.AppendData("\n", 1);
    memBuff.AppendData(bound, strlen(bound));
    memBuff.AppendData("--\n", 3);

    http.SetMethod("POST");
    http.SetPostBuffer("multipart/form-data; boundary=------------------------b29a7c2fe47b9481", memBuff);
    wxInputStream * is = http.GetInputStream("/" + loc + "/index.php");
    char buf[1024];
    is->Read(buf, 1024);
    //printf("%s\n", buf);
    delete is;
    http.Close();
}

void xScheduleFrame::OnKeyDown(wxKeyEvent& event)
{
    if (HandleSpecialKeys(event) || HandleHotkeys(event))
    {
        UpdateUI();
        UpdateSchedule();
        ValidateWindow();
    }
    else
    {
        event.Skip();
    }
}

bool xScheduleFrame::HandleSpecialKeys(wxKeyEvent& event)
{
    if (event.GetSkipped()) return false;

    wxChar uc = event.GetUnicodeKey();

    if (event.GetKeyCode() == WXK_DELETE)
    {
        DeleteSelectedItem();
        return true;
    }
    else if (uc == (wxChar)WXK_CONTROL_C || uc == 'C' || uc == 'c')
    {
        if (event.CmdDown() || event.ControlDown()) {
            DoCopy();
            return true;
        }
    }
    else if (uc == (wxChar)WXK_CONTROL_V || uc == 'V' || uc == 'v')
    {
        if (event.CmdDown() || event.ControlDown()) {
            DoPaste();
            return true;
        }
    }

    return false;
}

bool xScheduleFrame::HandleHotkeys(wxKeyEvent& event)
{
    auto bs = __schedule->GetOptions()->GetButtons();

    for (auto it = bs.begin(); it != bs.end(); ++it)
    {
        auto hk = (*it)->GetHotkey();
        if (hk == (char)event.GetRawKeyCode())
        {
            PlayList* playlist = nullptr;
            Schedule* schedule = nullptr;

            wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();

            if (IsPlayList(treeitem))
            {
                playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
            }
            else if (IsSchedule(treeitem))
            {
                schedule = (Schedule*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
            }

            size_t rate = _timer.GetInterval();
            std::string msg = "";
            __schedule->Action((*it)->GetLabel(), playlist, schedule, rate, msg);

            if (rate / 2 != _timer.GetInterval())
            {
                _timer.Stop();
                _timer.Start(rate / 2);
            }

            UpdateSchedule();
            return true;
        }
    }

    return false;
}

void xScheduleFrame::OnBitmapButton_VolumeDownClick(wxCommandEvent& event)
{
    __schedule->AdjustVolumeBy(-10);
    UpdateUI();
}

void xScheduleFrame::OnBitmapButton_VolumeUpClick(wxCommandEvent& event)
{
    __schedule->AdjustVolumeBy(10);
    UpdateUI();
}

void xScheduleFrame::OnCustom_VolumeLeftDown(wxMouseEvent& event)
{
    __schedule->ToggleMute();
    UpdateUI();
}

void xScheduleFrame::OnMenuItem_StandaloneSelected(wxCommandEvent& event)
{
    __schedule->SetMode(SYNCMODE::STANDALONE);
    UpdateUI();
}

void xScheduleFrame::OnMenuItem_FPPMasterSelected(wxCommandEvent& event)
{
    __schedule->SetMode(SYNCMODE::FPPMASTER);
    UpdateUI();
}

void xScheduleFrame::OnMenuItem_FPPRemoteSelected(wxCommandEvent& event)
{
    __schedule->SetMode(SYNCMODE::FPPSLAVE);
    UpdateUI();
}

void xScheduleFrame::OnMenuItem_WebInterfaceSelected(wxCommandEvent& event)
{
    ::wxLaunchDefaultBrowser(wxString::Format("http://localhost:%d", __schedule->GetOptions()->GetWebServerPort()));
}

void xScheduleFrame::OnMenuItem_AddPlayListSelected(wxCommandEvent& event)
{
    AddPlayList();
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::AddPlayList(bool forceadvanced)
{
    PlayList* playlist = new PlayList();
    if (playlist->Configure(this, forceadvanced || __schedule->GetOptions()->IsAdvancedMode()) == nullptr)
    {
        delete playlist;
    }
    else
    {
        wxTreeItemId  newitem = TreeCtrl_PlayListsSchedules->AppendItem(TreeCtrl_PlayListsSchedules->GetRootItem(), playlist->GetName(), -1, -1, new MyTreeItemData(playlist));
        TreeCtrl_PlayListsSchedules->Expand(newitem);
        TreeCtrl_PlayListsSchedules->EnsureVisible(newitem);
        __schedule->AddPlayList(playlist);
    }
}

void xScheduleFrame::OnButton_AddClick(wxCommandEvent& event)
{
    AddPlayList();
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::OnButton_EditClick(wxCommandEvent& event)
{
    EditSelectedItem();
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::OnButton_DeleteClick(wxCommandEvent& event)
{
    DeleteSelectedItem();
    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::EditSelectedItem(bool forceadvanced)
{
    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();
    if (IsPlayList(treeitem))
    {
        PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
        if (playlist->Configure(this, forceadvanced || __schedule->GetOptions()->IsAdvancedMode()) != nullptr)
        {
            TreeCtrl_PlayListsSchedules->SetItemText(treeitem, playlist->GetName());
        }
    }
    else if (IsSchedule(treeitem))
    {
        Schedule* schedule = (Schedule*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
        if (schedule->Configure(this) != nullptr)
        {
            TreeCtrl_PlayListsSchedules->SetItemText(treeitem, schedule->GetName());
            auto rs = __schedule->GetRunningSchedule(schedule);
            if (rs != nullptr) rs->Reset();
        }
    }
}

void xScheduleFrame::OnMenu_OutputProcessingSelected(wxCommandEvent& event)
{
    OutputProcessingDialog dlg(this, __schedule->GetOutputProcessing());

    if (dlg.ShowModal() == wxID_OK)
    {
        __schedule->OutputProcessingChanged();
    }

    UpdateUI();
    ValidateWindow();
}

// This is called when anything interesting happens in schedule manager
void xScheduleFrame::ScheduleChange(wxCommandEvent& event)
{
    UpdateUI();
}

void xScheduleFrame::UpdateUI()
{
    UpdateStatus();

    Brightness->SetValue(__schedule->GetBrightness());

    if (!__schedule->GetOptions()->IsSendOffWhenNotRunning() && __schedule->GetManualOutputToLights() == -1)
    {
        if (__schedule->GetRunningPlayList() == nullptr)
        {
            if (__schedule->IsOutputToLights())
                __schedule->SetOutputToLights(false);
        }
        else
        {
            if (!__schedule->IsOutputToLights())
                __schedule->SetOutputToLights(true);
        }
    }
    else
    {
        if (__schedule->GetManualOutputToLights() == 0)
        {
            if (__schedule->IsOutputToLights())
                __schedule->SetOutputToLights(false);
        }
        else if (__schedule->GetManualOutputToLights() == 1)
        {
            if (!__schedule->IsOutputToLights())
                __schedule->SetOutputToLights(true);
        }
    }

    ValidateWindow();
}

void xScheduleFrame::OnMenuItem_BackgroundPlaylistSelected(wxCommandEvent& event)
{
    int bid = -1;
    if (__schedule->GetBackgroundPlayList() != nullptr)
    {
        bid = __schedule->GetBackgroundPlayList()->GetId();
    }

    BackgroundPlaylistDialog dlg(this, bid, __schedule->GetPlayLists());

    if (dlg.ShowModal() == wxID_OK)
    {
        __schedule->SetBackgroundPlayList(__schedule->GetPlayList(bid));
    }

    ValidateWindow();
}

void xScheduleFrame::OnMenuItem_CheckScheduleSelected(wxCommandEvent& event)
{
    __schedule->CheckScheduleIntegrity(true);
}

void xScheduleFrame::OnButton_ScheduleClick(wxCommandEvent& event)
{
    AddSchedule();
}

void xScheduleFrame::AddSchedule()
{
    wxTreeItemId treeitem = TreeCtrl_PlayListsSchedules->GetSelection();

    if (IsPlayList(treeitem))
    {
        Schedule* schedule = new Schedule();
        if (schedule->Configure(this) == nullptr)
        {
            delete schedule;
        }
        else
        {
            wxTreeItemId  newitem = TreeCtrl_PlayListsSchedules->AppendItem(treeitem, GetScheduleName(schedule, __schedule->GetRunningSchedules()), -1, -1, new MyTreeItemData(schedule));
            PlayList* playlist = (PlayList*)((MyTreeItemData*)TreeCtrl_PlayListsSchedules->GetItemData(treeitem))->GetData();
            TreeCtrl_PlayListsSchedules->Expand(treeitem);
            TreeCtrl_PlayListsSchedules->EnsureVisible(newitem);
            playlist->AddSchedule(schedule);
        }
	}
}

void xScheduleFrame::OnMenuItem_MatricesSelected(wxCommandEvent& event)
{
    auto matrices = __schedule->GetOptions()->GetMatrices();
    MatricesDialog dlg(this, matrices);

    if (dlg.ShowModal() == wxID_OK)
    {
        // it is updated directly
    }

    UpdateUI();
    ValidateWindow();
}

void xScheduleFrame::OnMenuItem_ImportxLightsSelected(wxCommandEvent& event)
{
    if (FileDialog1->ShowModal() == wxID_OK)
    {
        __schedule->ImportxLightsSchedule(FileDialog1->GetPath().ToStdString());
        UpdateTree();
        UpdateUI();
        ValidateWindow();
    }
}
