#include "openGL_tutorial.h"

#include <iostream>
#include <vector>

const int WINDOW_W = 500;
const int WINDOW_H = 500;

using namespace std;

class Ponto
{
public:
	float x, y;
	Ponto(float x, float y) : x(x), y(y) {};

};

vector <Ponto> pontos;

double fatorial(int n)
{
	double resposta;

	if (n <= 1)
	{
		resposta = 1;
	}
	else
	{
		resposta = n;

		while (n > 1)
		{
			resposta *= (n - 1);
			n--;
		}
	}

	return resposta;
}

double coeficiente_binomial(int n, int k)
{
	double resposta;
	resposta = fatorial (n) / (fatorial (k) * fatorial (n - k));
	return resposta;
}

Ponto desenhaBezier(double t) {
	Ponto p(0.0f, 0.0f);
	int quantidade = pontos.size();
	for (int i = 0; i < quantidade; i++)
	{
		double aux = coeficiente_binomial((quantidade - 1), i) * pow((1 - t), (quantidade - 1 - i)) * pow(t, i);
		p.x = p.x + (aux * pontos.at(i).x);
		p.y = p.y + (aux * pontos.at(i).y);
	}
	return p;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if (pontos.size() > 0)
	{
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f);
		for (int i = 0; i < pontos.size(); i++)
			glVertex2d(pontos.at(i).x, pontos.at(i).y);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 1.0f, 1.0f);

		double t;

		for (t = 0.0; t <= 1.0; t += 0.01)
		{
			glVertex2d(desenhaBezier(t).x, desenhaBezier(t).y);
		}

		glVertex2d(desenhaBezier(t).x, desenhaBezier(t).y);

		glEnd();
	}

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, WINDOW_W, WINDOW_H, 0.0f, -5.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress(unsigned char key, int x, int y)
{
	static int x1 = 0;
	switch (key){
	case 27: // ESC
		exit(0);
		break;
	case 119:
		glLineWidth(5.0f);
		pontos.push_back(Ponto(x, y));
		glutPostRedisplay();
		x1++;
		break;
	}

}

int indicePonto(int x, int y) {
	int resposta = -1; // caso nao haja um ponto no vetor
	for (int i = 0; i < pontos.size(); i++){
		if ((pontos.at(i).x <= (x + 5)) && (pontos.at(i).x >= (x - 5)) && (pontos.at(i).y <= (y + 5)) && (pontos.at(i).y >= (y - 5))){
			return i;
		}
	}
	return resposta;
}

int i;

void handleMouseClick(int button, int state, int x, int y)
{
	i = indicePonto(x, y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if (i == -1){
			pontos.push_back(Ponto(x, y));
			i = pontos.size() - 1;
		}
	}
	glutPostRedisplay();  // avisa que a janela atual deve ser reimpressa
}

void drag(int x, int y){
	if (i > -1){
		pontos.at(i) = Ponto(x, y);
	}
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0); // a janela irá iniciar to topo esquerdo
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutCreateWindow("Exemplo OpenGL");
	glMatrixMode(GL_MODELVIEW); // altera a matrix do modelo da cena
	glLoadIdentity();

	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(handleMouseClick);
	glutMotionFunc(drag);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}