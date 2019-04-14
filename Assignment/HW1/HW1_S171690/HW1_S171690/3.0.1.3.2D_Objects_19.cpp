#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

														  // include glm/*.hpp only if necessary
														  //#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

int win_width = 1200, win_height = 700;
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;

GLfloat axes[4][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.5f; axes[0][1] = 0.0f;
	axes[1][0] = win_width / 2.5f; axes[1][1] = 0.0f;
	axes[2][0] = 0.0f; axes[2][1] = -win_height / 2.5f;
	axes[3][0] = 0.0f; axes[3][1] = win_height / 2.5f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.25f; axes[1][0] = win_width / 2.25f;
	axes[2][1] = -win_height / 2.25f;
	axes[3][1] = win_height / 2.25f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

GLfloat line[2][2];
GLfloat line_color[3] = { 1.0f, 0.0f, 0.0f };
GLuint VBO_line, VAO_line;

void prepare_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height;
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f;
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_line);
	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height;
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f;
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_line(void) { // Draw line in its MC.
					   // y = x - win_height/4
	glUniform3fv(loc_primitive_color, 1, line_color);
	glBindVertexArray(VAO_line);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

#define FACE 0
#define FACE_LEFT_EYE 1
#define FACE_RIGHT_EYE 2
#define FACE_MOUTH 3
GLfloat face[360][2];
GLfloat left_eye[4][2] = { { -15.0, 7.0 }, { -15.0, 12.0 }, { -5.0, 12.0 }, { -5.0, 7.0 } };
GLfloat right_eye[4][2] = { { 5.0, 7.0 },{ 5.0, 12.0 },{ 15.0, 12.0 },{ 15.0, 7.0 } };
GLfloat mouth[4][2] = { { -10.0, -16.0 }, { -10.0, -10.0 }, { 10.0, -10.0 }, { 10.0, -16.0 } };
GLfloat face_colors[4][3] = {
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
};

GLuint VBO_face, VAO_face;

void prepare_face() {
	GLsizeiptr buffer_size = sizeof(face) + sizeof(left_eye) + sizeof(right_eye) + sizeof(mouth);

	for (int i = 0; i < 360; i++) {
		face[i][0] = 30 * cos(i);
		face[i][1] = 30 * sin(i);
	}

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_face);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_face);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(face), face);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(face), sizeof(left_eye), left_eye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(face) + sizeof(left_eye), sizeof(right_eye), right_eye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(face) + sizeof(left_eye) + sizeof(right_eye), sizeof(mouth), mouth);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_face);
	glBindVertexArray(VAO_face);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_face);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_face() { // Draw airplane in its MC.
	glBindVertexArray(VAO_face);

	glUniform3fv(loc_primitive_color, 1, face_colors[FACE]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 360);

	glUniform3fv(loc_primitive_color, 1, face_colors[FACE_LEFT_EYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 360, 4);

	glUniform3fv(loc_primitive_color, 1, face_colors[FACE_RIGHT_EYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 364, 4);

	glUniform3fv(loc_primitive_color, 1, face_colors[FACE_MOUTH]);
	glDrawArrays(GL_TRIANGLE_FAN, 368, 4);

	glBindVertexArray(0);
}

// ANGRY
#define FACE_ANGRY 0
#define FACE_ANGRY_LEFT_EYE 1
#define FACE_ANGRY_RIGHT_EYE 2
#define FACE_ANGRY_MOUTH 3
GLfloat face_angry[360][2];
GLfloat angry_left_eye[4][2] = { { -14.0, 12.0 },{ -10.5, 15.5 },{ -1.0, 5.5 },{ -4.5, 2.0 } };
GLfloat angry_right_eye[4][2] = { { 4.0, 5.5 },{ 7.5, 2.0 },{ 16.0, 12.0 },{ 12.5, 15.5 } };
GLfloat angry_mouth[4][2] = { { -10.0, -16.0 },{ -10.0, -10.0 },{ 10.0, -10.0 },{ 10.0, -16.0 } };
GLfloat face_angry_colors[4][3] = {
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
};

GLuint VBO_face_angry, VAO_face_angry;

void prepare_face_angry() {
	GLsizeiptr buffer_size = sizeof(face_angry) + sizeof(angry_left_eye) + sizeof(angry_right_eye) + sizeof(angry_mouth);

	for (int i = 0; i < 360; i++) {
		face_angry[i][0] = 30 * cos(i);
		face_angry[i][1] = 30 * sin(i);
	}

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_face_angry);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_face_angry);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(face_angry), face_angry);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(face_angry), sizeof(angry_left_eye), angry_left_eye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(face_angry) + sizeof(angry_left_eye), sizeof(angry_right_eye), angry_right_eye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(face_angry) + sizeof(angry_left_eye) + sizeof(angry_right_eye), sizeof(angry_mouth), angry_mouth);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_face_angry);
	glBindVertexArray(VAO_face_angry);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_face_angry);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_face_angry() { // Draw airplane in its MC.
	glBindVertexArray(VAO_face_angry);

	glUniform3fv(loc_primitive_color, 1, face_angry_colors[FACE_ANGRY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 360);

	glUniform3fv(loc_primitive_color, 1, face_angry_colors[FACE_ANGRY_LEFT_EYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 360, 4);

	glUniform3fv(loc_primitive_color, 1, face_angry_colors[FACE_ANGRY_RIGHT_EYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 364, 4);

	glUniform3fv(loc_primitive_color, 1, face_angry_colors[FACE_ANGRY_MOUTH]);
	glDrawArrays(GL_TRIANGLE_FAN, 368, 4);

	glBindVertexArray(0);
}

#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6
GLfloat big_wing[6][2] = { { 0.0, 0.0 },{ -20.0, 15.0 },{ -20.0, 20.0 },{ 0.0, 23.0 },{ 20.0, 20.0 },{ 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 },{ -11.0, -12.0 },{ -12.0, -7.0 },{ 0.0, -10.0 },{ 12.0, -7.0 },{ 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 },{ -6.0, 0.0 },{ -6.0, 22.0 },{ 6.0, 22.0 },{ 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 },{ -7.0, 24.0 },{ -7.0, 21.0 },{ 7.0, 21.0 },{ 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 },{ -18.0, 3.0 },{ -16.0, 10.0 },{ -18.0, 20.0 },{ -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 },{ 18.0, 3.0 },{ 16.0, 10.0 },{ 18.0, 20.0 },{ 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

int airplane_clock = 0;
float airplane_s_factor = 1.0f;

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2) + sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

//car
#define CAR_BODY 0
#define CAR_FRAME 1
#define CAR_WINDOW 2
#define CAR_LEFT_LIGHT 3
#define CAR_RIGHT_LIGHT 4
#define CAR_LEFT_WHEEL 5
#define CAR_RIGHT_WHEEL 6

GLfloat car_body[4][2] = { { -16.0, -8.0 },{ -16.0, 0.0 },{ 16.0, 0.0 },{ 16.0, -8.0 } };
GLfloat car_frame[4][2] = { { -10.0, 0.0 },{ -10.0, 10.0 },{ 10.0, 10.0 },{ 10.0, 0.0 } };
GLfloat car_window[4][2] = { { -8.0, 0.0 },{ -8.0, 8.0 },{ 8.0, 8.0 },{ 8.0, 0.0 } };
GLfloat car_left_light[4][2] = { { -9.0, -6.0 },{ -10.0, -5.0 },{ -9.0, -4.0 },{ -8.0, -5.0 } };
GLfloat car_right_light[4][2] = { { 9.0, -6.0 },{ 8.0, -5.0 },{ 9.0, -4.0 },{ 10.0, -5.0 } };
GLfloat car_left_wheel[4][2] = { { -10.0, -12.0 },{ -10.0, -8.0 },{ -6.0, -8.0 },{ -6.0, -12.0 } };
GLfloat car_right_wheel[4][2] = { { 6.0, -12.0 },{ 6.0, -8.0 },{ 10.0, -8.0 },{ 10.0, -12.0 } };

GLfloat car_color[7][3] = {
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f }
};

GLuint VBO_car, VAO_car;
void prepare_car() {
	GLsizeiptr buffer_size = sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel) + sizeof(car_right_wheel);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car_body), car_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body), sizeof(car_frame), car_frame);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame), sizeof(car_window), car_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window), sizeof(car_left_light), car_left_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light),
		sizeof(car_right_light), car_right_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light), sizeof(car_left_wheel), car_left_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel), sizeof(car_right_wheel), car_right_wheel);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car);
	glBindVertexArray(VAO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car() {
	glBindVertexArray(VAO_car);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_FRAME]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glBindVertexArray(0);
}

