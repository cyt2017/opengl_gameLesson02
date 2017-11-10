#include "qgleswidget.h"

QGLESWIDGET::QGLESWIDGET(QWidget *parent) : QWidget(parent)
{

}

QGLESWIDGET::~QGLESWIDGET()
{
    destroyOpenGLES20();
}

bool QGLESWIDGET::init_QGW()
{
    if(!initOpenGLES20())
    {
        return false;
    }
    _shader.initialize();
}

bool QGLESWIDGET::initOpenGLES20()
{
    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE,24,
        EGL_NONE
    };
    EGLint 	format(0);
    EGLint	numConfigs(0);
    EGLint  major;
    EGLint  minor;

    //! 1
    _display	    =	eglGetDisplay(EGL_DEFAULT_DISPLAY);

    //! 2init
    eglInitialize(_display, &major, &minor);

    //! 3
    eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);

    eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);
    //!!! 4 使opengl与qt的窗口进行绑定<this->winId()>
    _surface	    = 	eglCreateWindowSurface(_display, _config, this->winId(), NULL);

    //! 5
    EGLint attr[]   =   { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    _context 	    = 	eglCreateContext(_display, _config, 0, attr);
    //! 6
    if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
    {
        return false;
    }

    eglQuerySurface(_display, _surface, EGL_WIDTH,  &_width);
    eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

    return  true;
}

void QGLESWIDGET::destroyOpenGLES20()
{
    if (_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(_display, _context);
        }
        if (_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
    }
    _display    =   EGL_NO_DISPLAY;
    _context    =   EGL_NO_CONTEXT;
    _surface    =   EGL_NO_SURFACE;
}

void QGLESWIDGET::render()
{
    //! 清空缓冲区
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //! 视口，在Windows窗口指定的位置和大小上绘制OpenGL内容
    glViewport(0,0,_width,_height);

    //! 创建一个投影矩阵
    //当图像显示出来与坐标的位置出现导致现象时，可以改变投影矩阵的坐标，使之对齐
    CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(_width),float(_height),0,-100.0f,100);
    _shader.begin();
    {
        float   x   =   100;
        float   y   =   100;
        float   w   =   100;
        float   h   =   100;
        CELL::float2  pos[]   =
        {
            CELL::float2(x,y),
            CELL::float2(x + w,y),
            CELL::float2(y,y + h),
            CELL::float2(x + w, y + h),
        };
        //设置mvp矩阵使绘制的图像移动
        static float inc= 0;
        CELL::matrix4 matran;
        matran.translate(inc,-inc,0);
        CELL::matrix4 mat;
        mat = matran*screenProj;
        inc+=0.001f;

        //========================================================
        //!生成MVP矩阵（matModel、matProject、matView），使图像具有旋转、缩放、平移的功能
        glUniformMatrix4fv(_shader._MVP, 1, false, mat.data());
        //=========================================================
        //!设置图像上的颜色，现在使用的是不变色采用uniform<全局变量>，如果设置的是变色的话，那么就需要把颜色变量设置成attribute<局部变量>
        glUniform4f(_shader._color,1,0,0,1);
        //==========================================================
        //!设置顶点坐标，由于定点是逐一进行复制，所以设置成attribute<局部变量>
        glVertexAttribPointer(_shader._position,2,GL_FLOAT,false,sizeof(CELL::float2),pos);
        //==========================================================
        //这个函数是把以上的数据进行组装，相应的点对应相应的颜色，或者相应的文理，目前没有涉及到文理
        //!也就是绘制过程
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
    _shader.end();
}

void QGLESWIDGET::drawImage()
{
    render();
    eglSwapBuffers(_display,_surface);
}
