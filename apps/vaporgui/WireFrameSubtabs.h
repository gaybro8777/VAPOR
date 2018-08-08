#ifndef WIREFRAMESUBTABS_H
#define WIREFRAMESUBTABS_H

#include "ui_WireFrameAppearanceGUI.h"
#include "ui_WireFrameVariablesGUI.h"
#include "ui_WireFrameGeometryGUI.h"
#include "Flags.h"

namespace VAPoR {
class ControlExec;
class RenderParams;
class ParamsMgr;
class DataMgr;
}    // namespace VAPoR

class WireFrameVariablesSubtab : public QWidget, public Ui_WireFrameVariablesGUI {
    Q_OBJECT

public:
    WireFrameVariablesSubtab(QWidget *parent)
    {
        setupUi(this);
        _variablesWidget->Reinit((VariableFlags)(SCALAR | HEIGHT), (DimFlags)(THREED | TWOD));
    }

    void Update(VAPoR::DataMgr *dataMgr, VAPoR::ParamsMgr *paramsMgr, VAPoR::RenderParams *rParams) { _variablesWidget->Update(dataMgr, paramsMgr, rParams); }
};

class WireFrameAppearanceSubtab : public QWidget, public Ui_WireFrameAppearanceGUI {
    Q_OBJECT

public:
    WireFrameAppearanceSubtab(QWidget *parent)
    {
        setupUi(this);
        _TFWidget->Reinit((TFFlags)(CONSTANT));
    }

    void Update(VAPoR::DataMgr *dataMgr, VAPoR::ParamsMgr *paramsMgr, VAPoR::RenderParams *rParams)
    {
        _TFWidget->Update(dataMgr, paramsMgr, rParams);
        _ColorbarWidget->Update(dataMgr, paramsMgr, rParams);
    }
};

class WireFrameGeometrySubtab : public QWidget, public Ui_WireFrameGeometryGUI {
    Q_OBJECT

public:
    WireFrameGeometrySubtab(QWidget *parent)
    {
        setupUi(this);
        _geometryWidget->Reinit(THREED, MINMAX, SCALAR);
    }

    void Update(VAPoR::ParamsMgr *paramsMgr, VAPoR::DataMgr *dataMgr, VAPoR::RenderParams *rParams)
    {
        _geometryWidget->Update(paramsMgr, dataMgr, rParams);
        _copyRegionWidget->Update(paramsMgr, rParams);
        _transformTable->Update(rParams->GetTransform());
    }

private:
};

#endif    // WIREFRAMESUBTABS_H
