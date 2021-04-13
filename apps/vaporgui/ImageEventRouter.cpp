#include <vapor/ImageParams.h>
#include "PWidgets.h"
#include "PTMSLODInput.h"
#include "VPushButton.h"
#include "PSection.h"
#include "PFileSelector.h"
#include "ImageEventRouter.h"

using namespace VAPoR;
typedef ImageParams IP;

static RenderEventRouterRegistrar<ImageEventRouter> registrar(ImageEventRouter::GetClassType());

ImageEventRouter::ImageEventRouter(QWidget *parent, ControlExec *ce) : RenderEventRouterGUI(ce, ImageParams::GetClassType())
{

    // clang-format off

    AddVariablesSubtab(new PGroup({
        new PSection("Variable Selection", {
            new PHeightVariableSelector
        }),
        new PFidelitySection
    }));
    
    AddAppearanceSubtab(
        new PSection("Image", {
            new PCheckbox(IP::_isGeoRefTag, "Geo Reference"),
            new PCheckbox(IP::_ignoreTransparencyTag, "Ingore Transparency"),
            (new PDoubleSliderEdit(IP::_constantOpacityTag, "Opacity"))->EnableDynamicUpdate(),
            (new PFileOpenSelector(IP::_fileNameTag, "Image File"))->SetFileTypeFilter("TIFF files, tiled images (*.tiff *.tif *.gtif *.tms)"),
            new PTMSLODInput(),
            //new PSection("Create custom GeoTIFF", {
                new PLabel( "" ),
                new PLabel( "Create custom GeoTIFF" ),
                (new PFileSaveSelector(IP::_tiffCreateFileName, "File name"))->SetFileTypeFilter("TIFF files, tiled images (*.tiff *.tif *.gtif *.tms)"),
                (new PIntegerInput(IP::_tiffCreateWidth, "Width"))->SetRange(1, 8000),
                (new PIntegerInput(IP::_tiffCreateHeight, "Height"))->SetRange(1, 8000),
                (new PDoubleSliderEdit(IP::_tiffCreateNorth, "North"))->SetRange(-85, 85),
                (new PDoubleSliderEdit(IP::_tiffCreateSouth, "South"))->SetRange(-85, 85),
                (new PDoubleSliderEdit(IP::_tiffCreateEast, "East"))->SetRange(-180, 180),
                (new PDoubleSliderEdit(IP::_tiffCreateWest, "West"))->SetRange(-180, 180),
                new PCheckbox(IP::_tiffIncludeCoastline, "Include Coastline"),
                new PButton("Create GeoTIFF",nullptr)
            //})
        })
    );

    //VPushButton* newImage = new VPushButton("Create new image");
    //connect( newImage, &VPushButton::ButtonClicked, _newImageDialog, &QDialog::show );
    
 
    //_newImageDialog = new NewImageDialog(this);
    /*_newImageDialog = new QDialog(this);
    _newImageDialog->setLayout(new QVBoxLayout);
    PGroup* newImageGroup = new PGroup({
        new PSection("Create custom GeoTIFF", {
            (new PDoubleSliderEdit(IP::_tiffCreateNorth, "North"))->SetRange(-85, 85),
            (new PDoubleSliderEdit(IP::_tiffCreateSouth, "South"))->SetRange(-85, 85),
            (new PDoubleSliderEdit(IP::_tiffCreateEast, "East"))->SetRange(-180, 180),
            (new PDoubleSliderEdit(IP::_tiffCreateWest, "West"))->SetRange(-180, 180)
        })
    });
    _newImageDialog->layout()->addWidget(newImageGroup);*/
    
    AddGeometrySubtab(new PGeometrySubtab);

    // clang-format on
}

string ImageEventRouter::_getDescription() const
{
    return ("Displays a "
            "georeferenced image that is automatically reprojected and fit to the user's"
            "data, as long as the data contains georeference metadata.  The image "
            "renderer may be offset by a height variable to show bathymetry or mountainous"
            " terrain.\n\n ");
}

/*NewImageDialog::NewImageDialog( QWidget* parent ) : QDialog(parent) {
    _settings = new PGroup({
        new PSection("Create custom GeoTIFF", {
            (new PDoubleSliderEdit(IP::_tiffCreateNorth, "North"))->SetRange(-85, 85),
            (new PDoubleSliderEdit(IP::_tiffCreateSouth, "South"))->SetRange(-85, 85),
            (new PDoubleSliderEdit(IP::_tiffCreateEast, "East"))->SetRange(-180, 180),
            (new PDoubleSliderEdit(IP::_tiffCreateWest, "West"))->SetRange(-180, 180)
        })
    });
}

void NewImageDialog::Update(VAPoR::ParamsBase *p, VAPoR::ParamsMgr *paramsMgr, VAPoR::DataMgr *dataMgr )
{
    _params = dynamic_cast<SettingsParams *>(p);
    VAssert(_params != nullptr);

    _settings->Update(_params);
}*/
