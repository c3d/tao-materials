// *****************************************************************************
// marble.cpp                                                      Tao3D project
// *****************************************************************************
//
// File description:
//
//   Marble material implementation.
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
#include "marble.h"


// ============================================================================
//
//   Marble Material
//
// ============================================================================

bool                  Marble::failed = false;
QGLShaderProgram*     Marble::pgm = NULL;
std::map<text, GLint> Marble::uniforms;
const QGLContext*     Marble::context = NULL;

Marble::Marble(uint unit, float scale)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : Material(&context), unit(unit), scale(scale)
{
    IFTRACE(materials)
            debug() << "Create marble material" << "\n";

    checkGLContext();
}


Marble::~Marble()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void Marble::setFirstColor(GLfloat color[3])
// ----------------------------------------------------------------------------
//   Set first marble color
// ----------------------------------------------------------------------------
{
    first_color[0] = color[0];
    first_color[1] = color[1];
    first_color[2] = color[2];
}


void Marble::setSecondColor(GLfloat color[3])
// ----------------------------------------------------------------------------
//   Set second marble color
// ----------------------------------------------------------------------------
{
    second_color[0] = color[0];
    second_color[1] = color[1];
    second_color[2] = color[2];
}


void Marble::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Marble *)arg)->Draw();
}


void Marble::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void Marble::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (Marble *)arg;
}


void Marble::Draw()
// ----------------------------------------------------------------------------
//   Apply marble material
// ----------------------------------------------------------------------------
{
    checkGLContext();

    uint prg_id = 0;
    if(pgm)
        prg_id = pgm->programId();

    if(prg_id)
    {
        IFTRACE(materials)
                debug() << "Apply marble material" << "\n";

        // Set shader
        tao->SetShader(prg_id);

        // Set uniform values
        GL.Uniform(uniforms["noiseMap"], unit);

        GL.Uniform(uniforms["scale"], scale);
        GL.Uniform3fv(uniforms["first_color"], 1, first_color);
        GL.Uniform3fv(uniforms["second_color"], 1, second_color);
        GL.Uniform(uniforms["unit"], (GL.ActiveTextureUnitIndex() - GL_TEXTURE0));

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask =  GL.LightsMask();
            GL.Uniform(uniforms["lights"], lightsmask);
        }
    }
}


