#ifndef CCHUNK_H
#define CCHUNK_H

#define FUNC_TYPE1        0x00000001
#define FUNC_TYPE2        0x00000002
#define FUNC_TYPE3        0x00000003
#define FUNC_TYPE4        0x00000004
#define FUNC_TYPE5        0x00000005

struct function {
	unsigned int type;
	unsigned int offset;
	void *(*fptr)();
};

struct code_chunks {
	unsigned int nfns;
	unsigned int size;
	unsigned int pages;

	struct function *fns;

	unsigned char __attribute__((aligned(4))) code[0];
};

extern void call_function(struct code_chunks *chunk, unsigned int type, void *arg);
extern struct code_chunks *get_code_chunks(void);
extern void dump_chunks(struct code_chunks *chunk);
extern void cleanup_chunk(struct code_chunks *chunk);

#define __mute_array  __attribute__((section(".mute_array"))) __attribute__((unused))
#define __mutable     __attribute__((section(".mutable")))

#endif
