#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <bitset>
#include <string>
using namespace std;

void simulate(unsigned short);


unsigned char Mem[1024];
unsigned int Regs[16];

#define    PC    Regs[15]
#define    LR    Regs[14]
#define    SP    Regs[13]

unsigned int
op, offset5, rd, rs, offset3,offset8,
rn, word8, ro, rb, L,
B, S, sword7, R, Rlist,
cond, soffset8, value8, offset11, H, offset, cpsr, sign;
int temp;

void format1_2(unsigned short instr)
{
    op = (instr >> 11) & 0x3;
    rd = instr & 0x7;
    rs = (instr >>  3) & 0x7;
    offset5 = (instr >> 6) & 0x1F;
    
    if((offset5>>4)==1)
    {
        temp=0x7ffffff;
        temp=temp<<5;
        offset5=offset5+temp;
    }
    
    rn=offset3 = offset5 &0x7;
    switch(op)
    {
        case 0: //format 1
            printf("LSL\tr%d, r%d, #%d", rd, rs, offset5);
            Regs[rd] = Regs[rs] << offset5;
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
            
        case 1:
            printf("LSR\tr%d, r%d, #%d", rd, rs, offset5);
            Regs[rd] = Regs[rs] >> offset5;
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
            
        case 2:
            printf("ASR\tr%d, r%d, #%d", rd, rs, offset5);
            Regs[rd] = Regs[rs] >> offset5;
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
            
        case 3://format 2
            if((offset5 & 0x08) == 0)
            {
                printf("ADD\tr%d, r%d, ", rd, rs);
                if((offset5 & 0x10) == 0)
                {
                    printf("r%d", rn);
                    Regs[rd] = Regs[rs] + Regs[rn];
                    cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                }
                else
                {
                    printf("#%d", offset3);
                    Regs[rd] = Regs[rs] + offset3;
                    cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                }
            }
            else
            {
                printf("SUB\tr%d, r%d, ", rd, rs);
                if((offset5 & 0x10) == 0)
                {
                    printf("r%d", rn);
                    Regs[rd] = Regs[rs] - Regs[rn];
                    cout<<"\t\t---> r"<<rd<< " = "<<Regs[rd]<<endl;
                }
                else
                {
                    printf("#%d", offset3);
                    Regs[rd] = Regs[rs] - offset3;
                    cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                }
            }
            break;
    }
    
}
//----------------------------------------------------------
void format3(unsigned short instr)
{
    op = (instr >> 11) & 0x3;
    rd = (instr>>8) & 0x7;
    offset8 = instr & 0xFF;
    switch(op) //format 3
    {
        case 0:
            cout << "MOV\tr" << rd << ", #" << offset8;
            Regs[rd] = offset8;
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
        case 1:
            cout << "CMP\tr" << rd << ", #" << offset8;
            cpsr=Regs[rd]-offset8;
            cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
        case 2:
            cout << "ADD\tr"<< rd << ", #" << offset8;
            Regs[rd] += offset8;
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
        case 3:
            cout << "SUB\tr"<< rd << ", #" << offset8;
            Regs[rd] -= offset8;
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
    }
}
//----------------------------------------------------------
void format4_8(unsigned short instr)
{
    switch((instr >> 11) & 0x3)
    {
        case 0: //format 4
            if (((instr>>10) & 0x1)==0)
            {
                op = (instr >> 6) & 0xF;
                rd = instr & 0x7;
                rs = (instr >> 3) &0x7;
                switch(op)
                {
                    case 0:
                        cout << "AND\tr" << rd << ", r" << rs;
                        Regs[rd] = Regs[rd] & Regs[rs];
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 1:
                        cout << "EOR\tr" << rd << ", r"<< rs;
                        Regs[rd]= Regs[rd]^Regs[rs];
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 2:
                        cout << "LSL\tr" << rd << ", r"<< rs;
                        Regs[rd]= Regs[rd] << Regs[rs];
                        cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 3:
                        cout << "LSR\tr" << rd << ", r"<< rs;
                        Regs[rd]= Regs[rd] >> Regs[rs];
                        cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 4:
                        cout << "ASR\tr" << rd << ", r"<< rs;
                        Regs[rd]= Regs[rd] >>Regs[rs];
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 5:
                        cout << "ADC\tr" << rd << ", r"<< rs;
                        Regs[rd]= Regs[rd] + Regs[rs] ;
                        int c;
                        c=~(Regs[rd] | 0);
                        c=c>>15 & 1;
                        Regs[rd]=Regs[rd]+c;
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 6:
                        cout << "SBC\tr" << rd << ", r"<< rs;
                        Regs[rd]= Regs[rd] - Regs[rs] ;
                        c=~(Regs[rd] | 0);
                        c=c>>15 & 1;
                        Regs[rd]=Regs[rd]-c;
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 7:
                        cout << "ROR\tr" << rd << ", r"<< rs;
                        Regs[rd]=(Regs[rd]>>Regs[rs]) | (Regs[rd]<<(16-Regs[rs]));
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                        
                    case 8:
                        cout << "TST\tr" << rd << ", r" << rs<<endl;
                        cpsr=Regs[rd]&Regs[rs];
                        break;
                        
                    case 9:
                        cout << "NEG\tr" << rd << ", r"<< rs;
                        Regs[rd] = - Regs[rs];
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 10:
                        cout << "CMP\tr" << rd << ", r"<< rs<<endl;
                        cpsr=Regs[rd]-Regs[rs];
                        
                        break;
                        
                    case 11:
                        cout << "CMN\tr" << rd << ", r" << rs<<endl;
                        cpsr=Regs[rd]+Regs[rs];
                        break;
                        
                    case 12:
                        cout << "ORR\tr" << rd << ", r"<< rs;
                        Regs[rd] = Regs[rd]|Regs[rs];
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 13:
                        cout << "MUL\tr" << rd << ", r" << rs;
                        Regs[rd] = Regs[rd] * Regs[rs];
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 14:
                        cout << "BIC\tr" << rd << ", r" << rs;
                        Regs[rd]= Regs[rd] & ~(Regs[rs]);
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                        
                    case 15:
                        cout << "MVN\tr" << rd << ", r" << rs;
                        Regs[rd] = ~Regs[rs];
                        cout<<"\t\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                        break;
                }
            }
            else {
                unsigned int h1,h2;
                
                op = (instr >> 8) & 0x3;
                rd=instr & 0x7;
                rs = (instr>>3) & 0x7;
                h1= (instr>>6) & 0x1;
                h2= (instr >> 7) & 0x1;
                
                switch(op) //format 5 ( no implementation )
                {
                    case 0:
                        if(h1==0)
                        {
                            cout << "ADD\tr" << rd << ", r" << rs ;
                            
                        }
                        else {
                            if (h2==0)
                            {
                                cout << "ADD\tr" << rd << ", r" << rs;
                            }
                            else
                            {
                                cout << "ADD\tr" << rd << ", r" << rs;
                            }
                            
                        }
                        break;
                    case 1:
                        if( h1== 0)
                        {
                            cout << "CMP\tr" << rd << ", r" << rs;
                        }
                        else
                        {
                            if ( h2== 0 )
                            {
                                cout << "CMP\tr" << rd << ", r" << rs;
                            }
                            else
                            {
                                cout << "CMP\tr" << rd << ", r" << rs;
                            }
                        }
                        break;
                    case 2:
                        if( h1== 0)
                        {
                            cout << "MOV\tr" << rd << ", r" << rs;
                        }
                        else
                        {
                            if ( h2== 0 )
                            {
                                cout << "MOV\tr" << rd << ", r" << rs;
                            }
                            else
                            {
                                cout << "MOV\tr" << rd << ", r" << rs;
                            }
                        }
                        break;
                    case 3:
                        if (h2==0)
                        {
                            cout << "BX\tr" << rs;
                        }
                        else
                        {
                            cout << "BX\tr" << rs;
                        }
                        break;
                        
                        
                }
                cout<<"\t\t\tFORMAT 5"<<endl;
                
            }
            break;
            
        case 1: // format 6
            rd=(instr>>8)&0x7;
            word8= instr & 0xFF;
            cout << "LDR\tr" << rd << ", [PC, #" << word8<< "]";
            Regs[rd]= PC + word8;
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
            break;
            
        default:
            if (((instr>>9) & 0x1) == 0) //format 7
            {
                rd=instr & 0x7;
                rb=(instr>>3) & 0x7;
                ro=(instr>>6 ) & 0x7;
                B= (instr>>10) & 0x1;
                L= (instr>>11) & 0x1;
                
                if ( L==0)
                {
                    if(B==0)
                    {
                        cout << "STR\tr" << rd << ", [r" << rb << ", r" << ro << "]"<<endl;
                        int x = Regs[rb]+Regs[ro];
                        if(x<1024)
                            Mem[x]= Regs[rd];
                        
                    }
                    else
                    {
                        cout << "STRB\tr" << rd << ", [r" << rb << ", r" << ro << "]"<<endl;
                        int x = Regs[rb]+Regs[ro];
                        if(x<1024)
                            Mem[x]= Regs[rd]&0xFF;
                    }
                }
                else if(L==1){
                    if (B==0)
                    {
                        cout << "LDR\tr" << rd << ", [r" << rb << ", r" << ro << "]";
                        int x = Regs[rb]+Regs[ro];
                        if(x<1024)
                            Regs[rd]=Mem[x];
                        cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                    }
                    else
                    {
                        cout << "LDRB\tr" << rd << ", [r" << rb << ", r" << ro << "]";
                        int x = Regs[rb]+Regs[ro];
                        if(x<1024)
                            Regs[rd]=Mem[x]&0xFF;
                        cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
                    }
                }
            }
            else { //format 8
                rd=instr & 0x7;
                rb=(instr>>3) & 0x7;
                ro=(instr>>6 ) & 0x7;
                unsigned int s= (instr>>10) & 0x1;
                unsigned int h= (instr>>11) & 0x1;
                
                if ( s==0)
                {
                    if(h==0)
                    {
                        cout << "STRH\tr" << rd << ", [" << rb << ", r" << ro << "]" ;
                        
                    }
                    else
                    {
                        cout << "LDRH\tr" << rd << ", [" << rb << ", r" << ro << "]";
                        
                    }
                }
                else if(s==1){
                    if (h==0)
                    {
                        cout << "LDSB\tr" << rd << ", [" << rb << ", r" << ro << "]";
                        
                    }
                    else
                    {
                        cout << "LDSH\tr" << rd << ", [" << rb << ", r" << ro << "]";
                    }
                }
                cout<<"\t\tFORMAT 8"<<endl;
                
            }
            break;
    }
}
//----------------------------------------------------------
void format9(unsigned short instr)
{
    rd=instr &0x7;
    rb=(instr>>3) & 0x7;
    offset5= (instr>>6) & 0x1F;
    L= (instr>>11 ) & 0x1;
    B=(instr>>12 )& 0x1;
    
    if ( L==0)
    {
        if(B==0)
        {
            cout << "STR\tr" << rd << ", [r" << rb << ", #" << offset5 << "]"<<endl;
            int x = Regs[rb]+offset5;
            if(x<1024)
                Mem[x]= Regs[rd];
        }
        else
        {
            cout << "STRB\tr" << rd << ", [r" << rb << ", #" << offset5 << "]"<<endl;
            int x = Regs[rb]+offset5;
            if(x<1024)
                Mem[x]= Regs[rd]&0xFF;
            
        }
    }
    else if(L==1)
    {
        if (B==0)
        {
            cout << "LDR\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
            int x = Regs[rb]+offset5;
            if(x<1024)
                Regs[rd]=Mem[x];
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
        }
        else
        {
            cout << "LDRB\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
            
            int x = Regs[rb]+offset5;
            if(x<1024)
                Regs[rd]= Mem[x]&0xFF;
            
            cout<<"\t\t---> r"<<rd<<" = "<<Regs[rd]<<endl;
        }
    }
    
}
//----------------------------------------------------------
void format10_11(unsigned short instr)
{
    if(((instr>>12)&0x1)==0) //format 10 (no implementation)
    {
        rd=instr &0x7;
        rb=(instr>>3) & 0x7;
        offset5=(instr>>6) &0x1F;
        L=(instr>>11) & 0x1;
        if ( L==0)
        {
            cout << "STRH\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
        }
        else{
            cout << "LDRH\tr" << rd << ", [r" << rb << ", #" << offset5 << "]";
        }
        cout<<"\t\tFORMAT 10"<<endl;
        
    }
    else//format 11 ( no implementation)
    {
        word8= instr & 0xFF;
        rd=(instr>>8)&0x7;
        L=(instr>>11)& 0x1;
        if ( L==0)
        {
            cout << "STR\tr" << rd << ", [SP, #" << offset5 << "]";
        }
        else{
            cout << "LDR\tr" << rd << ", [SP, #" << offset5 << "]";
        }
        cout<<"\t\tFORMAT 11"<<endl;
    }
}
//----------------------------------------------------------
void format12_14(unsigned short instr)
{
    if((( instr>>12)&0x1)==0) //format 12 ( no implementation)
    {
        //masking
        word8=instr &0xFF;
        rd=(instr>>8)&0x7;
        SP=(instr>>11)&0x1;
        if(SP==0)
        {
            cout<<"ADD\tr"<<rd<<", "<< PC << ", #" << word8;
        }
        else
        {
            cout<< "ADD\tr" << rd << ", " << SP << ", #" << word8;
            
        }
        cout<<"\t\tFORMAT 12"<<endl;
    }
    else
    {
        if(((instr>>10)& 0x1)==0) //format 13
        {
            sword7=instr & 0x7F;
            S=(instr>>7)& 0x1;
            
            if(S==0)
            {
                cout<< "ADD\tSP , #" << sword7;
                SP=SP+sword7;
                cout<<"\t\t\t---> SP= "<< SP <<endl;
                
            }
            else
            {
                sword7= - sword7;
                cout<< "ADD\tSP , #" << sword7;
                SP=SP+sword7;
                cout<<"\t\t\t---> SP= "<< SP <<endl;
            }
        }
        else
        {
            vector<int> rlst;
            Rlist= instr & 0xFF;
            for(int i=0;i<8;i++)
            {
                if((Rlist &0x1) == 1)
                    rlst.push_back(i);
                Rlist = Rlist>>1;
            }
            
            R=(instr>>8)&0x1;
            L=(instr>>11)&0x1;
            
            if(L==0 && R == 0)
            {
                cout<<"PUSH\t{ ";
                for (int x : rlst)
                {
                    cout <<"r"<< x << " ";
                    SP--;
                }
                cout<<"}\n";
            }
            else if(L==0 && R == 1)
            {
                cout<<"PUSH\t{ ";
                for (int x : rlst)
                {
                    cout <<"r"<< x << " ";
                    SP--;
                }
                cout<<"LR }\n";
                SP--;
                
            }
            else if(L==1 && R == 0 && ((instr>>12)&0xF)==11)
            {
                cout<<"POP\t{ ";
                for (int x : rlst)
                {
                    cout <<"r"<< x << " ";
                    SP++;
                }
                cout<<"}\n";
            }
            else
            {
                cout<<"POP\t{ ";
                for (int x : rlst)
                {
                    cout <<"r"<< x << " ";
                    SP++;
                }
                cout<<"PC }\n";
                SP++;
            }
            
        }
    }
    
}
//----------------------------------------------------------
void format15_17(unsigned short instr)
{
    if (((instr>> 12)& 0x1)==0)// format 15 ( no implementation)
    {
        
        Rlist=instr&0xFF;
        rb=(instr>>8)& 0x7;
        L=(instr>>11)&0x1;
        if(L==0)
        {
            cout<<"stmia\tr"<<rb<<"! "<<"{"<<Rlist<<"}";
            
        }
        else
        {
            cout<<"ldmia\tr"<<rb<<"! "<<"{"<<Rlist<<"}";
            
        }
        
        cout<<"\t\tFORMAT 15\n";
    }
    else
    {
        cond=(instr>>8)&0xF;
        value8=instr & 0xFF;
        soffset8=instr & 0xFF;
        if(cond==0xF)//format 17
        {
            // reference: https://www.lri.fr/~de/ARM-Tutorial.pdf
            cout << "SWI\t" << value8;
            switch(value8)
            {
                case 0x6c: // Read int
                    int integer;
                    cin>>integer;
                    Regs[0]=integer;
                    break;
                    
                case 0x6a:
                    if(Regs[2]==1) // Read Char
                    {
                        char my_character;
                        cin>>my_character;
                        Regs[0]=my_character; // char stored in R0
                    }
                    else // Read String
                    {
                        string my_string;
                        string null_term = "\n";
                        cin>>my_string;// read string
                        my_string=my_string.substr(0,Regs[2]);//R2 represents max number of bytes to store in memory
                        my_string=my_string.append(null_term);//null byte terminator stored at the end
                        Regs[0]=my_string.length();//number of bytes stored in memory is returned to R0
                    }
                    break;
                    
                case 0x02: // print str
                    cout<<"\t\t\tString = "<<Regs[0]<<endl;//R0 is the address of a Null terminated ASCII string
                    break;
                    
                case 0x00: // Display Character
                    cout<<"\t\t\tChar = "<<Regs[0]<<endl;//R0 is the character
                    break;
                    
                case 0x11: // Terminate
                    abort();
                    break;
                    
                default:
                    cout<<"\t\t\tUNKNOWN SWI CODE\n";
            }
        }
        else
        {
            if(cond==0b1110)
            {
                cout<<"undefined instruction word"<<endl;//undef
            }
            else //format 16
            {
                switch(cond){
                        
                    case 0:
                        cout<< "BEQ\t"<< soffset8<<endl;
                        PC= PC+soffset8-2;
                        break;
                    case 1:
                        cout<< "BNE\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 2:
                        cout<< "BCS\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 3:
                        cout<< "BCC\t"<< soffset8<<endl;
                        PC=PC+ soffset8 -2;
                        break;
                    case 4:
                        cout<< "BMI\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 5:
                        cout<< "BPL\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 6:
                        cout<< "BVS\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 7:
                        cout<< "BVC\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 8:
                        cout<< "BHI\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 9:
                        cout<< "BLS\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 10:
                        cout<< "BGE\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 11 :
                        cout<< "BLT\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 12:
                        cout<< "BGT\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                    case 13:
                        cout<< "BLE\t"<< soffset8<<endl;
                        PC=PC+ soffset8-2;
                        break;
                }
            }
        }
    }
}
//----------------------------------------------------------
void format18_19(unsigned short instr)
{
    offset11=instr&0b11111111110;
    unsigned int temp = (instr>>12)&0x1;
    if(temp==0)
    {
        cout << "B\t" <<offset11;
        PC = PC + (offset11 << 1);
        cout<<"\t\t\t---> PC= "<< PC <<endl;
    }
    else//format 19
    {
        H=(instr>>11)&0x1;
        if(H==0)
        {
            cout << "BL\t"<<offset11;
            LR = (offset11<<12)+PC;
            cout<<"\t\t\t---> LR= "<< LR <<endl;
        }
        else
        {
            cout << endl;
            temp = PC+1;
            PC = LR + (offset11 << 1);
            LR = temp | 1;
        }
    }
}
//----------------------------------------------------------
