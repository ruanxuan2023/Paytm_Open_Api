#ifndef __PTABLE_H__
#define __PTABLE_H__

#include <stddef.h>
#include <stdint.h>
#include <string>

#define MAX_PTABLE_ENTRY_NAME   32
#define MAX_PTABLE_ENTRY_TYPE   16
#define MAX_PTABLE_PARTS        32
#define PTABLE_MAGIC_A          0x41505442
#define PTABLE_MAGIC            0x42505442

#define PTABLE_TYPE_LFS         "lfs"   /* littlefs */
#define PTABLE_TYPE_FFS         "ffs"   /* fatfs */
#define PTABLE_TYPE_RFS         "rfs"   /* romfs */
#define PTABLE_TYPE_ZIPFS       "zipfs" /* zipfs */
#define PTABLE_TYPE_RAW         "raw"
#define PTABLE_TYPE_UBI         "ubi"
#define PTABLE_TYPE_CUST        "cust"
#define PTABLE_TYPE_PART        "part"
#define PTABLE_TYPE_AREA        "area"  /* eMMC hardware area */
#define PTABLE_TYPE_FLASH       "flash"
#define PTABLE_TYPE_GROUP       "group"
#define PTABLE_TYPE_PRIVATE     "priv"

#define PTABLE_FLASH_EXTERNAL   "external"
#define PTABLE_FLASH_INTERNAL   "internal"

/*
 * @name: partition name, e.g. bootloader, radio
 * @type: partition type, e.g. ubi or raw
 * @start: partition start address within flash
 * @size: partition size in bytes
 * @vstart: partition image load address at runtime
 * @vsize: partition image real size in bytes
 * @depth: partition nested depth
 */
struct ptentry_a {
    char name[MAX_PTABLE_ENTRY_NAME];
    char type[MAX_PTABLE_ENTRY_TYPE];
    uint32_t start;
    uint32_t size;
    uint32_t vstart;
    uint32_t vsize;
    uint32_t depth;
};

struct ptentry {
    char name[MAX_PTABLE_ENTRY_NAME];
    char type[MAX_PTABLE_ENTRY_TYPE];
    uint64_t start;
    uint64_t size;
    uint64_t vstart;
    uint64_t vsize;
    uint8_t depth;
    uint8_t reserved[7];
};

/*
 * @magic: ptable magic id
 * @count: partition numbers
 * @parts: info for each partition entry
 */
struct ptable_a {
    uint32_t magic;
    uint32_t count;
    struct ptentry_a parts[MAX_PTABLE_PARTS];
    uint32_t crc;
};

struct ptable {
    uint32_t magic;
    uint32_t count;
    uint32_t crc;
    uint32_t reserved;
    struct ptentry parts[0];
};

/* tools to populate and query the partition table */
int ptable2json(const void *data, size_t size, std::string *json);

#endif /* __PTABLE_H__ */