//draw cocktail
#define COCKTAIL_NECK 0
#define COCKTAIL_LIQUID 1
#define COCKTAIL_REMAIN 2
#define COCKTAIL_STRAW 3
#define COCKTAIL_DECO 4

GLfloat neck[6][2] = { { -6.0, -12.0 },{ -6.0, -11.0 },{ -1.0, 0.0 },{ 1.0, 0.0 },{ 6.0, -11.0 },{ 6.0, -12.0 } };
GLfloat liquid[6][2] = { { -1.0, 0.0 },{ -9.0, 4.0 },{ -12.0, 7.0 },{ 12.0, 7.0 },{ 9.0, 4.0 },{ 1.0, 0.0 } };
GLfloat remain[4][2] = { { -12.0, 7.0 },{ -12.0, 10.0 },{ 12.0, 10.0 },{ 12.0, 7.0 } };
GLfloat straw[4][2] = { { 7.0, 7.0 },{ 12.0, 12.0 },{ 14.0, 12.0 },{ 9.0, 7.0 } };
GLfloat deco[8][2] = { { 12.0, 12.0 },{ 10.0, 14.0 },{ 10.0, 16.0 },{ 12.0, 18.0 },{ 14.0, 18.0 },{ 16.0, 16.0 },{ 16.0, 14.0 },{ 14.0, 12.0 } };

