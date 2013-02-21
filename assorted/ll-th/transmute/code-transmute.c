#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <errno.h>
#include <error.h>

#include <unistd.h>
#include <sys/mman.h>

#include <dis-asm.h>

#include "cchunk.h"

/* fix plts: well, the tricky part is: this is so weird. we go by
 * matching the call instruction 'e8' and then relatively move the
 * offset! well, trivially, this would screw things up if some other
 * instruction coincidentally matches to have 'e8' as the part of its
 * opcode!
 *
 * this also assumes that the functions are arranged, address-wise!!
 */
static int insn_read_memory(bfd_vma vma, bfd_byte *buf, unsigned int length,
			    struct disassemble_info *info)
{
	if (vma < info->buffer_vma ||
	    (vma + length) > info->buffer_vma + info->buffer_length)
		return EIO;

	memcpy(buf, info->buffer + (vma - info->buffer_vma), length);
	return 0;
}

static void fix_plts(struct code_chunks *chunk)
{
	int opc, fn, c;
	extern unsigned int mutable_start;
	unsigned long csz, pc, addr, fn_orig_base, fnplt;
	unsigned char *code, *reldist;

	/* BFD stuffs */
	struct disassemble_info dis;
	//char disbuf[16384];

	/* Initialize BFD */
	//INIT_DISASSEMBLE_INFO(dis, disbuf, sprintf);
	INIT_DISASSEMBLE_INFO(dis, stdout, fprintf);
	dis.read_memory_func = insn_read_memory;
	dis.endian = BFD_ENDIAN_LITTLE;
	dis.mach = bfd_mach_i386_i386;

	fn_orig_base = (unsigned long)chunk->code;

	for (fn = 0; fn < chunk->nfns; fn++) {
		code = (unsigned char *)chunk->fns[fn].fptr;

		if (fn+1 == chunk->nfns) {
			csz = (unsigned long)chunk->code + chunk->size -
				(unsigned long)code;
		} else {
			csz = (unsigned long)chunk->fns[fn+1].fptr -
				(unsigned long)code;
		}

		pc = (unsigned long)code;
		opc = 0;

		dis.buffer = code;
		dis.buffer_vma = pc;
		dis.buffer_length = csz;

		printf("%s: code = %p, size = %lu\n", __func__, code, csz);

		/* opc -> offset of pc */
		while (opc < csz) {
			c = print_insn_i386_att(pc, &dis);
			if (c < 0) break;
			opc += c;
			pc += c;
			printf("\n");
		}

		opc = 0;
		while (opc < csz) {
			if (code[opc] == 0xe8 && opc+4 < csz) {
				reldist = (unsigned char *)&addr;
				reldist[0] = code[opc+1];
				reldist[1] = code[opc+2];
				reldist[2] = code[opc+3];
				reldist[3] = code[opc+4];

				/* patch-address = PC - _PLT_ADDRESS_ */
				/* 5 = 0xe8 + <32-bit-offset> */
				fnplt = ((unsigned long)&mutable_start +
					 (pc - fn_orig_base) + addr) + 5;
				addr = fnplt - (pc + 5);

				code[opc+1] = reldist[0];
				code[opc+2] = reldist[1];
				code[opc+3] = reldist[2];
				code[opc+4] = reldist[3];

				opc += 4;
				pc += 4;
			}
			opc++;
			pc++;
		}
	}

	printf("\n ************ Disassembled output *******************\n");
	//printf("%s\n", disbuf);
}

/* read the code chunks */
struct code_chunks *get_code_chunks(void)
{
	extern unsigned int mute_hdr_start, mute_hdr_end,
		mutable_start, mutable_end;
	struct code_chunks *chunk;
	char *code;
	size_t page = getpagesize();
	int nf, csz, np, i;

	/* number of functions, also includes magic header */
	nf = (&mute_hdr_end - &mute_hdr_start) >> 1;

	printf("%s: start = %p, end = %p\n", __func__,
	       &mute_hdr_start, &mute_hdr_end);

	for (i = 0; &mute_hdr_start+i <= &mute_hdr_end; i++)
		printf("\t=== %x\n", *(&mute_hdr_start+i));

	/* code size */
	csz = (char *)&mutable_end - (char *)&mutable_start;

	/* map pages with EXEC flag set */
	np = csz/page + 1;
	chunk = mmap(NULL, page * np, PROT_EXEC | PROT_READ | PROT_WRITE,
		     MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (chunk == (void *)-1) {
		perror("mmap");
		return NULL;
	}

	chunk->nfns = nf;
	chunk->size = csz;
	chunk->pages = page * np;

	chunk->fns = (struct function *)calloc(nf, sizeof(struct function));
	if (!chunk->fns) {
		munmap(chunk, chunk->pages);
		return NULL;
	}

	/* copy the code */
	code = ((unsigned char *)chunk) + sizeof(struct code_chunks);
	memcpy(code, &mutable_start, csz);

	/* calculate relative offsets & link up function pointers */
	for (i = 0; i < nf; i++) {
		chunk->fns[i].offset = *(&mute_hdr_start + ((i << 1)+1)) -
			(unsigned int)&mutable_start;
		chunk->fns[i].type = *(&mute_hdr_start + (i << 1));
		chunk->fns[i].fptr = (void *(*)())((unsigned int)code +
						   chunk->fns[i].offset);
	}

	/* fix PLTs for each of the functions! */
	fix_plts(chunk);

	return chunk;
}

static void test_gots(void)
{
	extern unsigned int _GLOBAL_OFFSET_TABLE_, _init, ____got_table;;
	unsigned int *got = &_GLOBAL_OFFSET_TABLE_, *init = &_init,
		*got_table = &____got_table;

	printf("GOT offset @ %p, _init @ %p, GOT table @ %p\n",
	       got, init, got_table);
}

void dump_chunks(struct code_chunks *chunk)
{
	int i;

	test_gots();

	printf("\n\t\t *****  CHUNK INFORMATION  *****\n");
	printf("Functions: %u\n", chunk->nfns);
	printf("Code size: %u bytes\n", chunk->size);
	printf("Code dump:\n");

	for (i = 0; i < chunk->nfns; i++) {
		printf("\tOffset: 0x%08x, Type: 0x%08x, Address: %p\n",
		       chunk->fns[i].offset, chunk->fns[i].type,
		       chunk->fns[i].fptr);
	}
	for (i = 0; i < chunk->size; i++) {
		if ((i % 20) == 0) printf("\n\t");
		printf("%02x ", chunk->code[i]);
	}
	printf("\n\n");
}

#define MAX_PARAM_LENGTH     1024
struct magic {
	char magic[MAX_PARAM_LENGTH];
};

void call_function(struct code_chunks *chunk, unsigned int type, void *arg)
{
	int i;

	for (i = 0; i < chunk->nfns; i++) {
		if (chunk->fns[i].type == type) {
			printf("\ncalling function @ %p\n", chunk->fns[i].fptr);
			(chunk->fns[i].fptr)(*(struct magic *)arg);
		}
	}
}

void cleanup_chunk(struct code_chunks *chunk)
{
	if (!chunk->fns)
		free(chunk->fns);

	munmap(chunk, chunk->pages);
}
