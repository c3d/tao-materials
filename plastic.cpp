// ****************************************************************************
//  plastic.cpp                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//   Plastic material implementation.
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
#include "plastic.h"

// ============================================================================
//
//   Plastic Material
//
// ============================================================================

bool                  Plastic::failed = false;
QGLShaderProgram*     Plastic::pgm = NULL;
std::map<text, GLint> Plastic::uniforms;

Plastic::Plastic()
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
{
    if(!pgm && !failed)
    {
        pgm = new QGLShaderProgram();
        bool ok = false;

        static string vSrc;
        static string fSrc;
        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {            
            // If the extension is available, use this vertex shader
            // to handle multiple lights
            vSrc =
                "varying float ratio;"
                "uniform vec3 camera;"
                "uniform mat4 modelMatrix;"

                "varying vec3 viewDir;"
                "varying vec3 normal;"
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
                "}";

            // If the extension is available, use this fragment shader
            // to handle multiple lights
            fSrc =
                "#extension GL_EXT_gpu_shader4 : require\n"

                "uniform int  lights;"
                "uniform vec3 color;"

                "varying float ratio;"
                "varying vec3  viewDir;"
                "varying vec3  normal;"

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
                "       lighting_color  = (ambient + diffuse) * renderColor + specular;"
                "   }"
                "   else"
                "   {"
                "       /* Define new render color */"
                "       lighting_color = renderColor;"
                "   }"

                "   return lighting_color;"
                "}"

                "void main()"
                "{"
                "   vec4 renderColor = vec4(ratio, ratio, ratio, 1.0) * vec4(color, 1.0);"
                "   gl_FragColor = computeRenderColor(renderColor);"
                "}";
        }
        else
        {
            // If the extension is not available, use this vertex shader
            // to handle an unique light.
            vSrc =
                "varying float ratio;"

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
                "}";

            // If the extension is not available, use this fragment shader
            // to handle an unique light.
            fSrc =
               "varying float ratio;"
               "uniform vec3 color;"
               "void main()"
               "{"
               "    gl_FragColor = vec4(ratio * color, 1.0);"
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


        pgm->link();

        // Save uniform locations
        uint id = pgm->programId();

        uniforms["color"] = glGetUniformLocation(id, "color");
        uniforms["lights"] = glGetUniformLocation(id, "lights");
        uniforms["camera"] = glGetUniformLocation(id, "camera");
        uniforms["modelMatrix"] = glGetUniformLocation(id, "modelMatrix");
    }

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


void Plastic::setColor(GLfloat r, GLfloat g, GLfloat b)
// ----------------------------------------------------------------------------
//   Set plastic color
// ----------------------------------------------------------------------------
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
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
    if (!tested)
    {
        licensed = tao->checkLicense("Materials 1.0", false);
        tested = true;
    }
    if (!licensed && !tao->blink(1.0, 0.2))
        return;

    uint prg_id = pgm->programId();
    if(prg_id)
    {
        // Set shader
        tao->SetShader(prg_id);

        // Set uniform values
        glUniform3fv(uniforms["color"], 1, color);
        glUniformMatrix4fv(uniforms["modelMatrix"], 1, 0, &model[0][0]);

        // Get and set camera position
        Vector3 cam;
        tao->getCamera(&cam, NULL, NULL);
        GLfloat camera[3] = {cam.x, cam.y, cam.z};
        glUniform3fv(uniforms["camera"], 1, camera);


        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask = tao->EnabledLights();
            glUniform1i(uniforms["lights"], lightsmask);
        }
    }
}

