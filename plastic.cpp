// *****************************************************************************
// plastic.cpp                                                     Tao3D project
// *****************************************************************************
//
// File description:
//
//   Plastic material implementation.
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011-2013, Jérôme Forissier <jerome@taodyne.com>
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
#include "plastic.h"


// ============================================================================
//
//   Plastic Material
//
// ============================================================================

bool                  Plastic::failed = false;
QGLShaderProgram*     Plastic::pgm = NULL;
std::map<text, GLint> Plastic::uniforms;
const QGLContext*     Plastic::context = NULL;

Plastic::Plastic()
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : Material(&context)
{
    IFTRACE(materials)
            debug() << "Create plastic material" << "\n";

    checkGLContext();

    // Get model matrix
    Matrix4 m = tao->ModelMatrix();
    std::copy(m.Data(), m.Data() + 16, model[0]);
}


Plastic::~Plastic()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}

void Plastic::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Plastic *)arg)->Draw();
}


void Plastic::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void Plastic::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (Plastic *)arg;
}


void Plastic::Draw()
// ----------------------------------------------------------------------------
//   Apply plastic material
// ----------------------------------------------------------------------------
{
    checkGLContext();

    uint prg_id = 0;
    if(pgm)
        prg_id = pgm->programId();

    if(prg_id)
    {
        IFTRACE(materials)
                debug() << "Apply plastic material" << "\n";

        // Set shader
        tao->SetShader(prg_id);

        // Set uniform values
        GL.UniformMatrix4fv(uniforms["modelMatrix"], 1, 0, &model[0][0]);

        // Get and set camera position
        Vector3 cam;
        tao->getCamera(&cam, NULL, NULL, NULL);
        GLfloat camera[3] = { (float) cam.x, (float) cam.y, (float) cam.z};
        GL.Uniform3fv(uniforms["camera"], 1, camera);

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask =  GL.LightsMask();
            GL.Uniform(uniforms["lights"], lightsmask);
        }
    }
}


