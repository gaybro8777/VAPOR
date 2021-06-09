#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include "vapor/VAssert.h"
#include <stdio.h>

#ifdef _WINDOWS
    #define _USE_MATH_DEFINES
    #pragma warning(disable : 4251 4100)
#endif
#include <cmath>

#include <vapor/GeoUtil.h>
#include <vapor/UDUnitsClass.h>
#include <vapor/DCBOV.h>
#include <vapor/DCUtils.h>

using namespace VAPoR;
using namespace std;

DCBOV::DCBOV() : _bovCollection(nullptr)
{
    _dimsMap.clear();
    _coordVarsMap.clear();
    _dataVarsMap.clear();
    _meshMap.clear();
    _coordVarKeys.clear();
}

DCBOV::~DCBOV()
{
    if (_bovCollection != nullptr) { delete _bovCollection; }
}

int DCBOV::initialize(const vector<string> &paths, const std::vector<string> &options)
{
    _bovCollection = new BOVCollection();
    int rc = _bovCollection->Initialize(paths);
    if (rc < 0) {
        SetErrMsg("Failure reading .bov file");
        return (-1);
    }

    //
    //  Get the dimensions of the grid.
    //	Initializes members: _dimsMap
    //
    rc = _InitDimensions();
    if (rc < 0) {
        SetErrMsg("No valid dimensions");
        return (-1);
    }

    // Set up the coordinate variables
    //
    rc = _InitCoordinates();
    if (rc < 0) { return (-1); }

    // Identify data and coordinate variables. Sets up members:
    // Initializes members: _dataVarsMap, _meshMap
    //
    rc = _InitVars();
    if (rc < 0) return (-1);

    return (0);
}

int DCBOV::_InitDimensions()
{
    _dimsMap.clear();
    std::vector<std::string> dimnames = _bovCollection->GetSpatialDimensions();
    std::vector<size_t>      dimlens = _bovCollection->GetDataSize();
    VAssert(dimnames.size() == 3);
    VAssert(dimnames.size() == dimlens.size());

    for (int i = 0; i < dimnames.size(); i++) {
        Dimension dim(dimnames[i], dimlens[i]);
        _dimsMap[dimnames[i]] = dim;
    }

    string    timeDim = _bovCollection->GetTimeDimension();
    size_t    numTimes = _bovCollection->GetUserTimes().size();
    Dimension dim(timeDim, numTimes);
    _dimsMap[timeDim] = dim;

    return 0;
}

int DCBOV::_InitCoordinates()
{
    bool         uniformHint = true;
    vector<bool>             periodic(false, 4);
    std::string  units = "m";
    std::vector<std::string> dims = _bovCollection->GetSpatialDimensions();

    _coordVarsMap[dims[0]] = CoordVar(dims[0], units, DC::FLOAT, periodic, 0, uniformHint, {dims[0]}, "");
    _coordVarsMap[dims[1]] = CoordVar(dims[1], units, DC::FLOAT, periodic, 1, uniformHint, {dims[1]}, "");
    _coordVarsMap[dims[2]] = CoordVar(dims[2], units, DC::FLOAT, periodic, 2, uniformHint, {dims[2]}, "");

    std::string timeDim = _bovCollection->GetTimeDimension();
    _coordVarsMap[timeDim] = CoordVar(timeDim, "seconds", DC::FLOAT, periodic, 3, true, {timeDim}, timeDim);

    return 0;
}

// Collect metadata for all data variables found in the CF data
// set. Initialize the _dataVarsMap member
//
int DCBOV::_InitVars()
{
    _dataVarsMap.clear();
    _meshMap.clear();

    vector<bool> periodic(3, false);

    std::vector<std::string> dimnames = _bovCollection->GetSpatialDimensions();
    std::vector<std::string> vars = _bovCollection->GetDataVariableNames();
    DC::XType                format = _bovCollection->GetDataFormat();
    std::string              timeCoordVar = _bovCollection->GetTimeDimension();
    string                   units = "m";

    for (auto var : vars) {
        Mesh mesh(var, dimnames, dimnames);

        // Create new mesh. We're being lazy here and probably should only
        // createone if it doesn't ready exist
        //
        _meshMap[mesh.GetName()] = mesh;

        _dataVarsMap[var] = DataVar(var, units, format, periodic, mesh.GetName(), timeCoordVar, DC::Mesh::NODE);
    }

    return (0);
}

bool DCBOV::getDimension(string dimname, DC::Dimension &dimension) const
{
    map<string, DC::Dimension>::const_iterator itr;

    itr = _dimsMap.find(dimname);
    if (itr == _dimsMap.end()) return (false);

    dimension = itr->second;
    return (true);
}

std::vector<string> DCBOV::getDimensionNames() const
{
    map<string, DC::Dimension>::const_iterator itr;

    vector<string> names;

    for (itr = _dimsMap.begin(); itr != _dimsMap.end(); ++itr) { names.push_back(itr->first); }

    return (names);
}

vector<string> DCBOV::getMeshNames() const
{
    vector<string>                         mesh_names;
    std::map<string, Mesh>::const_iterator itr = _meshMap.begin();
    for (; itr != _meshMap.end(); ++itr) { mesh_names.push_back(itr->first); }
    return (mesh_names);
}

bool DCBOV::getMesh(string mesh_name, DC::Mesh &mesh) const
{
    map<string, Mesh>::const_iterator itr = _meshMap.find(mesh_name);
    if (itr == _meshMap.end()) return (false);

    mesh = itr->second;
    return (true);
}

