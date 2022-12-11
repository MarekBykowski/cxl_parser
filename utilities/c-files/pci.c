#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* user-defined header files */
#include <kernel_types.h>
#include <pci.h>
#include <pci_regs.h>
#include <debug_or_not.h>

/**
 * Based on:
 *	arch/x86/pci/mmconfig_64.c
 *	include/linux/pci.h
 *	include/uapi/linux/pci_regs.h
 */

#define PCI_byte_BAD 0
#define PCI_word_BAD (pos & 1)
#define PCI_dword_BAD (pos & 3)

#define PCI_OP_READ(size, type, len) \
int __attribute__((__noinline__)) pci_read_config_##size		\
		(FILE *fp, int pos, type *value)			\
{									\
	int res;							\
	u32 data = 0;							\
									\
	if (PCI_##size##_BAD) return PCIBIOS_BAD_REGISTER_NUMBER;	\
	res = fseek(fp, (unsigned long)pos, SEEK_SET);			\
	if (res) {							\
		fprintf(stderr, "fseek failed: %s\n", strerror(res));	\
		return PCIBIOS_BAD_REGISTER_NUMBER;			\
	}								\
	res = fread(&data, len, 1, fp);					\
	pr_debug("fp=%p data=%x res=%d\n", fp, data, res);		\
	if (0 == res)							\
		PCI_SET_ERROR_RESPONSE(value);				\
	else {								\
		*value = (type)data;					\
		pr_debug("fp=%p value=%x (from *value=(type)data)\n",	\
			 fp, *value);					\
	}								\
									\
	return !res;							\
}

PCI_OP_READ(byte, u8, 1)
PCI_OP_READ(word, u16, 2)
PCI_OP_READ(dword, u32, 4)

/**
 * pci_find_next_ext_capability - Find an extended capability
 * @fp: FILE pointer to query
 * @start: address at which to start looking (0 to start at beginning of list)
 * @cap: capability code
 *
 * Returns the address of the next matching extended capability structure
 * within the device's PCI configuration space or 0 if the device does
 * not support it.  Some capabilities can occur several times, e.g., the
 * vendor-specific capability, and this provides a way to find them all.
 */
u16 pci_find_next_ext_capability(FILE *fp, u16 start, int cap)
{
	u32 header;
	int ttl;
	u16 pos = PCI_CFG_SPACE_SIZE;

	/* minimum 8 bytes per capability */
	ttl = (PCI_CFG_SPACE_EXP_SIZE - PCI_CFG_SPACE_SIZE) / 8;

#define cfg_size 4096
	if (cfg_size <= PCI_CFG_SPACE_SIZE)
		return 0;

	if (start)
		pos = start;

	if (pci_read_config_dword(fp, pos, &header) != PCIBIOS_SUCCESSFUL)
		return 0;

	/*
	 * If we have no capabilities, this is indicated by cap ID,
	 * cap version and next pointer all being 0.
	 */
	if (header == 0)
		return 0;

	while (ttl-- > 0) {
		if (PCI_EXT_CAP_ID(header) == cap && pos != start)
			return pos;

		pos = PCI_EXT_CAP_NEXT(header);
		if (pos < PCI_CFG_SPACE_SIZE)
			break;

		if (pci_read_config_dword(fp, pos, &header) != PCIBIOS_SUCCESSFUL)
			break;
	}

	return 0;
}
