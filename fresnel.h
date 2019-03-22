#ifndef FRESNEL_H
#define FRESNEL_H
// *****************************************************************************
// fresnel.h                                                       Tao3D project
// *****************************************************************************
//
// File description:
//
//      Define fresnel material.
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2012, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011,2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************

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
