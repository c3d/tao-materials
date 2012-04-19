// ****************************************************************************
//  wood.cpp                                                        Tao project
// ****************************************************************************
//
//   File Description:
//
//   Wood material implementation.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************
#include "wood.h"

// ============================================================================
//
//   Wood Material
//
// ============================================================================

bool                  Wood::failed = false;
QGLShaderProgram*     Wood::pgm = NULL;
std::map<text, GLint> Wood::uniforms;
const QGLContext*     Wood::context = NULL;

Wood::Wood(uint unit, float scale, float ring, float noise)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : Material(&context), unit(unit), scale(scale), ring(ring), noise(noise)
{
    checkGLContext();
}


Wood::~Wood()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void Wood::setFirstColor(GLfloat color[3])
// ----------------------------------------------------------------------------
//   Set first wood color
// ----------------------------------------------------------------------------
{
    first_color[0] = color[0];
    first_color[1] = color[1];
    first_color[2] = color[2];
}


void Wood::setSecondColor(GLfloat color[3])
// ----------------------------------------------------------------------------
//   Set second wood color
// ----------------------------------------------------------------------------
{
    second_color[0] = color[0];
    second_color[1] = color[1];
    second_color[2] = color[2];
}


void Wood::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Wood *)arg)->Draw();
}


void Wood::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void Wood::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (Wood *)arg;
}


void Wood::Draw()
// ----------------------------------------------------------------------------
//   Apply wood material
// ----------------------------------------------------------------------------
{
    if (!tested)
    {
        licensed = tao->checkLicense("Materials 1.0", false);
        tested = true;
    }
    if (!licensed && !tao->blink(1.0, 0.2, 300.0))
        return;

    checkGLContext();

    uint prg_id = 0;
    if(pgm)
        prg_id = pgm->programId();

    if(prg_id)
    {
        // Set shader
        tao->SetShader(prg_id);

        // Set uniform values
        glUniform1i(uniforms["noiseMap"], unit);

        glUniform1f(uniforms["scale"], scale);
        glUniform1f(uniforms["ringSize"], ring);
        glUniform1f(uniforms["noiseRatio"], noise);

        glUniform3fv(uniforms["first_color"], 1, first_color);
        glUniform3fv(uniforms["second_color"], 1, second_color);

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask = tao->EnabledLights();
            glUniform1i(uniforms["lights"], lightsmask);
        }
    }
}


void Wood::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        pgm = new QGLShaderProgram();
        bool ok = false;

        // Basic vertex shader
        static string vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "varying vec3 viewDir;"
                "varying vec3 normal;"

                "uniform float scale;"
                "void main()"
                "{"
                "   /* Compute position */"
                "   gl_Position = ftransform();"

                "   /* Compute texture coordinates */"
                "   gl_TexCoord[0] = (scale * gl_Vertex) / 100.0;"

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
               "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
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
               "uniform float     noiseRatio;"
               "uniform float     ringSize;"

               "uniform int       lights;"

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
               "   vec3 texCoord = 0.5 * gl_TexCoord[0].xyz;"
               "   float noise   = texture3D(noiseMap, texCoord).x - 1.0;"

               "   /* Define some rings in the wood */"
               "   float rings = fract(ringSize * gl_TexCoord[0].x + noiseRatio * noise);"
               "   float ratio = (rings - pow(rings, 20.0));"

               "   vec3 mainColor = mix(first_color, second_color, ratio);"
               "   gl_FragColor   = computeRenderColor(vec4(mainColor, 1.0));"
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
               "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
               "** It can be only used in the software and these modules.                        \n"
               "**                                                                               \n"
               "** If you have questions regarding the use of this file, please contact          \n"
               "** Taodyne at contact@taodyne.com.                                               \n"
               "**                                                                               \n"
               "********************************************************************************/\n"
               "uniform vec3      first_color;"
               "uniform vec3      second_color;"
               "uniform sampler3D noiseMap;"
               "uniform float     noiseRatio;"
               "uniform float     ringSize;"

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
               "   vec3 texCoord = 0.5 * gl_TexCoord[0].xyz;"
               "   float noise   = texture3D(noiseMap, texCoord).x - 1.0;"

               "   /* Define some rings in the wood */"
               "   float rings = fract(ringSize * gl_TexCoord[0].x + noiseRatio * noise);"
               "   float ratio = (rings - pow(rings, 20.0));"

               "   vec3 mainColor = mix(first_color, second_color, ratio);"
               "   gl_FragColor   = computeRenderColor(vec4(mainColor, 1.0));"
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
                std::cerr << "Error loading fragment shader code : " << "\n";
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

            uniforms["scale"] = glGetUniformLocation(id, "scale");
            uniforms["lights"] = glGetUniformLocation(id, "lights");
            uniforms["ringSize"] = glGetUniformLocation(id, "ringSize");
            uniforms["noiseMap"] = glGetUniformLocation(id, "noiseMap");
            uniforms["noiseRatio"] = glGetUniformLocation(id, "noiseRatio");
            uniforms["first_color"] = glGetUniformLocation(id, "first_color");
            uniforms["second_color"] = glGetUniformLocation(id, "second_color");
        }
    }
}