GLfloat cocktail_color[5][3] = {
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 191 / 255.0f, 255 / 255.0f, 0 / 255.0f },
	{ 218 / 255.0f, 165 / 255.0f, 32 / 255.0f }
};

GLuint VBO_cocktail, VAO_cocktail;
void prepare_cocktail() {
	GLsizeiptr buffer_size = sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw)
		+ sizeof(deco);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(neck), neck);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck), sizeof(liquid), liquid);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid), sizeof(remain), remain);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain), sizeof(straw), straw);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw),
		sizeof(deco), deco);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cocktail);
	glBindVertexArray(VAO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cocktail() {
	glBindVertexArray(VAO_cocktail);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_NECK]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_LIQUID]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_REMAIN]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_STRAW]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_DECO]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 8);

	glBindVertexArray(0);
}

//draw car2
#define CAR2_BODY 0
#define CAR2_FRONT_WINDOW 1
#define CAR2_BACK_WINDOW 2
#define CAR2_FRONT_WHEEL 3
#define CAR2_BACK_WHEEL 4
#define CAR2_LIGHT1 5
#define CAR2_LIGHT2 6

GLfloat car2_body[8][2] = { { -18.0, -7.0 },{ -18.0, 0.0 },{ -13.0, 0.0 },{ -10.0, 8.0 },{ 10.0, 8.0 },{ 13.0, 0.0 },{ 18.0, 0.0 },{ 18.0, -7.0 } };
GLfloat car2_front_window[4][2] = { { -10.0, 0.0 },{ -8.0, 6.0 },{ -2.0, 6.0 },{ -2.0, 0.0 } };
GLfloat car2_back_window[4][2] = { { 0.0, 0.0 },{ 0.0, 6.0 },{ 8.0, 6.0 },{ 10.0, 0.0 } };
GLfloat car2_front_wheel[8][2] = { { -11.0, -11.0 },{ -13.0, -8.0 },{ -13.0, -7.0 },{ -11.0, -4.0 },{ -7.0, -4.0 },{ -5.0, -7.0 },{ -5.0, -8.0 },{ -7.0, -11.0 } };
GLfloat car2_back_wheel[8][2] = { { 7.0, -11.0 },{ 5.0, -8.0 },{ 5.0, -7.0 },{ 7.0, -4.0 },{ 11.0, -4.0 },{ 13.0, -7.0 },{ 13.0, -8.0 },{ 11.0, -11.0 } };
GLfloat car2_light1[3][2] = { { -18.0, -1.0 },{ -17.0, -2.0 },{ -18.0, -3.0 } };
GLfloat car2_light2[3][2] = { { -18.0, -4.0 },{ -17.0, -5.0 },{ -18.0, -6.0 } };

GLfloat car2_color[7][3] = {
	{ 100 / 255.0f, 141 / 255.0f, 159 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f }
};

GLuint VBO_car2, VAO_car2;
void prepare_car2() {
	GLsizeiptr buffer_size = sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1) + sizeof(car2_light2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car2_body), car2_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body), sizeof(car2_front_window), car2_front_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window), sizeof(car2_back_window), car2_back_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window), sizeof(car2_front_wheel), car2_front_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel),
		sizeof(car2_back_wheel), car2_back_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel), sizeof(car2_light1), car2_light1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1), sizeof(car2_light2), car2_light2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car2);
	glBindVertexArray(VAO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car2() {
	glBindVertexArray(VAO_car2);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);

	glBindVertexArray(0);
}

// sword

#define SWORD_BODY 0
#define SWORD_BODY2 1
#define SWORD_HEAD 2
#define SWORD_HEAD2 3
#define SWORD_IN 4
#define SWORD_DOWN 5
#define SWORD_BODY_IN 6

