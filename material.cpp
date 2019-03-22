// *****************************************************************************
// material.cpp                                                    Tao3D project
// *****************************************************************************
//
// File description:
//
//   Basic material implementation.
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
#include "material.h"

const Tao::ModuleApi *Material::tao = NULL;


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

