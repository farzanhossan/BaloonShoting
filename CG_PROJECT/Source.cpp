#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include<string>
#include<MMSystem.h>
#include<windows.h>
#include<fstream>

using namespace std;
# define PI           3.14159265358979323846

const int WIDTH = 800, HEIGHT = 600;
char buf[100] = { 0 };
void reshape(int, int);
void display();
void makeCircle(GLfloat, GLfloat, GLfloat, GLenum, GLubyte, GLubyte, GLubyte);
void makeAxisLines();
void renderBitmapString(float, float, char*, void*);
void idle();
void repeat(int);
void specialKeys(int, int, int);
void baloonORbomb();
void balloonBlast();
void BALOONBLASTED(int);
void bombBlast();
void Speed(int x, int y);
void Speed();
void handleMouse(int button, int state, int x, int y);

void circle1(GLfloat x, GLfloat y, GLfloat radius);
void tree1();
void tree2();
void tree3();
void nightcolorchange();

void SaveData(int Score);
void TakeData(int Score);


GLfloat position = -700;
bool baloonPosition = true;
int scoreCount = 0;
int speed = rand() % 10 + 1;
int missed = 0;
int i = 0;
int b1x1 = 150, b1x2 = 160, bulletCount = 50;
int totalbullets = bulletCount;
bool b1x = false, canNew = true;
bool gameOver = false;
bool baloon = true, bomb = false, restart = false;
bool baloonBlasted = false;
ofstream outfile;
ifstream infile;
int HighScore=0;
bool highscore=false;
int bonusCount = 1;
int bonusRand = 0;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("OpenGL Setup Test");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	//glutIdleFunc(idle);
	glutTimerFunc(1, repeat, 1);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(handleMouse);
	glutMainLoop();
	return 0;
}

void idle()
{
	glutPostRedisplay();
}
void repeat(int)
{
	display();
	glutTimerFunc(5, repeat, 1);
}

void baloonORbomb() {
	int x = rand() % 5 + 1;

	if (x == 1 || x == 3) {

		baloon = false; bomb = true;
	}
	else {
		bonusCount++;
		bonusRand = 6;
		
		baloon = true; bomb = false;
	}
	cout << x;
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-WIDTH, WIDTH, -HEIGHT, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}

void specialKeys(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		if (i >= -30) { i -= 6; }
		break;
	case GLUT_KEY_DOWN:
		if (i <= 30) { i += 6; }
		break;
	case GLUT_KEY_LEFT:
		if (bulletCount >0) {
			if (canNew == true) {
				b1x = true;
				canNew = false;
				b1x1 = 150; b1x2 = 160;
				bulletCount--;
				PlaySound(TEXT("Bullet.wav"), NULL, SND_ASYNC);
			}

		}
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_F1:
		if (restart)
		{
			restart = false;
			position = -700;
			baloonPosition = true;
			scoreCount = 0;
			Speed(5, 2);
			missed = 0;
			i = 0;
			b1x1 = 150, b1x2 = 160, bulletCount = 50;
			totalbullets = bulletCount;
			b1x = false, canNew = true;
			gameOver = false;
			baloon = true, bomb = false, restart = false;
			repeat(1);
		}
		break;
	}
}

void handleMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (bulletCount >0) {
				if (canNew == true) {
					b1x = true;
					canNew = false;
					b1x1 = 150; b1x2 = 160;
					bulletCount--;
					PlaySound(TEXT("Bullet.wav"), NULL, SND_ASYNC);
				}

			}
		}
	}

	glutPostRedisplay();
}

void Speed(int x, int y)
{
	speed = rand() % x + y;
}

void TakeData(int Score)
{
	infile.open("HighScore.txt");
	if (infile.fail())
	{
		cerr << "Error opening file";
	}
	infile >> Score;
	infile.close();
	HighScore = Score;
	//cout << "High Score" << Score << endl;;
	
}

void SaveData(int Score)
{
		outfile.open("HighScore.txt");
		outfile << Score;
		outfile.close();
}