GLfloat sword_body[4][2] = { { -6.0, 0.0 },{ -6.0, -4.0 },{ 6.0, -4.0 },{ 6.0, 0.0 } };
GLfloat sword_body2[4][2] = { { -2.0, -4.0 },{ -2.0, -6.0 } ,{ 2.0, -6.0 },{ 2.0, -4.0 } };
GLfloat sword_head[4][2] = { { -2.0, 0.0 },{ -2.0, 16.0 } ,{ 2.0, 16.0 },{ 2.0, 0.0 } };
GLfloat sword_head2[3][2] = { { -2.0, 16.0 },{ 0.0, 19.46 } ,{ 2.0, 16.0 } };
GLfloat sword_in[4][2] = { { -0.3, 0.7 },{ -0.3, 15.3 } ,{ 0.3, 15.3 },{ 0.3, 0.7 } };
GLfloat sword_down[4][2] = { { -2.0, -6.0 } ,{ 2.0, -6.0 },{ 4.0, -8.0 },{ -4.0, -8.0 } };
GLfloat sword_body_in[4][2] = { { 0.0, -1.0 } ,{ 1.0, -2.732 },{ 0.0, -4.464 },{ -1.0, -2.732 } };

GLfloat sword_color[7][3] = {
	{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f }
};

GLuint VBO_sword, VAO_sword;

void prepare_sword() {
	GLsizeiptr buffer_size = sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down) + sizeof(sword_body_in);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sword_body), sword_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body), sizeof(sword_body2), sword_body2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2), sizeof(sword_head), sword_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head), sizeof(sword_head2), sword_head2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2), sizeof(sword_in), sword_in);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in), sizeof(sword_down), sword_down);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down), sizeof(sword_body_in), sword_body_in);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_sword);
	glBindVertexArray(VAO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_sword() {
	glBindVertexArray(VAO_sword);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY2]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD2]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_DOWN]);
	glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 23, 4);

	glBindVertexArray(0);
}

void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
	GLuint VBO_line, VAO_line;
	GLfloat line[2][2];
	line[0][0] = x1; line[0][1] = y1;
	line[1][0] = x2; line[1][1] = y2;

	ModelViewProjectionMatrix = ViewProjectionMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	glGenBuffers(1, &VBO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_line);
	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_line);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

	glBindVertexArray(0);
}

void drawPoint(GLfloat x1, GLfloat y1) {
	//clear color and depth buffer 
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();//load identity matrix

	glTranslatef(0.0f, 0.0f, -4.0f);//move forward 4 units

	glColor3f(0.0f, 0.0f, 1.0f); //blue color

	glPointSize(10.0f);//set point size to 10 pixels

	glBegin(GL_POINTS); //starts drawing of points
	glVertex3f(1.0f, 1.0f, 0.0f);//upper-right corner
	glVertex3f(-1.0f, -1.0f, 0.0f);//lower-left corner
	glEnd();//end drawing of points

}

float max_win_size = win_width < win_height ? win_width : win_height;
float radius = win_width < win_height ? win_width / 2.5f : win_height / 2.5f;
int collisionDetected(float distX, float distY, float xRadius, float yRadius);
void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void drawPoint(GLfloat x1, GLfloat y1);

// AIRPLANE FACTORS
int theta = 0;

// CAR FACTORS
float car_scale_ratio = 1.0f;
float car1_x = -(win_width / 3), car1_y = 50;
float car2_x = win_width / 3, car2_y = 50;
float car1_width, car2_width;

int hitWall = 0;
int flyAway = 0;
int flyOrder = 0;
int flyRotate = 0;
float grad;

// HOUSE FACTORS
float house_scale_x = 1, house_scale_y = 1, house_ratio = 0;
int house_trans_order = 0;
glm::mat4 shearingMat = glm::mat4x4(1.0f);

// SWORD FACTORS
float sword_timer_pos = radius;

// COCKTAIL FACTORS
int cocktail_timer_rotation = 0;
int cocktail_angle = 0;
int cocktail_line_angle = 0;
float cock_timer_x = 0, cock_timer_y = max_win_size / 16;
int cock_trans_order = 0;
int isLine = 0;
glm::mat4 initTransform(glm::mat4 ModelMatrix);

// CUSTOMIZE CODE
#define FACE_NUM 24
float face_x[FACE_NUM] = { 0, }, face_y[FACE_NUM] = { 0, };
float x_basis[FACE_NUM] = { 0, }, y_basis[FACE_NUM] = { 0, };
int face_rotate[FACE_NUM] = { 0, }, face_prev_rotate[FACE_NUM] = { 0, };
int face_angle = 0;
int face_num = 0, CREATED = 0;

int ANGRY = 0;
float angry_x = -((float)win_width / 2) - ((float)win_height / 2);

