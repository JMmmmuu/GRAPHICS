#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define INDEX_VERTEX_POSITION 0
#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2


GLuint plane_VBO, plane_VAO;
GLfloat plane_vertices[6][8] = {
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 4.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 4.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
};

void prepare_plane() {
	// initialize vertex buffer object
	glGenBuffers(1, &plane_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), &plane_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object
	glGenVertexArrays(1, &plane_VAO);
	glBindVertexArray(plane_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_plane(void) {
	glFrontFace(GL_CCW);

	GLfloat plane_color[3] = { 30.0f / 255, 30.0f / 255, 30.0f / 255 };
	glBindVertexArray(plane_VAO);
	glUniform3fv(loc_primitive_color, 1, plane_color);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

GLuint partial_VBO, partial_VAO;
GLfloat partial_vertices[366][8];

void prepare_partial() {
	// initialize vertices
	GLfloat x = 1.0f, y = (11 + 20 * x) / 17.0f;
	GLfloat ra = y / 2.0f * 3 / 8;

	int i, j;
	partial_vertices[0][0] = 0; partial_vertices[0][1] = 0; partial_vertices[0][2] = 0;
	partial_vertices[0][3] = 0; partial_vertices[0][4] = 0; partial_vertices[0][5] = 1;
	partial_vertices[0][6] = 0; partial_vertices[0][7] = 0;

	i = 1;
	for (j = 0; j <= 90; j++, i++) {
		partial_vertices[i][0] = x - ra + ra * cos(j * TO_RADIAN); partial_vertices[i][1] = y - ra + ra * sin(j * TO_RADIAN);
		partial_vertices[i][2] = 0.0f; partial_vertices[i][3] = 0.0f; partial_vertices[i][4] = 0.0f; partial_vertices[i][5] = 1.0f;
		partial_vertices[i][6] = partial_vertices[i][7] = 0.0f;
	}
	for (j = 90; j <= 180; j++, i++) {
		partial_vertices[i][0] = -x + ra + ra * cos(j * TO_RADIAN); partial_vertices[i][1] = y - ra + ra * sin(j * TO_RADIAN);
		partial_vertices[i][2] = 0.0f; partial_vertices[i][3] = 0.0f; partial_vertices[i][4] = 0.0f; partial_vertices[i][5] = 1.0f;
		partial_vertices[i][6] = partial_vertices[i][7] = 0.0f;
	}
	for (j = 180; j <= 270; j++, i++) {
		partial_vertices[i][0] = -x + ra + ra * cos(j * TO_RADIAN); partial_vertices[i][1] = -y + ra + ra * sin(j * TO_RADIAN);
		partial_vertices[i][2] = 0.0f; partial_vertices[i][3] = 0.0f; partial_vertices[i][4] = 0.0f; partial_vertices[i][5] = 1.0f;
		partial_vertices[i][6] = partial_vertices[i][7] = 0.0f;
	}
	for (j = 270; j <= 360; j++, i++) {
		partial_vertices[i][0] = x - ra + ra * cos(j * TO_RADIAN); partial_vertices[i][1] = -y + ra + ra * sin(j * TO_RADIAN);
		partial_vertices[i][2] = 0.0f; partial_vertices[i][3] = 0.0f; partial_vertices[i][4] = 0.0f; partial_vertices[i][5] = 1.0f;
		partial_vertices[i][6] = partial_vertices[i][7] = 0.0f;
	}
	partial_vertices[i][0] = x; partial_vertices[i][1] = y-ra; partial_vertices[i][2] = 0;
	partial_vertices[i][3] = 0; partial_vertices[i][4] = 0; partial_vertices[i][5] = 1;
	partial_vertices[i][6] = 0; partial_vertices[i][7] = 0;
	fprintf(stdout, "%d %f %f %f\n", i, x, y, ra);

	// Initialize vertex buffer object.

	glGenBuffers(1, &partial_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, partial_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(partial_vertices), &partial_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &partial_VAO);
	glBindVertexArray(partial_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, partial_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void draw_partial(GLfloat r, GLfloat g, GLfloat b) {
	glFrontFace(GL_CCW);
	GLfloat partial_color[3];
	partial_color[0] = r / 255;
	partial_color[1] = g / 255;
	partial_color[2] = b / 255;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(partial_VAO);
	glUniform3fv(loc_primitive_color, 1, partial_color);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 366);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void draw_partial_line(GLfloat r, GLfloat g, GLfloat b) {
	glFrontFace(GL_CCW);
	GLfloat partial_color[3];
	partial_color[0] = r / 255;
	partial_color[1] = g / 255;
	partial_color[2] = b / 255;

	glBindVertexArray(partial_VAO);
	glUniform3fv(loc_primitive_color, 1, partial_color);
	glDrawArrays(GL_LINE_LOOP, 1, 365);
	glBindVertexArray(0);
}

GLuint axes_VBO, axes_VAO;
GLfloat axes_vertices[6][3] = {
	{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f }
};
GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } };

void prepare_axes(void) {
	// Initialize vertex buffer object.
	glGenBuffers(1, &axes_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), &axes_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &axes_VAO);
	glBindVertexArray(axes_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(INDEX_VERTEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_axes(void) {
	glBindVertexArray(axes_VAO);
	glUniform3fv(loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
}

#define NUMBER_OF_POINTS 1
GLuint points_VBO, points_VAO;
GLfloat points_vertices[NUMBER_OF_POINTS][3] = { { 0.0f, 0.0f, 0.0f } };

void prepare_points(void) {
	// Initialize vertex buffer object.
	glGenBuffers(1, &points_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points_vertices), &points_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &points_VAO);
	glBindVertexArray(points_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(INDEX_VERTEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_points(float r, float g, float b) {
	glBindVertexArray(points_VAO);
	glUniform3f(loc_primitive_color, r, g, b);
	glDrawArrays(GL_POINTS, 0, NUMBER_OF_POINTS);
	glBindVertexArray(0);
}

GLuint circle_VBO, circle_VAO;
GLfloat circle_vertices[362][8];

void prepare_circle() {
	// set circle vertices
	int i;
	circle_vertices[0][0] = 0; circle_vertices[0][1] = 0; circle_vertices[0][2] = 0;
	circle_vertices[0][3] = circle_vertices[0][4] = circle_vertices[0][6] = circle_vertices[0][7] = 0;
	circle_vertices[0][5] = 1;

	for (i = 0; i < 360; i++) {
		circle_vertices[i + 1][0] = cos(i * TO_RADIAN); circle_vertices[i + 1][1] = sin(i * TO_RADIAN);
		circle_vertices[i + 1][2] = circle_vertices[i + 1][3] = circle_vertices[i + 1][4] = circle_vertices[i + 1][6] = circle_vertices[i + 1][7] = 0;
		circle_vertices[i + 1][5] = 1;
	}
	circle_vertices[361][0] = 1; circle_vertices[361][1] = 0; circle_vertices[361][2] = 0;
	circle_vertices[361][3] = circle_vertices[361][4] = circle_vertices[361][6] = circle_vertices[361][7] = 0;
	circle_vertices[361][5] = 1;

	// Initialize vertex buffer object.
	glGenBuffers(1, &circle_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, circle_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle_vertices), &circle_vertices[0][0], GL_STATIC_DRAW);


	// Initialize vertex array object.
	glGenVertexArrays(1, &circle_VAO);
	glBindVertexArray(circle_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, circle_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_circle() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat color[3] = { 180.0f / 255, 0.0f / 255, 0.0f / 255 };
	glBindVertexArray(circle_VAO);
	glUniform3fv(loc_primitive_color, 1, color);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 362);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/*GLuint cylinder_VBO, cylinder_VAO;
GLfloat cylinder_vertices[722][8];

void prepare_cylinder() {
	// set cylinder vertices
	int i;
	for (i = 0; i < 722; i += 2) {
		cylinder_vertices[i][0] = cos(i * TO_RADIAN); cylinder_vertices[i][1] = sin(i * TO_RADIAN); cylinder_vertices[i][2] = 0;
		cylinder_vertices[i][3] = cos(i * TO_RADIAN);
		cylinder_vertices[1][4] = sin(i * TO_RADIAN);
		cylinder_vertices[i][5] = cylinder_vertices[i][6] = cylinder_vertices[i][7] = 0;


		cylinder_vertices[i + 1][0] = cos(i * TO_RADIAN); cylinder_vertices[i + 1][1] = sin(i * TO_RADIAN); cylinder_vertices[i+1][2] = 1;
		cylinder_vertices[i + 1][3] = cos(i * TO_RADIAN);
		cylinder_vertices[i + 1][4] = sin(i * TO_RADIAN);
		cylinder_vertices[i + 1][5] = cylinder_vertices[i + 1][6] = cylinder_vertices[i + 1][7] = 0;

		//printf("%d\t%.2f %.2f %.2f\n\t%.2f %.2f %.2f\n\n", i, cylinder_vertices[i][0], cylinder_vertices[i][1], cylinder_vertices[i][2],
			//cylinder_vertices[i + 1][0], cylinder_vertices[i + 1][1], cylinder_vertices[i + 1][2]);
	}

	// Initialize vertex buffer object.
	glGenBuffers(1, &cylinder_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, cylinder_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_vertices), &cylinder_vertices[0][0], GL_STATIC_DRAW);


	// Initialize vertex array object.
	glGenVertexArrays(1, &cylinder_VAO);
	glBindVertexArray(cylinder_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cylinder_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cylinder() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat color[3] = { 255.0f / 255, 0.0f / 255, 0.0f / 255 };
	glBindVertexArray(cylinder_VAO);
	glUniform3fv(loc_primitive_color, 1, color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 722);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}*/



// tiger object
#define N_TIGER_FRAMES 12
GLuint tiger_VBO, tiger_VAO;
int tiger_n_triangles[N_TIGER_FRAMES];
int tiger_vertex_offset[N_TIGER_FRAMES];
GLfloat *tiger_vertices[N_TIGER_FRAMES];
int cur_frame_tiger = 0;

// cow object
GLuint cow_VBO, cow_VAO;
int cow_n_triangles;
GLfloat *cow_vertices;

// spider object
#define N_SPIDER_FRAMES 16
GLuint spider_VBO, spider_VAO;
int spider_n_triangles[N_SPIDER_FRAMES];
int spider_vertex_offset[N_SPIDER_FRAMES];
GLfloat *spider_vertices[N_SPIDER_FRAMES];
int cur_frame_spider = 0;

// ben object
#define N_BEN_FRAMES 30
GLuint ben_VBO, ben_VAO;
int ben_n_triangles[N_BEN_FRAMES];
int ben_vertex_offset[N_BEN_FRAMES];
GLfloat *ben_vertices[N_BEN_FRAMES];
int cur_frame_ben = 0;

// ironman object
GLuint ironman_VBO, ironman_VAO;
int ironman_n_triangles;
GLfloat *ironman_vertices;


int read_geometry(GLfloat **object, int bytes_per_primitive, char *filename) {
	int n_triangles;
	FILE *fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}
	fread(&n_triangles, sizeof(int), 1, fp);

	*object = (float *)malloc(n_triangles*bytes_per_primitive);
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_triangles, fp);
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}

void prepare_tiger(void) { // vertices enumerated clockwise
	int i, n_bytes_per_vertex, n_bytes_per_triangle, tiger_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_TIGER_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/tiger/Tiger_%d%d_triangles_vnt.geom", i / 10, i % 10);
		tiger_n_triangles[i] = read_geometry(&tiger_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		tiger_n_total_triangles += tiger_n_triangles[i];

		if (i == 0)
			tiger_vertex_offset[i] = 0;
		else
			tiger_vertex_offset[i] = tiger_vertex_offset[i - 1] + 3 * tiger_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &tiger_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glBufferData(GL_ARRAY_BUFFER, tiger_n_total_triangles*n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_TIGER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, tiger_vertex_offset[i] * n_bytes_per_vertex,
			tiger_n_triangles[i] * n_bytes_per_triangle, tiger_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_TIGER_FRAMES; i++)
		free(tiger_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &tiger_VAO);
	glBindVertexArray(tiger_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void prepare_cow(void) {
	int n_bytes_per_vertex, n_bytes_per_triangle, cow_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/cow_vn.geom");
	cow_n_triangles = read_geometry(&cow_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	cow_n_total_triangles += cow_n_triangles;


	// initialize vertex buffer object
	glGenBuffers(1, &cow_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, cow_VBO);
	glBufferData(GL_ARRAY_BUFFER, cow_n_total_triangles * 3 * n_bytes_per_vertex, cow_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(cow_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &cow_VAO);
	glBindVertexArray(cow_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cow_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void prepare_spider(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, spider_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_SPIDER_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/spider/spider_vnt_%d%d.geom", i / 10, i % 10);
		spider_n_triangles[i] = read_geometry(&spider_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		spider_n_total_triangles += spider_n_triangles[i];

		if (i == 0)
			spider_vertex_offset[i] = 0;
		else
			spider_vertex_offset[i] = spider_vertex_offset[i - 1] + 3 * spider_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &spider_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, spider_VBO);
	glBufferData(GL_ARRAY_BUFFER, spider_n_total_triangles*n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_SPIDER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, spider_vertex_offset[i] * n_bytes_per_vertex,
			spider_n_triangles[i] * n_bytes_per_triangle, spider_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_SPIDER_FRAMES; i++)
		free(spider_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &spider_VAO);
	glBindVertexArray(spider_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, spider_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void prepare_ben(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, ben_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_BEN_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/ben/ben_vn%d%d.geom", i / 10, i % 10);
		ben_n_triangles[i] = read_geometry(&ben_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		ben_n_total_triangles += ben_n_triangles[i];

		if (i == 0)
			ben_vertex_offset[i] = 0;
		else
			ben_vertex_offset[i] = ben_vertex_offset[i - 1] + 3 * ben_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &ben_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, ben_VBO);
	glBufferData(GL_ARRAY_BUFFER, ben_n_total_triangles*n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_BEN_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, ben_vertex_offset[i] * n_bytes_per_vertex,
			ben_n_triangles[i] * n_bytes_per_triangle, ben_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_BEN_FRAMES; i++)
		free(ben_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &ben_VAO);
	glBindVertexArray(ben_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, ben_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void prepare_ironman(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, ironman_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/ironman_vnt.geom");
	ironman_n_triangles = read_geometry(&ironman_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	ironman_n_total_triangles += ironman_n_triangles;


	// initialize vertex buffer object
	glGenBuffers(1, &ironman_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, ironman_VBO);
	glBufferData(GL_ARRAY_BUFFER, ironman_n_total_triangles * 3 * n_bytes_per_vertex, ironman_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(ironman_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &ironman_VAO);
	glBindVertexArray(ironman_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, ironman_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void draw_tiger(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(tiger_VAO);
	glDrawArrays(GL_TRIANGLES, tiger_vertex_offset[cur_frame_tiger], 3 * tiger_n_triangles[cur_frame_tiger]);
	glBindVertexArray(0);
}
void draw_cow(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(cow_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * cow_n_triangles);
	glBindVertexArray(0);
}
void draw_spider(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(spider_VAO);
	glDrawArrays(GL_TRIANGLES, spider_vertex_offset[cur_frame_spider], 3 * spider_n_triangles[cur_frame_spider]);
	glBindVertexArray(0);
}
void draw_ben(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(ben_VAO);
	glDrawArrays(GL_TRIANGLES, ben_vertex_offset[cur_frame_ben], 3 * ben_n_triangles[cur_frame_ben]);
	glBindVertexArray(0);
}
void draw_ironman(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(ironman_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * ironman_n_triangles);
	glBindVertexArray(0);
}


/***************************
*
*  PREPARE & DRAW CAR
*
****************************/


#define N_GEOMETRY_OBJECTS 3
#define GEOM_OBJ_ID_CAR_BODY 0
#define GEOM_OBJ_ID_CAR_WHEEL 1
#define GEOM_OBJ_ID_CAR_NUT 2

GLuint geom_obj_VBO[N_GEOMETRY_OBJECTS];
GLuint geom_obj_VAO[N_GEOMETRY_OBJECTS];

int geom_obj_n_triangles[N_GEOMETRY_OBJECTS];
GLfloat *geom_obj_vertices[N_GEOMETRY_OBJECTS];

typedef enum _GEOM_OBJ_TYPE { GEOM_OBJ_TYPE_V = 0, GEOM_OBJ_TYPE_VN, GEOM_OBJ_TYPE_VNT } GEOM_OBJ_TYPE;
int GEOM_OBJ_ELEMENTS_PER_VERTEX[3] = { 3, 6, 8 };


int read_geometry_file(GLfloat **object, char *filename, GEOM_OBJ_TYPE geom_obj_type) {
	int i, n_triangles;
	float *flt_ptr;
	FILE *fp;

	fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the geometry file %s ...", filename);
		return -1;
	}

	fscanf(fp, "%d", &n_triangles);
	*object = (float *)malloc(3 * n_triangles*GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(float));
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	flt_ptr = *object;
	for (i = 0; i < 3 * n_triangles * GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type]; i++)
		fscanf(fp, "%f", flt_ptr++);
	fclose(fp);

	fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);

	return n_triangles;
}

void prepare_geom_obj(int geom_obj_ID, const char *filename, GEOM_OBJ_TYPE geom_obj_type) {
	int n_bytes_per_vertex;

	n_bytes_per_vertex = GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(float);
	geom_obj_n_triangles[geom_obj_ID] = read_geometry_file(&geom_obj_vertices[geom_obj_ID], (char*)filename, geom_obj_type);

	// Initialize vertex array object.
	glGenVertexArrays(1, &geom_obj_VAO[geom_obj_ID]);
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	glGenBuffers(1, &geom_obj_VBO[geom_obj_ID]);
	glBindBuffer(GL_ARRAY_BUFFER, geom_obj_VBO[geom_obj_ID]);
	glBufferData(GL_ARRAY_BUFFER, 3 * geom_obj_n_triangles[geom_obj_ID] * n_bytes_per_vertex,
		geom_obj_vertices[geom_obj_ID], GL_STATIC_DRAW);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	if (geom_obj_type >= GEOM_OBJ_TYPE_VN) {
		glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	if (geom_obj_type >= GEOM_OBJ_TYPE_VNT) {
		glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	free(geom_obj_vertices[geom_obj_ID]);
}

void draw_geom_obj(int geom_obj_ID) {
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	glDrawArrays(GL_TRIANGLES, 0, 3 * geom_obj_n_triangles[geom_obj_ID]);
	glBindVertexArray(0);
}

void free_geom_obj(int geom_obj_ID) {
	glDeleteVertexArrays(1, &geom_obj_VAO[geom_obj_ID]);
	glDeleteBuffers(1, &geom_obj_VBO[geom_obj_ID]);
}


float rad = 1.7f;
float ww = 1.0f;
float rotation_angle_wheel = 0.0f;
float rotation_angle_vert_wheel = 30.0f;

void draw_wheel_and_nut() {
	// angle is used in Hierarchical_Car_Correct later
	int i;

	glUniform3f(loc_primitive_color, 0.000f, 0.808f, 0.820f); // color name: DarkTurquoise
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL); // draw wheel

	glm::vec3 nut_pos;

	for (i = 0; i < 5; i++) {
		ModelMatrix_CAR_NUT = glm::rotate(ModelMatrix_CAR_WHEEL, TO_RADIAN*72.0f*i, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix_CAR_NUT = glm::translate(ModelMatrix_CAR_NUT, glm::vec3(rad - 0.5f, 0.0f, ww));
		ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_CAR_NUT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		glUniform3f(loc_primitive_color, 0.690f, 0.769f, 0.871f); // color name: LightSteelBlue
		draw_geom_obj(GEOM_OBJ_ID_CAR_NUT); // draw i-th nut
	}
}

void draw_car_dummy(void) {
	glUniform3f(loc_primitive_color, 0.498f, 1.000f, 0.831f); // color name: Aquamarine
	draw_geom_obj(GEOM_OBJ_ID_CAR_BODY); // draw body

	glLineWidth(2.0f);
	draw_axes(); // draw MC axes of body
	glLineWidth(1.0f);

	ModelMatrix_CAR_DRIVER = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.0f, 0.5f, 2.5f));
	ModelMatrix_CAR_DRIVER = glm::rotate(ModelMatrix_CAR_DRIVER, TO_RADIAN*90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_CAR_DRIVER;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes(); // draw camera frame at driver seat
	glLineWidth(1.0f);

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, 4.5f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_vert_wheel * TO_RADIAN, glm::vec3(0, 1, 0));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_wheel * TO_RADIAN, glm::vec3(0, 0, 1));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 0

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, 4.5f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_vert_wheel * TO_RADIAN, glm::vec3(0, 1, 0));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_wheel * TO_RADIAN, glm::vec3(0, 0, 1));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 1

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, -4.5f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_vert_wheel * TO_RADIAN, glm::vec3(0, 1, 0));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_wheel * TO_RADIAN, glm::vec3(0, 0, 1));
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 2

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, -4.5f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_vert_wheel * TO_RADIAN, glm::vec3(0, 1, 0));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, rotation_angle_wheel * TO_RADIAN, glm::vec3(0, 0, 1));
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 3
}

GLuint obj1_VBO, obj1_VAO;
GLfloat obj1_vertices[26][8] = {
	{ -5.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -10.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -10.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -2.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -2.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -2.0f, -12.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -2.0f, -12.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 10.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 10.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },

	{ 5.0f, -10.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -2.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -2.0f, -12.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 10.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },

	{ -5.0f, -10.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -2.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -2.0f, -12.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 10.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
};

void prepare_obj1() {
	// initialize vertex buffer object
	glGenBuffers(1, &obj1_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, obj1_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obj1_vertices), &obj1_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object
	glGenVertexArrays(1, &obj1_VAO);
	glBindVertexArray(obj1_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, obj1_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_obj1(void) {
	glFrontFace(GL_CCW);

	GLfloat color[3] = { 226.0f / 255, 226.0f / 255, 226.0f / 255 };
	glBindVertexArray(obj1_VAO);
	glUniform3fv(loc_primitive_color, 1, color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
	glDrawArrays(GL_TRIANGLE_STRIP, 14, 6);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 6);
	glBindVertexArray(0);
}

GLuint obj2_VBO, obj2_VAO;
GLfloat obj2_vertices[26][8] = {
	{ -5.0f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -15.0f, -21.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -15.0f, -21.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 1.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 1.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 1.0f, -13.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 1.0f, -13.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 15.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 15.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },

	{ 5.0f, -15.0f, -21.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 1.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 1.0f, -13.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 15.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },

	{ 5.0f, -15.0f, -21.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 1.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 1.0f, -13.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 15.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
};

void prepare_obj2() {
	// initialize vertex buffer object
	glGenBuffers(1, &obj2_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, obj2_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obj2_vertices), &obj2_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object
	glGenVertexArrays(1, &obj2_VAO);
	glBindVertexArray(obj2_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, obj2_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_obj2(void) {
	glFrontFace(GL_CCW);

	GLfloat color[3] = { 226.0f / 255, 226.0f / 255, 226.0f / 255 };
	glBindVertexArray(obj2_VAO);
	glUniform3fv(loc_primitive_color, 1, color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
	glDrawArrays(GL_TRIANGLE_STRIP, 14, 6);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 6);
	glBindVertexArray(0);
}

GLuint obj3_VBO, obj3_VAO;
GLfloat obj3_vertices[22][8] = {
	{ -5.0f, -6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -6.0f, -18.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -6.0f, -18.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 0.0f, -6.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 0.0f, -6.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 0.0f, -11.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 0.0f, -11.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 6.0f, -0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 6.0f, -0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },

	{ 5.0f, -6.0f, -18.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, -6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 0.0f, -6.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 5.0f, 0.0f, -11.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },

	{ -5.0f, -6.0f, -18.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, -6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 0.0f, -6.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ -5.0f, 0.0f, -11.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
};

void prepare_obj3() {
	// initialize vertex buffer object
	glGenBuffers(1, &obj3_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, obj3_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obj3_vertices), &obj3_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object
	glGenVertexArrays(1, &obj3_VAO);
	glBindVertexArray(obj3_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, obj3_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_obj3(void) {
	glFrontFace(GL_CCW);

	GLfloat color[3] = { 226.0f / 255, 226.0f / 255, 226.0f / 255 };
	glBindVertexArray(obj3_VAO);
	glUniform3fv(loc_primitive_color, 1, color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 5);
	glDrawArrays(GL_TRIANGLE_STRIP, 17, 5);
	glBindVertexArray(0);
}