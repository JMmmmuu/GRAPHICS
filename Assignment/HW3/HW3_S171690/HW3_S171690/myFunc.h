float car_pos_x, car_pos_y, car_rotation_angle;
int car_left_flag = 1;

float tiger_pos_x = -100.0f, tiger_pos_y = 30.0f;
int tiger_rotation_angle = 0;
float ben_pos_x, ben_pos_y, ben_rotation_angle;

glm::vec3 cow_pos = { -100.0f, 80.0f, 13.0f };
glm::vec3 cylinder_pos = cow_pos; 

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