void Speed()
{
	if (scoreCount <= 5)
	{
		Speed(5, 2);
	}
	if (scoreCount>5 && scoreCount <= 10)
	{
		Speed(8, 5);
	}
	if (scoreCount>10 && scoreCount <= 15)
	{
		Speed(11, 8);
	}
	if (scoreCount>15 && scoreCount <= 20)
	{
		Speed(15, 11);
	}
	if (scoreCount>20)
	{
		Speed(22, 15);
	}
}

void BALOONBLASTED(int iteration) {
	baloonBlasted = false;
	position = -900;
	scoreCount++;
	
	TakeData(1);
	if (HighScore<scoreCount)
	{
		HighScore = scoreCount;
		SaveData(scoreCount);
	}
	speed = rand() % 10 + 5;
	baloonORbomb();
}

void display() {

	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	nightcolorchange();
	glLoadIdentity();

	if (missed == totalbullets / 2) { gameOver = true; }

	

	if (gameOver == true) {
		b1x = false;
		baloonPosition = false;
		
		sprintf_s(buf, "GAME OVER");
		renderBitmapString(-150, 450, buf, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf_s(buf, "High Score: %d", HighScore);
		renderBitmapString(-150, 400, buf, GLUT_BITMAP_TIMES_ROMAN_24);
		if(HighScore == scoreCount)
		{
			sprintf_s(buf, "Congratulation.You are High Scorer !!!");
			renderBitmapString(-150, 500, buf, GLUT_BITMAP_TIMES_ROMAN_24);
		}
		sprintf_s(buf, "Press F1 For Restart.");
		renderBitmapString(-150, 350, buf, GLUT_BITMAP_TIMES_ROMAN_24);
		restart = true;
		bombBlast();
		glLoadIdentity();
	}

	if (b1x == true) {
		b1x1 += 50;
		b1x2 += 50;
	}

	glColor3ub(255, 255, 255);

	sprintf_s(buf, "Bullets: %d", bulletCount);
	renderBitmapString(-750, 550, buf, GLUT_BITMAP_TIMES_ROMAN_24);
	sprintf_s(buf, "Score: %d", scoreCount);
	renderBitmapString(500, 550, buf, GLUT_BITMAP_TIMES_ROMAN_24);
	sprintf_s(buf, "Missed: %d", missed);
	renderBitmapString(-100, 550, buf, GLUT_BITMAP_TIMES_ROMAN_24);

	makeCircle(50, -800, 0, GL_POLYGON, 255, 255, 255);

	glTranslatef(-800, 0, 0);
	glPushMatrix();
	glRotatef(i, 0, 0, -1);
	glBegin(GL_POLYGON);
	glVertex2f(0, -20);
	glVertex2f(150, -20);
	glVertex2f(150, 20);
	glVertex2f(0, 20);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(b1x1, -5);
	glVertex2f(b1x2, -5);
	glVertex2f(b1x2, 5);
	glVertex2f(b1x1, 5);
	glEnd();


	glPopMatrix();

	glPushMatrix();
	if (baloonBlasted == true) {
		balloonBlast();
	}
	else if (gameOver == false) {
		if (baloonPosition == true) { position += speed; }

		glTranslatef(1500, position, 0);//baloon
		
		if (baloon == true) {
			
			if (bonusCount == bonusRand)
			{
				if (missed>=3)
				{
					missed = missed - 2;
				}
				else
				{
					missed = 0;
				}
				cout << "Bonus Called" << endl;
				makeCircle(37, 0, 0, GL_POLYGON, 229, 229, 229);
				makeCircle(34, 0, -7, GL_POLYGON, 229, 229, 229);
				makeCircle(30, 0, -15, GL_POLYGON, 229, 229, 229);
				makeCircle(26, 0, -23, GL_POLYGON, 229, 229, 229);
				makeCircle(22, 0, -31, GL_POLYGON, 229, 229, 229);
				makeCircle(18, 0, -39, GL_POLYGON, 229, 229, 229);
				makeCircle(15, 0, -47, GL_POLYGON, 229, 229, 229);

				glBegin(GL_TRIANGLES);//baloon bellow side
				glColor3ub(229, 229, 229);
				glVertex2f(-6, -68);
				glVertex2f(6, -68);
				glVertex2f(0, -62);
				glEnd();

				glBegin(GL_LINES);//baloon twist
				glColor3ub(255, 255, 255);
				glVertex2f(0, -68);
				glVertex2f(-5, -72);
				glVertex2f(-5, -72);
				glVertex2f(5, -76);
				glVertex2f(5, -76);
				glVertex2f(0, -80);
				glVertex2f(0, -80);
				glVertex2f(0, -84);
				glEnd();
				
			}
			else
			{
				cout << "x :" << bonusRand << endl;
				cout << "Bonus Count" << bonusCount << endl;
				makeCircle(37, 0, 0, GL_POLYGON, 255, 153, 0);
				makeCircle(34, 0, -7, GL_POLYGON, 255, 153, 0);
				makeCircle(30, 0, -15, GL_POLYGON, 255, 153, 0);
				makeCircle(26, 0, -23, GL_POLYGON, 255, 153, 0);
				makeCircle(22, 0, -31, GL_POLYGON, 255, 153, 0);
				makeCircle(18, 0, -39, GL_POLYGON, 255, 153, 0);
				makeCircle(15, 0, -47, GL_POLYGON, 255, 153, 0);

				glBegin(GL_TRIANGLES);//baloon bellow side
				glColor3ub(255, 153, 0);
				glVertex2f(-6, -68);
				glVertex2f(6, -68);
				glVertex2f(0, -62);
				glEnd();

				glBegin(GL_LINES);//baloon twist
				glColor3ub(255, 255, 255);
				glVertex2f(0, -68);
				glVertex2f(-5, -72);
				glVertex2f(-5, -72);
				glVertex2f(5, -76);
				glVertex2f(5, -76);
				glVertex2f(0, -80);
				glVertex2f(0, -80);
				glVertex2f(0, -84);
				glEnd();
			}

			
		}




		if (bomb == true) {
			makeCircle(50, 0, 0, GL_POLYGON, 31, 32, 33);


			glBegin(GL_POLYGON);//boom
			glColor3ub(31, 32, 33);

			glVertex2f(-6, 68);
			glVertex2f(6, 68);
			glVertex2f(6, 52);
			glVertex2f(-6, 52);
			glEnd();

			glBegin(GL_LINES);//boom twist
			glColor3ub(160, 40, 40);
			glVertex2f(0, 68);
			glVertex2f(-5, 72);
			glVertex2f(-5, 72);
			glVertex2f(5, 76);
			glVertex2f(5, 76);
			glVertex2f(0, 80);
			glVertex2f(0, 80);
			glVertex2f(0, 84);

			glEnd();
		}

		if (bonusCount > 7)
		{
			bonusCount = 0;
		}

		glPopMatrix();


		//Collision Detection
		if ((((-1)*position) / i >= 20 && ((-1)*position) / i <= 32 && b1x1 >= 1430)
			|| (i == 0 && b1x1 >= 1430 && position > -80 && position < 100)) {
			if (bomb == true) {
				gameOver = true;
			}
			else {
				baloonBlasted = true;
				glutTimerFunc(100, BALOONBLASTED, 0);
				//Speed();
				/*position = -900;
				scoreCount++;
				speed = rand() % 10 + 5;
				baloonORbomb();*/
			}
		}
		if (b1x1 >= 1500) {
			b1x1 = 150; b1x2 = 160; canNew = true; b1x = false;
			if (bulletCount == 0) {
				gameOver = true;
			}
		}

		if (position >= 900) {
			position = -900;
			Speed();
			if (bomb!=true)
			{
				missed++;
			}
			
			baloonORbomb();
		}
	}
	glFlush();
}

void renderBitmapString(float x, float y, char *string, void* font)
{
	char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

}

void makeCircle(GLfloat radius, GLfloat x, GLfloat y, GLenum mode, GLubyte red, GLubyte green, GLubyte blue)
{

	int i;
	int lineAmount = 100;
	GLfloat twicePi = 2.0f * PI;
	glBegin(mode);
	glColor3ub(red, green, blue);
	for (i = 0; i <= lineAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / lineAmount)),
			y + (radius* sin(i * twicePi / lineAmount))
		);
	}
	glEnd();
	glColor3ub(255, 255, 255);
}

