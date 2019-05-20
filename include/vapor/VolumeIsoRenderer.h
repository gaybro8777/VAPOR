#pragma once

#include <vapor/VolumeRenderer.h>
#include <vapor/VolumeAlgorithm.h>
#include <glm/fwd.hpp>

using std::string;
using std::vector;

namespace VAPoR {

class RENDER_API VolumeIsoRenderer : public VolumeRenderer {
public:
    VolumeIsoRenderer(const ParamsMgr *pm, std::string &winName, std::string &dataSetName, std::string &instName, DataMgr *dataMgr);
    ~VolumeIsoRenderer();

    static std::string GetClassType() { return ("IsoSurface"); }

    virtual bool        _usingColorMapData() const;
    virtual void        _setShaderUniforms(const ShaderProgram *shader, const bool fast) const;
    virtual std::string _getDefaultAlgorithmForGrid(const Grid *grid) const;
    virtual void        _getLUTFromTF(const MapperFunction *tf, float *LUT) const;
};

};    // namespace VAPoR
