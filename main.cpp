//页表转换
//给定mem.txt模拟内存，运行后输入虚地址，如0x2c03 
#include <iostream> 
#define MEM_SIZE 32768 

using namespace std; 
unsigned char mem[MEM_SIZE]; 


int main(int argc, char** argv) 
{ 
    // init Memory 
    FILE *fin = fopen("mem.txt","r"); 
    for (int i = 0; i < MEM_SIZE; ++i) 
    { 
        unsigned int ch; 
        fscanf(fin,"%x",&ch); 
        mem[i] = (unsigned char)ch; 
    } 
    fclose(fin); 
    // init Memory
	
    unsigned int ptdr = 0x220; 
    unsigned int vaddr,paddr,offset; 
    unsigned int pde,pde_addr,pde_index,pde_valid;
    unsigned int pte,pte_addr,pte_index,pte_valid;
    unsigned char data; 

    printf("input address:"); 
    scanf("%x",&vaddr); 
    printf("Virtual Address 0x%04x\n", vaddr); 

    offset=(vaddr&0x1f);
    pde_index=(vaddr&(0x1f<<10))>> 10; 
    printf("  pde index:0x%02x ",pde_index);
     
    pde=mem[ptdr+pde_index];
    pde_valid=(pde&0x80)>>7; 
    if (!pde_valid) 
    { 
        printf("    Fault (page directory entry not valid)\n"); 
        return -1; 
    }
    pde_addr=(pde&0x7f); 
    printf("pde contents:(valid %d, pfn 0x%02x)\n",pde_valid,pde_addr); 
     
    pte_index=(vaddr&(0x1f<<5))>>5;
    printf("    pte index:0x%02x ",pte_index); 
	
    pte=mem[pde_addr*32+pte_index]; 
    pte_valid=(pte&0x80)>>7; 
    if (!pte_valid)
    { 
        printf("      Fault (page table entry not valid)\n"); 
        return -1; 
    } 
    pte_addr=(pte&0x7f);      
    printf("pte contents:(valid %d, pfn 0x%02x)\n",pte_valid,pte_addr); 

    paddr=pte_addr*32+offset; 
    data=mem[paddr]; 
    printf("      Translates to Physical Address 0x%04x --> Value: 0x%02x\n",paddr,data); 
    return 0; 
} 
