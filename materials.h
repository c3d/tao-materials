#ifndef MATERIALS_H
#define MATERIALS_H
// ****************************************************************************
//  materials.h                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//    Prototype of the function used by materials.tbl to
//    implement the new XL primitive
//
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************
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
