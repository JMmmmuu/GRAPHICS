typedef struct {
	glm::vec3 prp, vrp, vup; // in this HW, make vup always equal to the v direction.
	float fov_y, aspect_ratio, near_clip, far_clip, zoom_factor;
} CAMERA;

typedef struct {
	int x, y, w, h;
} VIEWPORT;

typedef enum {
	VIEW_WORLD, VIEW_TIGER, VIEW_CAR
} VIEW_MODE;

#define NUMBER_OF_CAMERAS 2

CAMERA camera[NUMBER_OF_CAMERAS];
VIEWPORT viewport[NUMBER_OF_CAMERAS];