// ****************************************************************************
//  materials.cpp                                                   Tao project
// ****************************************************************************
//
//   File Description:
//
//     Implementation of the XL primitives for the "materials" module.
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

#include "materials.h"
#include <iostream>
#include "coords3d.h"

XL_DEFINE_TRACES

// Marble colors
static GLfloat firstMarbleColor[3] = {1.0, 1.0, 1.0};
static GLfloat secondMarbleColor[3] = {0.60, 0.50, 0.40};

// Granite colors
static GLfloat firstGraniteColor[3] = {0.80, 0.73, 0.71};
static GLfloat secondGraniteColor[3] = {0.30, 0.15, 0.10};
static GLfloat thirdGraniteColor[3] = {0.20, 0.20, 0.20};

// Wood colors
static GLfloat firstWoodColor[3] = {0.92, 0.51, 0.13};
static GLfloat secondWoodColor[3] = {0.44, 0.21, 0.0};

// -------------------------------------------------------------------------------------------------------------------
//   PLASTIC MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p plastic(Tree_p)
// ----------------------------------------------------------------------------
//   Define plastic material
// ----------------------------------------------------------------------------
{
    Plastic* plastic = new Plastic();
    Material::tao->AddToLayout2(Plastic::render_callback,
                                Plastic::identify_callback,
                                plastic, Plastic::delete_callback);

    return xl_true;
}

// -------------------------------------------------------------------------------------------------------------------
//   MARBLE MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p marble_first_color(Tree_p, float r, float g, float b)
// ----------------------------------------------------------------------------
//   Define first marble color
// ----------------------------------------------------------------------------
{
    firstMarbleColor[0] = r;
    firstMarbleColor[1] = g;
    firstMarbleColor[2] = b;

    return xl_true;
}


Tree_p marble_second_color(Tree_p, float r, float g, float b)
// ----------------------------------------------------------------------------
//   Define second marble color
// ----------------------------------------------------------------------------
{
    secondMarbleColor[0] = r;
    secondMarbleColor[1] = g;
    secondMarbleColor[2] = b;

    return xl_true;
}

Tree_p marble(Tree_p, uint unit, float scale)
// ----------------------------------------------------------------------------
//   Define marble material
// ----------------------------------------------------------------------------
{
    Marble* marble = new Marble(unit, scale);
    marble->setFirstColor(firstMarbleColor);
    marble->setSecondColor(secondMarbleColor);

    Material::tao->AddToLayout2(Marble::render_callback,
                                Marble::identify_callback,
                                marble, Marble::delete_callback);

    return xl_true;
}

// -------------------------------------------------------------------------------------------------------------------
//   GRANITE MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p granite_first_color(Tree_p, float r, float g, float b)
// ----------------------------------------------------------------------------
//   Define first granite color
// ----------------------------------------------------------------------------
{
    firstGraniteColor[0] = r;
    firstGraniteColor[1] = g;
    firstGraniteColor[2] = b;

    return xl_true;
}


Tree_p granite_second_color(Tree_p, float r, float g, float b)
// ----------------------------------------------------------------------------
//   Define second granite color
// ----------------------------------------------------------------------------
{
    secondGraniteColor[0] = r;
    secondGraniteColor[1] = g;
    secondGraniteColor[2] = b;

    return xl_true;
}

Tree_p granite_third_color(Tree_p, float r, float g, float b)
// ----------------------------------------------------------------------------
//   Define third granite color
// ----------------------------------------------------------------------------
{
    thirdGraniteColor[0] = r;
    thirdGraniteColor[1] = g;
    thirdGraniteColor[2] = b;

    return xl_true;
}


Tree_p granite(Tree_p, uint unit, float scale)
// ----------------------------------------------------------------------------
//   Define granite material
// ----------------------------------------------------------------------------
{
    Granite* granite = new Granite(unit, scale);
    granite->setFirstColor(firstGraniteColor);
    granite->setSecondColor(secondGraniteColor);
    granite->setThirdColor(thirdGraniteColor);

    Material::tao->AddToLayout2(Granite::render_callback,
                                Granite::identify_callback,
                                granite, Granite::delete_callback);

    return xl_true;
}

// -------------------------------------------------------------------------------------------------------------------
//   WOOD MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p wood_first_color(Tree_p, float r, float g, float b)
// ----------------------------------------------------------------------------
//   Define first wood color
// ----------------------------------------------------------------------------
{
    firstWoodColor[0] = r;
    firstWoodColor[1] = g;
    firstWoodColor[2] = b;

    return xl_true;
}


Tree_p wood_second_color(Tree_p, float r, float g, float b)
// ----------------------------------------------------------------------------
//   Define second wood color
// ----------------------------------------------------------------------------
{
    secondWoodColor[0] = r;
    secondWoodColor[1] = g;
    secondWoodColor[2] = b;

    return xl_true;
}

Tree_p wood(Tree_p, uint unit, float scale, float ring, float noise)
// ----------------------------------------------------------------------------
//   Define wood material
// ----------------------------------------------------------------------------
{
    Wood* wood = new Wood(unit, scale, ring, noise);
    wood->setFirstColor(firstWoodColor);
    wood->setSecondColor(secondWoodColor);

    Material::tao->AddToLayout2(Wood::render_callback,
                                Wood::identify_callback,
                                wood, Wood::delete_callback);

    return xl_true;
}

// -------------------------------------------------------------------------------------------------------------------
//   FRESNEL MATERIAL
// -------------------------------------------------------------------------------------------------------------------

Tree_p fresnel_material(Tree_p, uint unit, float ratio, float IoR, float roughness)
// ----------------------------------------------------------------------------
//   Define fresnel material
// ----------------------------------------------------------------------------
{       
    Fresnel* fresnel = new Fresnel(unit, IoR, ratio, roughness);

    Material::tao->AddToLayout2(Fresnel::render_callback,
                                Fresnel::identify_callback,
                                fresnel, Fresnel::delete_callback);

    return xl_true;
}

int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    glewInit();
    XL_INIT_TRACES();

    Material::tao = api;

    return 0;
}


int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module
// ----------------------------------------------------------------------------
{
    return 0;
}
