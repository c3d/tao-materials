#ifndef GRANITE_H
#define GRANITE_H
// ****************************************************************************
//  granite.h                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define granite material.
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
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


struct Granite : public Material
{
    Granite(uint unit, float scale);
    ~Granite();

    void setFirstColor(GLfloat color[3]);
    void setSecondColor(GLfloat color[3]);
    void setThirdColor(GLfloat color[3]);

    // Draw granite material
    virtual void    Draw();

    static void     render_callback(void *arg);
    static void     identify_callback(void *arg);
    static void     delete_callback(void *arg);

protected:
    virtual void    createShaders();

private:
    uint    unit;            // Unit of the noise map
    float   scale;           // Size of material
    GLfloat first_color[3];  // First color of the material
    GLfloat second_color[3]; // Second color of the material
    GLfloat third_color[3];  // Third color of the material

    static bool failed;
    static QGLShaderProgram* pgm;
    static std::map<text, GLint> uniforms;
    static const QGLContext* context;
};


#endif
