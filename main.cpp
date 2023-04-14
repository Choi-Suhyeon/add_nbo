#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <memory.h>

typedef struct {
	uint8_t  * arr;
	uint32_t size;
} Bytes, * pBytes;

void freeBytes(pBytes * ppbytes) {
	memset((*ppbytes)->arr, 0, (*ppbytes)->size);
	free((*ppbytes)->arr);

	memset(*ppbytes, 0, sizeof(Bytes));
	free(*ppbytes);
	
	*ppbytes = NULL;
}	

pBytes readFile(char * fileName) {
	FILE * fp = fopen(fileName, "rb");

	if (!fp) {
		printf("Error: file not found: %s\n", fileName);
		exit(1);
	}

	pBytes result = (pBytes)calloc(1, sizeof(Bytes));

	fseek(fp, 0, SEEK_END);

	result->size = ftell(fp) + 1;
	result->arr  = (uint8_t *)calloc(result->size, sizeof(uint8_t));

	fseek(fp, 0, SEEK_SET);
	fgets((char *)result->arr, result->size, fp);
	fclose(fp);

	return result;
}

uint32_t ntohl(uint8_t * arr) {
	uint32_t result = 0;

	__asm__ __volatile__ (
		"movl   (%%rdi), %%eax \t\n"
		"bswapl %%eax          \t\n"
		: "=a"(result)
		: "D"(arr)
		:
	);

	return result;
}
		
int main(int argc, char ** argv) {
	if (argc ^ 3) {
		puts("Error: The number of passed arguments is not correct (expected: 3)");

		return 1;
	}

	pBytes   fst     = readFile(argv[1]),
             snd     = readFile(argv[2]);
	uint32_t fst_num = ntohl(fst->arr),
			 snd_num = ntohl(snd->arr),
			 sum_res = fst_num + snd_num;

	freeBytes(&fst);
	freeBytes(&snd);

	printf("%u(%#x) + ", fst_num, fst_num);
	printf("%u(%#x) = ", snd_num, snd_num);
	printf("%u(%#x) \n", sum_res, sum_res);
	
	return 0;
}
