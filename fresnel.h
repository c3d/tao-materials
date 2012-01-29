#ifndef FRESNEL_H
#define FRESNEL_H
// ****************************************************************************
//  fresnel.h                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define fresnel material.
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriÃ©tÃ© de Taodyne SAS - Confidentiel
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************

#include "tao/coords3d.h"
#include "tao/matrix.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include "material.h"
#include <QObject>

using namespace std;
using namespace Tao;


struct Fresnel : public Material
{
    Fresnel(uint unit, float IoR, float ratio, float roughness);
    ~Fresnel();

    // Draw fresnel material
    virtual void    Draw();

    static void     render_callback(void *arg);
    static void     identify_callback(void *arg);
    static void     delete_callback(void *arg);

protected:
    virtual void    createShaders();

private:
    uint    unit;        // Unit of the noise map
    float   IoR;         // Indice of refraction
    float   ratio;       // ratio color
    float   roughness;   // roughness of the material
    GLfloat model[4][4]; // model matrix

    static bool failed;
    static QGLShaderProgram* pgm;
    static std::map<text, GLint> uniforms;
    static const QGLContext* context;
};


#endif
