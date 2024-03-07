#include <GL/freeglut_std.h>
#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

#include <cstdlib>

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

float tx = 0.0f;
float ty = 0.0f;
float tz = 0.0f;

float rx = 0.0f;
float ry = 0.0f;
float rz = 0.0f;

float sx = 1.0f;
float sy = 1.0f;
float sz = 1.0f;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void drawPlane(float l, int d) {
    
    float x = l/2;
    float z = -l/2;
    float diff = l/d;

    glBegin(GL_TRIANGLES);
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++) {
            
            glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
            glVertex3f(x, 0, z+diff);
            glVertex3f(x, 0, z);
            glVertex3f(x-diff, 0, z);
            
            glVertex3f(x-diff, 0, z);
            glVertex3f(x-diff, 0, z+diff);
            glVertex3f(x, 0, z+diff);

            x -= diff;
        }
        x = l/2;
        z += diff;
    }
    glEnd();
}

void drawBox(float l, int d) {

    float x = l/2;
    float y = l/2;
    float z = -l/2;
    float diff = l/d;

    glBegin(GL_TRIANGLES);
    // Bottom and Top Faces
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++) {
            glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

            glVertex3f(x, -y, z+diff);
            glVertex3f(x, -y, z);
            glVertex3f(x-diff, -y, z);

            glVertex3f(x-diff, -y, z);
            glVertex3f(x-diff, -y, z+diff);
            glVertex3f(x, -y, z+diff);

            glVertex3f(x, y, z+diff);
            glVertex3f(x, y, z);
            glVertex3f(x-diff, y, z);

            glVertex3f(x-diff, y, z);
            glVertex3f(x-diff, y, z+diff);
            glVertex3f(x, y, z+diff);

            x -= diff;
        }
        x = l/2;
        z += diff;
    }

    x = l/2;
    z = -l/2;

    // Right and Left faces
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++) {
            glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

            glVertex3f(x, y, z);
            glVertex3f(x, y-diff, z+diff);
            glVertex3f(x, y-diff, z);

            glVertex3f(x, y, z);
            glVertex3f(x, y, z+diff);
            glVertex3f(x, y-diff, z+diff);

            glVertex3f(-x, y, z);
            glVertex3f(-x, y-diff, z+diff);
            glVertex3f(-x, y-diff, z);

            glVertex3f(-x, y, z);
            glVertex3f(-x, y, z+diff);
            glVertex3f(-x, y-diff, z+diff);

            z += diff;
        }
        z = -l/2;
        y -= diff;
    }
    
    y = l/2;
    z = l/2;

    // Front and Back faces
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++) {
            glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

            glVertex3f(x, y, z);
            glVertex3f(x-diff, y-diff, z);
            glVertex3f(x, y-diff, z);

            glVertex3f(x, y, z);
            glVertex3f(x-diff, y, z);
            glVertex3f(x-diff, y-diff, z);

            glVertex3f(x, y, -z);
            glVertex3f(x-diff, y-diff, -z);
            glVertex3f(x, y-diff, -z);

            glVertex3f(x, y, z);
            glVertex3f(x-diff, y, -z);
            glVertex3f(x-diff, y-diff, -z);

            x -= diff;
        }
        x = l/2;
        y -= diff;
    }
    glEnd();
}


void drawCone(float radius, float height, int slices, int stacks) {
    
    float y=0;
    float angle = 0;
    float cur_rad = radius;
    float angle_diff = 2*M_PI/slices;
    float xz_diff = radius/stacks;
    float y_diff = height/stacks;
    
    glBegin(GL_TRIANGLES);
    for(int i=0; i<stacks; i++) {
        for(int j=0; j<slices; j++) {
            glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
            // Bottom face
            if (i==0) {
                glVertex3f(cur_rad*sin(angle+angle_diff), 0, cur_rad*cos(angle+angle_diff));
                glVertex3f(0,0,0);
                glVertex3f(cur_rad*sin(angle), 0, cur_rad*cos(angle));
            }
           
            if (i==stacks-1){              
                glVertex3f(cur_rad*sin(angle+angle_diff), y, cur_rad*cos(angle+angle_diff));
                glVertex3f(0, height, 0);
                glVertex3f(cur_rad*sin(angle), y, cur_rad*cos(angle));
            }
            else {
                glVertex3f((cur_rad-xz_diff)*sin(angle+angle_diff), y+y_diff, (cur_rad-xz_diff)*cos(angle+angle_diff));
                glVertex3f((cur_rad-xz_diff)*sin(angle), y+y_diff, (cur_rad-xz_diff)*cos(angle));
                glVertex3f(cur_rad*sin(angle+angle_diff), y, cur_rad*cos(angle+angle_diff));

                glVertex3f((cur_rad-xz_diff)*sin(angle), y+y_diff, (cur_rad-xz_diff)*cos(angle));                
                glVertex3f(cur_rad*sin(angle), y, cur_rad*cos(angle));
                glVertex3f(cur_rad*sin(angle+angle_diff), y, cur_rad*cos(angle+angle_diff));
            }
            angle += angle_diff;
        }
        angle = 0;
        y += y_diff;
        cur_rad -= xz_diff;
    }
    glEnd();
}


