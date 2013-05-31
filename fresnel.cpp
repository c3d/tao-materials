// ****************************************************************************
//  fresnel.cpp                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//   Fresnel material implementation.
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
#include "fresnel.h"

#define GL (*graphic_state)

// ============================================================================
//
//   Fresnel Material
//
// ============================================================================

bool                  Fresnel::failed = false;
QGLShaderProgram*     Fresnel::pgm = NULL;
std::map<text, GLint> Fresnel::uniforms;
const QGLContext*     Fresnel::context = NULL;

Fresnel::Fresnel(uint unit, float IoR, float ratio, float roughness)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : Material(&context),
      unit(unit), IoR(IoR), ratio(ratio), roughness(roughness)
{
    IFTRACE(materials)
            debug() << "Create fresnel material" << "\n";

    checkGLContext();

    // Get model matrix
    Matrix4 m = tao->ModelMatrix();
    std::copy(m.Data(), m.Data() + 16, model[0]);
}


Fresnel::~Fresnel()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void Fresnel::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Fresnel *)arg)->Draw();
}


void Fresnel::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void Fresnel::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (Fresnel *)arg;
}


void Fresnel::Draw()
// ----------------------------------------------------------------------------
//   Apply fresnel material
// ----------------------------------------------------------------------------
{
    if (!tested)
    {
        licensed = tao->checkImpressOrLicense("Materials 1.005");
        tested = true;
    }

    checkGLContext();

    uint prg_id = 0;
    if(pgm)
        prg_id = pgm->programId();

    if(prg_id)
    {
        IFTRACE(materials)
                debug() << "Apply fresnel material" << "\n";

        // Set shader
        tao->SetShader(prg_id);

        // Activate pixel blur
        GL.HasPixelBlur(true);

        // Set uniform values
        GL.Uniform(uniforms["environmentMap"], unit);
        GL.Uniform(uniforms["IoR"], IoR);
        GL.Uniform(uniforms["ratio"], ratio);
        GL.Uniform(uniforms["roughness"], roughness);
        GL.UniformMatrix4fv(uniforms["modelMatrix"], 1, 0, &model[0][0]);

        // Get and set camera position
        Vector3 cam;
        tao->getCamera(&cam, NULL, NULL, NULL);
        GLfloat camera[3] = { (float) cam.x, (float) cam.y, (float) cam.z};
        GL.Uniform3fv(uniforms["camera"], 1, camera);

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask = tao->EnabledLights();
            GL.Uniform(uniforms["lights"], lightsmask);
        }
    }
}


