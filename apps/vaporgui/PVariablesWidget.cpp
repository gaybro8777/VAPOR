#include "PVariablesWidget.h"
#include <vapor/RenderParams.h>
#include "VComboBox.h"

using namespace VAPoR;

// ==================================
//      PFieldVariablesSelector
// ==================================

PFieldVariablesSelector::PFieldVariablesSelector()
{
    Add((new PVariableSelector(RenderParams::_xFieldVariableNameTag, "X"))->AddNullOption());
    Add((new PVariableSelector(RenderParams::_yFieldVariableNameTag, "Y"))->AddNullOption());
    Add((new PVariableSelector(RenderParams::_zFieldVariableNameTag, "Z"))->AddNullOption());
}

// ==================================
//         PDimensionSelector
// ==================================

PDimensionSelector::PDimensionSelector() : PLineItem("", "Variable Dimension", _vComboBox = new VComboBox({"2D", "3D"}))
{
    connect(_vComboBox, &VComboBox::ValueChanged, this, &PDimensionSelector::dropdownTextChanged);
}

void PDimensionSelector::updateGUI() const
{
    RenderParams *rp = dynamic_cast<RenderParams *>(getParams());
    assert(rp && "Params must be RenderParams");

    if (rp->GetBox()->GetOrientation() == Box::XYZ)
        _vComboBox->SetValue("3D");
    else
        _vComboBox->SetValue("2D");
}

void PDimensionSelector::dropdownTextChanged(std::string text)
{
    RenderParams *rp = (RenderParams *)getParams();
    int           dim = text == "2D" ? 2 : 3;

    rp->BeginGroup("Change dim");

    if (dim == 2) {
        rp->GetBox()->SetPlanar(true);
        rp->GetBox()->SetOrientation(VAPoR::Box::XY);
    } else {
        rp->GetBox()->SetPlanar(false);
        rp->GetBox()->SetOrientation(VAPoR::Box::XYZ);
    }
    rp->SetDefaultVariables(dim, true);

    rp->EndGroup();
}

// ==================================
//         Variable Selectors
// ==================================

PScalarVariableSelector::PScalarVariableSelector() : PVariableSelector(RenderParams::_variableNameTag, "Variable Name") {}
PColorMapVariableSelector::PColorMapVariableSelector() : PVariableSelector(RenderParams::_colorMapVariableNameTag, "Color mapped variable") {}
PHeightVariableSelector::PHeightVariableSelector() : PVariableSelector2D(RenderParams::_heightVariableNameTag, "Height variable") { AddNullOption(); }
