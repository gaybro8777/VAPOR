#include <vapor/ImageParams.h>
#include <vapor/ResourcePath.h>

using namespace VAPoR;

const std::string ImageParams::_fileNameTag = "FileName";
const std::string ImageParams::_isGeoRefTag = "IsGeoRefTag";
const std::string ImageParams::_ignoreTransparencyTag = "IgnoreTransparency";
const std::string ImageParams::_opacityTag = "Opacity";
const std::string ImageParams::_orientationTag = "Orientation";
const std::string ImageParams::_TMSLODTag = "TMSLevelOfDetail";
const std::string ImageParams::_numTMSLODTag = "numTMSLevelOfDetail";
const std::string ImageParams::_tiffCreateWest = "West";
const std::string ImageParams::_tiffCreateNorth = "North";
const std::string ImageParams::_tiffCreateEast = "East";
const std::string ImageParams::_tiffCreateSouth = "South";
const std::string ImageParams::_tiffCreateWidth = "Width";
const std::string ImageParams::_tiffCreateHeight = "Height";
const std::string ImageParams::_tiffCreateURL = "URL";
const std::string ImageParams::_tiffCreateLayer = "Layer";
const std::string ImageParams::_tiffCreateFileName = "FileName";
const std::string ImageParams::_tiffIncludeCoastline = "Coastline";

//
// Register class with object factory
//
static RenParamsRegistrar<ImageParams> registrar(ImageParams::GetClassType());

ImageParams::ImageParams(DataMgr *dataManager, ParamsBase::StateSave *stateSave) : RenderParams(dataManager, stateSave, ImageParams::GetClassType(), 2)
{
    SetVariableName("");
    SetFieldVariableNames(vector<string>());
    SetDiagMsg("ImageParams::ImageParams() this=%p", this);
}

ImageParams::ImageParams(DataMgr *dataManager, ParamsBase::StateSave *stateSave, XmlNode *node) : RenderParams(dataManager, stateSave, node, 2) { _initialized = true; }

ImageParams::~ImageParams() { SetDiagMsg("ImageParams::~ImageParams() this=%p", this); }

int ImageParams::Initialize()
{
    int rc = RenderParams::Initialize();
    if (rc < 0) return (rc);
    if (_initialized) return 0;
    _initialized = true;

    // The image renderer behaves like a 2D renderer, but it doesn't operate
    // on any data variables. Make sure the box is planar.
    //
    GetBox()->SetOrientation(VAPoR::Box::XY);
    GetBox()->SetPlanar(true);

    SetImagePath(Wasp::GetSharePath("images/NaturalEarth.tms"));
    SetIsGeoRef(true);
    SetIgnoreTransparency(false);

    // SetValueString( _tiffCreateFileName, "Custom geotiff path", Wasp::GetSharePath("images/customGeoTiff.tif") );
    SetValueLong(_tiffIncludeCoastline, "Include coastline in custom geotiff", 0);
    SetValueLong(_tiffCreateWidth, "Custom geotiff width", 1920);
    SetValueLong(_tiffCreateHeight, "Custom geotiff height", 1080);
    SetValueDouble(_tiffCreateWest, "", -120.0);
    SetValueDouble(_tiffCreateNorth, "", 30.0);
    SetValueDouble(_tiffCreateEast, "", 120.0);
    SetValueDouble(_tiffCreateSouth, "", -30.0);

    return (0);
}

std::string ImageParams::GetImagePath() const
{
    std::string defaultImage = Wasp::GetSharePath("images/NaturalEarth.tms");

    return GetValueString(_fileNameTag, defaultImage);
}
