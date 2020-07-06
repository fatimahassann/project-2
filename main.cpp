#include "stdio.h"
#include "stdlib.h"
#include "iostream"
using namespace std;

void simulate(unsigned short);

unsigned char Mem[1024];
unsigned int Regs[16];

#define    PC    Regs[15]
#define    LR    Regs[14]

int main() {
    FILE *fp;
    unsigned short inst_word;
    
    fp = fopen("test.s.bin","rb");
    
    if(NULL==fp) {
        printf("Cannot open the file\n");
        exit(0);
    }
    
    while(fread(&inst_word, 2,1, fp))
    {
        printf("%08x\t%04x\t", PC, inst_word);
        simulate(inst_word);
        PC += 2;
    }
    fclose(fp);
    return 0;
}


void simulate(unsigned short instr)
{
    unsigned int fmt, op, offset5, rd, rs, offset3, rn, offset8,sp, sword7,rb,ro,word8;
    
    fmt = (instr) >> 13;
    
    switch(fmt){
        case 0:             // format 1/2
            op = (instr >> 11) & 0x3;
            rd = instr & 0x7;
            rs = (instr >>  3) & 0x7;
            offset5 = (instr >> 6) & 0x1F;
            if(op!=3) {     // format 1
                switch(op){
                    case 0: printf("lsl\tr%d, r%d, #%d\n", rd, rs, offset5);
                        Regs[rd] = Regs[rs] << offset5;
                        break;
                    case 1: printf("lsr\tr%d, r%d, #%d\n", rd, rs, offset5);
                        Regs[rd] = Regs[rs] >> offset5;
                        break;
                    case 2: printf("asr\tr%d, r%d, #%d\n", rd, rs, offset5);
                        Regs[rd] = Regs[rs] >> offset5;//*************** sign thingy
                        break;
                }
            } else { //format 2
                offset3 = rn = offset5 & 0x07;
                if((offset5 & 0x08) == 0){
                    printf("add\tr%d, r%d, ", rd, rs);
                    if((offset5 & 0x10) == 0){
                        printf("r%d\n", rn);
                        Regs[rd] = Regs[rs] + Regs[rn];
                    }
                    else {
                        printf("#%d\n", offset3);
                        Regs[rd] = Regs[rs] + offset3;
                    }
                }
                else {
                    printf("sub\tr%d, r%d, ", rd, rs);
                    if((offset5 & 0x10) == 0){
                        printf("r%d\n", rn);
                        Regs[rd] = Regs[rs] - Regs[rn];
                    }
                    else {
                        printf("#%d\n", offset3);
                        Regs[rd] = Regs[rs] - offset3;
                    }
                }
            }
            break;
        case 1: //format 3
            op = (instr >> 11) & 0x3;
            rd = (instr>>8) & 0x7;
            offset8= instr & 0xFF;
            switch(op){
                case 0:
                    cout << "mov\tr" << rd << ", #" << offset8;
                    Regs[rd] = offset8;
                    break;
                case 1:
                    cout << "cmp\tr" << rd << ", #" << offset8;
                    if(Regs[rd]==offset8)//********************* benefty hena
                        Regs[rd]=1;
                    else Regs[rd] =0;
                    break;
                case 2:
                    cout << "add\tr"<< rd << ", #" << offset8;
                    Regs[rd] += offset8;
                    break;
                case 3:
                    cout << "sub\tr"<< rd << ", #" << offset8;
                    Regs[rd] -= offset8;
                    break;
            }
            break;
        case 2: //fomrat 4/5/6/7/8
            switch((instr >> 11) & 0x3){
                case 0:
                    if (((instr>>10) & 0x1)==0){ //format 4
                        op = (instr >> 6) & 0xF;
                        rd = instr & 0x7;
                        rs = (instr >> 3) &0x7;
                        switch(op){
                            case 0:
                                cout << "and\tr" << rd << ", r" << rs;
                                Regs[rd] = Regs[rd] & Regs[rs];
                                break;
                            case 1:
                                cout << "eor\tr" << rd << ", r"<< rs;//*************
                                break;
                            case 2:
                                cout << "lsl\tr" << rd << ", r"<< rs;
                                Regs[rd]= Regs[rd] << Regs[rs];
                                break;
                            case 3:
                                cout << "lsr\tr" << rd << ", r"<< rs;
                                Regs[rd]= Regs[rd] >> Regs[rs];
                                break;
                            case 4:
                                cout << "asr\tr" << rd << ", r"<< rs;
                                Regs[rd]= Regs[rd] >>Regs[rs];//*********************
                                break;
                            case 5:
                                cout << "adc\tr" << rd << ", r"<< rs;//*******
                                Regs[rd]= Regs[rd] + Regs[rs] + C;
                                break;
                            case 6:
                                cout << "sbc\tr" << rd << ", r"<< rs;//***********
                                Regs[rd]= Regs[rd] - Regs[rs] - C;
                                break;
                            case 7:
                                cout << "ror\tr" << rd << ", r"<< rs;//*********
                                
                                break;
                            case 8:
                                cout << "tst\tr" << rd << ", r" << rs;//********
                                
                                break;
                            case 9:
                                cout << "neg\tr" << rd << ", r"<< rs;
                                Regs[rd] = - Regs[rs];
                                break;
                            case 10:
                                cout << "cmp\tr" << rd << ", r"<< rs; //******
                                break;
                            case 11:
                                cout << "cmn\tr" << rd << ", r" << rs;//********
                                break;
                            case 12:
                                cout << "orr\tr" << rd << ", r"<< rs;
                                Regs[rd] = Regs[rd]|Regs[rs];
                                break;
                            case 13:
                                cout << "mul\tr" << rd << ", r" << rs;
                                Regs[rd] = Regs[rd] * Regs[rs];
                                break;
                            case 14:
                                cout << "bic\tr" << rd << ", r" << rs;
                                Regs[rd]= Regs[rd] & ~(Regs[rs]);
                                break;
                            case 15:
                                cout << "mvn\tr" << rd << ", r" << rs;
                                Regs[rd] = ~Regs[rs];
                                break;
                        }
                    }
                    else//format 5
                    {
                        unsigned int h1,h2;
                        
                        op = (instr >> 8) & 0x3;
                        rd=instr & 0x7;
                        rs = (instr>>3) & 0x7;
                        h1= (instr>>6) & 0x1;
                        h2= (instr >> 7) & 0x1;
                        
                        switch(op)
                        {
                            case 0:
                                if(h1==0)
                                {
                                    cout << "add\tr" << rd << ", r" << rs;
                                    
                                }
                                else {
                                    if (h2==0)
                                    {
                                        cout << "add\tr" << rd << ", r" << rs;
                                    }
                                    else
                                    {
                                        cout << "add\tr" << rd << ", r" << rs;
                                    }
                                    
                                }
                                break;
                            case 1:
                                if( h1== 0)
                                {
                                    cout << "cmp\tr" << rd << ", r" << rs;
                                }
                                else
                                {
                                    if ( h2== 0 )
                                    {
                                        cout << "cmp\tr" << rd << ", r" << rs;
                                    }
                                    else
                                    {
                                        cout << "cmp\tr" << rd << ", r" << rs;
                                    }
                                }
                                break;
                            case 2:
                                if( h1== 0)
                                {
                                    cout << "mov\tr" << rd << ", r" << rs;
                                }
                                else
                                {
                                    if ( h2== 0 )
                                    {
                                        cout << "mov\tr" << rd << ", r" << rs;
                                    }
                                    else
                                    {
                                        cout << "mov\tr" << rd << ", r" << rs;
                                    }
                                }
                                break;
                            case 3:
                                if (h2==0)
                                {
                                    cout << "bx\tr" << rs;
                                }
                                else
                                {
                                    cout << "bx\tr" << rs;
                                }
                                break;
                                
                        }
                        
                    }
                    break;
                    
                case 1: //format 6
                    unsigned int word8;
                    rd=(instr>>8)&0x7;
                    word8= instr & 0xFF;
                    cout << "LDR\tR" << rd << ", [PC, #" << word8<< "]";
                    Regs[rd]= PC + word8;
                    break;
                    
                default:
                    unsigned int temp= (instr>>9) & 0x1;
                    
                    if (((instr>>9) & 0x1) == 0) //format7
                    {
                        unsigned int b, l;
                        rd=instr & 0x7;
                        rb=(instr>>3) & 0x7;
                        ro=(instr>>6 ) & 0x7;
                        b= (instr>>10) & 0x1;
                        l= (instr>>11) & 0x1;
                        
                        if ( l==0)
                        {
                            if(b==0)
                            {
                                cout << "str\tr" << rd << ", [" << rb << ", r" << ro << "]";
                                Regs[ (Regs[rb]+Regs[ro]) ]= Regs[rd];//*****
                                
                            }
                            else
                            {
                                cout << "strb\tr" << rd << ", [" << rb << ", r" << ro << "]";
                                Regs[ (Regs[rb]+Regs[ro]) ]= Regs[rd] &0xFF;
                            }
                        }
                        else if(l==1){
                            if (b==0)
                            {
                                cout << "ldr\tr" << rd << ", [" << rb << ", r" << ro << "]";
                                Regs[rd]= Regs[ (Regs[rb]+Regs[ro]) ];
                            }
                            else
                            {
                                cout << "ldrb\tr" << rd << ", [" << rb << ", r" << ro << "]";
                                Regs[rd]= Regs[ (Regs[rb]+Regs[ro]) ] &0xFF;
                                
                            }
                        }
                    }
                    else//format 8
                    {
                        rd=instr & 0x7;
                        rb=(instr>>3) & 0x7;
                        ro=(instr>>6 ) & 0x7;
                        unsigned int s= (instr>>10) & 0x1;
                        unsigned int h= (instr>>11) & 0x1;
                        
                        if ( s==0)
                        {
                            if(h==0)
                            {
                                cout << "strh\tr" << rd << ", [" << rb << ", r" << ro << "]";
                                
                            }
                            else
                            {
                                cout << "ldrh\tr" << rd << ", [" << rb << ", r" << ro << "]";
                                
                            }
                        }
                        else if(s==1){
                            if (h==0)
                            {
                                cout << "ldsb\tr" << rd << ", [" << rb << ", r" << ro << "]";
                                
                            }
                            else
                            {
                                cout << "ldsh\tr" << rd << ", [" << rb << ", r" << ro << "]";
                            }
                        }
                        
                    }
                    
                    
            }
            break;
        case 3: //format 9
            unsigned int b,l;
            rd=instr &0x7;
            rb=(instr>>3) & 0x7;
            offset5= (instr>>6) & 0x1F;
            l= (instr>>11 ) & 0x1;
            b=(instr>>12 )& 0x1;
            
            if ( l==0)
            {
                if(b==0)
                {
                    cout << "str\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
                    Regs[ (Regs[rb]+offset5) ]= Regs[rd];
                    
                }
                else
                {
                    cout << "strb\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
                    Regs[ (Regs[rb]+offset5) ]= Regs[rd]&0xFF;                }
            }
            else if(l==1){
                if (b==0)
                {
                    cout << "ldr\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
                    Regs[rd]= Regs[ (Regs[rb]+offset5) ];                }
                else
                {
                    cout << "ldrb\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
                    Regs[rd]= ( Regs[ (Regs[rb]+offset5) ] ) &0xFF;                }
            }
            break;
            
        case 4: //format 10/11
            if(((instr>>12)&0x1)==0) //format 10
            {
                rd=instr &0x7;
                rb=(instr>>3) & 0x7;
                offset5=(instr>>6) &0x1F;
                l=(instr>>11) & 0x1;
                if ( l==0)
                {
                    cout << "strh\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
                }
                else{
                    cout << "ldrh\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
                }
                
            }
            else//format 11
            {
                word8= instr & 0xFF;
                rd=(instr>>8)&0x7;
                l=(instr>>11)& 0x1;
                if ( l==0)
                {
                    cout << "str\tr" << rd << ", [SP, #" << offset5 << "]";
                }
                else{
                    cout << "ldr\tr" << rd << ", [SP, #" << offset5 << "]";
                }
            }
            break;
        case 5: // format 12/13/14
            if((( instr>>12)&0x1)==0)//format 12
            {
                word8=instr &0xFF;
                rd=(instr>>8)&0x7;
                sp=(instr>>11)&0x1;
                if(sp==0)
                {
                    //add rd,pc , imm
                }
                else
                {
                    //add rd, sp, imm
                }
                
            }
            else
            {
                unsigned int temp2;
                temp2= (instr>>10)& 0x1;
                if(temp2==0)// format 13
                {   unsigned int s;
                    sword7=instr & 0x7F;
                    s=(instr>>7)& 0x1;
                    
                    if(s==0)
                    {
                        // add sp, imm
                    }else
                    {
                        //add sp, -imm
                    }
                }
                else //format 14
                {
                    unsigned int rlist,r, l;
                    rlist= instr & 0xFF;
                    r=(instr>>8)&0x1;
                    l=(instr>>11)&0x1;
                    
                    if(l==0)
                    {
                        if(r==0)
                        {
                            //push rlist
                        }
                        else
                        {
                            //push rlist, lr
                        }
                    }
                    else
                    {
                        if(r==0)
                        {
                            //pop rlist
                        }
                        else
                        {
                            //pop rlist, pc
                        }
                    }
                    
                }
                
            }
            break;
        case 6: //format 15//16/17
            if (((instr>> 12)& 0x1)==0)// format 15
            {
                unsigned int rlist, l;
                rlist=instr&0xFF;
                rb=(instr>>8)& 0x7;
                l=(instr>>11)&0x1;
                if(l==0)
                {
                    //stmia rb!
                }
                else
                {//ldmia
                    
                }
                
            }
            else //format 16/17
            {
                unsigned int cond,value8;
                cond=(instr>>8)&0xF;
                value8=instr & 0xFF;
                if(cond==0xF)//format 17
                {
                    cout << "SWI " << value8;;
                    //************ bonus
                }
                else
                {
                    if(cond==0b1110)
                    {
                        //undef
                    }
                    else //format 16
                    {
                        switch(cond){
                            case 0:
                                cout<< "beq label";
                                break;
                            case 1:
                                //bne
                                break;
                            case 2:
                                //bcs
                                break;
                            case 3:
                                //bcc
                                break;
                            case 4:
                                // bmi
                                break;
                            case 5:
                                //bpl
                                break;
                            case 6:
                                //bvs
                                break;
                            case 7:
                                //bvc
                                break;
                            case 8:
                                //bhi
                                break;
                            case 9:
                                //bls
                                break;
                            case 10:
                                //bge
                                break;
                            case 11 :
                                //blt
                                break;
                            case 12:
                                //bgt
                                break;
                            case 13:
                                //ble
                                break;
                                
                        }
                    }
                }
            }
            break;
        case 7: //format 18/19
            unsigned int offset11,h,temp;
            temp=(instr>>12)&0x1;
            offset11=instr&0x7FF;
            if(temp==0)//format 18
            {
                cout << "B #" << offset11;
                PC = PC + (offset11 << 1);
                
            }
            else//format 19
            {
                h=(instr>>11)&0x1;
                if(h==0)
                {
                    cout << "BL #" << offset11;
                    LR = (offset11<<12)+PC;
                }
                else
                {
                    temp = PC+1;
                    PC = LR + (offset11 << 1);
                    LR = temp | 1;
                }
            }
            break;
        default:
            printf("UNKNOWN INSTRUCTION!\n");
    }
    
}
