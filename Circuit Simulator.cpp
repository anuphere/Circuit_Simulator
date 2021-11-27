#include<gl/glut.h>
#include<math.h>
#include<cstring>
#define PI 3.14159265f
#define RES 1
#define BAT 2
#define LED 3
#define WIRE 4
#include<iostream>

using namespace std;


const int font=(int)GLUT_BITMAP_9_BY_15;
//const int font1=(int)GLUT_BITMAP_8_BY_13;
const char s=49;


char *title= "simulation--2";
int Width = 640;
int Height = 480;
int PosX = 50;
int PosY = 50;
int w,h;
static int chooseObject=0;
float x,y;
static int resI=0, batI=0, ledI=0, wireI=0;
float A=1,B=1;
float ledXSaved[1000], ledYSaved[1000], batteryXSaved[1000], batteryYSaved[1000],resistorXSaved[1000], resistorYSaved[1000];
float wireXSaved[1000], wireYSaved[1000];
static int res=0, bat=0, led=0, wire=0;
float easyX, easyY;
float refreshMillis = 30;
static float a=1;
static int M;
const char *Z;
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
void drawResistor(int);
void drawBattery(int);
void drawLED(int);
void drawWire(int);
static void renderBitmapString(float x, float y, void *font,const char *string)
{
    const char *c;

    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c );


    }

}

void showGrids()
{
    glColor4f(0.5,0.5,0.5,1.0);
    glLineWidth(1.0);
    for(int i=-160; i<=300;)
    {
        glBegin(GL_LINES);
        glVertex2f((i)*A,-200/B);
        glVertex2f((i)*A,200/B);
        glVertex2f((-160)*A,(i-40)/B);
        glVertex2f((200)*A,(i-40)/B);
        glEnd();
        i+=8;
    }
}





void initGL()
{
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glShadeModel(GL_FLAT);
    glClearColor(0.1,0.1,0.1,1.0);

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor4f(0.0,1.0,0.0,1.0);


    if(a>1)
    {
        A=a;
        B=1;
    }
    else
    {

        A=1;
        B=a;

    }

    //***rendering the play and pause buttons
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    glVertex2f(-200*A,-180/B);
    glVertex2f(-200*A,-200/B);      //bar for the play and the pause buttons
    glVertex2f(200*A,-200/B);
    glVertex2f(200*A,-180/B);
    glEnd();

    glColor4f(0.0,1.0,0.0,1.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-160*A,-183/B);
    glVertex2f(-160*A,-197/B);       //triangle as the play button
    glVertex2f(-147*A,-190/B);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-141*A,-184/B);
    glVertex2f(-141*A,-196/B);
    glVertex2f(-138*A,-196/B);
    glVertex2f(-138*A,-184/B);       //quads for the pause button
    glVertex2f(-135*A,-184/B);
    glVertex2f(-135*A,-196/B);
    glVertex2f(-132*A,-196/B);
    glVertex2f(-132*A,-184/B);
    glEnd();


    //***rendering the wire symbol
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f((-190)*A, (70)/B);
    glVertex2f((-170)*A, (90)/B);   //line for the wire
    glEnd();

    glPointSize(4.0);
    glBegin(GL_POINTS);
    glVertex2f((-191)*A, (70)/B);     //end points of the wire symbol
    glVertex2f((-170)*A, (90)/B);
    glEnd();
    renderBitmapString(-200*A,60/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,"WIRE");


    glLineWidth(2.0);

    //****rendering the battery symbol
    glBegin(GL_LINES);
    glVertex2f(-180*A,50/B);
    glVertex2f(-180*A,42/B);
    glVertex2f(-188.5*A,42/B);        //lines for battery
    glVertex2f(-172.5*A,42/B);
    glVertex2f(-184*A,38/B);
    glVertex2f(-176*A,38/B);
    glVertex2f(-180*A,38/B);
    glVertex2f(-180*A,30/B);
    glEnd();
    renderBitmapString(-200*A,20/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,"SIGNAL");


    //***rendering the resistor symbol
    glBegin(GL_QUADS);
    glVertex2f(-190*A,0/B);
    glVertex2f(-170*A,0/B);       //rectangle for resistor
    glVertex2f(-170*A,10/B);
    glVertex2f(-190*A,10/B);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-190*A,5/B);          //horizontal lines for the resistor
    glVertex2f(-197*A,5/B);
    glVertex2f(-170*A,5/B);
    glVertex2f(-163*A,5/B);
    glEnd();
    renderBitmapString(-200*A,-12/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,"RESISTOR");


    //***rendering the led symbol

    glBegin(GL_LINES);
    glVertex2f(-180*A,-15/B);
    glVertex2f(-180*A,-25/B);
    glVertex2f(-187*A,-35/B);   //lines for led
    glVertex2f(-173*A,-35/B);
    glVertex2f(-180*A,-35/B);
    glVertex2f(-180*A,-43/B);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-190*A,-23/B);
    glVertex2f(-180*A,-35/B);     //triangle for led
    glVertex2f(-170*A,-23/B);
    glEnd();

    renderBitmapString(-200*A,-47/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,"LED");


    glLineWidth(1.0);
    glBegin(GL_LINES);
    glVertex2f(-160*A,-180/B);
    glVertex2f(-160*A,200/B);
    glEnd();

    if(resI>0)                           //if the resistor is selected and mouse is clicked chooseObjected afterwards, a call to
    {
        // drawResistor function is invoked
        for(int integer =1; integer<=resI; integer++)
            drawResistor(integer);


    }

    if(batI>0)
    {
        for(int integer =1; integer<=batI; integer++)
            drawBattery(integer);
    }

    if(ledI>0)
    {
        for(int integer =1; integer<=ledI; integer++)
            drawLED(integer);
    }
    {

        if(wireI>0&&wireI%2==0)
        {

            for(int integer=1; integer<=wireI; integer++)
                drawWire(integer);
        }

        if(wireI>0&&wireI%2!=0)
        {

            for(int integer=1; integer<=wireI-1; integer++)
                drawWire(integer);
        }

        glutSwapBuffers();

    }
}