void display(void) {
	int i;
	float x, r, s, delx, delr, dels;
	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();


	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// my Own Code

	// 1) AIRPLANE TRANSFORMATION - ROTATION & TRANSLATION
	float air_x, air_y;
	float ra;
	float sinTheta, cosTheta;
	float dx = 5 * cos(theta) * cos(5 * theta) - sin(theta) * sin(5 * theta);
	float dy = 5 * sin(theta) * cos(5 * theta) + cos(theta) * sin(5 * theta);
	ra = radius * sin(5 * theta * TO_RADIAN);
	air_x = ra * cos(theta * TO_RADIAN);
	air_y = ra * sin(theta * TO_RADIAN);

	drawLine(air_x, air_y, air_x+1, air_y+1);
	float air_angle = theta;
	float arcTan = atan(dy / dx) * TO_DEGREE;

	if (dx * dy == 0) air_angle = (int)(air_angle + 90) % 360;
	if (arcTan < 0) arcTan += 360;
	air_angle += arcTan + 90;
	//fprintf(stdout, "%f\n", 1.0f / 5.0f / radius / cos(5 * theta));

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(air_x, air_y, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, air_angle * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();


	// 2) CAR TRANSFORMATION - TRANSLATION & ROTATION
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(car1_x, car1_y, 0.0f));

	ModelMatrix = glm::rotate(ModelMatrix, flyRotate * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(car_scale_ratio, 1.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(car1_width / 16.0f, car1_width / 16.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car();
	drawLine(car1_x + car1_width, -300, car1_x + car1_width, 300);
	//
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(car2_x, car2_y, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(car2_width / 18.0f, car2_width / 18.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car2();
	drawLine(car2_x - car2_width, -300, car2_x - car2_width, 300);



	// 3) SWORD TRANSFORMATION - ROTATION & SCALING & TRANSLATION
	float angle;
	float sword_size;
	glm::vec3 sword_co;
	for (angle = 0; angle < 360; angle += 30) {
		sword_co = glm::vec3(sword_timer_pos * cos((float)angle * TO_RADIAN), sword_timer_pos * sin((float)angle * TO_RADIAN), 0.0f);
		sword_size = sword_timer_pos / radius * 3.5f;
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		ModelMatrix = glm::translate(ModelMatrix, sword_co);
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(sword_size, sword_size, 1.0f));
		ModelMatrix = glm::rotate(ModelMatrix, (90 + angle)*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));

		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_sword();
	}

	// 4) HOUSE TRANSFORMATION - SCALING & SHEARING
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
	switch (house_trans_order) {
	case 0: case 4: case 5: case 6: case 7:
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(house_scale_x, house_scale_y, 1.0f));
		break;
	case 1: case 2: case 3:
		shearingMat[0][1] = 0;
		shearingMat[1][0] = house_ratio;
		ModelMatrix *= shearingMat;
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 3.0f, 1.0f));
		break;
	}
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_house();

	// 5) COCKTAIL TRANSFORMATION - REFLECTION(TRANSLATION & ROTATION)
	if (isLine) {
		drawLine(radius * cos((float)cocktail_line_angle * TO_RADIAN), radius * sin((float)cocktail_line_angle * TO_RADIAN), -radius * cos((float)cocktail_line_angle * TO_RADIAN), -radius * sin((float)cocktail_line_angle * TO_RADIAN));
		drawLine(radius * cos((float)cocktail_angle * TO_RADIAN) - 50, radius * sin((float)cocktail_angle * TO_RADIAN), radius * cos((float)cocktail_angle * TO_RADIAN) + 50, radius * sin((float)cocktail_angle * TO_RADIAN));
		drawLine(radius * cos((float)cocktail_angle * TO_RADIAN), radius * sin((float)cocktail_angle * TO_RADIAN) - 50, radius * cos((float)cocktail_angle * TO_RADIAN), radius * sin((float)cocktail_angle * TO_RADIAN) + 50);
		drawLine(radius * cos((float)(cocktail_line_angle + 60) * TO_RADIAN) - 50, radius * sin((float)(cocktail_line_angle + 60) * TO_RADIAN), radius * cos((float)(cocktail_line_angle + 60) * TO_RADIAN) + 50, radius * sin((float)(cocktail_line_angle + 60) * TO_RADIAN));
		drawLine(radius * cos((float)(cocktail_line_angle + 60) * TO_RADIAN), radius * sin((float)(cocktail_line_angle + 60) * TO_RADIAN) - 50, radius * cos((float)(cocktail_line_angle + 60) * TO_RADIAN), radius * sin((float)(cocktail_line_angle + 60) * TO_RADIAN) + 50);
	}
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));

	// draw first format of cocktail
	ModelMatrix = initTransform(ModelMatrix);

	glm::mat4 MCtoWC = glm::mat4(1.0f);
	MCtoWC = glm::translate(MCtoWC, glm::vec3(radius * cos((float)cocktail_angle * TO_RADIAN), radius * sin((float)cocktail_angle * TO_RADIAN), 0.0f));
	glm::mat4 reflectLine = glm::mat4(1.0f);
	reflectLine = rotate(reflectLine, cocktail_line_angle * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	reflectLine = scale(reflectLine, glm::vec3(1.0f, -1.0f, 1.0f));
	reflectLine = rotate(reflectLine, -(cocktail_line_angle * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));

	ModelViewProjectionMatrix = ViewProjectionMatrix * MCtoWC * ModelMatrix;		// Modeling Coordinate to World Coordinate
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cocktail();

	ModelViewProjectionMatrix = ViewProjectionMatrix * reflectLine * MCtoWC * ModelMatrix;		// reflect a cocktail to the line
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cocktail();

	// relect a cocktail to origin in MC
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-1.0f, -1.0f, 1.0f));
	ModelMatrix = initTransform(ModelMatrix);
	
	ModelViewProjectionMatrix = ViewProjectionMatrix * MCtoWC * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cocktail();
	ModelViewProjectionMatrix = ViewProjectionMatrix * reflectLine * MCtoWC * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cocktail();


	// CUSTOMIZE OBJECT
	int idx;
	if (!CREATED) {
		if (face_angle % 15 == 0) {
			idx = face_angle / 15;
			face_rotate[idx] = face_angle;
			face_prev_rotate[idx] = face_angle;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, face_angle * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_face();

			face_num++;
			//if (face_angle == 355) CREATED = 1;
		}
		for (int i = 0; i < face_num; i++) {
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(face_x[i], face_y[i], 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, face_rotate[i] * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_face();
		}
		if (face_angle == 355) CREATED = 1;
	}
	else {
		for (int i = 0; i < FACE_NUM; i++) {
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(face_x[i], face_y[i], 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, face_rotate[i] * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_face();
		}
	}

	// ANGRY
	if (ANGRY) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(angry_x, 0.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3((float)win_height / 2 / 30, (float)win_height / 2 / 30, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_face_angry();
	}


	glFlush();
}

glm::mat4 initTransform(glm::mat4 ModelMatrix) {
	glm::vec3 cock_co = glm::vec3(cock_timer_x, cock_timer_y, 0.0f);
	ModelMatrix = glm::translate(ModelMatrix, cock_co);
	ModelMatrix = glm::rotate(ModelMatrix, cocktail_timer_rotation * TO_RADIAN, glm::vec3(0.0f, 0.0f, 0.1f));
	return ModelMatrix;
}

int collisionDetected(float distX, float distY, float xRadius, float yRadius) {
	float dist = sqrt(distX * distX + distY + distY);
	//(stdout, "%f %f\n", dist, xRadius + yRadius);
	if (dist <= xRadius + yRadius)
		return 1;
	return 0;
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case 'l': case 'L':
		isLine = isLine ? 0 : 1;
		break;
	case 'a': case 'A':
		ANGRY = 1;
	}
}

void reshape(int width, int height) {
	win_width = width, win_height = height;
	radius = win_width < win_height ? win_width / 2.5f : win_height / 2.5f;
	max_win_size = win_width < win_height ? win_width : win_height;
	angry_x = -(float)win_width / 2 - (float)win_height / 2;

	car1_width = win_width / 250.0f < 5 ? win_width / 250.05 : 5.0f;
	car2_width = win_width / 250.0f < 5 ? win_width / 250.05 : 5.0f;
	car1_width *= 16.0f;
	car2_width *= 18.0f;



	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0,
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();
	update_line();

	glutPostRedisplay();
}

int outOfScreen(float x, float y) {
	//if (x > -(win_width / 2.0f) && x < win_width / 2.0f && y > -(win_height / 2.0f) && y < win_height / 2.0f)
		//return 0;

	if (y > win_height / 2.0f - 20) return 1;
	if (x > win_width / 2.0f - 20) return 2;
	if (y < -(win_height / 2.0f) + 20) return 3;
	if (x < -(win_width / 2.0f) + 20) return 4;

	return 0;
}

void timer_scene(int timestamp_scene) {
	// AIRPLANE
	theta = (theta + 1) % 360;
	float tmpX = car1_x, tmpY = car1_y;

	// CARS
	float car_reflect_angle;
	if (!flyAway) {
		if ( collisionDetected(car1_x - car2_x, 0, 16.0f * (float)win_width / 250.0f, 18.0f * (float)win_width / 250.0f) ) {
			car_scale_ratio -= ((1.0f / 3.0f) / ((car1_width / 3.0f) / 10)) ;
			car2_x -= 10;
		}
		else car2_x -= 20;

		if ((car2_x - car2_width / 2.0f) - (car1_x + car1_width / 2.0f) <= car1_width / 3.0f) {
			flyAway = 1;
			flyOrder = 0;
			tmpX = car1_x;
			tmpY = car1_y;

			grad = rand() % 2 + 1 - rand() / (float)RAND_MAX;
			car_reflect_angle = atan(grad);
			grad = rand() % 2 == 0 ? grad : -grad;

			//fprintf(stdout, "%f %f\n", grad, (30.0f * abs(cos(car_reflect_angle))));
		}
	}
	else {
		flyRotate = (flyRotate + 50) % 360;
		int prevWall, prevGrad;
		switch (flyOrder) {
		case 0:
			car1_x -= (30.0f * abs(cos(car_reflect_angle)));
			car1_y = grad * (car1_x - tmpX) + tmpY;
			if ((hitWall = outOfScreen(car1_x, car1_y)) ) {
				flyOrder = (flyOrder + 1) % 3;
				tmpX = car1_x;
				tmpY = car1_y;
				prevGrad = grad;
				grad = rand() % 2 + 1 - (float)rand() / RAND_MAX;
				car_reflect_angle = atan(grad);

				if (hitWall == 3) grad *= -1;
				else if (hitWall == 4 && prevGrad > 0) grad *= -1;
\
				fprintf(stdout, "%f %f %d\n", grad, (30.0f * abs(cos(car_reflect_angle))), hitWall);
			}
			break;
		case 1:
			switch (hitWall) {
				case 1: case 3:
				car1_x -= (30.0f * abs(cos(car_reflect_angle)));
				break;
			default:
				car1_x += (30.0f * abs(cos(car_reflect_angle)));
			}
			prevWall = hitWall;
			car1_y = grad * (car1_x - tmpX) + tmpY;
			if ( (hitWall = outOfScreen(car1_x, car1_y)) > 0 ) {
				flyOrder = (flyOrder + 1) % 3;
				tmpX = car1_x;
				tmpY = car1_y;
				prevGrad = grad;
				grad = rand() % 2 + 1 - (float)rand() / RAND_MAX;
				car_reflect_angle = atan(grad);

				if (hitWall == 4 && prevGrad > 0) grad *= -1;
				else if (hitWall == 2 && prevGrad > 0) grad *= -1;
				else if (hitWall == 1) {
					if (prevWall == 4) grad *= -1;
					else if (prevWall == 3 && prevGrad > 0) grad *= -1;
				}
				else if (hitWall == 3) {
					if (prevWall == 2) grad *= -1;
					else if (prevWall == 1 && prevGrad > 0) grad *= -1;
				}

				fprintf(stdout, "%f %f %d\n", grad, (30.0f * abs(cos(car_reflect_angle))), hitWall);
			}
			break;
		case 2:
			switch (hitWall) {
			case 1: case 3: case 4:
				car1_x += (30.0f * abs(cos(car_reflect_angle)));
				break;
			default:
				car1_x -= (30.0f * abs(cos(car_reflect_angle)));
			}
			car1_y = grad * (car1_x - tmpX) + tmpY;
			if (outOfScreen(car1_x, car1_y) > 0) {
				flyOrder = (flyOrder + 1) % 3;
				car1_x = -(win_width / 3.0f);
				car1_y = 50.0f;
				car2_x = win_width / 3.0f;
				car_scale_ratio = 1.0f;
				flyRotate = 0;
				flyAway = 0;
				fprintf(stdout, "\n\n");
			}
			break;
		}
	}

	// HOUSE
	switch (house_trans_order) {
		case 0:
			house_scale_y += 0.1f;
			if (house_scale_y > 3.0f) {
				house_scale_y = 3.0f;
				house_trans_order = (house_trans_order + 1) % 7;
			}
			break;
		case 1:
			house_ratio += 0.1f;
			if (house_ratio > 2) {
				house_ratio = 2.0f;
				house_trans_order = (house_trans_order + 1) % 7;
			}
			break;
		case 2:
			house_ratio -= 0.1f;
			if (house_ratio < -2) {
				house_ratio = -2.0f;
				house_trans_order = (house_trans_order + 1) % 7;
			}
			break;
		case 3:
			house_ratio += 0.1f;
			if (house_ratio > 0) {
				house_ratio = 0.0f;
				house_trans_order = (house_trans_order + 1) % 7;
			}
			break;
		case 4:
			house_scale_x += 0.1f;
			if (house_scale_x > 3.0f) {
				house_scale_x = 3.0f;
				house_trans_order = (house_trans_order + 1) % 7;
			}
			break;
		case 5:
			house_scale_y -= 0.1f;
			if (house_scale_y < 1.0f) {
				house_scale_y = 1.0f;
				house_trans_order = (house_trans_order + 1) % 7;
			}
			break;
		case 6:
			house_scale_x -= 0.1f;
			if (house_scale_x < 1.0f) {
				house_scale_x = 1.0f;
				house_trans_order = (house_trans_order + 1) % 7;
			}
			break;
	}

	// SWORD
	sword_timer_pos -= 5.0f;
	if (sword_timer_pos < 0)
		sword_timer_pos = radius;

	// COCKTAIL
	cocktail_timer_rotation = (cocktail_timer_rotation + 30) % 360;
	cocktail_line_angle = (cocktail_line_angle + 1) % 360;
	cocktail_angle = (cocktail_line_angle + 300) % 360;
	switch (cock_trans_order) {
		case 0:
			cock_timer_x -= 5;
			cock_timer_y = cock_timer_x + (max_win_size / 16);
			if (cock_timer_x <= -(max_win_size / 16)) {
				cock_timer_x = -(max_win_size / 16);
				cock_timer_y = 0;
				cock_trans_order = (cock_trans_order + 1) % 4;
			}
			break;
		case 1:
			cock_timer_x += 5;
			cock_timer_y = -cock_timer_x - (max_win_size / 16);
			if (cock_timer_x >= 0) {
				cock_timer_x = 0;
				cock_timer_y = -(max_win_size / 16);
				cock_trans_order = (cock_trans_order + 1) % 4;
			}
			break;
		case 2:
			cock_timer_x += 5;
			cock_timer_y = cock_timer_x - (max_win_size / 16);
			if (cock_timer_x >= (max_win_size / 16)) {
				cock_timer_x = (max_win_size / 16);
				cock_timer_y = 0;
				cock_trans_order = (cock_trans_order + 1) % 4;
			}
			break;
		case 3:
			cock_timer_x -= 5;
			cock_timer_y = -cock_timer_x + (max_win_size / 16);
			if (cock_timer_x <= 0) {
				cock_timer_x = 0;
				cock_timer_y = max_win_size / 16;
				cock_trans_order = (cock_trans_order + 1) % 4;
			}
			break;
	}


	// CUSTOMIZE OBJECT
	face_angle = (face_angle + 5) % 360;
	int face_hit_wall;
	float face_ro, face_grad;
	for (int i = 0; i < face_num; i++) {
		face_x[i] += 10 * cos(face_rotate[i] * TO_RADIAN);
		face_y[i] = tan(face_rotate[i] * TO_RADIAN) * (face_x[i] - x_basis[i]) + y_basis[i];
		face_hit_wall = outOfScreen(face_x[i], face_y[i]);
		if (face_hit_wall) {
			face_ro = rand() % 60;


			switch (face_hit_wall) {
			case 1:
				if (face_rotate[i] < 90) face_ro = 360 - face_ro;
				else face_ro += 180;
				break;
			case 2:
				if (face_rotate[i] < 90) face_ro = 180 - face_ro;
				else face_ro += 180;
				break;
			case 3:
				if (face_rotate[i] < 270) face_ro = 180 - face_ro;
				else face_ro = face_ro;
				break;
			case 4:
				if (face_rotate[i] < 180) face_ro = face_ro;
				else face_ro = 360 - face_ro;
				break;
			}
			face_prev_rotate[i] = face_rotate[i];
			face_rotate[i] = face_ro;
			x_basis[i] = face_x[i];
			y_basis[i] = face_y[i];
		}
	}

	

	if (ANGRY) {
		angry_x += 8;
		if (angry_x >= (float)win_width / 2 + (float)win_height / 2) {
			ANGRY = 0;
			angry_x = -((float)win_width / 2) - ((float)win_height / 2);
		}
		for (int i = 0; i < FACE_NUM; i++) {
			if (collisionDetected(face_x[i] - angry_x, face_y[i], (float)win_height / 2, 30)) {
				face_prev_rotate[i] = face_rotate[i];
				x_basis[i] = face_x[i];
				y_basis[i] = face_y[i];
				if (face_y[i] > 0) face_rotate[i] = atan(face_y[i] / (face_x[i] - angry_x)) * TO_DEGREE;
				else if (face_y[i] == 0) face_rotate[i] = 0;
				else face_rotate[i] = face_rotate[i] = 360 - atan((-face_y[i]) / (face_x[i] - angry_x)) * TO_DEGREE;
			}
		}
	}


	glutPostRedisplay();
	glutTimerFunc(40, timer_scene, 1);
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_line);
	glDeleteBuffers(1, &VBO_line);

	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);

	// Delete others here too!!!
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(40, timer_scene, 1);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(250 / 255.0f, 128 / 255.0f, 114 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_line();
	prepare_airplane();
	prepare_house();
	prepare_car();
	prepare_cocktail();
	prepare_car2();
	prepare_sword();

	prepare_face();
	prepare_face_angry();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 2DObjects_GLSL_3.0.1.3";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC' "
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 700);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}


