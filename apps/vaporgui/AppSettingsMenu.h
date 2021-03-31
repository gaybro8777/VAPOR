#pragma once

#include <QDialog>
#include "PWidget.h"
#include "Updateable.h"
#include "PFileSelectorHLI.h"
#include "VFileSelector.h"

class QWidget;
class SettingsParams;
class PGroup;
class PSection;

//! \class Preferences Menu
//! \ingroup Public_GUI
//! \brief Menu for global application preferences
//! \author Scott Pearse

class AppSettingsMenu : public QDialog, public Updateable {
    Q_OBJECT

public:
    AppSettingsMenu(QWidget *parent);
    virtual void Update(VAPoR::ParamsBase *p, VAPoR::ParamsMgr *paramsMgr = nullptr, VAPoR::DataMgr *dataMgr = nullptr);

private:
    PGroup *_settings;
};
