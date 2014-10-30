#ifndef MARBLE_H
#define MARBLE_H
// ****************************************************************************
//  marble.h                                                        Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define marble material.
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


struct Marble : public Material
{
    Marble(uint unit, float scale);
    ~Marble();

    void setFirstColor(GLfloat color[3]);
    void setSecondColor(GLfloat color[3]);

    // Draw marble material
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

    static bool failed;
    static QGLShaderProgram* pgm;
    static std::map<text, GLint> uniforms;
    static const QGLContext* context;
};


#endif