void Marble::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        delete pgm;

        IFTRACE(materials)
                debug() << "Create marble shader" << "\n";

        pgm = new QGLShaderProgram(*pcontext);
        bool ok = false;

        // Basic vertex shader
        static string vSrc =
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
                "varying vec3 viewDir;"
                "varying vec3 normal;"
                "uniform int unit;"

                "uniform float scale;"
                "void main()"
                "{"
                "   /* Compute position */"
                "   gl_Position = ftransform();"

                "   /* Compute texture coordinates */"
                "   if(unit == 0)"
                "       gl_TexCoord[0] = (scale * gl_TextureMatrix[0] * gl_MultiTexCoord0);"
                "   else if(unit == 1)"
                "       gl_TexCoord[0] = (scale * gl_TextureMatrix[1] * gl_MultiTexCoord1);"
                "   else if(unit == 2)"
                "       gl_TexCoord[0] = (scale * gl_TextureMatrix[2] * gl_MultiTexCoord2);"
                "   else if(unit == 3)"
                "       gl_TexCoord[0] = (scale * gl_TextureMatrix[3] * gl_MultiTexCoord3);"

                "   /* Compute world position and normal */"
                "   normal  = gl_NormalMatrix * gl_Normal;"
                "   viewDir = -vec3(gl_ModelViewMatrix * gl_Vertex);"
                "}";

        static string fSrc;
        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
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

               "uniform vec3      first_color;"
               "uniform vec3      second_color;"
               "uniform sampler3D noiseMap;"

               "uniform int       lights;"
               "uniform int       unit;"

               "varying vec3 viewDir;"
               "varying vec3 normal;"

               "/**"
               "* Compute render color according to materials,"
               "* lights and colors parameters which are"
               "* set in the current scene."
               "**/"
               "vec4 computeRenderColor(vec4 renderColor)"
               "{"
               "    vec4 lighting_color;"
               "    vec3 N = normalize(normal);"
               "    vec3 V = normalize(viewDir);"

               "    vec4 ambient  = vec4(0.0);"
               "    vec4 diffuse  = vec4(0.0);"
               "    vec4 specular = vec4(0.0);"

               "    if(lights > 0)"
               "    {"
               "        /* Define a maximum of lights supported*/"
               "        int MAX_LIGHTS = 8;"

               "        /* Get current scene color */"
               "        ambient = gl_FrontLightModelProduct.sceneColor;"

               "        /* Compute render for each enabled lights */"
               "        for(int i = 0; i < MAX_LIGHTS; i++)"
               "        {"
               "            if(bool(lights & (1 << i)))"
               "            {"
               "                vec3 L = normalize(gl_LightSource[i].position.xyz);"

               "                /* Compute ambient part */"
               "                ambient += gl_LightSource[i].ambient;"

               "                /* Diffuse coefficient */"
               "                float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
               "                if (nDotL > 0.0)"
               "                {"
               "                    /* Compute diffuse part */"
               "                    diffuse += gl_LightSource[i].diffuse * nDotL;"

               "                    /* Compute specular coefficient */"
               "                    float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
               "                    if (nDotV > 0.0)"
               "                    {"
               "                        /* Compute specular part */"
               "                        specular += gl_LightSource[i].specular * pow(nDotV, 76.8);"
               "                    }"
               "                }"
               "            }"
               "        }"

               "        /* Define new render color */"
               "        lighting_color = (ambient + diffuse) * renderColor + specular;"
               "    }"
               "    else"
               "    {"
               "        /* Get current scene color */"
               "        ambient = gl_FrontLightModelProduct.sceneColor;"

               "        vec3 L = normalize(vec3(0.0, 0.0, 50.0));"

               "        /* Compute ambient part */"
               "        ambient += vec4(0.0, 0.0, 0.0, 1.0);"

               "        /* Diffuse coefficient */"
               "        float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
               "        if (nDotL > 0.0)"
               "        {"
                           // Compute diffuse part
               "            diffuse += 0.5 * vec4(1.0, 1.0, 1.0, 1.0) * nDotL;"

                           // Compute specular coefficient
               "            float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
               "            if (nDotV > 0.0)"
               "            {"
               "                /* Compute specular part */"
               "                specular += 0.4 * vec4(1.0, 1.0, 1.0, 1.0) * pow(nDotV, 76.8);"
               "            }"
               "        }"

               "        /* Define new render color */"
               "        lighting_color = (ambient + diffuse) * renderColor + specular;"

               "    }"

               "    return lighting_color;"
               "}"

               "void main()"
               "{"
               "    float scaling    = 1.0;"
               "    float turbulence = 0.0;"
               "    vec3 texCoord    = vec3(0.0);"

               "    /* Use 5 octaves for a better result */"
               "    for (int i = 0; i < 5; i++)"
               "    {"
               "        texCoord    = (0.15 / scaling) * gl_TexCoord[0].xyz;"
               "        turbulence += scaling * (texture3D(noiseMap, texCoord).z - 0.5);"
               "        scaling    *= 0.4;"
               "    }"

               "    /* Compute ratio according to turbulence */"
               "    float ratio = exp(-5.0 * abs(turbulence));"

               "    /* Compute final color */"
               "    vec3 mainColor = mix(first_color, second_color, ratio);"
               "    gl_FragColor = computeRenderColor(vec4(mainColor, 1.0));"
               "}";
        }
        else
        {
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
               "uniform vec3      first_color;"
               "uniform vec3      second_color;"
               "uniform vec3      third_color;"

               "uniform sampler3D noiseMap;"
               "uniform int       unit;"

               "varying vec3 viewDir;"
               "varying vec3 normal;"
               "/**"
               "* Compute render color according to an unique"
               "* light and material."
               "**/"
               "vec4 computeRenderColor(vec4 renderColor)"
               "{"
               "    vec4 lighting_color;"
               "    vec3 N = normalize(normal);"
               "    vec3 V = normalize(viewDir);"

               "    vec4 ambient  = vec4(0.0);"
               "    vec4 diffuse  = vec4(0.0);"
               "    vec4 specular = vec4(0.0);"

               "    /* Get current scene color */"
               "    ambient = gl_FrontLightModelProduct.sceneColor;"

               "    vec3 L = normalize(vec3(0.0, 0.0, 50.0));"

               "    /* Compute ambient part */"
               "    ambient += vec4(0.0, 0.0, 0.0, 1.0);"

               "    /* Diffuse coefficient */"
               "    float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
               "    if (nDotL > 0.0)"
               "    {"
               "        /* Compute diffuse part */"
               "        diffuse += 0.5 * vec4(1.0, 1.0, 1.0, 1.0) * nDotL;"

               "        /* Compute specular coefficient */"
               "        float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
               "        if (nDotV > 0.0)"
               "        {"
               "            /* Compute specular part */"
               "            specular += 0.4 * vec4(1.0, 1.0, 1.0, 1.0) * pow(nDotV, 76.8);"
               "        }"
               "    }"

               "    /* Define new render color */"
               "    lighting_color = (ambient + diffuse) * renderColor + specular;"

               "    return lighting_color;"
               "}"

               "void main()"
               "{"
               "    float scaling    = 1.0;"
               "    float turbulence = 0.0;"
               "    vec3 texCoord    = vec3(0.0);"

               "    /* Use 5 octaves for a better result */"
               "    for (int i = 0; i < 5; i++)"
               "    {"
               "        texCoord    = (0.15 / scaling) * gl_TexCoord[0].xyz;"
               "        turbulence += scaling * (texture3D(noiseMap, texCoord).z - 0.5);"
               "        scaling    *= 0.4;"
               "    }"

               "    /* Compute ratio according to turbulence */"
               "    float ratio = exp(-5.0 * abs(turbulence));"

               "    /* Compute final color */"
               "    vec3 mainColor = mix(first_color, second_color, ratio);"
               "    gl_FragColor = computeRenderColor(vec4(mainColor, 1.0));"
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

            uniforms["scale"] = GL.GetUniformLocation(id, "scale");
            uniforms["unit"] = GL.GetUniformLocation(id, "unit");
            uniforms["lights"] = GL.GetUniformLocation(id, "lights");
            uniforms["noiseMap"] = GL.GetUniformLocation(id, "noiseMap");
            uniforms["first_color"] = GL.GetUniformLocation(id, "first_color");
            uniforms["second_color"] = GL.GetUniformLocation(id, "second_color");
        }
    }
}
