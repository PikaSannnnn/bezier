// Name: Stephen Dong
// Quarter, Year: Fall 2021
// Lab:
//
// This file is to be modified by the student.
// main.cpp
////////////////////////////////////////////////////////////
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <vector>
#include <cstdio>
#include <math.h>
#include "vec.h"
#include <iostream>

using namespace std;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

vector<vec2> points(0);
float* c;

float Factorial(int n) {
    if (n == 0) return 1;
    else if (n<0) return false;
    return n * Factorial(n-1);
}

float Combination(int n, int k) {
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

float Binomial(int n, int k, float t) { // n = pointsize, k = curpoint, t = part of line
    return Combination(n, k) * pow(t, k) * pow(1- t, n - k);
}

void coefficients(float* c, int n, float t) {   // n = size, t = part of line
    float* r = new float[n + 1];
    float* s = new float[n + 1];
    r[0] = 1;
    s[n] = 1;

    for (int i = 1; i < n + 1; i++) {
        r[i] = (t * ((float)(n - i + 1) / i)); // * r[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        s[i] = (1 - t) * s[i + 1];
    }

    for (int i = 0; i < n + 1; i++) {
        c[i] = r[i] * s[i];
    }

    delete[] r;
    delete[] s;
}

void GL_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();

    glBegin(GL_LINES);
    glColor3f(0.50f,0.45f,1.0f);

    // TODO: just for example, you will need to change this.
    for (unsigned i = 1; i < points.size(); i++) {  // control points
        glVertex2f(points[i][0], points[i][1]);
        glVertex2f(points[i - 1][0], points[i - 1][1]);
    }

    for (float t = 0; t <= 1.00; t += 0.1) {   // bezier curve
        if (points.size() > 1) {
            vec2 sum(0.0,0.0);
            // float* c = new float[points.size() + 1];
            // coefficients(c, points.size(), t);
            // for (unsigned i = 0; i < points.size(); i++) {
            //     sum[0] += c[i] * points[i][0];
            //     sum[1] += c[i] * points[i][1];

            //     cout << sum[0] << "," <<sum[1] << "\t";
            //     cout << c[i] << "\t";
            //     cout << points[i][0] << "," << points[i][1] << endl;
            // }
            
            for (unsigned i = 0; i < points.size(); i++) {
                float B = Binomial(points.size()-1, i, t);
                sum[0] += B * points[i][0];
                sum[1] += B * points[i][1];
                cout << sum[0] << "," <<sum[1] << "\t";
                cout << B << "\t";
                cout << points[i][0] << "," << points[i][1] << endl;
            }

        
            glVertex2f(sum[0], sum[1]);
            //glVertex2f(sum[0], sum[1]);
        }

        delete[] c;
    }

    // if(points.size()) cout << points[points.size() - 1][0] << ',' << points[points.size() - 1][1] << endl;

    glEnd();
    glFlush();
}

void GL_mouse(int button,int state,int x,int y)
{
    y=WINDOW_HEIGHT-y;
    GLdouble mv_mat[16];
    GLdouble proj_mat[16];
    GLint vp_mat[4];
    glGetDoublev(GL_MODELVIEW_MATRIX,mv_mat);
    glGetDoublev(GL_PROJECTION_MATRIX,proj_mat);
    glGetIntegerv(GL_VIEWPORT,vp_mat);

    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        double px,py,dummy_z; // we don't care about the z-value but need something to pass in
        gluUnProject(x,y,0,mv_mat,proj_mat,vp_mat,&px,&py,&dummy_z);
        // TODO: the mouse click coordinates are (px,py).
        vec2 pt(px, py);
        points.push_back(pt);

        glutPostRedisplay();
    }
}

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
    glutInit(argc, argv);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    //glMatrixMode(GL_PROJECTION_MATRIX);
    //glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    glutCreateWindow("CS 130 - <Stephen Dong>");
    glutDisplayFunc(GL_render);
    glutMouseFunc(GL_mouse);
}

int main(int argc, char** argv)
{
    GLInit(&argc, argv);
    glutMainLoop();
    return 0;
}
