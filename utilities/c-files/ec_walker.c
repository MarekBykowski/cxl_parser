#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int
main(void)
{
	FILE *fp = fopen("qemu_cxldev_mem_dump", "rb");
	if (!fp) {
		perror("fopen");
		return EXIT_FAILURE;
	}

	unsigned char buffer[4];

	size_t ret = fread(buffer, ARRAY_SIZE(buffer), sizeof(*buffer), fp);
	if (ret != sizeof(*buffer)) {
		fprintf(stderr, "fread() failed: %zu\n", ret);
		exit(EXIT_FAILURE);
	}

	printf("vendor-device: %02x%02x%02x%02x\n",
	       buffer[0], buffer[1], buffer[2], buffer[3]);

	ret = fread(buffer, 1, 1, fp);
	if (ret != 1) {
		fprintf(stderr, "fread() failed: %zu\n", ret);
		exit(EXIT_FAILURE);
	}

	printf("Class: %#04x\n", buffer[0]);

	fclose(fp);

	exit(EXIT_SUCCESS);
}

