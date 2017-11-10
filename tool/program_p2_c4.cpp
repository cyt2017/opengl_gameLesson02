#include "program_p2_c4.h"

PROGRAM_P2_C4::PROGRAM_P2_C4()
{
    _position   =   -1;
    _color      =   -1;
    _MVP        =   -1;
}

bool PROGRAM_P2_C4::initialize()
{
    const char* vs  =
    {
        "precision lowp float; "
        "uniform   mat4 _MVP;"
        "attribute vec2 _position;"
        "void main()"
        "{"
        "   vec4    pos =   vec4(_position,0,1);"
        "   gl_Position =   _MVP * pos;"
        "}"
    };
    const char* ps  =
    {
        "precision  lowp float; "
        "uniform    vec4 _color;"
        "void main()"
        "{"
        "   gl_FragColor   =   _color;"
        "}"
    };

    bool    res =   createProgram(vs,ps);
    if(res)
    {
        _position   =   glGetAttribLocation(_programId,"_position");
        _color      =   glGetUniformLocation(_programId,"_color");
        _MVP        =   glGetUniformLocation(_programId,"_MVP");
    }
    return  res;
}

void PROGRAM_P2_C4::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);

}

void PROGRAM_P2_C4::end()
{
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}
