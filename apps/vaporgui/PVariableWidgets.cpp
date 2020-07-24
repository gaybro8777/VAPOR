#include "PVariableWidgets.h"
#include <vapor/RenderParams.h>
#include "VComboBox.h"
#include "PSection.h"
#include "PFidelitySection.h"

using namespace VAPoR;

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

#define NULL_TEXT "<none>"

PVariableSelector::PVariableSelector(const std::string &tag, const std::string &label) : PStringDropdown(tag, {}, label) {}

void PVariableSelector::updateGUI() const
{
    RenderParams *rp = dynamic_cast<RenderParams *>(getParams());
    assert(rp && "Params must be RenderParams");
    static_cast<void>(rp);    // Silence unused variable warning

    int nDims = getDimensionality();

    auto varNames = getDataMgr()->GetDataVarNames(nDims);

    if (_addNull) varNames.insert(varNames.begin(), NULL_TEXT);

    SetItems(varNames);
    PStringDropdown::updateGUI();
}

bool PVariableSelector::isShown() const
{
    if (_onlyShowForDim > 0) return getRendererDimension() == _onlyShowForDim;
    return true;
}

int PVariableSelector::getRendererDimension() const
{
    RenderParams *rp = dynamic_cast<RenderParams *>(getParams());
    return rp->GetBox()->GetOrientation() == Box::XYZ ? 3 : 2;
}

int PVariableSelector::getDimensionality() const
{
    int dims = getDataMgr()->GetNumDimensions(getParamsString());
    if (dims > 0) return dims;

    return getRendererDimension();
}

void PVariableSelector::dropdownTextChanged(std::string text)
{
    if (_addNull && text == NULL_TEXT) text = "";

    PStringDropdown::dropdownTextChanged(text);
}

PScalarVariableSelector::PScalarVariableSelector() : PVariableSelector("", "Variable Name") {}

PColorMapVariableSelector::PColorMapVariableSelector() : PVariableSelector("", "Color mapped variable") {}

PHeightVariableSelector::PHeightVariableSelector() : PVariableSelector2D("", "Height variable")
{
    AddNullOption();
    OnlyShowForDim(2);
}

PXFieldVariableSelector::PXFieldVariableSelector() : PVariableSelector("", "X") { AddNullOption(); }

PYFieldVariableSelector::PYFieldVariableSelector() : PVariableSelector("", "Y") { AddNullOption(); }

PZFieldVariableSelector::PZFieldVariableSelector() : PVariableSelector("", "Z") { AddNullOption(); }

PVariablesGroup::PVariablesGroup() : PGroup()
{
    _varSection = new PSection("Variable Selection");

    PGroup *pGroup = new PGroup;
    pGroup->Add(_varSection);
    pGroup->Add(new PFidelitySection);

    PGroup::Add(pGroup);

    _scrollArea = new QScrollArea();
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scrollArea->setWidget(pGroup);
    _scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
}

QScrollArea *PVariablesGroup::GetScrollArea() const { return _scrollArea; }

void PVariablesGroup::AddVar(PWidget *pw) { _varSection->Add(pw); }
