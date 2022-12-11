#include <stdio.h>
#include <stdlib.h>
#include <kernel_types.h>
#include <pci.h>
#include <pci_regs.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	u32 header, cap, ctrl, status;
	u16 p = 0;

	if (argc != 2) {
		fprintf(stderr, "argv[1] must be an input file\n");
		exit(EXIT_FAILURE);
	}
	printf("Processing config space=%s\n", argv[1]);

	fp = fopen(argv[1], "rb");
	if (!fp) {
		perror("fopen");
		return EXIT_FAILURE;
	}

	if (pci_read_config_dword(fp, 0, &header) != PCIBIOS_SUCCESSFUL)
		return 0;

	printf("vendor-device=%x\n", header);

	while ((p = pci_find_next_ext_capability(fp, p, PCI_EXT_CAP_ID_DOE))) {
		pci_read_config_dword(fp, p + PCI_DOE_CAP, &header);
		pci_read_config_dword(fp, p + PCI_DOE_CAP, &cap);
		pci_read_config_dword(fp, p + PCI_DOE_CTRL, &ctrl);
		pci_read_config_dword(fp, p + PCI_DOE_STATUS, &status);
		printf("\tdoe at %x\n", p);
		printf("\t\theader %x\n", header);
		printf("\t\tcap %x\n", cap);
		printf("\t\tctrl %x\n", ctrl);
		printf("\t\tstatus %x\n", status);
		printf("\t\tnot touching read/write as it will affect dev\n");
	}

	exit(EXIT_SUCCESS);
}