void drawWire(int take)
{
    static float x1,y1,x2,y2;
    static int count = 1;

    if(take%2!=0)
    {
        x1 = wireXSaved[take];
        y1 = wireYSaved[take];
    }

    else
    {
        x2 = wireXSaved[take];
        y2 = wireYSaved[take];
    }

    if(count==2)
    {
        glLineWidth(1);
        glColor4f(0.0,1.0,0.0,1.0);
        glBegin(GL_LINES);
        glVertex2f(x1*A, y1/B);
        glVertex2f(x2*A, y2/B);
        glEnd();

        glColor4f(0.0,1.0,0.0,2.0);
        glBegin(GL_POINTS);
        glVertex2f(x1*A, y1/B);
        glVertex2f(x2*A, y2/B);
        glEnd();
    }

    count++;
    if(count>2)
        count = 1;
    glFlush();
}

void drawBattery(int take)
{

    float x, y;
    x=batteryXSaved[take];
    y =  batteryYSaved[take];
    glColor4f(0.0,1.0,0.0,1.0);

    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f((x-8.5)*A, (y+2)/B);
    glVertex2f((x+8.5)*A, (y+2)/B);
    glVertex2f((x)*A, (y+10)/B);
    glVertex2f((x)*A, (y+2)/B);
    glVertex2f((x-4)*A, (y-2)/B);
    glVertex2f((x+4)*A, (y-2)/B);
    glVertex2f((x)*A, (y-2)/B);
    glVertex2f((x)*A, (y-10)/B);


    glEnd();
    renderBitmapString((x+7)*A,(y-7)/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,"DC");
    glFlush();
}

void drawLED(int take)
{
    float x, y;
    x=ledXSaved[take];
    y =  ledYSaved[take];
    glColor4f(0.0,1.0,0.0,1.0);

    glLineWidth(2.0);

    glBegin(GL_LINES);
    glVertex2f((x)*A, (y+14)/B);
    glVertex2f((x)*A, (y+6)/B);
    glVertex2f((x)*A, (y-6)/B);
    glVertex2f((x)*A, (y-14)/B);
    glVertex2f((x-7)*A, (y-6)/B);
    glVertex2f((x+7)*A, (y-6)/B);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f((x-10)*A, (y+6)/B);
    glVertex2f((x)*A, (y-6)/B);
    glVertex2f((x+10)*A, (y+6)/B);
    glEnd();
    renderBitmapString((x+7)*A,(y-17)/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,"LED");
    glFlush();
}

void drawResistor(int take)           //this function is called whenever there is resistor drawing request and
{
    // it takes the parameter 'take' which indicates the number of times mouse is clicked
    //after selecting the object mouse
    float x, y;
    x=resistorXSaved[take];
    y =  resistorYSaved[take];
    glColor4f(0.0,1.0,0.0,1.0);

    glBegin(GL_QUADS);
    glVertex2f((x-10.0)*A,(y-5.0)/B);
    glVertex2f((x+10.0)*A,(y-5.0)/B);    //rectangle for the resistor taking mouse click position as the center
    glVertex2f((x+10.0)*A,(y+5.0)/B);
    glVertex2f((x-10.0)*A,(y+5.0)/B);
    glEnd();

    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f((x-10)*A, y/B);
    glVertex2f((x-17)*A, y/B);     //lines for the resistor taking mouse click position as the center
    glVertex2f((x+10)*A, y/B);
    glVertex2f((x+17)*A, y/B);
    glEnd();
    renderBitmapString((x-5)*A,(y+10)/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,"R");
    renderBitmapString((x)*A,(y+10)/B,(void *)GLUT_BITMAP_TIMES_ROMAN_10,(int)s);




    glFlush();

}

