#ifndef SLICERENDERER_H
#define SLICERENDERER_H

#include <vapor/glutil.h>

#ifdef Darwin
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <vapor/DataMgr.h>
#include <vapor/utils.h>
#include <vapor/Renderer.h>
//#include <vapor/ConvexHull.h>

namespace VAPoR {

class RENDER_API SliceRenderer : public Renderer {
public:
    SliceRenderer(const ParamsMgr *pm, string winName, string dataSetName, string instName, DataMgr *dataMgr);

    static string GetClassType() { return ("Slice"); }

    virtual ~SliceRenderer();

    std::vector<glm::vec3> orderedVertices;
    std::vector<glm::vec3> square;
    std::vector<glm::vec2> square2D;

    void getVecs(glm::vec3& vec1,glm::vec3& vec2,glm::vec3& vec3,glm::vec3& vec4) {
        vec1=v1;
        vec2=v2;
        vec3=v3;
        vec4=v4;
    }

protected:
    virtual int _initializeGL();
    virtual int _paintGL(bool fast);

private:
    struct {
        string              varName;
        string              heightVarName;
        size_t              ts;
        int                 refinementLevel;
        int                 compressionLevel;
        int                 textureSampleRate;
        int                 orientation;
        double              xRotation;
        double              yRotation;
        double              zRotation;
        std::vector<float>  tf_lut;
        std::vector<double> tf_minMax;
        std::vector<double> boxMin, boxMax;
        std::vector<double> domainMin, domainMax;
        std::vector<double> sampleLocation;
    } _cacheParams;

    struct _point {
        glm::vec3 threeD;
        glm::vec2 twoD;
    }; 

    void _rotate();
    void _initVAO();
    void _initTexCoordVBO();
    void _initVertexVBO();

    bool _isColormapCacheDirty() const;
    bool _isDataCacheDirty() const;
    bool _isBoxCacheDirty() const;
    void _getModifiedExtents(vector<double> &min, vector<double> &max) const;
    int  _saveCacheParams();
    void _resetColormapCache();
    int  _resetBoxCache();
    int  _resetDataCache();
    void _initTextures();
    void _createDataTexture(float *dataValues);
    int  _saveTextureData();
    void _populateDataXY(float *dataValues, Grid *grid) const;
    void _populateDataXZ(float *dataValues, Grid *grid) const;
    void _populateDataYZ(float *dataValues, Grid *grid) const;
    glm::vec3 _getOrthogonal( const glm::vec3 u ) const;
    glm::vec3 _rotateVector(glm::vec3 vector, glm::quat rotation) const;

    double _newWaySeconds;
    double _newWayInlineSeconds;
    double _oldWaySeconds;

    std::vector<double> _calculateDeltas() const;

    int _getConstantAxis() const;

    void _configureShader();
    void _resetState();
    void _initializeState();
    void _resetTextureCoordinates();

    void _setVertexPositions();
    void _setXYVertexPositions(std::vector<double> min, std::vector<double> max);
    void _setXZVertexPositions(std::vector<double> min, std::vector<double> max);
    void _setYZVertexPositions(std::vector<double> min, std::vector<double> max);

    bool _initialized;
    int  _textureSideSize;

    GLuint _colorMapTextureID;
    GLuint _dataValueTextureID;

    std::vector<double> _vertexCoords;
    std::vector<float>  _texCoords;

    GLuint _VAO;
    GLuint _vertexVBO;
    GLuint _texCoordVBO;

    glm::vec3 v1, v2, v3, v4;

    int _colorMapSize;

    void _clearCache() { _cacheParams.varName.clear(); }
};

};    // namespace VAPoR

#endif
