#ifndef MATERIAL_H
#define MATERIAL_H
// ****************************************************************************
//  material.h                                                      Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define a basic material.
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

#include "tao/coords3d.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include <QObject>

using namespace std;
using namespace Tao;

struct Material : public QObject
{
    Material(const QGLContext **pcontext);
    ~Material();

    // Draw material
    virtual void    Draw();

    // Re-create shaders if GL context has changed
    void            checkGLContext();
    virtual void    createShaders();

    std::ostream &  debug();

    static void     render_callback(void *arg);
    static void     identify_callback(void *arg);
    static void     delete_callback(void *arg);

public:
    const QGLContext    **pcontext;

public:
    // Pointer to Tao functions
    static const Tao::ModuleApi *tao;
};


#endif
