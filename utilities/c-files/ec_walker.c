#include <stdio.h>
#include <stdlib.h>
#include <kernel_types.h>
#include <pci.h>
#include <pci_regs.h>
#include <cxlpci.h>

#define NR_EC PCI_EXT_CAP_ID_MAX + 1
static const char *ext_cap_types[NR_EC] = {
#define S(x,s)	[x] = s
	S(PCI_EXT_CAP_ID_ERR, "Advanced Error Reporting"),
	S(PCI_EXT_CAP_ID_VC, "Virtual Channel Capability"),
	S(PCI_EXT_CAP_ID_DSN, "Device Serial Number"),
	S(PCI_EXT_CAP_ID_PWR, "Power Budgeting"),
	S(PCI_EXT_CAP_ID_RCLD, "Root Complex Link Declaration"),
	S(PCI_EXT_CAP_ID_RCILC, "Root Complex Internal Link Control"),
	S(PCI_EXT_CAP_ID_RCEC, "Root Complex Event Collector"),
	S(PCI_EXT_CAP_ID_MFVC, "Multi-Function VC Capability"),
	S(PCI_EXT_CAP_ID_VC9, "same as _VC"),
	S(PCI_EXT_CAP_ID_RCRB, "Root Complex RB?"),
	S(PCI_EXT_CAP_ID_VNDR, "Vendor-Specific"),
	S(PCI_EXT_CAP_ID_CAC, "Config Access - obsolete"),
	S(PCI_EXT_CAP_ID_ACS, "Access Control Services"),
	S(PCI_EXT_CAP_ID_ARI, "Alternate Routing ID"),
	S(PCI_EXT_CAP_ID_ATS, "Address Translation Services"),
	S(PCI_EXT_CAP_ID_SRIOV, "Single Root I/O Virtualization"),
	S(PCI_EXT_CAP_ID_MRIOV, "Multi Root I/O Virtualization"),
	S(PCI_EXT_CAP_ID_MCAST, "Multicast"),
	S(PCI_EXT_CAP_ID_PRI, "Page Request Interface"),
	S(PCI_EXT_CAP_ID_AMD_XXX, "Reserved for AMD"),
	S(PCI_EXT_CAP_ID_REBAR, "Resizable BAR"),
	S(PCI_EXT_CAP_ID_DPA, "Dynamic Power Allocation"),
	S(PCI_EXT_CAP_ID_TPH, "TPH Requester"),
	S(PCI_EXT_CAP_ID_LTR, "Latency Tolerance Reporting"),
	S(PCI_EXT_CAP_ID_SECPCI, "Secondary PCIe Capability"),
	S(PCI_EXT_CAP_ID_PMUX, "Protocol Multiplexing"),
	S(PCI_EXT_CAP_ID_PASID, "Process Address Space ID"),
	S(PCI_EXT_CAP_ID_DPC, "Downstream Port Containment"),
	S(PCI_EXT_CAP_ID_L1SS, "L1 PM Substates"),
	S(PCI_EXT_CAP_ID_PTM, "Precision Time Measurement"),
	S(PCI_EXT_CAP_ID_DVSEC, "Designated Vendor-Specific"),
	S(PCI_EXT_CAP_ID_DLF, "Data Link Feature"),
	S(PCI_EXT_CAP_ID_PL_16GT, "Physical Layer 16.0 GT/s"),
	S(PCI_EXT_CAP_ID_DOE, "Data Object Exchange"),
};

#define READ_DOE do {							\
	u32 header, cap, ctrl, status;					\
	pci_read_config_dword(fp, p, &header);				\
	pci_read_config_dword(fp, p + PCI_DOE_CAP, &cap);		\
	pci_read_config_dword(fp, p + PCI_DOE_CTRL, &ctrl);		\
	pci_read_config_dword(fp, p + PCI_DOE_STATUS, &status);		\
	printf("\t\theader %x\n", header);				\
	printf("\t\tcap %x\n", cap);					\
	printf("\t\tctrl %x\n", ctrl);					\
	printf("\t\tstatus %x\n", status);				\
	printf("\t\tnot touching read/write as it will affect dev\n");	\
	} while (0)

#define READ_DVSEC do {							\
	u32 header, v, id, i;						\
	u16 cap, ctrl;							\
	pci_read_config_dword(fp, p, &header);				\
	pci_read_config_dword(fp, p + PCI_DVSEC_HEADER1, &v);		\
	pci_read_config_dword(fp, p + PCI_DVSEC_HEADER2, &id);		\
	pci_read_config_word(fp, p + CXL_DVSEC_CAP_OFFSET, &cap);	\
	pci_read_config_word(fp, p + CXL_DVSEC_CTRL_OFFSET, &ctrl);	\
	printf("\t\theader %x\n", header);				\
	printf("\t\tvendorID %x\n", PCI_DVSEC_HEADER1_VID(v));		\
	printf("\t\trevisionID %x\n", PCI_DVSEC_HEADER1_REV(v));	\
	printf("\t\tlength %x\n", PCI_DVSEC_HEADER1_LEN(v));		\
	printf("\t\tID %x\n", PCI_DVSEC_HEADER2_ID(id));		\
	printf("\t\tcap %x\n", cap);					\
	printf("\t\tctrl %x\n", ctrl);					\
	for (i = 0; i < 2; i++)						\
		printf("\t\trange[%d]: start-end %s\n",			\
			 i, "0xdeadbeef aka later :-)");		\
	} while (0)

int main(int argc, char *argv[])
{
	FILE *fp;
	u32 header;
	u16 p = 0;
	unsigned int i;

	if (argc != 2) {
		fprintf(stderr, "argv[1] must be an input file\n");
		exit(EXIT_FAILURE);
	}

	printf("# List of all PCIe Extended Capabilites\n");
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
	for (i = 0; i <= ARRAY_SIZE(ext_cap_types); i++) {
		if (ext_cap_types[i] == NULL)
			continue;
		printf("\tEC[%x] -> %s\n", i, ext_cap_types[i]);
	}

	puts("\n");

	fp = fopen(argv[1], "rb");
	if (!fp) {
		perror("fopen");
		return EXIT_FAILURE;
	}

	if (pci_read_config_dword(fp, 0, &header) != PCIBIOS_SUCCESSFUL)
		return 0;

	printf("# Search for all the ECs for vendor:device \"%04x:%04x\" from mem-dumped config space \"%s\"\n",
	       header & 0xffff, (header >> 16) & 0xffff, argv[1]);


	while ((p = pci_find_next_ext_capability(fp, p, PCI_EXT_CAP_ID_DOE))) {
	}

	for (i = 0; i <= ARRAY_SIZE(ext_cap_types); i++) {
		if (ext_cap_types[i] == NULL)
			continue;
		while ((p = pci_find_next_ext_capability(fp, p, i))) {
			printf("\tEC[%x] -> %s @ %x\n", i, ext_cap_types[i], p);
			switch (i) {
			case PCI_EXT_CAP_ID_DOE:
				READ_DOE; break;
			case PCI_EXT_CAP_ID_DVSEC:
				READ_DVSEC; break;
			default:
			{
				int it = 0;
				u32 val;
				do {
					if (pci_read_config_dword(fp, p + it, &val) == PCIBIOS_SUCCESSFUL)
						printf("\t\t %x @ %x\n", val, p + it);
				} while((it += 4) < 8);
			}
			}
		}
	}

	exit(EXIT_SUCCESS);
}
