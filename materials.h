#ifndef MATERIALS_H
#define MATERIALS_H
// *****************************************************************************
// materials.h                                                     Tao3D project
// *****************************************************************************
//
// File description:
//
//    Prototype of the function used by materials.tbl to
//    implement the new XL primitive
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011,2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011,2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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
#include "tree.h"
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "tao/module_api.h"
#include "fresnel.h"
#include "plastic.h"
#include "wood.h"
#include "granite.h"
#include "marble.h"

using namespace XL;

// -------------------------------------------------------------------------------------------------------------------
//   PLASTIC MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p plastic(Tree_p, float);

// -------------------------------------------------------------------------------------------------------------------
//   MARBLE MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p marble_first_color(Tree_p, float r, float g, float b);
Tree_p marble_second_color(Tree_p, float r, float g, float b);
Tree_p marble(Tree_p, uint unit, float scale);

// -------------------------------------------------------------------------------------------------------------------
//   GRANITE MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p granite_first_color(Tree_p, float r, float g, float b);
Tree_p granite_second_color(Tree_p, float r, float g, float b);
Tree_p granite_third_color(Tree_p, float r, float g, float b);
Tree_p granite(Tree_p, uint unit, float scale);

// -------------------------------------------------------------------------------------------------------------------
//   WOOD MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p wood_first_color(Tree_p, float r, float g, float b);
Tree_p wood_second_color(Tree_p, float r, float g, float b);
Tree_p wood(Tree_p, uint unit, float scale, float ring, float noise);

// -------------------------------------------------------------------------------------------------------------------
//   FRESNEL MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p fresnel_material(Tree_p, uint unit, float ratio, float IoR, float roughness);

#endif
