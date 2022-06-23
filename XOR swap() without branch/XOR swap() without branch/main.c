#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

/* 
Reference : ����õ(Megayuchi)
https://megayuchi.com/2015/09/10/xor%EC%9D%84-%EC%9D%B4%EC%9A%A9%ED%95%98%EC%97%AC-%EB%B6%84%EA%B8%B0-%EC%97%86%EC%9D%B4-%EB%91%90-%EB%B3%80%EC%88%98%EC%9D%98-%EA%B0%92-%EA%B5%90%ED%99%98%ED%95%98%EA%B8%B0/)
*/

int main()
{
    int a = 5;
    int b = 13;

    int ret_a;
    int ret_b;

    printf("Original\n");
    printf("a = %d, b = %d\n", a, b);

    /* C Original Code */
    if (a < b)
    {
        //swap
        int tmp = a;
        ret_a = b;
        ret_b = tmp;
    }
    printf("C Original Code\n");
    printf("a = %d, b = %d\n", ret_a, ret_b);

    /* ASM Code without Branch */
    __asm
    {
		mov eax, [a]
		mov ebx, [b]

		; carry mask
		xor ecx, ecx; ecx = 0
		cmp eax, ebx
		sbb ecx, 0; IF a < b, THEN 0xFFFFFFFF, ELSE 0

		; xor mask
		mov edx, eax;
		xor edx, ebx; a ^ b
		and edx, ecx; IF a < b, THEN a ^ b, ELSE 0

		; change
		xor eax, edx
		xor ebx, edx

		mov[ret_a], eax
		mov[ret_b], ebx
    }
    printf("ASM Code Without Branch\n");
    printf("a = %d, b = %d\n", ret_a, ret_b);

    /* C Code without Branch */
    {
        const int carry_mask = (a - b) >> ((CHAR_BIT * sizeof(int)) - 1); // IF a < b, THEN 0xFFFFFFFF, ELSE 0
        const int xor_mask = (a ^ b) & carry_mask; // IF a < b, THEN a ^ b, ELSE 0
        ret_a = a ^ xor_mask;
        ret_b = b ^ xor_mask;
    }
    printf("C Code Without Branch\n");
    printf("a = %d, b = %d\n", ret_a, ret_b);

    return 0;
}