void makeAxisLines() {
	glBegin(GL_LINES);
	glVertex2f(-WIDTH, 0);
	glVertex2f(WIDTH, 0);
	glVertex2f(0, HEIGHT);
	glVertex2f(0, -HEIGHT);
	glEnd();
}

void balloonBlast()
{
	glTranslatef(1490, position, 0);
	glScalef(0.5, 0.5, 0);
	glBegin(GL_POLYGON);//balloon blast (x,y)
	glColor3ub(255, 153, 0);
	glVertex2f(0, 0);
	glVertex2f(180, 0);
	glVertex2f(100, 25);
	glVertex2f(125, 80);
	glVertex2f(55, 35);
	glVertex2f(73, 90);
	glVertex2f(25, 50);
	glVertex2f(0, 100);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//balloon blast (x,-y)
	glColor3ub(255, 153, 0);
	glVertex2f(0, 0);
	glVertex2f(180, 0);
	glVertex2f(100, -25);
	glVertex2f(125, -80);
	glVertex2f(55, -35);
	glVertex2f(73, -90);
	glVertex2f(25, -50);
	glVertex2f(0, -100);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//balloon blast (-x,-y)
	glColor3ub(255, 153, 0);
	glVertex2f(0, 0);
	glVertex2f(-180, 0);
	glVertex2f(-100, -25);
	glVertex2f(-125, -80);
	glVertex2f(-55, -35);
	glVertex2f(-73, -90);
	glVertex2f(-25, -50);
	glVertex2f(0, -100);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//balloon blast (-x,y)
	glColor3ub(255, 153, 0);
	glVertex2f(0, 0);
	glVertex2f(-180, 0);
	glVertex2f(-100, 25);
	glVertex2f(-125, 80);
	glVertex2f(-55, 35);
	glVertex2f(-73, 90);
	glVertex2f(-25, 50);
	glVertex2f(0, 100);
	glVertex2f(0, 0);
	glEnd();
}

