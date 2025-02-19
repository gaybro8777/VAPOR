//************************************************************************
//									*
//		     Copyright (C)  2004				*
//     University Corporation for Atmospheric Research			*
//		     All Rights Reserved				*
//									*
//************************************************************************/
//
//	File:		VizWinMgr.cpp
//
//	Author:		Alan Norton
//			National Center for Atmospheric Research
//			PO 3000, Boulder, Colorado
//
//	Date:		Sept 2004
//
//	Description:  Implementation of VizWinMgr class
//		This class manages the VizWin visualizers
//		Its main function is to catch events from the visualizers and
//		to route them to the appropriate params class, and in reverse,
//		to route events from tab panels to the appropriate visualizer.
//
#ifdef WIN32
    #pragma warning(disable : 4251 4100)
#endif
#include <vapor/glutil.h>    // Must be included first!!!
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include "vapor/VAssert.h"
#include "GL/glew.h"
#include <qapplication.h>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <vapor/ControlExecutive.h>
#include <vapor/ParamsMgr.h>
#include <vapor/DataStatus.h>

#include <vapor/AnimationParams.h>
#include <vapor/MouseModeParams.h>
#include <vapor/TrackBall.h>
#include "VizWin.h"
#include "VizWinMgr.h"
#include "ErrorReporter.h"

using namespace VAPoR;

namespace {
string make_viz_name(vector<string> currentNames)
{
    int    index = 0;
    bool   found = false;
    string name;
    while (!found) {
        std::stringstream out;
        out << index;
        name = "Visualizer_No._" + out.str();

        found = true;
        for (int i = 0; i < currentNames.size(); i++) {
            if (currentNames[i] == name) found = false;
        }
        index++;
    }
    return (name);
}
};    // namespace

VizWinMgr::VizWinMgr(QWidget *parent, QMdiArea *mdiArea, ControlExec *ce) : QObject(parent)
{
    _parent = parent;
    VAssert(mdiArea);
    VAssert(ce);

    _mdiArea = mdiArea;
    _controlExec = ce;

    _setActiveViz("");

    _vizWindow.clear();
    _vizMdiWin.clear();

    _trackBall = new Trackball();

    _initialized = false;
}

/***********************************************************************
 *  Destroys the object and frees any allocated resources
 ***********************************************************************/
VizWinMgr::~VizWinMgr()
{
    if (_trackBall) delete _trackBall;
}

void VizWinMgr::_attachVisualizer(string vizName)
{
    if (_vizWindow.find(vizName) != _vizWindow.end()) return;

    // Don't record events generated by window activation here:

    QString qvizname = QString::fromStdString(vizName);

    int       major = 4, minor = 1;
    QGLFormat glFormat;
    glFormat.setVersion(major, minor);
    glFormat.setProfile(QGLFormat::CoreProfile);

    _vizWindow[vizName] = new VizWin(glFormat, _parent, qvizname, vizName, _controlExec, _trackBall

    );

    connect(_vizWindow[vizName], SIGNAL(Closing(const string &)), this, SLOT(_vizAboutToDisappear(const string &)));
    connect(_vizWindow[vizName], SIGNAL(HasFocus(const string &)), this, SLOT(_setActiveViz(const string &)));

    connect(_vizWindow[vizName], SIGNAL(EndNavigation(const string &)), this, SLOT(_syncViewpoints(const string &)));

    QMdiSubWindow *qsbw = _mdiArea->addSubWindow(_vizWindow[vizName]);
    _vizMdiWin[vizName] = qsbw;
    _vizWindow[vizName]->setFocusPolicy(Qt::ClickFocus);
    _vizWindow[vizName]->setWindowTitle(qvizname);

    GUIStateParams *p = _getStateParams();
    string          prevActiveVizName = p->GetActiveVizName();

    _setActiveViz(vizName);

    _vizWindow[vizName]->showMaximized();

    int numWins = _controlExec->GetNumVisualizers();
    // Tile if more than one visualizer:
    if (numWins > 1) FitSpace();

    // Set non-renderer tabbed panels to use global parameters:

    _vizWindow[vizName]->show();

    // notify the window selector:
    emit(newViz(qvizname));

    // When we go from 1 to 2 windows, need to enable multiple
    // viz panels and signals.
    //
    if (numWins > 1) {
        emit enableMultiViz(true);
        _syncViewpoints(prevActiveVizName);
    }
}