void Fresnel::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        delete pgm;

        IFTRACE(materials)
                debug() << "Create fresnel shader" << "\n";

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
                "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "uniform vec3 camera;"
                "uniform mat4 modelMatrix;"

                "varying vec3 normalWorld;"
                "varying vec3 viewWorld;"

                "varying vec3 normal;"
                "varying vec3 viewDir;"

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

                "   /* World-space */"
                "   vec3 eyePos = vec3(gl_TextureMatrix[0] * modelMatrix * gl_Vertex);"
                "   normalWorld = normalize(normalMatrix * gl_Normal);"
                "   viewWorld   = normalize(eyePos - camera);"

                "   /* Eye-space */"
                "   eyePos  = (gl_ModelViewMatrix * gl_Vertex).xyz;"
                "   normal  = normalize(gl_NormalMatrix * gl_Normal);"
                "   viewDir = normalize(-eyePos);"
                "}";

        static string fSrc;
        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            // If the extension is available, use this shader
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

                    "/* Material parameters */"
                    "uniform float IoR;" // Index of refraction
                    "uniform float ratio;"
                    "uniform float roughness;"
                    "uniform samplerCube environmentMap;"

                    "uniform int  lights;"

                    "varying vec3 normalWorld;"
                    "varying vec3 viewWorld;"

                    "varying vec3 normal;"
                    "varying vec3 viewDir;"

                    "const float roughness_ratio = 50.0;"

                    "/**"
                    "* Compute render color according to materials,"
                    "* lights and colors parameters which are"
                    "* set in the current scene (Lambertian model)."
                    "**/"
                    "vec4 computeRenderColor()"
                    "{"
                    "   /* Define a maximum of lights supported */"
                    "   int MAX_LIGHTS = 8;"

                    "   vec4 lighting_color;"
                    "   vec3 N = normalize(normal);"
                    "   vec3 V = normalize(viewDir);"

                    "   vec4 ambient  = vec4(0.0);"
                    "   vec4 diffuse  = vec4(0.0);"
                    "   vec4 specular = vec4(0.0);"

                    "   if(lights > 0)"
                    "   {"
                    "       ambient = gl_FrontLightModelProduct.sceneColor"
                    "               * gl_FrontMaterial.ambient;"

                    "       for(int i = 0; i < MAX_LIGHTS; i++)"
                    "       {"
                    "           if(bool(lights & (1 << i)))"
                    "           {"
                    "               vec3 L = normalize(gl_LightSource[i].position.xyz);"

                    "               /* Compute ambient part */"
                    "               ambient += gl_LightSource[i].ambient"
                    "                        * gl_FrontMaterial.ambient;"

                    "               /* Diffuse coefficient */"
                    "               float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
                    "               if (nDotL > 0.0)"
                    "               {"
                    "                   /* Compute diffuse part */"
                    "                   diffuse += gl_LightSource[i].diffuse"
                    "                            * gl_FrontMaterial.diffuse"
                    "                            * nDotL;"

                    "                   /* Compute specular coefficient */"
                    "                   float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
                    "                   if (nDotV > 0.0)"
                    "                   {"
                    "                       /* Compute specular part */"
                    "                       specular += gl_LightSource[i].specular"
                    "                                 * gl_FrontMaterial.specular"
                    "                                 * pow(nDotV, gl_FrontMaterial.shininess);"
                    "                   }"
                    "               }"
                    "           }"
                    "       }"

                    "       /* Define new render color */"
                    "       lighting_color  = ambient + diffuse + specular;"
                    "   }"
                    "   else"
                    "   {"
                    "       /* We define a default light in case there are no one defined. */"
                    "       vec3 L = normalize(vec3(0.0, 0.0, 50.0));"

                    "       /* Compute ambient part */"
                    "       ambient = gl_FrontMaterial.ambient;"

                    "       /* Diffuse coefficient */"
                    "       float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
                    "       if (nDotL > 0.0)"
                    "       {"
                    "           /* Compute diffuse part */"
                    "           diffuse = gl_FrontMaterial.diffuse * nDotL;"

                    "           /* Compute specular coefficient */"
                    "           float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
                    "           if (nDotV > 0.0)"
                    "           {"
                    "               /* Compute specular part */"
                    "               specular = gl_FrontMaterial.specular"
                    "                        * pow(nDotV, gl_FrontMaterial.shininess);"
                    "           }"
                    "       }"

                    "       /* Define new render color */"
                    "       lighting_color = ambient + diffuse + specular;"
                    "   }"

                    "   return lighting_color;"
                    "}"


                    "void main()"
                    "{"
                    "   /* Harmonization of the index of refraction */"
                    "   float Eta = 0.086 * IoR + 0.93;"

                    "   vec3 reflectDir, refractDir;"
                    "   reflectDir = normalize(reflect(viewWorld, normalWorld));"
                    "   refractDir = normalize(refract(viewWorld, normalWorld, Eta));"

                    "   vec3 reflectColor, refractColor;"
                    "   reflectColor = textureCube(environmentMap, reflectDir).xyz;"
                    "   refractColor = textureCube(environmentMap, refractDir).xyz;"

                    "   /* Compute fresnel ratio */"
                    "   float fresnel = 0.1 + 2.0 * pow(1.0 + (dot(viewWorld, normalWorld)),"
                    "                   1.0 / (roughness * roughness_ratio));"

                    "   /* Compute final color */"
                    "   vec3 fresnelColor = mix(refractColor, reflectColor, clamp(fresnel, 0.0, 1.0));"
                    "   vec3 renderColor  = computeRenderColor().rgb;"
                    "   vec3 finalColor   = mix(fresnelColor, renderColor, ratio);"

                    "   gl_FragColor  = vec4(finalColor, 1.0);"
                    "}";
        }
        else
        {
            // If the extension is not available, use this shader
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
                    "/* Material parameters */"
                    "uniform float IoR;" // Index of refraction
                    "uniform float ratio;"
                    "uniform float roughness;"
                    "uniform samplerCube environmentMap;"

                    "varying vec3 normalWorld;"
                    "varying vec3 viewWorld;"

                    "varying vec3 normal;"
                    "varying vec3 viewDir;"

                    "const float roughness_ratio = 50.0;"

                    "/**"
                    "* Compute render color according to an unique"
                    "* light and material."
                    "**/"
                    "vec4 computeRenderColor()"
                    "{"
                    "   vec4 lighting_color;"
                    "   vec3 N = normalize(normal);"
                    "   vec3 V = normalize(viewDir);"

                    "   vec4 ambient  = vec4(0.0);"
                    "   vec4 diffuse  = vec4(0.0);"
                    "   vec4 specular = vec4(0.0);"

                    "   /* We define a default light in case there are no one defined. */"
                    "   vec3 L = normalize(vec3(0.0, 0.0, 50.0));"

                    "   /* Compute ambient part */"
                    "   ambient = gl_FrontMaterial.ambient;"

                    "   /* Diffuse coefficient */"
                    "   float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
                    "   if (nDotL > 0.0)"
                    "   {"
                    "       /* Compute diffuse part */"
                    "       diffuse = gl_FrontMaterial.diffuse * nDotL;"

                    "       /* Compute specular coefficient */"
                    "       float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
                    "       if (nDotV > 0.0)"
                    "       {"
                    "           /* Compute specular part */"
                    "           specular = gl_FrontMaterial.specular"
                    "                    * pow(nDotV, gl_FrontMaterial.shininess);"
                    "       }"
                    "   }"

                    "   lighting_color = ambient + diffuse + specular;"

                    "   return lighting_color;"
                    "}"


                    "void main()"
                    "{"
                    "   /* Harmonization of the index of refraction */"
                    "   float Eta = 0.086 * IoR + 0.93;"

                    "   vec3 reflectDir, refractDir;"
                    "   reflectDir = normalize(reflect(viewWorld, normalWorld));"
                    "   refractDir = normalize(refract(viewWorld, normalWorld, Eta));"

                    "   vec3 reflectColor, refractColor;"
                    "   reflectColor = textureCube(environmentMap, reflectDir).xyz;"
                    "   refractColor = textureCube(environmentMap, refractDir).xyz;"

                    "   /* Compute fresnel ratio */"
                    "   float fresnel = 0.1 + 2.0 * pow(1.0 + (dot(viewWorld, normalWorld)),"
                    "                   1.0 / (roughness * roughness_ratio));"

                    "   /* Compute final color */"
                    "   vec3 fresnelColor = mix(refractColor, reflectColor, clamp(fresnel, 0.0, 1.0));"
                    "   vec3 renderColor  = computeRenderColor().rgb;"
                    "   vec3 finalColor   = mix(fresnelColor, renderColor, ratio);"

                    "   gl_FragColor  = vec4(finalColor, 1.0);"
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

            uniforms["IoR"] = glGetUniformLocation(id, "IoR");
            uniforms["ratio"] = glGetUniformLocation(id, "ratio");
            uniforms["lights"] = glGetUniformLocation(id, "lights");
            uniforms["roughness"] = glGetUniformLocation(id, "roughness");
            uniforms["environmentMap"] = glGetUniformLocation(id, "environmentMap");
            uniforms["camera"] = glGetUniformLocation(id, "camera");
            uniforms["modelMatrix"] = glGetUniformLocation(id, "modelMatrix");
        }
    }
}
