// ****************************************************************************
//  material.cpp                                                    Tao project
// ****************************************************************************
//
//   File Description:
//
//   Basic material implementation.
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
#include "material.h"

const Tao::ModuleApi *Material::tao = NULL;

bool Material::tested = false;
bool Material::licensed = false;


DLL_PUBLIC Tao::GraphicState * graphic_state = NULL;

// ============================================================================
//
//    Material
//
// ============================================================================

Material::Material(const QGLContext **pcontext)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : pcontext(pcontext)
{
}

Material::~Material()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void Material::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Material *)arg)->Draw();
}


void Material::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void Material::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (Material *)arg;
}


void Material::Draw()
// ----------------------------------------------------------------------------
//   Draw material
// ----------------------------------------------------------------------------
{
}


void Material::checkGLContext()
// ----------------------------------------------------------------------------
//   Re-create context-dependent resources if GL context has changed
// ----------------------------------------------------------------------------
{
    tao->makeGLContextCurrent();
    if (*pcontext != QGLContext::currentContext())
    {
        IFTRACE(materials)
                debug() << "Context has changed" << "\n";

        *pcontext = QGLContext::currentContext();
        createShaders();
    }
}


void Material::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs for the material
// ----------------------------------------------------------------------------
{
}

std::ostream & Material::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[Materials] " << (void*)this << " ";
    return std::cerr;
}