void VizWinMgr::LaunchVisualizer()
{
    ParamsMgr *paramsMgr = _controlExec->GetParamsMgr();
    paramsMgr->BeginSaveStateGroup(_controlExec->GetNewVisualizerUndoTag());

    string vizName = make_viz_name(_controlExec->GetVisualizerNames());

    int rc = _controlExec->NewVisualizer(vizName);
    if (rc < 0) {
        MSG_ERR("Failed to create new visualizer");
        paramsMgr->EndSaveStateGroup();
        return;
    }

    _attachVisualizer(vizName);

    paramsMgr->EndSaveStateGroup();
}

/**************************************************************
 * Methods that arrange the viz windows:
 **************************************************************/
void VizWinMgr::Cascade()
{
    _mdiArea->cascadeSubWindows();
    // Now size them up to a reasonable size:
    map<string, VizWin *>::iterator it;
    for (it = _vizWindow.begin(); it != _vizWindow.end(); it++) { (it->second)->resize(400, 400); }
}

void VizWinMgr::FitSpace() { _mdiArea->tileSubWindows(); }

/**********************************************************************
 * Method called when the user makes a visualizer active:
 **********************************************************************/
void VizWinMgr::_setActiveViz(string vizName)
{
    GUIStateParams *p = _getStateParams();
    string          currentVizName = p->GetActiveVizName();
    if (currentVizName != vizName) {
        p->SetActiveVizName(vizName);
        emit(activateViz(QString::fromStdString(vizName)));

        if (vizName.empty()) return;

        // Need to cause a redraw in all windows if we are not in navigate mode,
        // So that the manips will change where they are drawn:

        MouseModeParams *p = _getStateParams()->GetMouseModeParams();
        if (p->GetCurrentMouseMode() != MouseModeParams::GetNavigateModeName()) {
            map<string, VizWin *>::iterator it;
            for (it = _vizWindow.begin(); it != _vizWindow.end(); it++) { (it->second)->Render(false); }
        }
    }
}

void VizWinMgr::_syncViewpoints(string vizName)
{
    if (vizName.empty()) return;

    ParamsMgr *paramsMgr = _controlExec->GetParamsMgr();

    bool enabled = paramsMgr->GetSaveStateEnabled();
    paramsMgr->SetSaveStateEnabled(false);

    ViewpointParams *currentVP = _getViewpointParams(vizName);

    vector<string> winNames = _getVisualizerNames();
    for (int i = 0; i < winNames.size(); i++) {
        if (winNames[i] != vizName) {
            ViewpointParams *vpParams = _getViewpointParams(winNames[i]);
            vpParams->SetModelViewMatrix(currentVP->GetModelViewMatrix());
            vpParams->SetRotationCenter(currentVP->GetRotationCenter());
        }
    }

    paramsMgr->SetSaveStateEnabled(enabled);
}

vector<string> VizWinMgr::_getVisualizerNames() const
{
    vector<string>                             names;
    std::map<string, VizWin *>::const_iterator itr = _vizWindow.begin();
    for (; itr != _vizWindow.end(); ++itr) { names.push_back(itr->first); }
    return (names);
}

// Method to enable closing of a vizWin
void VizWinMgr::_killViz(string vizName)
{
    VAssert(_vizWindow.find(vizName) != _vizWindow.end());

    _mdiArea->removeSubWindow(_vizMdiWin[vizName]);

    // This will trigger a closeEvent on VizWin, which will in turn
    // call vizAboutToDisappear
    //
    _vizWindow[vizName]->setEnabled(false);
    _vizWindow[vizName]->close();
}

/*
 * Tell the parameter panels when there are or are not multiple viz's
 */

/********************************************************************
 *					SLOTS
 ********************************************************************/
/*
 * Slot that responds to user setting of VizSelectCombo
 */
void VizWinMgr::SetWinActive(const QString &qS)
{
    string vizName = qS.toStdString();
    // Truly make specified visualizer active:

    _vizWindow[vizName]->setFocus();
}

