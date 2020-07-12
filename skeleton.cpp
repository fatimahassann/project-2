#include "formats.h"

int main() {
    FILE *fp;
    unsigned short inst_word;
    
    fp = fopen("test1.bin","rb");
    
    if(NULL==fp) {
        printf("Cannot open the file\n");
        exit(0);
    }
    
    while(fread(&inst_word,2,1,fp))
    {
        if(inst_word == 0xdead)
            exit(0);
        else{
            printf("%08x\t%04x\t",PC,inst_word);
            simulate(inst_word);
            PC += 2;}
    }
    fclose(fp);
    cout<<endl;
    return 0;
}


void simulate(unsigned short instr)
{
    unsigned char fmt;
    fmt = (instr) >> 13;
    
    switch(fmt)
    {
        case 0:             // format 1/2
            format1_2(instr);
            break;
        case 1:             // format 3
            format3(instr);
            break;

        case 2:             // format 4->8
            format4_8(instr);
            break;

        case 3:             // format 9
            format9(instr);
            break;

        case 4:             // format 10->11
            format10_11(instr);
            break;

        case 5:            
            format12_14(instr);
            break;

        case 6:             // format 15->17
            format15_17(instr);
            break;

        case 7:
            format18_19(instr);
            break;
            
        default:
            printf("UNKNOWN INSTR!\n");
    }
    
}
