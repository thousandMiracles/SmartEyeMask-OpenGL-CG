#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <unistd.h>
#include <math.h>
#include "param.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void init(void)
{
    glClearColor(0.235, 0.235, 0.235, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    
}

void surface(const GLfloat* points, GLfloat color[], GLint u_val[], GLint v_val[]){
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 5, 0, 1, 15, 3, points);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(40, 0.0, 1.0, 40, 0.0, 1.0);
    glColor3f(color[0], color[1], color[2]);
    glEvalMesh2(GL_FILL, u_val[0], u_val[1], v_val[0], v_val[1]);
    glEnd();
}

void metal_outfit(GLfloat (*color)[3]){
    surface(&outfit_ctrl_point_front[0][0][0], color[0], u_val[0], v_val[0]);
    surface(&outfit_ctrl_point_back[0][0][0], color[1], u_val[0], v_val[0]);
    surface(&outfit_ctrl_point_side1[0][0][0], color[2], u_val[0], v_val[0]);
    surface(&outfit_ctrl_point_side2[0][0][0], color[2], u_val[0], v_val[0]);
    surface(&outfit_ctrl_point_upper[0][0][0], color[2], u_val[0], v_val[0]);
    surface(&outfit_ctrl_point_lower[0][0][0], color[2], u_val[0], v_val[0]);
    GLfloat black[3] = {0, 0, 0};
    surface(&outfit_ctrl_point_front[0][0][0], black , u_val[1], v_val[1]);
}

void rounded_rectangle(GLdouble r, GLfloat (*color)[3]){
    
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 3, 0, 1, 9, 3, &buffer_ctrl_point_side1[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(40, 0.0, 1.0, 40, 0.0, 1.0);
    glColor3f(color[2][0], color[2][1], color[2][2]);
    glEvalMesh2(GL_FILL, 0, 40, 0, 40);
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(color[0][0], color[0][1], color[0][2]);
    for(int i = 0; i < 360; i ++){
        glVertex3f(-7.0, 0.0 + r * sin(i * DEG2RAD), -3.0+ r * cos(i * DEG2RAD));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(color[0][0], color[0][1], color[0][2]);
    for(int i = 0; i < 360; i ++){
        glVertex3f(7.0, 0.0 + r * sin(i * DEG2RAD), -3.0+ r * cos(i * DEG2RAD));
    }
    glEnd();
    
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 3, 0, 1, 9, 3, &buffer_ctrl_point_side2[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(40, 0.0, 1.0, 40, 0.0, 1.0);
    glColor3f(color[2][0], color[2][1], color[2][2]);
    glEvalMesh2(GL_FILL, 0, 40, 0, 40);
    glEnd();
}

void ribbon(){
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 3, 0, 1, 9, 2, &ribbon_ctrl_point[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(40, 0.0, 1.0, 40, 0.0, 1.0);
    glColor3f(0, 0, 0);
    glEvalMesh2(GL_FILL, 0, 40, 0, 40);
    glEnd();
}

void background(int i){
    const char *filename = background_name[i];
    GLuint texture = 0;
    int width = 0, height = 0, channels = 0;
    stbi_uc * image = stbi_load(filename, &width, &height, &channels, 3);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_MIRRORED_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, width, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glLoadIdentity();
    glBegin(GL_QUADS);
    glRotatef(45, 0, 0, 0);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(12, 12, -12);//右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f(12, -12, -12);//右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-12, -12, -12);//左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-12, 12, -12);//左下
    glEnd();
    glLoadIdentity();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    background(background_option);
    glPopMatrix();
    
    //metal outfit
    glLoadIdentity();
    gluLookAt(x_direction, y_direction, z_direction, 0, 0, 0, 0, 1, 0);
    glPushMatrix();
    glRotatef(auto_rotate_angle, 0.0, 1.0, 0.0);
    glRotatef(control_rotate_angle[0], -1.0, 0.0, 0.0);
    glRotatef(control_rotate_angle[1], 0.0, 0.0, -1.0);
    glRotatef(control_rotate_angle[2], 1.0, 0.0, 0.0);
    glRotatef(control_rotate_angle[3], 0.0, 0.0, 1.0);
    metal_outfit(color_ptr);
    rounded_rectangle(1.0, color_ptr);
    ribbon();
    glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
}

void AutoRotation(){
    auto_rotate_angle += 10;
    if(auto_rotate_angle > 360) auto_rotate_angle = 0;
    //sleep(20);
    glutPostRedisplay();
}
void mouseFunction(GLint button, GLint state, GLint x, GLint y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        glutIdleFunc(AutoRotation);
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
        glutIdleFunc(NULL);
    }
}


//3D空间中绘制2D效果，采用正交投影
void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-12.0, 12.0, -12.0*(GLfloat)h / (GLfloat)w, 12.0*(GLfloat)h / (GLfloat)w, -12.0, 12.0);
    else
        glOrtho(-12.0*(GLfloat)w / (GLfloat)h, 12.0*(GLfloat)w / (GLfloat)h, -12.0, 12.0, -12.0, 12.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
            control_rotate_angle[1] += 15;
            glutPostRedisplay();
            break;
        case 'b':
            color_ptr = outfit_color_blue;
            background_option = 1;
            glutPostRedisplay();
            break;
        case 'd':
            control_rotate_angle[3] += 15;
            glutPostRedisplay();
            break;
        case 'g':
            color_ptr = outfit_color_green;
            background_option = 3;
            glutPostRedisplay();
            break;
        case 'w':
            control_rotate_angle[0] += 15;
            glutPostRedisplay();
            break;
        case 's':
            control_rotate_angle[2] += 15;
            glutPostRedisplay();
            break;
        case 'p':
            color_ptr = outfit_color_pink;
            background_option = 2;
            glutPostRedisplay();
            break;
        case '1':
            x_direction -= 0.5;
            glutPostRedisplay();
            break;
        case '2':
            y_direction -= 0.5;
            glutPostRedisplay();
            break;
        case '3':
            z_direction -= 0.5;
            glutPostRedisplay();
            break;
        case '4':
            x_direction += 0.5;
            glutPostRedisplay();
            break;
        case '5':
            y_direction += 0.5;
            glutPostRedisplay();
            break;
        case '6':
            z_direction += 0.5;
            glutPostRedisplay();
            break;
        case 27:   //ESC键
            exit(0);
            break;
        default:
            break;
    }
}



int main(int argc, char** argv)
{
    srand((unsigned int)time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//使用双缓存模式和深度缓存
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("2D Bezier曲线");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseFunction);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