//
// Slot that responds to VizWin Closing signal
//
void VizWinMgr::_vizAboutToDisappear(string vizName)
{
    std::map<string, VizWin *>::iterator itr = _vizWindow.find(vizName);
    if (itr == _vizWindow.end()) return;

    std::map<string, QMdiSubWindow *>::iterator itr2 = _vizMdiWin.find(vizName);
    VAssert(itr2 != _vizMdiWin.end());

    ParamsMgr *paramsMgr = _controlExec->GetParamsMgr();
    paramsMgr->BeginSaveStateGroup(_controlExec->GetRemoveVisualizerUndoTag());

    GUIStateParams *p = _getStateParams();
    string          activeViz = p->GetActiveVizName();

    itr->second->makeCurrent();

    _controlExec->RemoveVisualizer(vizName, true);

    // disconnect all signals from window
    //
    disconnect(_vizWindow[vizName], 0, this, 0);

    // Remove the vizwin and the vizmdiwin
    //
    delete _vizWindow[vizName];
    _vizWindow.erase(vizName);
    _vizMdiWin.erase(vizName);

    // If we are deleting the active viz, pick a new active viz
    //
    if (activeViz == vizName && _vizWindow.size()) {
        map<string, VizWin *>::iterator itr = _vizWindow.begin();
        _setActiveViz(itr->first);
    }

    // Remove the visualizer from the vizSelectCombo
    emit(removeViz(QString::fromStdString(vizName)));

    // When the number is reduced to 1, disable multiviz options.
    //
    if (_vizWindow.size() == 1) emit enableMultiViz(false);

    paramsMgr->EndSaveStateGroup();
}

/*******************************************************************
 *	Slots associated with VizTab:
 ********************************************************************/

void VizWinMgr::Update(bool fast)
{
    // Certain actions queue multiple renders within Qt's event queue (e.g. changing the
    // renderer variable dimension). Normally, Qt will then process each of these events
    // sequentially and render multiple times. While not ideal, this just results in extra
    // renders and computation time. When using the progress bar, however, it sometimes
    // requests a redraw of the entire GUI at which point Qt processes queued events.
    // When you have pending render events, it will now try to initiate a new render
    // before the first has finished. This prevents that but it does not fix the
    // underlying issue.
    if (_insideRender) return;
    _insideRender = true;

    map<string, VizWin *>::const_iterator it;
    for (it = _vizWindow.begin(); it != _vizWindow.end(); it++) { (it->second)->Render(fast); }

    _insideRender = false;
}

int VizWinMgr::EnableImageCapture(string filename, string winName)
{
    _vizWindow[winName]->makeCurrent();
    _vizWindow[winName]->_preRender();
    return _controlExec->EnableImageCapture(filename, winName);
    _vizWindow[winName]->_postRender();
}

void VizWinMgr::Shutdown()
{
    vector<string> vizNames = _getVisualizerNames();

    for (int i = 0; i < vizNames.size(); i++) { _killViz(vizNames[i]); }
    VAssert(_vizMdiWin.empty());
    VAssert(_vizWindow.empty());

    _initialized = false;
}

void VizWinMgr::Restart()
{
    // Must be shutdown before restarting
    //
    if (_initialized) return;

    GUIStateParams *p = _getStateParams();
    p->SetActiveVizName("");

    vector<string> vizNames = _controlExec->GetVisualizerNames();
    for (int i = 0; i < vizNames.size(); i++) { _attachVisualizer(vizNames[i]); }

    _initialized = true;

    Update(false);
}

void VizWinMgr::Reinit()
{
    if (_controlExec->GetDataNames().size() == 0) return;

    DataStatus *dataStatus = _controlExec->GetDataStatus();
    ParamsMgr * paramsMgr = _controlExec->GetParamsMgr();
    size_t      ts = _getAnimationParams()->GetCurrentTimestep();

    vector<double> minExts, maxExts;
    dataStatus->GetActiveExtents(paramsMgr, ts, minExts, maxExts);
    VAssert(minExts.size() == 3);
    VAssert(maxExts.size() == 3);

    double scale[3];
    scale[0] = scale[1] = scale[2] = max(maxExts[0] - minExts[0], (maxExts[1] - minExts[1]));
    _trackBall->SetScale(scale);
}
