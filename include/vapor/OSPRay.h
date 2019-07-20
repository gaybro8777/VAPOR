#pragma once

#include <ospray/ospray.h>
#include <vapor/common.h>
#include <glm/fwd.hpp>

extern RENDER_API OSPError    OSPInitStatus;
extern RENDER_API const char *OSPInitStatusMessage;

bool RENDER_API       OSPRayInitialized();
OSPTexture RENDER_API OSPRayGetDepthTextureFromOpenGLPerspective(const double &fovy, const double &aspect, const double &zNear, const double &zFar, const glm::vec3 &cameraDir,
                                                                 const glm::vec3 &cameraUp, const float *glDepthBuffer, const int &width, const int &height);