bool DCBOV::getCoordVarInfo(string varname, DC::CoordVar &cvar) const
{
    map<string, DC::CoordVar>::const_iterator itr;

    itr = _coordVarsMap.find(varname);
    if (itr == _coordVarsMap.end()) { return (false); }

    cvar = itr->second;
    return (true);
}

bool DCBOV::getDataVarInfo(string varname, DC::DataVar &datavar) const
{
    map<string, DC::DataVar>::const_iterator itr;

    itr = _dataVarsMap.find(varname);
    if (itr == _dataVarsMap.end()) { return (false); }

    datavar = itr->second;
    return (true);
}

bool DCBOV::getBaseVarInfo(string varname, DC::BaseVar &var) const
{
    map<string, DC::CoordVar>::const_iterator itr;

    itr = _coordVarsMap.find(varname);
    if (itr != _coordVarsMap.end()) {
        var = itr->second;
        return (true);
    }

    map<string, DC::DataVar>::const_iterator itr1 = _dataVarsMap.find(varname);
    if (itr1 != _dataVarsMap.end()) {
        var = itr1->second;
        return (true);
    }

    return (false);
}

std::vector<string> DCBOV::getDataVarNames() const
{
    map<string, DC::DataVar>::const_iterator itr;

    vector<string> names;
    for (itr = _dataVarsMap.begin(); itr != _dataVarsMap.end(); ++itr) { names.push_back(itr->first); }
    return (names);
}

std::vector<string> DCBOV::getCoordVarNames() const
{
    map<string, DC::CoordVar>::const_iterator itr;

    vector<string> names;
    for (itr = _coordVarsMap.begin(); itr != _coordVarsMap.end(); ++itr) { names.push_back(itr->first); }
    return (names);
}

bool DCBOV::getAtt(string varname, string attname, vector<double> &values) const { return false; }

bool DCBOV::getAtt(string varname, string attname, vector<long> &values) const { return false; }

bool DCBOV::getAtt(string varname, string attname, string &values) const { return false; }

std::vector<string> DCBOV::getAttNames(string varname) const { return {}; }

DC::XType DCBOV::getAttType(string varname, string attname) const { return INVALID; }

int DCBOV::getDimLensAtLevel(string varname, int, std::vector<size_t> &dims_at_level, std::vector<size_t> &bs_at_level) const
{
    dims_at_level.clear();
    bs_at_level.clear();

    bool ok = GetVarDimLens(varname, true, dims_at_level);
    if (!ok) {
        SetErrMsg("Undefined variable name : %s", varname.c_str());
        return (-1);
    }

    // Never blocked
    //
    bs_at_level = vector<size_t>(dims_at_level.size(), 1);

    return (0);
}

int DCBOV::openVariableRead(size_t ts, string varname)
{
    FileTable::FileObject *f = new FileTable::FileObject(ts, varname, 0, 0, 0);
    return (_fileTable.AddEntry(f));
}

int DCBOV::closeVariable(int fd)
{
    DC::FileTable::FileObject *w = _fileTable.GetEntry(fd);

    if (!w) {
        SetErrMsg("Invalid file descriptor : %d", fd);
        return (-1);
    }

    _fileTable.RemoveEntry(fd);

    return 0;
}

template<class T> int DCBOV::_readRegionTemplate(int fd, const vector<size_t> &min, const vector<size_t> &max, T *region)
{
    FileTable::FileObject *w = (FileTable::FileObject *)_fileTable.GetEntry(fd);
    if (!w) {
        SetErrMsg("Invalid file descriptor : %d", fd);
        return (-1);
    }

    std::string varname = w->GetVarname();
    size_t      ts = w->GetTS();

    std::vector<std::string> spatialDims = _bovCollection->GetSpatialDimensions();
    std::vector<size_t>      dataSize = _bovCollection->GetDataSize();
    std::vector<double>      origin = _bovCollection->GetBrickOrigin();
    std::vector<double>      brickSize = _bovCollection->GetBrickSize();

    // Return spatial coordinate variable values
    for (int dim = 0; dim < spatialDims.size(); dim++) {
        if (varname == spatialDims[dim]) {
            double increment = brickSize[dim] / (dataSize[dim] - 1);
            for (int i = 0; i < dataSize[dim]; i++) { region[i] = origin[dim] + i * increment; }
        }
    }

    // If a data variable is requested, read it with ReadRegion
    std::vector<std::string> varnames = _bovCollection->GetDataVariableNames();
    if (std::find(varnames.begin(), varnames.end(), varname) != varnames.end()) {
        int rc = _bovCollection->ReadRegion(varname, ts, min, max, region);
        if (rc < 0) {
            SetErrMsg("DCBOV::_readRegionTemplate error");
            return -1;
        }
    }
    // Otherwise return time values
    else if (varname == _bovCollection->GetTimeDimension()) {
        std::vector<float> times = _bovCollection->GetUserTimes();
        for (int i = 0; i < times.size(); i++) region[i] = (float)times[i];
    }
    return 0;
}

bool DCBOV::variableExists(size_t ts, string varname, int, int) const
{
    if (_coordVarsMap.find(varname) != _coordVarsMap.end()) return true;
    if (_dataVarsMap.find(varname) != _dataVarsMap.end()) return true;
    return false;
}
