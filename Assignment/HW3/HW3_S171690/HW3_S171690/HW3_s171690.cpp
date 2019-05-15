#define _CRT_SECURE_NO_WARNINGS

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables


#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> //inverse, affineInverse, etc.

#define NUMBER_OF_CAMERAS 2

glm::mat4 ViewProjectionMatrix[NUMBER_OF_CAMERAS], ViewMatrix[NUMBER_OF_CAMERAS], ProjectionMatrix[NUMBER_OF_CAMERAS];
glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when it is ready.

glm::mat4 ModelMatrix_CAR_BODY, ModelMatrix_CAR_WHEEL, ModelMatrix_CAR_NUT, ModelMatrix_CAR_DRIVER;
glm::mat4 ModelMatrix_CAR_BODY_to_DRIVER; // computed only once in initialize_camera()

glm::mat4 ModelMatrix_TIGER, ModelMatrix_TIGER_PERS;

#include "camera.h"
int flag_polygon_fill = 1;

#include "geomety.h"

#include "myFunc.h"

void display_camera(int camera_index) {

	glViewport(viewport[camera_index].x, viewport[camera_index].y, viewport[camera_index].w, viewport[camera_index].h);
	// At this point, the matrix ViewProjectionMatrix has been properly set up.

	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[camera_index], glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes(); // draw the WC axes.
	glLineWidth(1.0f);

	// draw floor
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(0, 0, -0.5));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(155, 155, 155));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(-2.0f, -1.5f, 0.0f));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_plane();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// draw green
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(-142.5f, 0.0f, 0.f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(105, 90, 90));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_partial(51, 204, 51);	// green
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(-142.5f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(115, 100, 100));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_partial_line(255, 255, 0);

	// draw water
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(142.5f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(115, 100, 100));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_partial(153, 255, 255);	// blue
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// draw car path
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(-142.5f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(142.5, 113, 120));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_partial_line(255, 255, 255);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(142.5f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(142.5, 113, 120));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_partial_line(255, 255, 255);

	// draw CAR
	if (car_left_flag) 
		ModelMatrix_CAR_BODY = glm::translate(glm::mat4(1.0f), glm::vec3(car_pos_x * 142.5f + 142.5f, car_pos_y * 113.0f, 16.0f));
	else 
		ModelMatrix_CAR_BODY = glm::translate(glm::mat4(1.0f), glm::vec3(car_pos_x * 142.5f - 142.5f, car_pos_y * 113.0f, 16.0f));
	ModelMatrix_CAR_BODY = glm::scale(ModelMatrix_CAR_BODY, glm::vec3(3.0f, 3.0f, 3.0f));
	ModelMatrix_CAR_BODY = glm::rotate(ModelMatrix_CAR_BODY, (car_rotation_angle + 90.0f) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_CAR_BODY = glm::rotate(ModelMatrix_CAR_BODY, 90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	//ModelMatrix_CAR_BODY = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(0.0f, 0.0f, 1.0f));

	if (view_mode == DRIVER_PERS) set_ViewMatrix_for_driver();
	else if (view_mode == VIEW_CAR) set_ViewMatrix_for_CAR(car_left_flag, car_pos_x, car_pos_y);

	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_CAR_BODY;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car_dummy();
	
	// DRAW TIGER
	getTigerPos();
	ModelMatrix_TIGER = glm::translate(glm::mat4(1.0f), glm::vec3(tiger_pos_x, tiger_pos_y, 0.0f));
	ModelMatrix_TIGER = glm::rotate(ModelMatrix_TIGER, (tiger_rotation_angle + 90) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_TIGER = glm::scale(ModelMatrix_TIGER, glm::vec3(0.30f, 0.30f, 0.30f));

	if (view_mode == TIGER_PERS) set_ViewMatrix_for_TIGER_PERS();
	else if (view_mode == VIEW_TIGER) set_ViewMatrix_for_TIGER(tiger_pos_x, tiger_pos_y);

	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_TIGER;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_tiger();


	// DRAW SPIDER
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], spider_pos);
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, spider_rotation_angle*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_spider();


	// DRAW BEN
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(ben_pos_x * 110.0f - 142.5f, ben_pos_y * 95.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(40.0f, 40.0f, 40.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, ben_rotation_angle * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_ben();
	

	// DRAW COW
	cow_tiger_collision();
	if (cow_tiger_collide) {
		
		// cylinder at current cow position
		ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], prev_cow_pos);
		ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(14.0f, 14.0f, 14.0f * cylinder_z_1));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUniform3f(loc_primitive_color, 178 / 255.0f, 132 / 255.0f, 190 / 255.0f);
		draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);
		

		// cylinder at rand position
		ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], cow_pos);
		ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(14.0f, 14.0f, 14.0f * cylinder_z_2));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUniform3f(loc_primitive_color, 178 / 255.0f, 132 / 255.0f, 190 / 255.0f);
		draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// draw cow
		ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], cow_pos);
		ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(40.0f, 40.0f, 40.0f));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cow();

	}
	else {
		ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], cow_pos);
		ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(40.0f, 40.0f, 40.0f));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cow();
	}

	// DRAW IRONMAN
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], ironman_pos);
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, ironman_rotation_angle * TO_RADIAN, glm::vec3(0, 0, 1));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 90 * TO_RADIAN, glm::vec3(1, 0, 0));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_ironman();


	// DRAW BRICKS
	float y = (11 + 20) / 17.0f;

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(202.5, -80, 100));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(4.5f, 4.5f, 4.5f));

	if (flag_polygon_fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_obj1();
	

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(82.5, 18, 100));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(4.5f, 4.5f, 4.5f));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_obj2();


	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(142.5, 110, 100));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(4.5f, 4.5f, 4.5f));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_obj3();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);






}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	display_camera(0);
	//display_camera(1);
	glutSwapBuffers();
}

