#ifndef LINUX_PCI_H
#define LINUX_PCI_H

#include <stdio.h>

/* Error values that may be returned by PCI functions */
#define PCIBIOS_SUCCESSFUL		0x00
#define PCIBIOS_FUNC_NOT_SUPPORTED	0x81
#define PCIBIOS_BAD_VENDOR_ID		0x83
#define PCIBIOS_DEVICE_NOT_FOUND	0x86
#define PCIBIOS_BAD_REGISTER_NUMBER	0x87
#define PCIBIOS_SET_FAILED		0x88
#define PCIBIOS_BUFFER_TOO_SMALL	0x89

/*
 * Reading from a device that doesn't respond typically returns ~0.  A
 * successful read from a device may also return ~0, so you need additional
 * information to reliably identify errors.
 */
#define PCI_ERROR_RESPONSE		(~0ULL)
#define PCI_SET_ERROR_RESPONSE(val)	(*(val) = ((typeof(*(val))) PCI_ERROR_RESPONSE))
#define PCI_POSSIBLE_ERROR(val)		((val) == ((typeof(val)) PCI_ERROR_RESPONSE))

/*
 * Conventional PCI and PCI-X Mode 1 devices have 256 bytes of
 * configuration space.  PCI-X Mode 2 and PCIe devices have 4096 bytes of
 * configuration space.
 */
#define PCI_CFG_SPACE_SIZE	256
#define PCI_CFG_SPACE_EXP_SIZE	4096

u16 pci_find_next_ext_capability(FILE *fp, u16 start, int cap);
int pci_read_config_byte(FILE *fp, int where, u8 *val);
int pci_read_config_word(FILE *fp, int where, u16 *val);
int pci_read_config_dword(FILE *fp, int where, u32 *val);

#endif
