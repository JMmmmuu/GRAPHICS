typedef struct {
	glm::vec3 prp, vrp, vup; // in this HW, make vup always equal to the v direction.
	float fov_y, aspect_ratio, near_clip, far_clip, zoom_factor;
} CAMERA;

typedef struct {
	int x, y, w, h;
} VIEWPORT;

typedef enum {
	VIEW_WORLD, VIEW_TIGER, VIEW_CAR, DRIVER_PERS, TIGER_PERS, VIEW_BRICK, VIEW_FIELD
} VIEW_MODE;

CAMERA camera[NUMBER_OF_CAMERAS];
VIEWPORT viewport[NUMBER_OF_CAMERAS];
VIEW_MODE view_mode;

void set_ViewMatrix_for_world_viewer() {

	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
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

void set_ViewMatrix_for_TIGER(float tiger_pos_x, float tiger_pos_y) {
	set_ViewMatrix_for_world_viewer();

	camera[0].vrp = glm::vec3(tiger_pos_x, tiger_pos_y, 0);

	camera[0].prp[0] = camera[0].vrp[0] + 30;
	camera[0].prp[1] = camera[0].vrp[1] + 30;
	camera[0].prp[2] = camera[0].vrp[2] + 30;
	camera[0].vup = glm::vec3(0.0f, 0.0f, 1.0f);

	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
}

int view_brick = 0;
int cam_moving = 0;
CAMERA cam_dif;
CAMERA target_cam;

void set_Cam_to_Brick() {
	//set_ViewMatrix_for_world_viewer();
	target_cam.vrp = glm::vec3(82.5f, 20.0f, 100.0f);
	target_cam.prp = glm::vec3(600.0f, 20.0f, 100.0f);
	target_cam.vup = glm::vec3(0.0f, 0.0f, 1.0f);
	target_cam.fov_y = 45.0f;
	target_cam.aspect_ratio = 1.0f;
	target_cam.near_clip = 0.1f;
	target_cam.far_clip = 1200.0f;
	target_cam.zoom_factor = 1.0f;

	cam_moving = 1;
	cam_dif.vrp = target_cam.vrp - camera[0].vrp;
	cam_dif.prp = target_cam.prp - camera[0].prp;
	cam_dif.vup = target_cam.vup - camera[0].vup;
	cam_dif.aspect_ratio = target_cam.aspect_ratio - camera[0].aspect_ratio;
	cam_dif.far_clip = target_cam.aspect_ratio -camera[0].far_clip;
	cam_dif.fov_y = target_cam.fov_y -camera[0].fov_y;
	cam_dif.near_clip = target_cam.near_clip -camera[0].near_clip;
	cam_dif.zoom_factor = target_cam.zoom_factor -camera[0].zoom_factor;
}

void set_Cam_to_VIEW_FIELD() {
	//set_ViewMatrix_for_world_viewer();
	target_cam.vrp = glm::vec3(-142.5f, 0.0f, 15.0f);
	target_cam.prp = glm::vec3(-400.0f, 400.0f, 15.0f);
	target_cam.vup = glm::vec3(0.0f, 0.0f, 1.0f);
	target_cam.fov_y = 45.0f;
	target_cam.aspect_ratio = 1.0f;
	target_cam.near_clip = 0.1f;
	target_cam.far_clip = 1200.0f;
	target_cam.zoom_factor = 1.0f;

	cam_moving = 1;
	cam_dif.vrp = target_cam.vrp - camera[0].vrp;
	cam_dif.prp = target_cam.prp - camera[0].prp;
	cam_dif.vup = target_cam.vup - camera[0].vup;
	cam_dif.aspect_ratio = target_cam.aspect_ratio - camera[0].aspect_ratio;
	cam_dif.far_clip = target_cam.aspect_ratio - camera[0].far_clip;
	cam_dif.fov_y = target_cam.fov_y - camera[0].fov_y;
	cam_dif.near_clip = target_cam.near_clip - camera[0].near_clip;
	cam_dif.zoom_factor = target_cam.zoom_factor - camera[0].zoom_factor;
}

int set_Cam_to_target() {
	if (camera[0].prp[0] - target_cam.prp[0] > 0.01 || camera[0].prp[0] - target_cam.prp[0] < -0.01)
		return 0;
	if (camera[0].prp[1] - target_cam.prp[1] > 0.01 || camera[0].prp[1] - target_cam.prp[1] < -0.01)
		return 0;
	if (camera[0].prp[2] - target_cam.prp[2] > 0.01 || camera[0].prp[2] - target_cam.prp[2] < -0.01)
		return 0;
	return 1;
}

#define MOVING_CAM_SCENE 30.0f
void moveCam() {
	camera[0].vrp += cam_dif.vrp / MOVING_CAM_SCENE;
	camera[0].prp += cam_dif.prp / MOVING_CAM_SCENE;
	camera[0].vup += cam_dif.vup / MOVING_CAM_SCENE;
	camera[0].aspect_ratio += cam_dif.aspect_ratio / MOVING_CAM_SCENE;
	camera[0].far_clip += cam_dif.far_clip / MOVING_CAM_SCENE;
	camera[0].fov_y += cam_dif.fov_y / MOVING_CAM_SCENE;
	camera[0].near_clip += cam_dif.near_clip / MOVING_CAM_SCENE;
	camera[0].zoom_factor += cam_dif.zoom_factor / MOVING_CAM_SCENE;

	if (set_Cam_to_target()) {
		camera[0].vrp = target_cam.vrp;
		camera[0].prp = target_cam.prp;
		camera[0].vup = target_cam.vup;
		camera[0].aspect_ratio = target_cam.aspect_ratio;
		camera[0].far_clip = target_cam.far_clip;
		camera[0].fov_y = target_cam.fov_y;
		camera[0].near_clip = target_cam.near_clip;
		camera[0].zoom_factor = target_cam.zoom_factor;
		cam_moving = 0;
	}
	
	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	//if (view_mode == VIEW_FIELD) {
		//ViewMatrix[0][2][0] = 0;
		//ViewMatrix[0][2][1] = 0;
		//ViewMatrix[0][2][2] = 1;
	//}
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

	glutPostRedisplay();
}

void reset_CAM() {
	target_cam.prp = glm::vec3(500.0f, 400.0f, 400.0f);
	target_cam.vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	target_cam.vup = glm::vec3(0.0f, 0.0f, 1.0f);

	target_cam.fov_y = 45.0f;
	target_cam.aspect_ratio = 1.0f;
	target_cam.near_clip = 0.1f;
	target_cam.far_clip = 1200.0f;
	target_cam.zoom_factor = 1.0f;

	view_mode = VIEW_WORLD;
	cam_moving = 1;

	cam_dif.vrp = target_cam.vrp - camera[0].vrp;
	cam_dif.prp = target_cam.prp - camera[0].prp;
	cam_dif.vup = target_cam.vup - camera[0].vup;
	cam_dif.aspect_ratio = target_cam.aspect_ratio - camera[0].aspect_ratio;
	cam_dif.far_clip = target_cam.aspect_ratio - camera[0].far_clip;
	cam_dif.fov_y = target_cam.fov_y - camera[0].fov_y;
	cam_dif.near_clip = target_cam.near_clip - camera[0].near_clip;
	cam_dif.zoom_factor = target_cam.zoom_factor - camera[0].zoom_factor;
}