void drawSphere(float radius, int slices, int stacks) {
	float px, py, pz;
	float alfa_diff = 2 * M_PI / slices;
	float beta_diff = M_PI / stacks;
	float alfa = 0;
	float beta = 0;
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {
			if (j != 0) {
				px = radius * cos(beta - M_PI_2) * cos(alfa);
				py = radius * sin(beta - M_PI_2);
				pz = radius * cos(beta - M_PI_2) * sin(alfa);
				glVertex3f(px, py, pz);
				px = radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa + alfa_diff);
				py = radius * sin(beta - M_PI_2 + beta_diff);
				pz = radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa + alfa_diff);
				glVertex3f(px, py, pz);
				px = radius * cos(beta - M_PI_2) * cos(alfa + alfa_diff);
				py = radius * sin(beta - M_PI_2);
				pz = radius * cos(beta - M_PI_2) * sin(alfa + alfa_diff);
				glVertex3f(px, py, pz);
			}
			if (j != stacks - 1) {
				px = radius * cos(beta - M_PI_2) * cos(alfa);
				py = radius * sin(beta - M_PI_2);
				pz = radius * cos(beta - M_PI_2) * sin(alfa);
				glVertex3f(px, py, pz);
				px = radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa);
				py = radius * sin(beta - M_PI_2 + beta_diff);
				pz = radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa);
				glVertex3f(px, py, pz);
				px = radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa + alfa_diff);
				py = radius * sin(beta - M_PI_2 + beta_diff);
				pz = radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa + alfa_diff);
				glVertex3f(px, py, pz);
			}
			beta += beta_diff;
		}
		beta = 0;
		alfa += alfa_diff;
	}
	glEnd();
}


void drawTorus(float inner_radius, float outer_radius, int slices, int stacks) {

	float alfa = 0, beta = 0;
	float alfa_diff = 2 * M_PI / slices;
	float beta_diff = 2*M_PI / stacks;
	float px, py, pz;

	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {

			px = (inner_radius + outer_radius * cos(beta -M_PI_2)) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa);
			glVertex3f(px, py, pz);
			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2+ beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			glVertex3f(px, py, pz);
			px = (inner_radius + outer_radius * cos(beta - M_PI_2 )) * cos(alfa+alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2);
			pz= (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa+alfa_diff);
			glVertex3f(px, py, pz);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			py =  outer_radius * sin(beta - M_PI_2 + beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			glVertex3f(px, py, pz);
			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa + alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2 + alfa_diff);
			pz = (inner_radius +outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa + alfa_diff);
			glVertex3f(px, py, pz);
			px = (inner_radius + outer_radius * cos(beta - M_PI_2)) * cos(alfa + alfa_diff);
			py = outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa + alfa_diff);
			glVertex3f(px, py, pz);

			beta += beta_diff;
		}
		beta = 0;
		alfa += alfa_diff;
	}
}

void drawHeart() {
    glPointSize(2.0);
    glBegin(GL_TRIANGLES);

    for (float t = 0.0; t <= 2 * M_PI; t += 0.01) {
        float x = 16 * pow(sin(t), 3);
        float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
        float z = 0.0;  // Adjust as needed

        glVertex3f(x, y, z);
        glVertex3f(x + 0.1, y, z);  // Creating a triangle, adjust the size as needed
        glVertex3f(x, y + 0.1, z);
    }

    glEnd();
}

void keys (unsigned char key, int x, int y){
	switch (key) {
		case 'w':
			tz -= 0.05 * sz;
			glutPostRedisplay();
			break;
		case 'a':
			tx -= 0.05 * sx;
			glutPostRedisplay();
			break;
		case 's':
			tz += 0.05 * sz;
			glutPostRedisplay();
			break;
		case 'd':
			tx += 0.05 * sx;
			glutPostRedisplay();
			break;
		case 'h':
			ty += 0.05 * sy;
			glutPostRedisplay();
			break;
		case 'j':
			ty -= 0.05 * sy;
			glutPostRedisplay();
			break;
		case 'x':
			rx -= 1;
			glutPostRedisplay();
			break;
		case 'c':
			rx += 1;
			glutPostRedisplay();
			break;
		case 'v':
			ry -= 1;
			glutPostRedisplay();
			break;
		case 'b':
			ry += 1;
			glutPostRedisplay();
			break;
		case 'n':
			rz -= 1;
			glutPostRedisplay();
			break;
		case 'm':
			rz += 1;
			glutPostRedisplay();
			break;
		case 't':
			sx += 0.5;
			glutPostRedisplay();
			break;
		case 'y':
			sx -= 0.5;
			glutPostRedisplay();
			break;
		case 'u':
			sy += 0.5;
			glutPostRedisplay();
			break;
		case 'i':
			sy -= 0.5;
			glutPostRedisplay();
			break;
		case 'o':
			sz += 0.5;
			glutPostRedisplay();
			break;
		case 'p':
			sz -= 0.5;
			glutPostRedisplay();
			break; 
	}
}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

    drawHeart();

    glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f,0.0f, 0.0f);

	// Y axis in green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	// Z axis in blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
    
    glEnd();


	// End of frame
	glutSwapBuffers();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

// Keyboard callbacks
    glPolygonMode(GL_FRONT, GL_LINE);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(keys);

	
//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}

