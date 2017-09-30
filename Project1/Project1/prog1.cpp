#include <stdlib.h>
#include <gl\glut.h>
#include <Xinput.h>
#include <stdio.h>
#include <math.h>

#define W 6                        /* �n�ʂ̕��̂Q���̂P�@ */
#define D 9                        /* �n�ʂ̒����̂Q���̂P */

#define WIDTH 320
#define HEIGHT 240

#define PAI 3.14159

int Mouse_X, Mouse_Y;

//�@�ړ��t���O
bool moveRight = false;
bool moveLeft = false;
bool moveUp = false;
bool moveDown = false;

//�@�ړ�����
static float moveX = 0.0f;
static float moveY = 0.0f;
static float moveZ = 0.0f;

//3�̃x�N�g��
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
//	{ 1.0, 0.0, 0.0 }, /* �� */
//	{ 0.0, 1.0, 0.0 }, /* �� */
//	{ 0.0, 0.0, 1.0 }, /* �� */
//	{ 1.0, 1.0, 0.0 }, /* �� */
//	{ 1.0, 0.0, 1.0 }, /* �}�[���^ */
//	{ 0.0, 1.0, 1.0 }  /* �V�A�� �@*/
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
	/* ���̂̐F */
	static GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
	static GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };
	static GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
	static GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };
	static GLfloat ground[][4] = {
		{ 0.6, 0.6, 0.6, 1.0 },
		{ 0.3, 0.3, 0.3, 1.0 }
	};

	int i, j;

	/* �Ԃ��� */
	glPushMatrix();
	glTranslated(0.0, 0.0, -3.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glutSolidCube(1.0);
	glPopMatrix();

	/* �΂̔� */
	glPushMatrix();
	glTranslated(0.0, 0.0, 3.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glutSolidCube(1.0);
	glPopMatrix();

	/* ���� */
	glPushMatrix();
	glTranslated(-3.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glutSolidCube(1.0);
	glPopMatrix();

	/* ���F���� */
	glPushMatrix();
	glTranslated(3.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
	glutSolidCube(1.0);
	glPopMatrix();

	/* �n�� */
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

//�N�H�[�^�j�I���\����
struct Quaternion
{
	double w;
	double x;
	double y;
	double z;
};
//��]�}�g���b�N�X
double Rotate[16];

Quaternion Target;
Quaternion current = {1.0, 0.0, 0.0, 0.0 };

//���Z�q�̃I�[�o�[���[�h Quaternion�̐�
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

//�N�H�[�^�j�I�������]�s����Z�o
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
	static int r = 0; /* ��]�p */

	
	bool flag = false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	/* ���_�ʒu�Ǝ������� */
	gluLookAt(moveX, 30.0 + moveY, 30.0 + moveZ,
				moveX, moveY, moveZ, 
				0.0, 1.0, 0.0);

	/* �����̈ʒu�ݒ� */
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

	//�N�H�[�^�j�I���ɂ���]
	glMultMatrixd(Rotate);

	/* ���f���r���[�ϊ��s��̕ۑ� */
	glPushMatrix();

	glTranslated(moveX, moveY, moveZ);
	glRotatef(rote, 0, 1, 0);

	/* �}�`�̐F (��)  */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);

	/* �}�`�̕`�� */
	cube();

	/* ���f���r���[�ϊ��s��̕��A */
	glPopMatrix();

	scene();

	glutSwapBuffers();

	/* �����������]�p�� 0 �ɖ߂� */
	if (++r >= 360) r = 0;

	if (moveX >= 50)moveX = 0.0f;
	if (moveZ >= 50)moveZ = 0.0f;

}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);


	/* �����ϊ��s��̐ݒ� */
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	//gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	/* ���f���r���[�ϊ��s��̐ݒ� */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 50.0, 0.0);
}


//void mousemove(int x, int y)
//{
//	//�ړ��ʂ��v�Z
//	double dx = (rote - Mouse_X) * 1.33 / WIDTH;
//	//double dy = (y - Mouse_Y) * 1.0 / HEIGHT;
//
//	//�N�H�[�^�j�I���̒���
//	double length = sqrt(dx * dx);
//
//	if (length != 0.0) {
//		double radian = length * PAI;
//		double theta = sin(radian) / length;
//		Quaternion after = { cos(radian), 0.0, dx * theta, 0.0 };//��]��̎p��
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
//		case GLUT_DOWN://�}�E�X�{�^�����������ʒu���L��
//			Mouse_X = x;
//			Mouse_Y = y;
//			break;
//		case GLUT_UP://�p����ۑ�
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
	case '\033':  /* '\033' �� ESC �� ASCII �R�[�h */
		exit(0);
	default:
		break;
	}
}

void joystick(unsigned int ButtonMask, int x, int y, int z)
{

	// �E�ړ�
	if (300 < x)
	{
		
		glutIdleFunc(idle);
		++moveX;
	}
	else {
		moveRight = false;
		/* �A�j���[�V������~ */
		glutIdleFunc(0);
	}

	// ���ړ�
	if (x<-300)
	{
		
		glutIdleFunc(idle);
		--moveX;
	}
	else {
		moveLeft = false;
		/* �A�j���[�V������~ */
		glutIdleFunc(0);
	}

	// �O�ړ�
	if (300<y)
	{
		
		glutIdleFunc(idle);
		++moveZ;
	}
	else {
		moveDown = false;
		/* �A�j���[�V������~ */
		glutIdleFunc(0);
	}

	// ���ړ�
	if (y<-300)
	{
		glutIdleFunc(idle);
		--moveZ;
	}
	else {
		moveUp = false;
		/* �A�j���[�V������~ */
		glutIdleFunc(0);
	}

	// ����]
	if (ButtonMask & 16)
	{
		glutIdleFunc(idle);
		printf("L2");
		rote += 1;

		//�ړ��ʂ��v�Z
		double dx = (rote - Mouse_X) * 1.33 / WIDTH;

		//�N�H�[�^�j�I���̒���
		double length = sqrt(dx * dx);

		if (length != 0.0) {
			double radian = length * PAI;
			double theta = sin(radian) / length;
			Quaternion after = { cos(radian), 0.0, dx * theta, 0.0 };//��]��̎p��

			Target = after * current;

			qtor(Rotate, Target);
		}
	}
	else
	{
		/* �A�j���[�V������~ */
		glutIdleFunc(0);
	}

	//�@�E��]
	if (ButtonMask & 32)
	{
		glutIdleFunc(idle);
		printf("R2");

		rote -= 1;

		//�ړ��ʂ��v�Z
		double dx = (rote - Mouse_X) * 1.33 / WIDTH;

		//�N�H�[�^�j�I���̒���
		double length = sqrt(dx * dx);

		if (length != 0.0) {
			double radian = length * PAI;
			double theta = sin(radian) / length;
			Quaternion after = { cos(radian), 0.0, dx * theta, 0.0 };//��]��̎p��

			Target = after * current;

			qtor(Rotate, Target);
		}
		
	}
	else
	{
		/* �A�j���[�V������~ */
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


	glutPostRedisplay();/* ��ʍĕ`�� */
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

	//�C�ӎ��̃x�N�g����ݒ�
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