void car_timer(int value) {
	if (!animation) return;

	// CAR
	static int car_idx = 1;
	static int tmp = 1;
	float del_pos;
	float car_speed = 0.01f;

	if (car_left_flag) {
		if (car_idx == 91) {
			car_pos_x -= car_speed;
			del_pos = car_speed * 142.5f;
			if (car_pos_x <= partial_vertices[car_idx + 1][0]) {
				car_idx++;
				tmp = 2;
			}
		}
		else if (car_idx == 182) {
			car_pos_y -= car_speed;
			del_pos = car_speed * 113.0f;
			if (car_pos_y <= partial_vertices[car_idx + 1][1]) {
				car_idx = 364;
				tmp = 4;
				car_left_flag = 0;
			}
		}
		else if (car_idx == 273) {
			car_pos_x += car_speed;
			del_pos = car_speed * 142.5f;
			if (car_pos_x >= partial_vertices[car_idx + 1][0]) {
				car_idx++;
				tmp = 4;
			}
		}
		else if (car_idx == 364) {
			car_pos_y += car_speed;
			del_pos = car_speed * 113.0f;
			if (car_pos_y >= partial_vertices[1][1]) {
				car_idx = 1;
				tmp = 1;
			}
		}
		else {	// corner
			car_pos_x = partial_vertices[car_idx][0];
			car_pos_y = partial_vertices[car_idx][1];
			car_rotation_angle = (car_idx - tmp + 180) % 360;
			car_idx++;
			del_pos = 1;

			rotation_angle_vert_wheel = (int)car_rotation_angle % 90;
			if (rotation_angle_vert_wheel >= 45)
				rotation_angle_vert_wheel = 90 - rotation_angle_vert_wheel;

			rotation_angle_vert_wheel *= 0.8f;
		}
	}
	else {
		if (car_idx == 274) {
			car_pos_x -= car_speed;
			del_pos = car_speed * 142.5f;
			if (car_pos_x <= partial_vertices[car_idx - 1][0]) {
				car_idx--;
				tmp = 3;
			}
		}
		else if (car_idx == 183) {
			car_pos_y += car_speed;
			del_pos = car_speed * 113.0f;
			if (car_pos_y >= partial_vertices[car_idx - 1][1]) {
				car_idx--;
				tmp = 2;
			}
		}
		else if (car_idx == 92) {
			car_pos_x += car_speed;
			del_pos = car_speed * 142.5f;
			if (car_pos_x >= partial_vertices[car_idx - 1][0]) {
				car_idx--;
				tmp = 1;
			}
		}
		else if (car_idx == 1) {
			car_pos_y -= car_speed;
			del_pos = car_speed * 113.0f;
			if (car_pos_y <= partial_vertices[364][1]) {
				car_idx = 183;
				tmp = 3;
				car_left_flag = 1;
			}
		}
		else {
			car_pos_x = partial_vertices[car_idx][0];
			car_pos_y = partial_vertices[car_idx][1];
			car_rotation_angle = (car_idx - tmp) % 360;
			car_idx--;
			del_pos = 1;

			rotation_angle_vert_wheel = (int)car_rotation_angle % 90;
			if (rotation_angle_vert_wheel >= 45)
				rotation_angle_vert_wheel = 90 - rotation_angle_vert_wheel;
			rotation_angle_vert_wheel *= -0.9f;
		}
	}

	float ra = 6.0f;
	rotation_angle_wheel += (del_pos / ra) * TO_DEGREE;
	if (rotation_angle_wheel >= 360) rotation_angle_wheel -= 360;

	glutPostRedisplay();
	glutTimerFunc(car_timer_scene, car_timer, (value + 1) % INT_MAX);
}
void timer_scene(int value) {
	if (!animation) return;

	// TIGER
	cur_frame_tiger = (value / 3) % N_TIGER_FRAMES;
	cur_frame_ben = value % N_BEN_FRAMES;
	cur_frame_spider = (value / 3) % N_SPIDER_FRAMES;

	//  BEN
	static int ben_idx = 1, ben_tmp = 1;
	float ben_speed = 0.03f;
	if (ben_idx == 91) {
		ben_pos_x -= ben_speed;
		if (ben_pos_x <= partial_vertices[ben_idx + 1][0]) {
			ben_idx++;
			ben_tmp = 2;
		}
	}
	else if (ben_idx == 182) {
		ben_pos_y -= ben_speed;
		if (ben_pos_y <= partial_vertices[ben_idx + 1][1]) {
			ben_idx++;
			ben_tmp = 3;
		}
	}
	else if (ben_idx == 273) {
		ben_pos_x += ben_speed;
		if (ben_pos_x >= partial_vertices[ben_idx + 1][0]) {
			ben_idx++;
			ben_tmp = 4;
		}
	}
	else if (ben_idx == 364) {
		ben_pos_y += ben_speed;
		if (ben_pos_y >= partial_vertices[1][1]) {
			ben_idx = 1;
			ben_tmp = 1;
		}
	}
	else {
		ben_pos_x = partial_vertices[ben_idx][0];
		ben_pos_y = partial_vertices[ben_idx][1];
		ben_rotation_angle = (ben_idx - ben_tmp + 180) % 360;
		ben_idx += 3;
	}

	// SPIDER
	get_spider_pos();

	// IRONMAN
	get_ironman_pos();

		
	// BRICK
	if (cam_moving)
		moveCam();
	

	glutPostRedisplay();
	glutTimerFunc(10, timer_scene, (value + 1) % INT_MAX);
}
void timer_scene_2(int value) {
	if (!animation) return;

	int sign;
	float y = (11 + 20) / 17.0f;
	float tmp;
	static int flag = 0;

	float blk_size = y * 113.0f / 3;

	if (cow_tiger_collide) {
		if (!flag) {
			flag = 1;
			prev_cow_pos = cow_pos;

			sign = rand() % 2 ? 1 : -1;
			cow_pos[0] = -142.5f + (rand() % 90) * sign;

			if (cow_pos[1] < -blk_size / 2) {
				cow_pos[1] = -blk_size / 2 + (rand() % (int)blk_size) * 2;
			}
			else if (cow_pos[1] < blk_size / 2) {
				sign = rand() % 2 ? 1 : -1;
				cow_pos[1] = (sign == 1) ? -(y * 113.0f / 2) : blk_size;
				cow_pos[1] += (rand() % (int)blk_size);
			}
			else {
				cow_pos[1] = -(y * 113.0f / 2) + (rand() % (int)blk_size * 2);
			}


			//sign = rand() % 2 ? 1 : -1;
			//cow_pos[1] = y * (rand() % 80) * sign;
		}

		cylinder_z_1 += 0.1;
		cylinder_z_2 += 0.1;
		if (cylinder_z_1 >= 1) {
			cylinder_z_1 = 0;
			cylinder_z_2 = 0;
			cow_tiger_collide = 0;
			flag = 0;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(1, timer_scene_2, (value + 1) % INT_MAX);
}

unsigned int leftbutton_pressed = 0;
int prevx, prevy;
void mousePressed(int button, int state, int x, int y) {
	view_brick = 0;
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		prevx = x, prevy = y;
		leftbutton_pressed = 1;
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) 
		leftbutton_pressed = 0;
	
}

#define CAM_ROT_SENSITIVITY		0.80f
#define CAM_ZOOM_STEP			0.03f
#define CAM_MAX_ZOOM_IN_FACTOR	0.10f
#define CAM_MAX_ZOOM_OUT_FACTOR	2.50f

void motion(int x, int y) {
	view_brick = 0;

	//if (!(view_mode == VIEW_WORLD || view_mode == VIEW_CAR || view_mode == VIEW_TIGER)) return;

	glm::mat4 mat4_tmp;
	glm::vec3 vec3_tmp;
	float delx, dely;

	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && leftbutton_pressed) {
		delx = (float)(x - prevx);
		if (delx < 0) {
			camera[0].zoom_factor += CAM_ZOOM_STEP;
			if (camera[0].zoom_factor > CAM_MAX_ZOOM_OUT_FACTOR)
				camera[0].zoom_factor = CAM_MAX_ZOOM_OUT_FACTOR;
			ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
		else if (delx > 0) {
			camera[0].zoom_factor -= CAM_ZOOM_STEP;
			if (camera[0].zoom_factor < CAM_MAX_ZOOM_IN_FACTOR)
				camera[0].zoom_factor = CAM_MAX_ZOOM_IN_FACTOR;
			ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
	}

}

void keyboard(unsigned char key, int x, int y) {
	view_brick = 0;
	switch (key) {
		/*case 'f':
			flag_fog = 1 - flag_fog;
			glUseProgram(h_ShaderProgram_TXPS);
			glUniform1i(loc_flag_fog, flag_fog);
			glUseProgram(0);
			glutPostRedisplay();
			break;*/
	case 'w':
		set_ViewMatrix_for_world_viewer();
		view_mode = VIEW_WORLD;
		break;
	case 'd':		// CAR DRIVER PERSPECTIVE
		view_mode = DRIVER_PERS;
		break;
	case 't':		// TIGER PERSPECTIVE
		view_mode = TIGER_PERS;
		break;
	case 'c':		// VIEW CAR
		set_ViewMatrix_for_world_viewer();
		view_mode = VIEW_CAR;

		break;
	case 'b':		// VIEW SPIDER on the BRICK
		set_Cam_to_Brick();
		view_mode = VIEW_BRICK;
		view_brick = 1;

		break;
	case 'l':
		set_Cam_to_VIEW_FIELD();
		view_mode = VIEW_FIELD;
		break;
	case 'i':
		ironman_fly = 1 - ironman_fly;
		break;
	case 'r':		// RESET CAMERA
		reset_CAM();
		view_mode = VIEW_WORLD;
		break;

	case 'm':
		car_timer_scene *= (8.0f / 10);
		printf("%f\n", car_timer_scene);

		if (car_timer_scene < MIN_CAR_TIME)
			car_timer_scene = MIN_CAR_TIME;
		printf("%f\n", car_timer_scene);
		break;
	case 'n':
		car_timer_scene *= (12.0f / 10);
		if (car_timer_scene > MAX_CAR_TIME)
			car_timer_scene = MAX_CAR_TIME;
		printf("%f\n", car_timer_scene);
		break;

	case 'a':
		animation = 1 - animation;
		if (animation) {
			glutTimerFunc(100, car_timer, 0);
			glutTimerFunc(100, timer_scene, 0);
			glutTimerFunc(1, timer_scene_2, 0);
		}
		break;
	case 'p':		// toggle polygon mode
		flag_polygon_fill = 1 - flag_polygon_fill;
		/*if (flag_polygon_fill)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();*/
		break;
	case 27:		// ESC key
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

#define SUB_CAM_MOVING_SCENE 15
void special(int key, int x, int y) {
	view_brick = 0;

	if (view_mode == DRIVER_PERS || view_mode == TIGER_PERS) return;
	glm::vec3 n, u, v;
	glm::mat4 tmp;

	n = camera[0].prp - camera[0].vrp;
	u = cross(camera[0].vup, n);
	v = cross(n, u);

	if (view_mode == VIEW_FIELD) {
		float view_angle;
		glm::vec3 dif;
		
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			switch (key) {
			case GLUT_KEY_UP:		// move upward
				dif = glm::vec3(0, 0, SUB_CAM_MOVING_SCENE);
				camera[0].vrp += dif;
				camera[0].prp += dif;

				break;
			case GLUT_KEY_DOWN:		// move downward
				dif = glm::vec3(0, 0, SUB_CAM_MOVING_SCENE);
				camera[0].vrp -= dif;
				camera[0].prp -= dif;

				break;
			case GLUT_KEY_LEFT:		// rotate left
				tmp = glm::translate(glm::mat4(1.0f), camera[0].prp);
				tmp = glm::rotate(tmp, CAM_ROT_SENSITIVITY * TO_RADIAN, glm::vec3(0, 0, 1));
				tmp = glm::translate(tmp, -camera[0].prp);

				camera[0].vrp = glm::vec3(tmp * glm::vec4(camera[0].vrp, 1.0f));

				break;
			case GLUT_KEY_RIGHT:	// rotate right
				tmp = glm::translate(glm::mat4(1.0f), camera[0].prp);
				tmp = glm::rotate(tmp, -CAM_ROT_SENSITIVITY * TO_RADIAN, glm::vec3(0, 0, 1));
				tmp = glm::translate(tmp, -camera[0].prp);

				camera[0].vrp = glm::vec3(tmp * glm::vec4(camera[0].vrp, 1.0f));

				break;
			}
		}
		else {		// move for/backward & left/rightward
			switch (key) {
			case GLUT_KEY_UP:		// move forward
				view_angle = atan(n[1] / n[0]);
				dif = glm::vec3(SUB_CAM_MOVING_SCENE * cos(view_angle), SUB_CAM_MOVING_SCENE * sin(view_angle), 0);
				camera[0].vrp += dif;
				camera[0].prp += dif;

				break;
			case GLUT_KEY_DOWN:		// move backward
				view_angle = atan(n[1] / n[0]);
				dif = glm::vec3(SUB_CAM_MOVING_SCENE * cos(view_angle), SUB_CAM_MOVING_SCENE * sin(view_angle), 0);
				camera[0].vrp -= dif;
				camera[0].prp -= dif;

				break;
			case GLUT_KEY_LEFT:		// move leftward
				view_angle = atan(u[1] / u[0]);
				dif = glm::vec3(SUB_CAM_MOVING_SCENE * cos(view_angle), SUB_CAM_MOVING_SCENE * sin(view_angle), 0);

				camera[0].vrp += dif;
				camera[0].prp += dif;

				break;
			case GLUT_KEY_RIGHT:	// move rightward
				view_angle = atan(u[1] / u[0]);
				dif = glm::vec3(SUB_CAM_MOVING_SCENE * cos(view_angle), SUB_CAM_MOVING_SCENE * sin(view_angle), 0);

				camera[0].vrp -= dif;
				camera[0].prp -= dif;

				break;
			}
		}
	}
	else {
		switch (key) {		// rotate camera around VRP
		case GLUT_KEY_UP:
			tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
			tmp = glm::rotate(tmp, -CAM_ROT_SENSITIVITY * TO_RADIAN, u);
			tmp = glm::translate(tmp, -camera[0].vrp);

			break;
		case GLUT_KEY_DOWN:
			tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
			tmp = glm::rotate(tmp, CAM_ROT_SENSITIVITY * TO_RADIAN, u);
			tmp = glm::translate(tmp, -camera[0].vrp);

			break;
		case GLUT_KEY_LEFT:
			tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
			tmp = glm::rotate(tmp, -CAM_ROT_SENSITIVITY * TO_RADIAN, v);
			tmp = glm::translate(tmp, -camera[0].vrp);

			break;
		case GLUT_KEY_RIGHT:
			tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
			tmp = glm::rotate(tmp, CAM_ROT_SENSITIVITY * TO_RADIAN, v);
			tmp = glm::translate(tmp, -camera[0].vrp);

			break;
		}
	}

	if (view_mode != VIEW_FIELD) {
		camera[0].prp = glm::vec3(tmp * glm::vec4(camera[0].prp, 1.0f));
		camera[0].vup = glm::vec3(tmp * glm::vec4(camera[0].vup, 0.0f));
	}

	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

	glutPostRedisplay();
}

void reshape(int width, int height) {
	camera[0].aspect_ratio = (float)width / height;
	viewport[0].x = viewport[0].y = 0;
	viewport[0].w = (int)(width); viewport[0].h = (int)(height);
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio,camera[0].near_clip, camera[0].far_clip);
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

	glDeleteVertexArrays(1, &partial_VAO);
	glDeleteBuffers(1, &partial_VBO);

	glDeleteVertexArrays(1, &points_VAO);
	glDeleteBuffers(1, &points_VBO);

	glDeleteVertexArrays(1, &circle_VAO);
	glDeleteBuffers(1, &circle_VBO);

	glDeleteVertexArrays(1, &plane_VAO);
	glDeleteBuffers(1, &plane_VBO);

	/*glDeleteVertexArrays(1, &cylinder_VAO);
	glDeleteBuffers(1, &cylinder_VBO);*/
	
	glDeleteVertexArrays(1, &cow_VAO);
	glDeleteBuffers(1, &cow_VBO);

	glDeleteVertexArrays(1, &circle_VAO);
	glDeleteBuffers(1, &circle_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);

	glDeleteVertexArrays(1, &spider_VAO);
	glDeleteBuffers(1, &spider_VBO);

	glDeleteVertexArrays(1, &ben_VAO);
	glDeleteBuffers(1, &ben_VBO);

	glDeleteVertexArrays(1, &ironman_VAO);
	glDeleteBuffers(1, &ironman_VBO);

	free_geom_obj(GEOM_OBJ_ID_CAR_BODY);
	free_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);
	free_geom_obj(GEOM_OBJ_ID_CAR_NUT);

	glDeleteVertexArrays(1, &obj1_VAO);
	glDeleteBuffers(1, &obj1_VBO);
	glDeleteVertexArrays(1, &obj2_VAO);
	glDeleteBuffers(1, &obj2_VBO);
	glDeleteVertexArrays(1, &obj3_VAO);
	glDeleteBuffers(1, &obj3_VBO);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousePressed);
	glutSpecialFunc(special);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);

	glutTimerFunc(100, car_timer, 0);
	glutTimerFunc(100, timer_scene, 0);
	glutTimerFunc(1, timer_scene_2, 0);
	
	glutCloseFunc(cleanup);
}

#include "initialize.h"

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