#ifndef WOOD_H
#define WOOD_H
// ****************************************************************************
//  wood.h                                                          Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define wood material.
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


struct Wood : public Material
{
    Wood(uint unit, float scale, float ring, float noise);
    ~Wood();

    void setFirstColor(GLfloat color[3]);
    void setSecondColor(GLfloat color[3]);

    // Draw wood material
    virtual void    Draw();

    static void     render_callback(void *arg);
    static void     identify_callback(void *arg);
    static void     delete_callback(void *arg);

private:
    uint    unit;            // Unit of the noise map
    float   scale;           // Size of material
    float   ring;            // Size of rings in the material
    float   noise;           // Ratio of noise in the material
    GLfloat first_color[3];  // First color of the material
    GLfloat second_color[3]; // Second color of the material

    static bool failed;
    static QGLShaderProgram* pgm;
    static std::map<text, GLint> uniforms;
};


#endif
