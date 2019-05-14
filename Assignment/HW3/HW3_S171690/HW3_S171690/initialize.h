GLuint h_ShaderProgram_simple, h_ShaderProgram_TXPS; // handles to shader programs

void prepare_shader_program() {
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

void initialize_flags() {
	flag_polygon_fill = 0;
}


#define PRINT_DEBUG_INFO  
void initialize_OpenGL() {
	// initialize the 0th camera.
	camera[0].prp = glm::vec3(500.0f, 400.0f, 400.0f);
	camera[0].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[0].vup = glm::vec3(0.0f, 0.0f, 1.0f);
	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	//camera[0].vup = glm::vec3(ViewMatrix[0][0].y, ViewMatrix[0][1].y, ViewMatrix[0][2].y); // in this example code, make vup always equal to the v direction.

	camera[0].fov_y = 45.0f;
	camera[0].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[0].near_clip = 0.1f;
	camera[0].far_clip = 1200.0f;
	camera[0].zoom_factor = 1.0f; // will be used for zoomming in and out.

			
	//initialize the 1st camera.
	camera[1].prp = glm::vec3(0.0f, 20.0f, 0.0f);
	camera[1].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[1].vup = glm::vec3(0.0f, -1.0f, 0.0f);
	ViewMatrix[1] = glm::lookAt(camera[1].prp, camera[1].vrp, camera[1].vup);
	camera[1].vup = glm::vec3(ViewMatrix[1][0].y, ViewMatrix[1][1].y, ViewMatrix[1][2].y); // in this example code, make vup always equal to the v direction.

	camera[1].fov_y = 16.0f;
	camera[1].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[1].near_clip = 0.1f;
	camera[1].far_clip = 100.0f;
	camera[1].zoom_factor = 1.0f; // will be used for zoomming in and out.

	view_mode = VIEW_WORLD;
	/*
	ModelMatrix_CAR_BODY = glm::mat4(1.0f);
	ModelMatrix_CAR_WHEEL = glm::mat4(1.0f);
	ModelMatrix_CAR_NUT = glm::mat4(1.0f);*/

	// the transformation that moves the driver's camera frame from car body's MC to driver seat
	ModelMatrix_CAR_BODY_to_DRIVER = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.5f, 2.5f));
	ModelMatrix_CAR_BODY_to_DRIVER = glm::rotate(ModelMatrix_CAR_BODY_to_DRIVER, TO_RADIAN*90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	ModelMatrix_TIGER_PERS = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -88.0f, 62.0f));
	ModelMatrix_TIGER_PERS = glm::rotate(ModelMatrix_TIGER_PERS, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_TIGER_PERS = glm::rotate(ModelMatrix_TIGER_PERS, 180.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	glClearColor(0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
}

void prepare_scene() {
	prepare_plane();
	prepare_partial();
	prepare_axes();
	prepare_points();
	prepare_circle();
	prepare_cylinder();

	prepare_obj1();
	prepare_obj2();
	prepare_obj3();
	
	prepare_ben();
	prepare_cow();
	prepare_plane();
	prepare_tiger();
	prepare_spider();
	prepare_ironman();
	
	prepare_geom_obj(GEOM_OBJ_ID_CAR_BODY, "Data/Car/car_body_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_WHEEL, "Data/Car/car_wheel_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_NUT, "Data/Car/car_nut_triangles_v.txt", GEOM_OBJ_TYPE_V);
}

void initialize_renderer() {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew() {
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
	fprintf(stdout, "  Created by Yuseok Jung\n");
	fprintf(stdout, "  Student ID: 20171690\n");
	fprintf(stdout, "  Date: 2019.05.20 Mon");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}