void Plastic::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        delete pgm;

        IFTRACE(materials)
                debug() << "Create plastic shader" << "\n";

        pgm = new QGLShaderProgram(*pcontext);
        bool ok = false;

        static string vSrc;
        static string fSrc;
        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            // If the extension is available, use this vertex shader
            // to handle multiple lights
            vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao3D application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "varying float ratio;"
                "uniform vec3 camera;"
                "uniform mat4 modelMatrix;"

                "varying vec3 viewDir;"
                "varying vec3 normal;"
                "varying vec4 color;"
                "void main()"
                "{"
                "   mat3 normalMatrix;"

                "   /* First column */"
                "   normalMatrix[0][0] = modelMatrix[0][0];"
                "   normalMatrix[0][1] = modelMatrix[0][1];"
                "   normalMatrix[0][2] = modelMatrix[0][2];"

                "   /* Second column */"
                "   normalMatrix[1][0] = modelMatrix[1][0];"
                "   normalMatrix[1][1] = modelMatrix[1][1];"
                "   normalMatrix[1][2] = modelMatrix[1][2];"

                "   /* Third column */"
                "   normalMatrix[2][0] = modelMatrix[2][0];"
                "   normalMatrix[2][1] = modelMatrix[2][1];"
                "   normalMatrix[2][2] = modelMatrix[2][2];"

                "   gl_Position = ftransform();"

                "   /* World-space lighting */"
                "   normal   =  normalize(normalMatrix * gl_Normal);"
                "   viewDir  = -normalize((modelMatrix * gl_Vertex).xyz);"

                "   vec3 eyePos = normalize(camera + viewDir);"
                "   ratio = 0.5 * (1.0 + dot(eyePos, normal));"

                "   normal   =  normalize(gl_NormalMatrix * gl_Normal);"
                "   viewDir  = -normalize((gl_ModelViewMatrix * gl_Vertex).xyz);"

                "   color = gl_Color;"
                "}";

            // If the extension is available, use this fragment shader
            // to handle multiple lights
            fSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao3D application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "#extension GL_EXT_gpu_shader4 : require\n"

                "uniform int  lights;"

                "varying float ratio;"
                "varying vec3  viewDir;"
                "varying vec3  normal;"

                "varying vec4  color;"

                "/**"
                "* Compute render color according to materials,"
                "* lights and colors parameters which are"
                "* set in the current scene."
                "**/"
                "vec4 computeRenderColor(vec4 renderColor)"
                "{"
                "   /* Define a maximum of lights supported */"
                "   int MAX_LIGHTS = 8;"

                "   vec4 lighting_color;"
                "   if(lights > 0)"
                "   {"
                "       vec3 N = normalize(normal);"
                "       vec3 V = normalize(viewDir);"

                "       vec4 ambient  = vec4(0.0);"
                "       vec4 diffuse  = vec4(0.0);"
                "       vec4 specular = vec4(0.0);"
                "       ambient  = gl_FrontLightModelProduct.sceneColor;"

                "       for(int i = 0; i < MAX_LIGHTS; i++)"
                "       {"
                "           if(bool(lights & (1 << i)))"
                "           {"
                "               vec3 L = normalize(gl_LightSource[i].position.xyz);"

                "               /* Compute ambient part */"
                "               ambient += gl_LightSource[i].ambient;"

                "               /* Diffuse coefficient */"
                "               float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
                "               if (nDotL > 0.0)"
                "               {"
                "                   /* Compute diffuse part */"
                "                   diffuse += gl_LightSource[i].diffuse * nDotL;"

                "                   /* Compute specular coefficient */"
                "                   float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
                "                   if (nDotV > 0.0)"
                "                   {"
                "                       /* Compute specular part */"
                "                       specular += gl_LightSource[i].specular * pow(nDotV, 76.8);"
                "                   }"
                "               }"
                "           }"
                "       }"

                "       /* Define new render color */"
                "       lighting_color  = (ambient + diffuse) * renderColor * color + specular;"
                "   }"
                "   else"
                "   {"
                "       /* Define new render color */"
                "       lighting_color = renderColor * color;"
                "   }"

                "   return lighting_color;"
                "}"

                "void main()"
                "{"
                "   vec4 renderColor = vec4(ratio, ratio, ratio, 1.0);"
                "   gl_FragColor = computeRenderColor(renderColor);"
                "}";
        }
        else
        {
            // If the extension is not available, use this vertex shader
            // to handle an unique light.
            vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao3D application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "varying float ratio;"
                "varying vec4  color;"

                "uniform vec3 camera;"
                "uniform mat4 modelMatrix;"

                "void main()"
                "{"
                "   mat3 normalMatrix;"

                "   /* First column */"
                "   normalMatrix[0][0] = modelMatrix[0][0];"
                "   normalMatrix[0][1] = modelMatrix[0][1];"
                "   normalMatrix[0][2] = modelMatrix[0][2];"

                "   /* Second column */"
                "   normalMatrix[1][0] = modelMatrix[1][0];"
                "   normalMatrix[1][1] = modelMatrix[1][1];"
                "   normalMatrix[1][2] = modelMatrix[1][2];"

                "   /* Third column */"
                "   normalMatrix[2][0] = modelMatrix[2][0];"
                "   normalMatrix[2][1] = modelMatrix[2][1];"
                "   normalMatrix[2][2] = modelMatrix[2][2];"

                "   gl_Position = ftransform();"

                "   /* World-space lighting */"
                "   vec3 normal   =  normalize(normalMatrix * gl_Normal);"
                "   vec3 viewDir  = -normalize((modelMatrix * gl_Vertex).xyz);"

                "   vec3 eyePos = normalize(camera + viewDir);"
                "   ratio = 0.5 * (1.0 + dot(eyePos, normal));"
                "   color = gl_Color;"
                "}";

            // If the extension is not available, use this fragment shader
            // to handle an unique light.
            fSrc =
               "/********************************************************************************\n"
               "**                                                                               \n"
               "** Copyright (C) 2011 Taodyne.                                                   \n"
               "** All rights reserved.                                                          \n"
               "** Contact: Taodyne (contact@taodyne.com)                                        \n"
               "**                                                                               \n"
               "** This file is part of the Tao3D application, developped by Taodyne.\n"
               "** It can be only used in the software and these modules.                        \n"
               "**                                                                               \n"
               "** If you have questions regarding the use of this file, please contact          \n"
               "** Taodyne at contact@taodyne.com.                                               \n"
               "**                                                                               \n"
               "********************************************************************************/\n"
               "varying float ratio;"
               "varying vec4  color;"
               "void main()"
               "{"
               "    gl_FragColor = vec4(ratio, ratio, ratio, 1.0);"
               "}";
        }

        if (pgm->addShaderFromSourceCode(QGLShader::Vertex, vSrc.c_str()))
        {
            if (pgm->addShaderFromSourceCode(QGLShader::Fragment, fSrc.c_str()))
            {
                ok = true;
            }
            else
            {
                std::cerr << "Error loading fragment shader code: " << "\n";
                std::cerr << pgm->log().toStdString();
            }
        }
        else
        {
            std::cerr << "Error loading vertex shader code: " << "\n";
            std::cerr << pgm->log().toStdString();
        }
        if (!ok)
        {
            delete pgm;
            pgm = NULL;
            failed = true;
        }
        else
        {
            pgm->link();

            // Save uniform locations
            uint id = pgm->programId();

            uniforms["lights"] = GL.GetUniformLocation(id, "lights");
            uniforms["camera"] = GL.GetUniformLocation(id, "camera");
            uniforms["modelMatrix"] = GL.GetUniformLocation(id, "modelMatrix");
        }
    }
}
