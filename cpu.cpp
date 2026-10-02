
/*
 * Copyright 2026 Breeze
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <iostream>
#include <cstdint>
//20kb of ram
#define MEM_SIZE = 20000;

uint8_t RAM[20000];

struct S_cpu
{
    uint8_t Cache_L3[10000];
    /*
    0 - 1000 instructions storing
    1001 - 9000 data storing
    9001 - 10000 register dumping
    balabkabka
    */
    uint8_t Cache_L2[1000]; // instructions space
    uint8_t Cache_L1[10];
    /*
    can store total of 255 numbers in a slot
    had 10 slots soo it can store 2550 numbers
    used for calculations when registers are full
    blablabakavka
    */

    uint8_t Regs[6];
    int PC = 0;
    int Flag_status = 0;
    /*
    0 means nothing 
    1 means equal to 0
    2 means if equal
    3 means if not equal
    4 means greater 
    5 means less
    */
    bool do_flags = 0;

    struct
    {
        //essential instructions 
        
        uint8_t HLT = 0x01;
        uint8_t ADD = 0x02;
        uint8_t SUB = 0x03;
        uint8_t MUL = 0x04;
        uint8_t DIV = 0x05;
        uint8_t RST = 0x06;
        uint8_t LOAD = 0x07;
        uint8_t STORE = 0x08;
        uint8_t JMP = 0x09;
        
        // memory banana stuff
        
        uint8_t MRTRG = 0x10;
        uint8_t MRGTR = 0x11;
        
        // conditional stuff 
        uint8_t JZ = 0x12; // (probably deprecated)
        uint8_t JE = 0x13;
        uint8_t JNE = 0x14;
        uint8_t JG = 0x15;
        uint8_t JL = 0x16;
        uint8_t CMP = 0x17;
        
        // basic inst
        
        uint8_t GET_ADDRS = 0X18;//(DEPRECATED)
        uint8_t MOV = 0x19;
        
        // Regs data inst saving 
        
        uint8_t STORE_R = 0x20;
        uint8_t STORE_I = 0x21;
        uint8_t STORE_D = 0x22;
        uint8_t LOAD_R = 0x23; // not done
        uint8_t LOAD_I = 0x24; // not done
        uint8_t LOAD_D = 0x25; // not done
        
        // stack stuff
        
        uint8_t INST = 0x26; //didn't even start
        uint8_t DCST = 0x27;
        uint8_t AMP = 0x28;
        uint8_t FAMP = 0x29;
        uint8_t JTP = 0x30;
        uint8_t SP = 0x31;
        
        // Virtual_mem 
        
        uint8_t ALOC_VM = 0x32;
        uint8_t FREE_VM = 0x33;
        uint8_t JUMP_VM = 0x34;
        uint8_t INCR_VM = 0x35;
        uint8_t DECR_VM = 0x36;
        
        
    } ISA;

    struct
    {
        uint8_t Reg1 = 0x90;
        uint8_t Reg2 = 0x91;
        uint8_t Reg3 = 0x92;
        uint8_t Reg4 = 0x93;
        uint8_t Reg5 = 0x94;

    } registers;
    
    
    struct MMU
    {
       uint8_t* ram_ptr = RAM;
       int Ram_PC = 0;
       
       struct Virtual_mem 
       {
           // variables for banan
           int VR_M_PC = 0;
           int VR_M_start = 0;
           int VR_M_end = 0;
           
           short VR_flag = 2;
           /*
           1 means has an allocation
           2 means no allocation
           */
           //counter to count allocations 
           
           static int VM_count;
       
       };
       
       //Virtual_mem stuff 
       
       Virtual_mem VIRM[20];
       
       
       void allocate_VM(
       int target_VM, int al_start, int al_end)
       {
          VIRM[target_VM].VR_M_start = al_start;
          VIRM[target_VM].VR_M_end = al_end; 
       }
       void free_alloc_VM(int target_VM)
       {
           VIRM[target_VM].VR_M_start = 0;
           VIRM[target_VM].VR_M_end = 0;
           VIRM[target_VM].VR_M_PC = 0;
       }
       
        struct {
            
            int L_INST_SPACE = 0;
            int H_INST_SPACE = 1000;
            int L_DATA_SPACE = 1001;
            int H_DATA_SPACE = 9000;
            int L_REDP_SPACE = 9001;
            int H_REDP_SPACE = 10000;
            
        }L3_TBL;
        
        int L3_INST_PC = L3_TBL.L_INST_SPACE;
        int L3_DATA_PC = L3_TBL.L_DATA_SPACE;
        int L3_REDP_PC = L3_TBL.L_REDP_SPACE;
        int R_address = 0;
        int SP[5];
        int SP_address_start[5];
        int SP_address_end[5];
        
        
        
        void run_mmu()
        {
            if(L3_INST_PC == L3_TBL.H_INST_SPACE)
            {L3_INST_PC = L3_TBL.L_INST_SPACE;}
            
            if(L3_DATA_PC == L3_TBL.H_DATA_SPACE)
            {L3_DATA_PC = L3_TBL.L_DATA_SPACE;}
            
            if(L3_REDP_PC == L3_TBL.H_REDP_SPACE)
            {L3_REDP_PC = L3_TBL.L_REDP_SPACE;}
            
            // debug
            
          //  std::cout << std::endl;
           // std::cout << Ram_PC << std::endl;
           for(int i = 0; i < 19; i++)
           {
               if(VIRM[i].VR_flag == 1)
               {
                   if(VIRM[i].VR_M_PC == 
                   VIRM[i].VR_M_end)
                   {
                       VIRM[i].VR_M_PC =
                        VIRM[i].VR_M_start;
                   }
               }
           }
        }
        
    }MMU;
    
     void get_addrs(int read_bytes)
        {
            //MMU.RAM_PC = position;
            MMU.R_address = 0;
            
            for(int i = 0; i < read_bytes; i++)
            {
                MMU.R_address += Cache_L2[PC++];
            }
            
          //std::cout << MMU.R_address << std::endl;
        }

    S_cpu()
    {
        Regs[0] = 0;
        Regs[1] = 0;
        Regs[2] = 0;
        Regs[3] = 0;
        Regs[4] = 0;
        Regs[5] = 0;

        for (int i = 0; i < 10; i++)
        {
            Cache_L1[i] = 0x00;
        }
    }

    void fetch()
{
    
    MMU.Ram_PC = 0;

    while (MMU.Ram_PC < 1000)
    {
     
    Cache_L2[MMU.Ram_PC] = MMU.ram_ptr[MMU.Ram_PC];


        MMU.Ram_PC++;
    }
}


    uint8_t send_to_Regs()
    {
        for (int v = 0; v < 4; v++)
        {
            for (int i = 0; i < 4; i++)
            {
                Regs[v] = Cache_L1[i];
            }
        }
    }

    //memory stuff

    void LFRIRG(int register_addr)
    //load from RAM into register
    {
        Regs[register_addr] = RAM[MMU.R_address];
    }

    void LFRGIR(int address, int register_addr)
    //load from Register into Ram
    {
        RAM[address] = Regs[register_addr];
    }
    
     void calc_flags(int IR1, int IR2)
    {
            if(IR1 > IR2) { Flag_status = 4; }
            else if(IR1 < IR2) { Flag_status = 5; }
            else if(IR1 == IR2) { Flag_status = 2; }
            else { Flag_status = 3; }
    }

    int execute()
    {
        if (Cache_L2[PC] == ISA.HLT)
        {
            return -1;
        }

        else if (Cache_L2[PC] == ISA.ADD)
        {
            // GET za dam values
            PC++;
            int Raddr1 = (int)Cache_L2[PC];
            PC++;
            int Raddr2 = (int)Cache_L2[PC];
            PC++;
            int Raddr3 = (int)Cache_L2[PC];
            PC++;
            do_flags = Cache_L2[PC];

            //finally do math
            Regs[Raddr1] = 
            (int)(Regs[Raddr2] + Regs[Raddr3]);
            
            if(do_flags == true)
            {
             calc_flags(Regs[Raddr2], Regs[Raddr3]);
            }
            do_flags = false;
        }

        else if (Cache_L2[PC] == ISA.SUB)
        {
            // GET za dam values
            PC++;
            int Raddr1 = (int)Cache_L2[PC];
            PC++;
            int Raddr2 = (int)Cache_L2[PC];
            PC++;
            int Raddr3 = (int)Cache_L2[PC];
            PC++;
            do_flags = Cache_L2[PC];

            //finally do math
            Regs[Raddr1] = 
            (int)(Regs[Raddr2] - Regs[Raddr3]);
            
              
            if(do_flags == true)
            {
             calc_flags(Regs[Raddr2], Regs[Raddr3]);
            }
            do_flags = false;
        }

        else if (Cache_L2[PC] == ISA.DIV)
        {
            // GET za dam values
            PC++;
            int Raddr1 = (int)Cache_L2[PC];
            PC++;
            int Raddr2 = (int)Cache_L2[PC];
            PC++;
            int Raddr3 = (int)Cache_L2[PC];
            PC++;
            do_flags = Cache_L2[PC];

            //finally do math
            Regs[Raddr1]
             = (int)(Regs[Raddr2] / Regs[Raddr3]);
             
               
            if(do_flags == true)
            {
             calc_flags(Regs[Raddr2], Regs[Raddr3]);
            }
            do_flags = false;
        }

        else if (Cache_L2[PC] == ISA.MUL)
        {
            // GET za dam values
            PC++;
            int Raddr1 = (int)Cache_L2[PC];
            PC++;
            int Raddr2 = (int)Cache_L2[PC];
            PC++;
            int Raddr3 = (int)Cache_L2[PC];
            PC++;
            do_flags = Cache_L2[PC];

            //finally do math
            Regs[Raddr1] =
             (int)(Regs[Raddr2] * Regs[Raddr3]);
             
               
            if(do_flags == true)
            {
             calc_flags(Regs[Raddr2], Regs[Raddr3]);
            }
            do_flags = false;
        }

        else if (Cache_L2[PC] == ISA.RST)
        {
            for (int i = 0; i < 5; i++)
            {
                Regs[i] = 0;
            }
            for (int I = 0; I < 1000; I++)
            {
              //  Cache_L2[I] = 0;
            }
            for (int i = 0; i < 10; i++)
            {
                Cache_L1[i] = 0;
            }
            PC = 0;
        }

        // memory to register stuff aka mess

        else if (Cache_L2[PC] == ISA.MRGTR)
        {
            PC++;
            int R_addr = (int)Cache_L2[PC];
            PC++;
            int Ram_addr = (int)Cache_L2[PC];
            
            
            LFRGIR(Ram_addr ,R_addr);
        }

        else if (Cache_L2[PC] == ISA.MRTRG)
        {
            PC++;
            
            int R_addr = (int)Cache_L2[PC];
            PC++;
            
            int read_h_bytes = (int)Cache_L2[PC];
            PC++;
            
            
            get_addrs(read_h_bytes);
            
            LFRIRG(R_addr);
            PC--;
        }

        else if (Cache_L2[PC] == ISA.STORE_R)
        {
           for(int i = 0; i < 6; i++)
           {
            Cache_L3[MMU.L3_TBL.L_REDP_SPACE + i] =
                Regs[i];
                MMU.L3_REDP_PC++;
           }
        }
        else if (Cache_L2[PC] == ISA.STORE_I)
        {
           for(int i = 0; i < 1000; i++)
           {
            Cache_L3[MMU.L3_TBL.L_INST_SPACE + i] =
                Cache_L2[i];
                MMU.L3_INST_PC++;
           }
        }
        else if (Cache_L2[PC] == ISA.STORE_D)
        {
           for(int i = 0; i < 6; i++)
           {
            Cache_L3[MMU.L3_TBL.L_DATA_SPACE + i] =
                Regs[i];
                MMU.L3_DATA_PC++;
           }
           /*
           IK THAT IT DOESN'T WORK SOOO ITS
               work in progress :)
           */
        }
        else if (Cache_L2[PC] == ISA.JMP)
        {
            PC++;
            PC = (int)(Cache_L2[PC] - 1);
        }
        else if (Cache_L2[PC] == ISA.JZ)
        {
            PC++;
            int address = Cache_L2[PC];
            
            if(Flag_status == 1)
            {PC = (address - 1);}
        }
        else if (Cache_L2[PC] == ISA.JE)
        {
            PC++;
            int address = Cache_L2[PC];
            
            if(Flag_status == 2)
            {PC = address - 1;}
        }
        else if (Cache_L2[PC] == ISA.JNE)
        {
            PC++;
            int address = Cache_L2[PC];
            
            if(Flag_status == 3)
            {PC = address - 1;}
        }
        else if (Cache_L2[PC] == ISA.JG)
        {
            PC++;
            int address = Cache_L2[PC];
            
            if(Flag_status == 4)
            {PC = address - 1;}
        }
        else if (Cache_L2[PC] == ISA.JL)
        {
            PC++;
            int address = Cache_L2[PC];
            
            if(Flag_status == 5)
            {PC = address - 1;}
        }
        else if (Cache_L2[PC] == ISA.CMP)
        {
            PC++;
            int Rdrs =  Cache_L2[PC];
            PC++;
            int Rdrs2 = Cache_L2[PC];
            
            calc_flags(Regs[Rdrs], Regs[Rdrs2]);
        }
        else if(Cache_L2[PC] == ISA.MOV)
        {
            PC++;
            int reg_addr = Cache_L2[PC];
            PC++;
            int value = Cache_L2[PC];
            Regs[reg_addr] = value;
        }

        PC++;
    }
};



    void inject_to_ram();



