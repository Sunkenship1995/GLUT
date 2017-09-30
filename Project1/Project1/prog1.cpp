#include <stdlib.h>
#include <gl\glut.h>
#include <Xinput.h>
#include <stdio.h>
#include <math.h>

#define W 6                        /* 地面の幅の２分の１　 */
#define D 9                        /* 地面の長さの２分の１ */

#define WIDTH 320
#define HEIGHT 240

#define PAI 3.14159

int Mouse_X, Mouse_Y;

//　移動フラグ
bool moveRight = false;
bool moveLeft = false;
bool moveUp = false;
bool moveDown = false;

//　移動方向
static float moveX = 0.0f;
static float moveY = 0.0f;
static float moveZ = 0.0f;

//3つのベクトル
struct Vector3f{
	float x;
	float y;
	float z;
}vec3d;

int rote = 0;


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

void scene(void)
{
	/* 物体の色 */
	static GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
	static GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };
	static GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
	static GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };
	static GLfloat ground[][4] = {
		{ 0.6, 0.6, 0.6, 1.0 },
		{ 0.3, 0.3, 0.3, 1.0 }
	};

	int i, j;

	/* 赤い箱 */
	glPushMatrix();
	glTranslated(0.0, 0.0, -3.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glutSolidCube(1.0);
	glPopMatrix();

	/* 緑の箱 */
	glPushMatrix();
	glTranslated(0.0, 0.0, 3.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glutSolidCube(1.0);
	glPopMatrix();

	/* 青い箱 */
	glPushMatrix();
	glTranslated(-3.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glutSolidCube(1.0);
	glPopMatrix();

	/* 黄色い箱 */
	glPushMatrix();
	glTranslated(3.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
	glutSolidCube(1.0);
	glPopMatrix();

	/* 地面 */
	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	for (j = -5; j < 5; ++j) {
		for (i = -5; i < 5; ++i) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ground[(i + j) & 1]);
			glVertex3d((GLdouble)i, -0.5, (GLdouble)j);
			glVertex3d((GLdouble)i, -0.5, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)(j + 1));
			glVertex3d((GLdouble)(i + 1), -0.5, (GLdouble)j);
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

//クォータニオン構造体
struct Quaternion
{
	double w;
	double x;
	double y;
	double z;
};
//回転マトリックス
double Rotate[16];

Quaternion Target;
Quaternion current = {1.0, 0.0, 0.0, 0.0 };

//演算子のオーバーロード Quaternionの積
Quaternion & operator *(Quaternion &q1, Quaternion &q2)
{
	Quaternion q0 = {
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
		q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
	};
	q1 = q0;
	return q1;
}

//クォータニオンから回転行列を算出
void qtor(double *r, Quaternion q)
{
	double xx = q.x * q.x * 2.0;
	double yy = q.y * q.y * 2.0;
	double zz = q.z * q.z * 2.0;
	double xy = q.x * q.y * 2.0;
	double yz = q.y * q.z * 2.0;
	double zx = q.z * q.x * 2.0;
	double xw = q.x * q.w * 2.0;
	double yw = q.y * q.w * 2.0;
	double zw = q.z * q.w * 2.0;
	double r1[16] = { 1.0 - yy - zz, xy + zw, zx - yw, 0.0,
		xy - zw, 1.0 - zz - xx, yz + xw, 0.0,
		zx + yw, yz - xw, 1.0 - xx - yy, 0.0,
		0.0, 0.0, 0.0, 1.0 };
	for (int i = 0; i < 16; i++) {
		r[i] = r1[i];
	}
}

void display(void)
{
	int i;
	int j;
	static int r = 0; /* 回転角 */

	
	bool flag = false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	/* 視点位置と視線方向 */
	gluLookAt(moveX, 30.0 + moveY, 30.0 + moveZ,
				moveX, moveY, moveZ, 
				0.0, 1.0, 0.0);

	/* 光源の位置設定 */
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

	//クォータニオンによる回転
	glMultMatrixd(Rotate);

	/* モデルビュー変換行列の保存 */
	glPushMatrix();

	glTranslated(moveX, moveY, moveZ);
	glRotatef(rote, 0, 1, 0);

	/* 図形の色 (赤)  */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);

	/* 図形の描画 */
	cube();

	/* モデルビュー変換行列の復帰 */
	glPopMatrix();

	scene();

	glutSwapBuffers();

	/* 一周回ったら回転角を 0 に戻す */
	if (++r >= 360) r = 0;

	if (moveX >= 50)moveX = 0.0f;
	if (moveZ >= 50)moveZ = 0.0f;

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
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 50.0, 0.0);
}


//void mousemove(int x, int y)
//{
//	//移動量を計算
//	double dx = (rote - Mouse_X) * 1.33 / WIDTH;
//	//double dy = (y - Mouse_Y) * 1.0 / HEIGHT;
//
//	//クォータニオンの長さ
//	double length = sqrt(dx * dx);
//
//	if (length != 0.0) {
//		double radian = length * PAI;
//		double theta = sin(radian) / length;
//		Quaternion after = { cos(radian), 0.0, dx * theta, 0.0 };//回転後の姿勢
//
//		Target = after * current;
//
//		qtor(Rotate, Target);
//	}
//}
//void mouse(int button, int state, int x, int y)
//{
//	if (button){
//		switch (state){
//		case GLUT_DOWN://マウスボタンを押した位置を記憶
//			Mouse_X = x;
//			Mouse_Y = y;
//			break;
//		case GLUT_UP://姿勢を保存
//			current = Target;
//			break;
//		default:
//			break;
//		}
//	}
//}

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

	// 右移動
	if (300 < x)
	{
		
		glutIdleFunc(idle);
		++moveX;
	}
	else {
		moveRight = false;
		/* アニメーション停止 */
		glutIdleFunc(0);
	}

	// 左移動
	if (x<-300)
	{
		
		glutIdleFunc(idle);
		--moveX;
	}
	else {
		moveLeft = false;
		/* アニメーション停止 */
		glutIdleFunc(0);
	}

	// 前移動
	if (300<y)
	{
		
		glutIdleFunc(idle);
		++moveZ;
	}
	else {
		moveDown = false;
		/* アニメーション停止 */
		glutIdleFunc(0);
	}

	// 後ろ移動
	if (y<-300)
	{
		glutIdleFunc(idle);
		--moveZ;
	}
	else {
		moveUp = false;
		/* アニメーション停止 */
		glutIdleFunc(0);
	}

	// 左回転
	if (ButtonMask & 16)
	{
		glutIdleFunc(idle);
		printf("L2");
		rote += 1;

		//移動量を計算
		double dx = (rote - Mouse_X) * 1.33 / WIDTH;

		//クォータニオンの長さ
		double length = sqrt(dx * dx);

		if (length != 0.0) {
			double radian = length * PAI;
			double theta = sin(radian) / length;
			Quaternion after = { cos(radian), 0.0, dx * theta, 0.0 };//回転後の姿勢

			Target = after * current;

			qtor(Rotate, Target);
		}
	}
	else
	{
		/* アニメーション停止 */
		glutIdleFunc(0);
	}

	//　右回転
	if (ButtonMask & 32)
	{
		glutIdleFunc(idle);
		printf("R2");

		rote -= 1;

		//移動量を計算
		double dx = (rote - Mouse_X) * 1.33 / WIDTH;

		//クォータニオンの長さ
		double length = sqrt(dx * dx);

		if (length != 0.0) {
			double radian = length * PAI;
			double theta = sin(radian) / length;
			Quaternion after = { cos(radian), 0.0, dx * theta, 0.0 };//回転後の姿勢

			Target = after * current;

			qtor(Rotate, Target);
		}
		
	}
	else
	{
		/* アニメーション停止 */
		glutIdleFunc(0);
	}




	if (ButtonMask & 1) 
	{
	}
	if (ButtonMask & 2) printf("B");
	if (ButtonMask & 4) printf("X");
	if (ButtonMask & 8) printf("Y");
	
	if (ButtonMask & 64) printf("L1");
	if (ButtonMask & 128) printf("R1");
	if (ButtonMask & 256) printf("START");
	if (ButtonMask & 512) printf("SELECT");


	glutPostRedisplay();/* 画面再描画 */
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

	//任意軸のベクトルを設定
	vec3d.x = 0.0f;
	vec3d.y = 0.0f;
	vec3d.z = 90.0f;

	qtor(Rotate, current);
}

int main(int argc, char *argv[])
{
	glutInitWindowSize(640, 480);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	//glutMouseFunc(mouse);
	//glutMotionFunc(mousemove);
	glutKeyboardFunc(keyboard);
	glutJoystickFunc(joystick, 10);
	init();
	glutMainLoop();
	return 0;
}