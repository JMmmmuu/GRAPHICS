float car_pos_x, car_pos_y, car_rotation_angle;
int car_left_flag = 1;

float tiger_pos_x = -100.0f, tiger_pos_y = 30.0f;
int tiger_rotation_angle = 0;
float ben_pos_x, ben_pos_y, ben_rotation_angle;

int outOfField() {
	float r = 30.0f;
	float y = (11 + 20) / 17.0f;

	if (tiger_pos_x + r >= -142.5f + 105.0f)
		return 1;
	else if (tiger_pos_x - r <= -142.5f - 105.0f)
		return 2;
	else if (tiger_pos_y + r >= y * 90.0f)
		return 3;
	else if (tiger_pos_y - r <= -y * 90.0f)
		return 4;

	return 0;
}
void getTigerPos() {
	srand(time(NULL));
	//glm::vec3 tiger_pos;
	const float tiger_speed = 1;
	static glm::vec3 prev_tiger_pos;
	static int theta = rand() % 360;

	prev_tiger_pos[0] = tiger_pos_x;
	prev_tiger_pos[1] = tiger_pos_y;

	tiger_pos_x += tiger_speed * cos(theta * TO_RADIAN);
	tiger_pos_y += tiger_speed * sin(theta * TO_RADIAN);
	tiger_rotation_angle = theta;

	//if (cow_tiger_collision(theta, tiger_pos_x, tiger_pos_y)) {


	//}

	int tmp;
	switch ((tmp = outOfField())) {
	case 0:
		return;
	case 1:			// x+++++
		theta = rand() % 180 + 90;
		break;
	case 2:			// x-----
		theta = rand() % 90;
		if (rand() % 2) theta = 360 - theta;
		break;
	case 3:			// y+++++
		theta = rand() % 180 + 180;
		break;
	case 4:			// y-----
		theta = rand() % 180;
		break;
	}

	//printf("\n\n%d\t%.4f %.4f\n", tmp, tiger_pos_x, tiger_pos_y);
	//printf("%d\n\n", theta);

	tiger_pos_x = prev_tiger_pos[0] + tiger_speed * cos(theta * TO_RADIAN);
	tiger_pos_y = prev_tiger_pos[1] + tiger_speed * sin(theta * TO_RADIAN);
	tiger_rotation_angle = theta;
}

glm::vec3 cow_pos = { -100.0f, 80.0f, 13.0f };
glm::vec3 prev_cow_pos = cow_pos;

int cylinder_status = 0;
int cow_tiger_collide = 0;
float cylinder_z_1 = 0, cylinder_z_2 = 0;

int cow_tiger_collision() { //int theta, float tiger_x, float tiger_y) {
	glm::vec3 tiger;
	tiger[0] = tiger_pos_x; tiger[1] = tiger_pos_y; tiger[2] = 0;
	int dist = glm::length(tiger - cow_pos);

	if (dist <= 50) {
		cow_tiger_collide = 1;
		return 1;
	}
	else {

		return 0;
	}
}

glm::vec3 spider_pos = { 82.5f, 0.0f, 100.0f };
int spider_rotation_angle = 0;
float spider_speed = 0.5f;
float spider_x_ratio = 5.0f;
enum _on_brick { first, second, third } on_brick;

void get_spider_pos() {
	float y_range[2];
	float brick_range[4] = { 18 - 135.0f / 2 - 90 + 30, 18 - 135.0f / 2, 18 + 135.0f / 2, 18 + 135.0f / 2 + 54 - 10 };

	if (view_brick) {
		y_range[0] = brick_range[0];
		y_range[1] = brick_range[3];
	}
	else {
		if (on_brick == first) {
			y_range[0] = brick_range[0];
			y_range[1] = brick_range[1];
		}
		else if (on_brick == second) {
			y_range[0] = brick_range[1];
			y_range[1] = brick_range[2];
		}
		else {
			y_range[0] = brick_range[2];
			y_range[1] = brick_range[3];
		}
	}

	if (spider_rotation_angle == 0) {
		spider_pos[1] += spider_speed;

		if (spider_pos[1] <= brick_range[1]) {
			spider_pos[0] = 202.5f;
			on_brick = first;
		}
		else if (spider_pos[1] <= brick_range[2]) {
			spider_pos[0] -= spider_x_ratio;
			if (spider_pos[0] <= 82.5f)
				spider_pos[0] = 82.5f;
			on_brick = second;
		}
		else {
			spider_pos[0] += spider_x_ratio;
			if (spider_pos[0] >= 142.5f)
				spider_pos[0] = 142.5f;
			on_brick = third;
		}

		if (spider_pos[1] + 15 >= y_range[1]) {
			spider_rotation_angle = 180;
		}
	}
	else {
		spider_pos[1] -= spider_speed;

		if (spider_pos[1] >= brick_range[2]) {
			spider_pos[0] = 142.5f;
			on_brick = third;
		}
		else if (spider_pos[1] >= brick_range[1]) {
			spider_pos[0] -= spider_x_ratio;
			if (spider_pos[0] <= 82.5f)
				spider_pos[0] = 82.5f;
			on_brick = second;
		}
		else {
			spider_pos[0] += spider_x_ratio;
			if (spider_pos[0] >= 202.5f)
				spider_pos[0] = 202.5f;

			on_brick = first;
		}

		if (spider_pos[1] - 15 <= y_range[0])
			spider_rotation_angle = 0;
	}
}