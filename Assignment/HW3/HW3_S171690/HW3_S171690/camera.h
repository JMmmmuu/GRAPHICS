typedef struct {
	glm::vec3 prp, vrp, vup; // in this HW, make vup always equal to the v direction.
	float fov_y, aspect_ratio, near_clip, far_clip, zoom_factor;
} CAMERA;

typedef struct {
	int x, y, w, h;
} VIEWPORT;

typedef enum {
	VIEW_WORLD, VIEW_TIGER, VIEW_CAR, DRIVER_PERS, TIGER_PERS
} VIEW_MODE;

CAMERA camera[NUMBER_OF_CAMERAS];
VIEWPORT viewport[NUMBER_OF_CAMERAS];
VIEW_MODE view_mode;

void set_ViewMatrix_for_world_viewer() {
	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
	//ViewProjectionMatrix[0] = glm::rotate(ViewProjectionMatrix[0], 120 * TO_RADIAN, glm::vec3(-1, -1, -1));
}

void set_ViewMatrix_for_driver() {
	glm::mat4 Matrix_CAMERA_driver_inverse;

	Matrix_CAMERA_driver_inverse = ModelMatrix_CAR_BODY * ModelMatrix_CAR_BODY_to_DRIVER;

	ViewMatrix[0] = glm::affineInverse(Matrix_CAMERA_driver_inverse);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
}

void set_ViewMatrix_for_TIGER_PERS() {
	glm::mat4 Matrix_CAMERA_TIGER_PERSPECTIVE_inverse;

	Matrix_CAMERA_TIGER_PERSPECTIVE_inverse = ModelMatrix_TIGER * ModelMatrix_TIGER_PERS;

	ViewMatrix[0] = glm::affineInverse(Matrix_CAMERA_TIGER_PERSPECTIVE_inverse);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
	//ViewProjectionMatrix[0] = glm::rotate(ViewProjectionMatrix[0], 90 * TO_RADIAN, glm::vec3(0, 1, 0));
}

void set_ViewMatrix_for_CAR(int car_left_flag, float car_pos_x, float car_pos_y) {
	set_ViewMatrix_for_world_viewer();

	if (car_left_flag)
		camera[0].vrp[0] = car_pos_x * 142.5f + 142.5f;
	else
		camera[0].vrp[0] = car_pos_x * 142.5f - 142.5f;
	camera[0].vrp[1] = car_pos_y * 113.0f;
	camera[0].vrp[2] = 0;

	camera[0].prp[0] = camera[0].vrp[0] + 30;
	camera[0].prp[1] = camera[0].vrp[1] + 30;
	camera[0].prp[2] = camera[0].vrp[2] + 30;
	camera[0].vup = glm::vec3(0.0f, 0.0f, 1.0f);

	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
}

void reset_CAM() {
	camera[0].prp = glm::vec3(500.0f, 400.0f, 400.0f);
	camera[0].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[0].vup = glm::vec3(0.0f, 0.0f, 1.0f);
	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);

	camera[0].fov_y = 45.0f;
	camera[0].aspect_ratio = 1.0f;
	camera[0].near_clip = 0.1f;
	camera[0].far_clip = 1200.0f;
	camera[0].zoom_factor = 1.0f;

	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
	//ViewProjectionMatrix[0] = glm::rotate(ViewProjectionMatrix[0], 120 * TO_RADIAN, glm::vec3(-1, -1, -1));
}