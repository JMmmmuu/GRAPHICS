#define _CRT_SECURE_NO_WARNINGS

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

#include "geomety.h"

#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> //inverse, affineInverse, etc.


#include "camera.h"

glm::mat4 ViewProjectionMatrix[NUMBER_OF_CAMERAS], ViewMatrix[NUMBER_OF_CAMERAS], ProjectionMatrix[NUMBER_OF_CAMERAS];
glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when it is ready.


VIEW_MODE view_mode;

void display_camera(int camera_index) {

	glViewport(viewport[camera_index].x, viewport[camera_index].y, viewport[camera_index].w, viewport[camera_index].h);

	//ViewProjectionMatrix[0] = glm::rotate(ViewProjectionMatrix[0], 90 * TO_RADIAN, glm::vec3(1, 0, 0));
	//ViewProjectionMatrix[0] = glm::rotate(ViewProjectionMatrix[0], -90 * TO_RADIAN, glm::vec3(1, 0, 0));
	// At this point, the matrix ViewProjectionMatrix has been properly set up.

	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[camera_index], glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes(); // draw the WC axes.
	glLineWidth(1.0f);


//	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(-1.5f, 0.0f, -2.0f));
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(0, 0, 0));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(300, 300, 300));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(-1.5f, 0.0f, 2.0f));

	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, -90 * TO_RADIAN, glm::vec3(1, 0, 0));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_plane();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);





//	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[camera_index], glm::vec3(2.0f, 2.0f, 2.0f));
	//glUniform
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	display_camera(0);
	//display_camera(1);
	glutSwapBuffers();


}

void timer_scene(int value) {

	glutPostRedisplay();
	//glutTimerFunc(100, timer_scene, (timestamp_scene + 1) % INT_MAX);
}

void motion(int x, int y) {

}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		/*case 'f':
			flag_fog = 1 - flag_fog;
			glUseProgram(h_ShaderProgram_TXPS);
			glUniform1i(loc_flag_fog, flag_fog);
			glUseProgram(0);
			glutPostRedisplay();
			break;*/
	case 27: // ESC key
		glutLeaveMainLoop();
		break;
	}
}

void reshape(int width, int height) {
	camera[0].aspect_ratio = (float)width / height;
	viewport[0].x = viewport[0].y = 0;
	viewport[0].w = (int)(width); viewport[0].h = (int)(height);
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio,
		camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

	camera[1].aspect_ratio = camera[0].aspect_ratio; // for the time being ...
	viewport[1].x = (int)(0.75f*width); viewport[1].y = (int)(0.75f*height);
	viewport[1].w = (int)(0.25f*width); viewport[1].h = (int)(0.25*height);
	ProjectionMatrix[1] = glm::perspective(camera[1].fov_y*TO_RADIAN, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
	ViewProjectionMatrix[1] = ProjectionMatrix[1] * ViewMatrix[1];
	
	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &plane_VAO);
	glDeleteBuffers(1, &plane_VBO);

}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//glutSpecialFunc(special);
	//glutMouseFunc(mousepress);
	//glutMotionFunc(motion_1);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
}


#include "intialize.h"

#define N_MESSAGE_LINES 1
int main(int argc, char* argv[]) {
	char program_name[64] = "20171690 Yuseok - 3D Objects";
	char messages[N_MESSAGE_LINES][256] = { "\tSogang CSE4170 3d Objects\n\t20171690\n\tYuseok\n\n\t- Key used: \n" };


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();

	return 0;
}