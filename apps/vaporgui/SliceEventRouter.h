#ifndef SLICEEVENTROUTER_H
#define SLICEEVENTROUTER_H

#include <qobject.h>
#include <vapor/MyBase.h>
#include "GL/glew.h"
#include "vapor/SliceRenderer.h"
#include "vapor/SliceParams.h"
#include "RenderEventRouter.h"
#include "VariablesWidget.h"
#include "PVariablesWidget.h"
#include "PSimpleWidget.h"
#include "PIntegerInput.h"
#include "PDoubleInput.h"
#include "VLineComboBox.h"
#include "VSliderEdit.h"
#include "SliceSubtabs.h"
#include <QTabWidget>

QT_USE_NAMESPACE

namespace VAPoR {
class ControlExec;
}

class GLSliceImageWindow;

//!
//! \class SliceEventRouter
//! \ingroup Public_GUI
//! \brief An EventRouter subclass that handles the Slice tab in the GUI
//! \author Scott Pearse
//! \version 3.0
//! \date  April 2016

//!	The SliceEventRouter class manages the Slice gui.  There are three sub-tabs,
//! for variables, geometry, and appearance.

// class SliceEventRouter : public QTabWidget,  public RenderEventRouter {
class SliceEventRouter : public QTabWidget, public RenderEventRouter {
    Q_OBJECT

public:
    SliceEventRouter(QWidget *parent, VAPoR::ControlExec *ce);

    void GetWebHelp(vector<pair<string, string>> &help) const;

    //
    static string GetClassType() { return (VAPoR::SliceRenderer::GetClassType()); }
    string        GetType() const { return GetClassType(); }

    virtual DimFlags GetDimFlags() const { return _variables->_variablesWidget->GetDimFlags(); }

protected:
    void           _updateTab();
    virtual string _getDescription() const;

    virtual string _getSmallIconImagePath() const { return ("Slice_small.png"); }
    virtual string _getIconImagePath() const { return ("Slice.png"); }

private:
    //! Override default wheel behavior on the tab.  This has the effect of
    //! ignoring wheel events over the tab.  This is because wheel events will always
    //! affect the combo boxes and other widgets in the tab, and it would be confusing
    //! if wheel events also scrolled the tab itself
    void wheelEvent(QWheelEvent *) {}

    VComboBox *  _vComboBox;
    VSliderEdit *_vSliderEdit;
    VLineItem *  _vli1;
    VLineItem *  _vli2;

    PIntegerInput *_pIntegerInput;
    PDoubleInput * _pDoubleInput;

    //! VariablesWidget is used as Variables tab
    SliceVariablesSubtab * _variables;
    SliceGeometrySubtab *  _geometry;
    GLSliceImageWindow *   _glSliceImageWindow;
    SliceAppearanceSubtab *_appearance;
    SliceAnnotationSubtab *_annotation;

#ifdef VAPOR3_0_0_ALPHA
    SliceImageGUI *_image;
#endif
};

#endif    // SLICEEVENTROUTER_H
