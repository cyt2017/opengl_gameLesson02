#ifndef QGLESWIDGET_H
#define QGLESWIDGET_H

#include <QWidget>
#include "tool/CELLMath.hpp"
#include "tool/program_p2_c4.h"
#define MESA_EGL_NO_X11_HEADERS
#include <EGL/egl.h>

class QGLESWIDGET : public QWidget
{
    Q_OBJECT
public:
    explicit QGLESWIDGET(QWidget *parent = 0);
    ~QGLESWIDGET();

    //! 窗口的高度
    int         _width;
    //! 窗口的宽度
    int         _height;
    /// for gles2.0
    EGLConfig   _config;
    EGLSurface  _surface;
    EGLContext  _context;
    EGLDisplay  _display;

    //! 增加shader
    PROGRAM_P2_C4   _shader;

    bool init_QGW();

    //! 初始化 OpenGLES2.0
    bool    initOpenGLES20();
    //! 销毁OpenGLES2.0
    void    destroyOpenGLES20();
    virtual void render();
    void drawImage();
signals:

public slots:
};

#endif // QGLESWIDGET_H