int main()
{
    S_cpu cpu;
    RAM[1] = 20;
    RAM[9] = 18;
    RAM[500] = 4;
    RAM[501] = 6;
    // uint8_t testcode[1000] = {0x02, 4, 7, 0x08};
    uint8_t testcode[1000] = {
        0x10, 1, 1, 1,
        0x10, 2, 1, 9, 
        0x02, 3, 2, 1, 0,
        0x10, 4, 2, 250, 250,
        0x03, 5, 3, 4, 1,
        0x11, 5, 200,
        0x15, 25, 
        0x01,
        0x11, 3, 200,
        0x01
    };
    
    uint8_t testcode2[200] = {
            0x06,
            0x10, 3, 2, 250, 250,
            0x10, 4, 2, 251, 250,
            0x02, 5, 3, 4, 0,
            0x11, 5, 200,
            0x01,
        
        
        
        };
        
        // RAM presets needed before running:
RAM[10] = 5;   // starting counter value
RAM[11] = 1;   // decrement amount

uint8_t testcode3[200] = {
    0x06,                // 0:  RST
    0x10, 0, 1, 10,       // 1-4:   R0 = RAM[10]        (counter = 5)
    0x10, 2, 1, 11,       // 5-8:   R2 = RAM[11]        (decrement = 1)
    0x03, 1, 1, 1, 0,     // 9-13:  R1 = R1 - R1         (sum = 0)
    0x03, 3, 3, 3, 0,     // 14-18: R3 = R3 - R3         (zero constant)
    // loop_start = 19
    0x02, 1, 1, 0, 0,     // 19-23: R1 = R1 + R0         (sum += counter)
    0x03, 0, 0, 2, 0,     // 24-28: R0 = R0 - R2         (counter -= 1)
    0x17, 0, 3,           // 29-31: CMP R0, R3
    0x15, 19,             // 32-33: JG loop_start
    0x11, 1, 200,         // 34-36: RAM[200] = R1
    0x01                  // 37: HLT
};

uint8_t testcode4[20] = {
        0x19, 1, 40,
        0x19, 2, 8,
        0x02, 0, 1, 2, 0,
        0x11, 0, 200,
    };
    
    uint8_t sum_loop[50] = {
    0x19, 0, 5,       // MOV R0, 5
    0x19, 1, 0,       // MOV R1, 0
    0x19, 2, 1,       // MOV R2, 1
    0x19, 3, 0,       // MOV R3, 0
    // loop starts at 16 (decimal) = 0x10 (hex)
    0x02, 1, 0, 0,    // ADD R1, R0  (R1 = R1 + R0, no flags)
    0x03, 0, 2, 0,    // SUB R0, R2  (R0 = R0 - 1, no flags)
    0x17, 0, 3,       // CMP R0, R3
    0x15, 16,       // JG 16 (jump to loop start in hex)
    0x11, 1, 200,     // STORE R1 -> RAM[200]
    0x01              // HLT
};

uint8_t sum_loop2[50] = {
    0x19, 0, 5,       // MOV R0, 5
    0x19, 1, 0,       // MOV R1, 0
    0x19, 2, 1,       // MOV R2, 1
    0x19, 3, 0,       // MOV R3, 0
    // loop_start = 16
    0x02, 1, 1, 0, 0, // ADD R1, R1, R0 (R1 = R1 + R0) 
    0x03, 0, 0, 2, 0, // SUB R0, R0, R2 (R0 = R0 - R2) 
    0x17, 0, 3,       // CMP R0, R3
    0x15, 16,         // JG loop_start
    0x11, 1, 200,     // STORE R1 -> RAM[200]
    0x01              // HLT
};

uint8_t sum_loop3[50] = {
    0x19, 0, 5,       // MOV R0, 5
    0x19, 1, 0,       // MOV R1, 0
    0x19, 2, 1,       // MOV R2, 1
    0x19, 3, 0,       // MOV R3, 0
    // loop_start = 16
    0x02, 1, 1, 0, 0, // ADD R1, R1, R0 (R1 = R1 + R0) 
    0x03, 0, 0, 2, 0, // SUB R0, R0, R2 (R0 = R0 - R2) 
    0x17, 0, 3,       // CMP R0, R3
    0x15, 12,         // JG loop_start
    0x11, 1, 200,     // STORE R1 -> RAM[200]
    0x01              // HLT
};
uint8_t ham[50] = {
        0x19, 1, 5,
        0x19, 2, 4,
        0x02, 0, 1, 2, 0,
        0x11, 0, 200,
        0x01
    };
    
    
    
    
    RAM[0] = 0x19;
    RAM[1] = 1;
    RAM[2] = 5;
    
    RAM[3] = 0x19;
    RAM[4] = 2;
    RAM[5] = 4;
    
    RAM[6] = 0x02;
    RAM[7] = 0;
    RAM[8] = 1;
    RAM[9] = 2;
    RAM[10] = 0;
    
    RAM[11] = 0x11;
    RAM[12] = 0;
    RAM[13] = 200;
    
    RAM[14] = 0x01;
    
    
    
    cpu.fetch();    
    
   while (cpu.execute() != -1){
            cpu.MMU.run_mmu();
        }

    std::cout << (int)RAM[200] << std::endl;
}

/*
Need to add

SAVE_REGS   ; Dump all 5 registers to a specific RAM address
LOAD_REGS   ; Restore all 5 registers from RAM

MOV_RAM_REG ; Move value from RAM to register(DONE)
MOV_REG_RAM ; Move value from register to RAM(DONE)

INCR_PTR    ; Increment the memory pointer (for arrays/loops)
DECR_PTR    ; Decrement the memory pointer

make load put vals in Regs(DONE)
make calcs use Regs instead of taking numbers(DONE)

make it soo data and instructs are separated(DONE)
add L3 soo cpu can dump stuff in(DONE)

NEEEDD TO ADD MOV AKA VALUES STUFFED INTO REGS
NEEED TO ADD SP FOR arrays and stuff
*/