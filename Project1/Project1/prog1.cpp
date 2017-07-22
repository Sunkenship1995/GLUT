#include <stdlib.h>
#include <gl\glut.h>
#include <Xinput.h>
#include <stdio.h>

#define W 6                        /* 地面の幅の２分の１　 */
#define D 9                        /* 地面の長さの２分の１ */

GLdouble vertex[][3] = {
	{ 0.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 }
};

int face[][4] = {
	{ 0, 1, 2, 3 },
	{ 1, 5, 6, 2 },
	{ 5, 4, 7, 6 },
	{ 4, 0, 3, 7 },
	{ 4, 5, 1, 0 },
	{ 3, 2, 6, 7 }
};

//GLdouble color[][3] = {
//	{ 1.0, 0.0, 0.0 }, /* 赤 */
//	{ 0.0, 1.0, 0.0 }, /* 緑 */
//	{ 0.0, 0.0, 1.0 }, /* 青 */
//	{ 1.0, 1.0, 0.0 }, /* 黄 */
//	{ 1.0, 0.0, 1.0 }, /* マゼンタ */
//	{ 0.0, 1.0, 1.0 }  /* シアン 　*/
//};


GLdouble normal[][3] = {
	{ 0.0, 0.0, -1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ -1.0, 0.0, 0.0 },
	{ 0.0, -1.0, 0.0 },
	{ 0.0, 1.0, 0.0 }
};

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };

static void myGround(double height)
{
	const static GLfloat ground[][4] = {
		{ 0.6, 0.6, 0.6, 1.0 },
		{ 0.3, 0.3, 0.3, 1.0 }
	};

	int i, j;

	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	for (j = -D; j < D; ++j) {
		for (i = -W; i < W; ++i) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
			glVertex3d((GLdouble)i, height, (GLdouble)j);
			glVertex3d((GLdouble)i, height, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), height, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), height, (GLdouble)j);
		}
	}
	glEnd();
}

void cube(void)
{
	int i;
	int j;

	glBegin(GL_QUADS);
	for (j = 0; j < 6; ++j) {
		glNormal3dv(normal[j]);
		for (i = 0; i < 4; ++i) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void idle(void)
{
	glutPostRedisplay();
}

void display(void)
{
	int i;
	int j;
	static int r = 0; /* 回転角 */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	/* 視点位置と視線方向 */
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	/* 光源の位置設定 */
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

	/* モデルビュー変換行列の保存 */
	glPushMatrix();

	/* 図形の回転 */
	glRotated((double)r, 0.0, 1.0, 0.0);

	/* 図形の色 (赤)  */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);

	///* 図形の描画 */
	//glColor3d(0.0, 0.0, 0.0);
	//glBegin(GL_QUADS);
	//for (j = 0; j < 6; ++j) {
	//	//glColor3dv(color[j]);
	//	glNormal3dv(normal[j]);
	//	for (i = 0; i < 4; ++i) {
	//		glVertex3dv(vertex[face[j][i]]);
	//	}
	//}
	//glEnd();


	myGround(0.0);

	/* 図形の描画 */
	cube();

	/* 二つ目の図形の描画 */
	glPushMatrix();
	glTranslated(1.0, 1.0, 1.0);
	glRotated((double)(2 * r), 0.0, 1.0, 0.0);
	glTranslatef(0.0,0.0,0.1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
	cube();
	glPopMatrix();

	/* モデルビュー変換行列の復帰 */
	glPopMatrix();

	glutSwapBuffers();

	/* 一周回ったら回転角を 0 に戻す */
	if (++r >= 360) r = 0;

}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);


	/* 透視変換行列の設定 */
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	//gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			/* アニメーション開始 */
			glutIdleFunc(idle);
		}
		else {
			/* アニメーション停止 */
			glutIdleFunc(0);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			/* コマ送り (1ステップだけ進める) */
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case '\033':  /* '\033' は ESC の ASCII コード */
		exit(0);
	default:
		break;
	}
}

void joystick(unsigned int ButtonMask, int x, int y, int z)
{
	if (300 < x)
	{
		printf("右");
	}
	if (x<-300) printf("左");
	if (300<y) printf("下");
	if (y<-300) printf("上");

	if (ButtonMask & 1) 
	{
		printf("A");
		glutIdleFunc(idle);
	}
	else {
		/* アニメーション停止 */
		glutIdleFunc(0);
	}
	if (ButtonMask & 2) printf("B");
	if (ButtonMask & 4) printf("X");
	if (ButtonMask & 8) printf("Y");
	if (ButtonMask & 16) printf("L2");
	if (ButtonMask & 32) printf("R2");
	if (ButtonMask & 64) printf("L1");
	if (ButtonMask & 128) printf("R1");
	if (ButtonMask & 256) printf("START");
	if (ButtonMask & 512) printf("SELECT");

	//glutPostRedisplay();/* 画面再描画 */
}
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT1, GL_SPECULAR, green);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutJoystickFunc(joystick, 10);
	init();
	glutMainLoop();
	return 0;
}