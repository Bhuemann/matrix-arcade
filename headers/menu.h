struct color {
	char r;
	char g;
	char b;
	char unused;
};

void init_canvas(int argc, char **argv, unsigned int lines);
void printm(const struct color *buf, unsigned int lines, unsigned int cols);
void delete_matrix();