void bombBlast()
{
	glScalef(2.5, 2.5, 0);
	glBegin(GL_POLYGON);//red portion bomb blast (x,y)
	glColor3ub(204, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(240, 0);
	glVertex2f(65, 30);
	glVertex2f(185, 45);
	glVertex2f(60, 52);
	glVertex2f(167, 80);
	glVertex2f(55, 70);
	glVertex2f(120, 113);
	glVertex2f(23, 88);
	glVertex2f(60, 140);
	glVertex2f(0, 100);
	//glVertex2f(20,115);
	//glVertex2f(0,148);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//bomb blast (x,-y)
	glColor3ub(204, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(240, 0);
	glVertex2f(65, -30);
	glVertex2f(185, -45);
	glVertex2f(60, -52);
	glVertex2f(167, -80);
	glVertex2f(55, -70);
	glVertex2f(120, -113);
	glVertex2f(23, -88);
	glVertex2f(60, -140);
	glVertex2f(0, -100);
	//glVertex2f(20, -115);
	//glVertex2f(0, -148);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//bomb blast (-x,-y)
	glColor3ub(204, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(-240, 0);
	glVertex2f(-65, -30);
	glVertex2f(-185, -45);
	glVertex2f(-60, -52);
	glVertex2f(-167, -80);
	glVertex2f(-55, -70);
	glVertex2f(-120, -113);
	glVertex2f(-23, -88);
	glVertex2f(-60, -140);
	glVertex2f(0, -100);
	//glVertex2f(-20, -115);
	//glVertex2f(0, -148);
	glVertex2f(0, 0); \
		glEnd();

	glBegin(GL_POLYGON);//bomb blast (-x,y)
	glColor3ub(204, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(-240, 0);
	glVertex2f(-65, 30);
	glVertex2f(-185, 45);
	glVertex2f(-60, 52);
	glVertex2f(-167, 80);
	glVertex2f(-55, 70);
	glVertex2f(-120, 113);
	glVertex2f(-23, 88);
	glVertex2f(-60, 140);
	glVertex2f(0, 100);
	//glVertex2f(-20, 115);
	//glVertex2f(0, 148);
	glVertex2f(0, 0);
	glEnd();


	glScalef(0.8, 0.8, 0);//yellow portion

	glBegin(GL_POLYGON);// bomb blast (x,y)
	glColor3ub(255, 255, 0);
	glVertex2f(0, 0);
	glVertex2f(240, 0);
	glVertex2f(65, 30);
	glVertex2f(185, 45);
	glVertex2f(60, 52);
	glVertex2f(167, 80);
	glVertex2f(55, 70);
	glVertex2f(120, 113);
	glVertex2f(23, 88);
	glVertex2f(60, 140);
	glVertex2f(0, 100);
	//glVertex2f(20,115);
	//glVertex2f(0,148);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//bomb blast (x,-y)
	glColor3ub(255, 255, 0);
	glVertex2f(0, 0);
	glVertex2f(240, 0);
	glVertex2f(65, -30);
	glVertex2f(185, -45);
	glVertex2f(60, -52);
	glVertex2f(167, -80);
	glVertex2f(55, -70);
	glVertex2f(120, -113);
	glVertex2f(23, -88);
	glVertex2f(60, -140);
	glVertex2f(0, -100);
	//glVertex2f(20, -115);
	//glVertex2f(0, -148);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//bomb blast (-x,-y)
	glColor3ub(255, 255, 0);
	glVertex2f(0, 0);
	glVertex2f(-240, 0);
	glVertex2f(-65, -30);
	glVertex2f(-185, -45);
	glVertex2f(-60, -52);
	glVertex2f(-167, -80);
	glVertex2f(-55, -70);
	glVertex2f(-120, -113);
	glVertex2f(-23, -88);
	glVertex2f(-60, -140);
	glVertex2f(0, -100);
	//glVertex2f(-20, -115);
	//glVertex2f(0, -148);
	glVertex2f(0, 0);
	glEnd();

	glBegin(GL_POLYGON);//bomb blast (-x,y)
	glColor3ub(255, 255, 0);
	glVertex2f(0, 0);
	glVertex2f(-240, 0);
	glVertex2f(-65, 30);
	glVertex2f(-185, 45);
	glVertex2f(-60, 52);
	glVertex2f(-167, 80);
	glVertex2f(-55, 70);
	glVertex2f(-120, 113);
	glVertex2f(-23, 88);
	glVertex2f(-60, 140);
	glVertex2f(0, 100);
	//glVertex2f(-20, 115);
	//glVertex2f(0, 148);
	glVertex2f(0, 0);
	glEnd();


	glBegin(GL_POLYGON);//printig M right side
	glColor3ub(0, 0, 0);
	glVertex2f(90, 0);
	glVertex2f(90, -20);
	glVertex2f(100, -20);
	glVertex2f(100, 30);
	glVertex2f(80, 10);
	glVertex2f(80, -10);
	glVertex2f(90, 0);
	glEnd();

	glBegin(GL_POLYGON);//printig M left side
	glColor3ub(0, 0, 0);
	glVertex2f(70, 0);
	glVertex2f(80, -10);
	glVertex2f(80, 10);
	glVertex2f(60, 30);
	glVertex2f(60, -20);
	glVertex2f(70, -20);
	glVertex2f(70, 0);
	glEnd();

	makeCircle(30, -13, 0, GL_POLYGON, 0, 0, 0);//O left side
	makeCircle(20, -13, 0, GL_POLYGON, 255, 255, 255);//O left side
	makeCircle(20, 35, 0, GL_POLYGON, 0, 0, 0);//O right side
	makeCircle(10, 35, 0, GL_POLYGON, 255, 255, 255);//O right side
	makeCircle(25, -69, 22, GL_POLYGON, 0, 0, 0);//B uper circle 
	makeCircle(15, -69, 22, GL_POLYGON, 255, 255, 255);//B upuer circle 
	makeCircle(27, -69, -22, GL_POLYGON, 0, 0, 0);//B lower circle 
	makeCircle(17, -69, -22, GL_POLYGON, 255, 255, 255);//B lower circle 

	glBegin(GL_POLYGON);//printig B left side
	glColor3ub(0, 0, 0);
	glVertex2f(-67, -49);
	glVertex2f(-67, 47);
	glVertex2f(-92, 47);
	glVertex2f(-92, -49);
	glVertex2f(-67, -49);
	glEnd();

	glBegin(GL_POLYGON);//printig B left side
	glColor3ub(255, 255, 0);
	glVertex2f(-80, -49);
	glVertex2f(-80, 47);
	glVertex2f(-98, 47);
	glVertex2f(-98, -49);
	glVertex2f(-80, -49);
	glEnd();
}

//// ------------------------------ Screen---------------------------------------


void circle1(GLfloat x, GLfloat y, GLfloat radius)
{
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i<100; i++)
	{
		angle = i * 2 * (PI / 100);
		glVertex2f(x + (cos(angle)*radius), y + (sin(angle)*radius));
	}
	glEnd();
}


void tree1()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(100, 250);
	glVertex2f(175, 250);
	glVertex2f(175, 75);
	glVertex2f(100, 75);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(100, 250);
	glVertex2f(0, 250);
	glVertex2f(75, 350);
	glVertex2f(25, 350);
	glVertex2f(100, 425);
	glVertex2f(50, 425);
	glVertex2f(140, 500);
	glVertex2f(225, 425);
	glVertex2f(175, 425);
	glVertex2f(250, 350);
	glVertex2f(200, 350);
	glVertex2f(275, 250);
	glVertex2f(175, 250);
	glEnd();
}

void tree3()
{
	glColor3f(0.4, 0.19, 0.05);
	glBegin(GL_POLYGON);
	glVertex3f(100, 250, 1);
	glVertex3f(175, 250, 1);
	glVertex3f(175, 75, 1);
	glVertex3f(100, 75, 1);
	glEnd();
	glColor3f(0.0, 0.3, 0.0);

	glBegin(GL_POLYGON);
	glVertex2f(100, 250);
	glVertex2f(0, 250);
	glVertex2f(75, 350);
	glVertex2f(25, 350);
	glVertex2f(100, 425);
	glVertex2f(50, 425);
	glVertex2f(140, 500);
	glVertex2f(225, 425);
	glVertex2f(175, 425);
	glVertex2f(250, 350);
	glVertex2f(200, 350);
	glVertex2f(275, 250);
	glVertex2f(175, 250);
	glEnd();
}


void tree2()
{
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(100, 250);
	glVertex2f(175, 250);
	glVertex2f(175, 75);
	glVertex2f(100, 75);
	glEnd();
	glColor3f(0.0, 0.3, 0.01);

	glBegin(GL_POLYGON);
	glVertex2f(100, 250);
	glVertex2f(0, 250);
	glVertex2f(75, 350);
	glVertex2f(25, 350);
	glVertex2f(100, 425);
	glVertex2f(50, 425);
	glVertex2f(140, 500);
	glVertex2f(225, 425);
	glVertex2f(175, 425);
	glVertex2f(250, 350);
	glVertex2f(200, 350);
	glVertex2f(275, 250);
	glVertex2f(175, 250);
	glEnd();
}


void nightcolorchange()
{
	
	glTranslatef(-800, -600, 0);
	glScalef(1.59, 1.5, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 400);

	glVertex2f(1000, 400);
	glVertex2f(1000, 1000);
	glVertex2f(0, 1000);
	glEnd();//blue backround
	glColor3f(0.0, 0.4, 0.0);
	glBegin(GL_POLYGON);//ground color
	glVertex2f(0, 400);
	glVertex2f(1000, 400);
	glVertex2f(1000, 0);
	glVertex2f(0, 0);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	circle1(50.0, 700.0, 2.0);
	circle1(150.0, 750.0, 1.0);
	circle1(550.0, 800.0, 1.0);
	circle1(600.0, 750.0, 1.0);
	circle1(450.0, 600.0, 1.0);
	//circle1(800.0,900.0,2.0);
	circle1(400.0, 850.0, 2.0);//exxtra
							   //circle1(950.0,750.0,2.0);
	circle1(350.0, 850.0, 1.0);
	circle1(55.0, 850.0, 2.0);
	circle1(65.0, 900.0, 2.0);
	circle1(400.0, 650.0, 1.0);
	circle1(200.0, 800.0, 2.0);


	glColor3f(0.2, 0.1, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 400);//mountain
	glVertex2f(250, 700);
	glVertex2f(500, 400);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(500, 400);
	glVertex2f(750, 700);
	glVertex2f(1000, 400);
	glEnd();
	glColor3f(0.8, 0.8, 0.4);
	glBegin(GL_POLYGON);
	glVertex2f(350, 225);//*front wall*//
	glVertex2f(350, 50);
	glVertex2f(600, 50);
	glVertex2f(600, 225);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);//front roof
	glVertex2f(400, 300);
	glVertex2f(325, 225);
	glVertex2f(625, 225);
	glVertex2f(550, 300);
	glEnd();
	glColor3f(0.8, 0.8, 0.4);
	glBegin(GL_POLYGON);
	glVertex2f(400, 300);
	glVertex2f(550, 300);
	glVertex2f(550, 425);//top wall
	glVertex2f(400, 425);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(390, 475);//roof
	glVertex2f(585, 650);
	glVertex2f(585, 600);
	glVertex2f(450, 475);
	glEnd();
	glColor3f(0.2, 0.3, 0.3);//top of roof
	glBegin(GL_POLYGON);
	glVertex2f(450, 475);
	glVertex2f(585, 600);
	glVertex2f(690, 600);
	glVertex2f(560, 475);
	glEnd();
	glColor3f(0.7, 0.7, 0.32);
	glBegin(GL_POLYGON);
	glVertex2f(550, 425);
	glVertex2f(550, 300);
	glVertex2f(725, 475);//top side wall
	glVertex2f(725, 600);
	glEnd();
	glColor3f(0.7, 0.7, 0.32);
	glBegin(GL_POLYGON);
	glVertex2f(600, 225);//side wall
	glVertex2f(600, 50);
	glVertex2f(800, 250);
	glVertex2f(800, 420);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(550, 300);//side roof
	glVertex2f(725, 475);
	glVertex2f(810, 420);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);//part of side roof
	glVertex2f(600, 223);
	glVertex2f(550, 300);
	glVertex2f(810, 420);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(585, 650);
	glVertex2f(585, 600);
	glVertex2f(700, 600);
	glVertex2f(730, 650);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(390, 425);
	glVertex2f(390, 475);
	glVertex2f(560, 475);//top roof
	glVertex2f(560, 425);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(560, 475);//top roof
	glVertex2f(730, 650);
	glVertex2f(730, 600);
	glVertex2f(560, 425);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);//top roof
	glVertex2f(585, 650);
	glVertex2f(730, 650);
	glEnd();
	glColor3f(0.3, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(585, 600);//top roof
	glVertex2f(685, 600);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(425, 350);//top window
	glVertex2f(425, 400);
	glVertex2f(510, 400);
	glVertex2f(510, 350);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(425, 50);//door
	glVertex2f(425, 150);
	glVertex2f(525, 150);
	glVertex2f(525, 50);
	glEnd();


	tree3();

}