void mouseClicked(int button, int state, int windowX, int windowY)
{
    easyX = (400.0/(float)w)*(float)windowX-200.0;
    easyY = (400.0/(float)h)*(float)(h-windowY)-200.0;

    if(button == GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&(easyX*A)>(-200*A)&&(easyX*A)<(-170*A)&&(easyY/B)>(61/B)&&(easyY/B)<(90/B))
    {
        chooseObject = WIRE;
    }

    if(button == GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&(easyX*A)>(-200.0*A)&&(easyX*A)<(-163*A)&&(easyY/B)>(-10.0/B)&&(easyY/B)<(13/B))
    {
        chooseObject = RES;
        if(wireI>0&&wireI%2!=0)
            wireI--;
    }

    if(button == GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&(easyX*A)>(-200.0*A)&&(easyX*A)<(-168*A)&&(easyY/B)>(24.0/B)&&(easyY/B)<(51/B))
    {
        chooseObject = BAT;
        if(wireI>0&&wireI%2!=0)
            wireI--;
    }

    if(button == GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&(easyX*A)>(-200.0*A)&&(easyX*A)<(-170*A)&&(easyY/B)>(-45.0/B)&&(easyY/B)<(-14/B))
    {
        chooseObject = LED;
        if(wireI>0&&wireI%2!=0)
            wireI--;
    }

    if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN)&&(chooseObject==WIRE)&&(A*easyX)>(-140*A)&&(easyY/B)>(-160/B))
    {
        wire=1;
        wireI++;
        led=0;
        bat=0;
        res=0;
        wireXSaved[wireI]=easyX;
        wireYSaved[wireI] = easyY;

    }

    if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN)&&(chooseObject==RES)&&(A*easyX)>(-140*A)&&(easyY/B)>(-160/B))
    {
        res=1;
        resI++;
        led=0;
        bat=0;
        wire=0;
        M=0;
        ++M;
        resistorXSaved[resI]=easyX;
        resistorYSaved[resI] = easyY;
        chooseObject = 0;

    }

    if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN)&&(chooseObject==BAT)&&(A*easyX)>(-140*A)&&(easyY/B)>(-160/B))
    {
        bat=1;
        batI++;
        res=0;
        led=0;
        batteryXSaved[batI]=easyX;
        batteryYSaved[batI] = easyY;
        chooseObject = 0;

    }

    if((button == GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN)&&(chooseObject==LED)&&(A*easyX)>(-140*A)&&(easyY/B)>(-160/B))
    {
        led=1;
        ledI++;
        res=0;
        bat=0;
        wire=0;
        ledXSaved[ledI]=easyX;
        ledYSaved[ledI] = easyY;
        chooseObject = 0;

    }

    if(button ==GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
    {

        cout<<easyX<<" , "<<easyY<<endl;
    }

    glutPostRedisplay();
}

/*static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1.0, 0.0, 0.0);
    setOrthographicProjection();
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(200,200,(void *)font,"Font Rendering - Programming Techniques");
    renderBitmapString(300,220, (void*)font, s);
    renderBitmapString(300,240,(void *)font,"Esc - Quit");
    glPopMatrix();
    resetPerspectiveProjection();
    glutSwapBuffers();
}*/


void reshape(GLsizei width, GLsizei height)
{
    w=width;
    h= height;
    if (height == 0)
        height = 1;
    glViewport(0,0,width,height);
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    a = aspect;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height)
    {
        clipAreaXLeft   = -200 * aspect;
        clipAreaXRight  = 200.0 * aspect;
        clipAreaYBottom = -200.0;
        clipAreaYTop    = 200.0;
    }
    else
    {
        clipAreaXLeft   = -200.0;
        clipAreaXRight  = 200.0;
        clipAreaYBottom = -200.0 / aspect;
        clipAreaYTop    = 200.0 / aspect;
    }
    gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);

}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(550.0, 550.0);
    glutInitWindowPosition(PosX, PosY);
    glutCreateWindow(title);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    //glutDisplayFunc(showGrids);
    glutMouseFunc(mouseClicked);
    //glutPassiveMotionFunc(passiveMouse);
    initGL();
    //glutTimerFunc(25, update, 0);
    glutMainLoop();
    return EXIT_SUCCESS;
}

