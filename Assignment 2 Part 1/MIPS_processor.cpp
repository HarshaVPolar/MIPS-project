#include <bits/stdc++.h>
using namespace std;

int PC=0;
char[8][4000] memory='';
char[32] instr='';
char[6] opcode='';
char[5] rs=''; 
char[5] rt='';
char[5] rd='';
char[6] function='';
char[5] shftamt='';
char[16] imm='';
char[26] jmp='';
char[32][32] regs = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'}; 
int rsvalue=0;
int rtvalue=0;
int immvalue=0;

//0 = const value 0, 1 = assembler temporary, 2-3 = procedure return values, 4-7 = procedure arguements, 8-15 = tempvariables, 16-23 = saved variables, 24-25 = temp variables, 26-27 = OS temps, 28 = global pointer, 29 = stack pointer, 30 = frame pointer, 31 = procedure return address 

int regdst=0;
int branch = 0;
int memread = 0;
int memtoreg = 0;
char[2] aluop = "";
int memwrite = 0;
int regwrite = 0;
int alusrc = 0;




string decimalToTwosComplement(int num, int size) {             //This is necessary to store values in registers or memory 
    if (num >= 0) {
        string binary = bitset<32>(num).to_string();
        return binary.substr(32 - size);
    } else {
        string binary = bitset<32>(num & ((1 << size) - 1)).to_string();
        return binary.substr(32 - size);
    }
}

int twosComplementToDecimal(const string& binary) {             //This is necessary to do any alu operations
    int length = binary.length();
    if (binary[0] == '0') {
        return stoi(binary, nullptr, 2);
    }
    return stoi(binary, nullptr, 2) - (1 << length);
}

void fetch(){
    cout<<"Fetching instruction"<<endl;
    instr = "";
    for(int i=PC;i<PC+4;i++){
        instr = instr + memory[i];
    }
    cout << "Instruction :"<<instr<<endl;

    opcode = "";
    rs = "";
    rt = "";
    rd = "";
    function = "";
    shftamt = "";
    imm = "";
    jmp = "";
}

void decode(){
    cout<<"Decodeing instructions"<<endl;
    opcode = instr[0:6];
    rs = instr[6:11];
    rt= instr[11:16];
    rd= instr[16:21];
    shftamt= instr[21:26];
    function= instr[26:32];
    imm= instr[16:32];
    jmp= instr[6:32];

    if (opcode == "101011") {
        cout << "STOR:" << endl;
        rsvalue = twosComplementToDecimal(Reg[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(Reg[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp, 32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    } 
    else if (opcode == "100011") {
        cout << "LOAD:" << endl;
        rsvalue = twosComplementToDecimal(Reg[stoi(rs, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp, 32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    } 
    else if (opcode == "000000") {
        if (funct == "100000") {
            cout << "ADD:" << endl;
            rsvalue = twosComplementToDecimal(Reg[stoi(rs, nullptr, 2)]);
            rtvalue = twosComplementToDecimal(Reg[stoi(rt, nullptr, 2)]);
            cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << endl;
        } 
        else if (funct == "100010") {
            cout << "SUB:" << endl;
            rsvalue = twosComplementToDecimal(Reg[stoi(rs, nullptr, 2)]);
            rtvalue = twosComplementToDecimal(Reg[stoi(rt, nullptr, 2)]);
            cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << endl;
        }
    } 
    else if (opcode == "011100") {
        cout << "MUL:" << endl;
        funct = "000010";
        rsvalue = twosComplementToDecimal(Reg[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(Reg[stoi(rt, nullptr, 2)]);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << endl;
    } 
    else if (opcode == "000100") {
        cout << "BEQ:" << endl;
        rsvalue = twosComplementToDecimal(Reg[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(Reg[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp, 32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    } 
    else if (opcode == "000101") {
        cout << "BNE:" << endl;
        rsvalue = twosComplementToDecimal(Reg[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(Reg[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp, 32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    } 
    else if (opcode == "000010") {
        cout << "JUMP:" << endl;
        jmp = "0000" + jmp + "00";
        immvalue = twosComplementToDecimal(jmp);
    }
        
}

void ctrl_circuit() {
    if (opcode == "101011") {
        regdst = 2;
        branch = 0;
        memread = 0;
        memtoreg = 1;
        aluop = "00";
        memwrite = 1;
        alusrc = 1;
        regwrite = 0;
    } 
    else if (opcode == "100011") {
        regdst = 0;
        branch = 0;
        memread = 1;
        memtoreg = 1;
        aluop = "00";
        memwrite = 0;
        alusrc = 1;
        regwrite = 1;
    } 
    else if (opcode == "000000" || opcode == "011100") {
        regdst = 1;
        branch = 0;
        memread = 0;
        memtoreg = 0;
        aluop = "10";
        memwrite = 0;
        alusrc = 0;
        regwrite = 1;
    } 
    else if (opcode == "000100") {
        regdst = 2;
        branch = 1;
        memread = 0;
        memtoreg = 1;
        aluop = "01";
        memwrite = 0;
        alusrc = 0;
        regwrite = 0;
        if (rsval == rtval) {
            zero = 1;
        }
    } 
    else if (opcode == "000101") {
        regdst = 2;
        branch = 1;
        memread = 0;
        memtoreg = 1;
        aluop = "01";
        memwrite = 0;
        alusrc = 0;
        regwrite = 0;
        if (rsval != rtval) {
            zero = 1;
        }
    } 
    else if (opcode == "000010") {
        regdst = 2;
        branch = 1;
        memread = 0;
        memtoreg = 1;
        aluop = "00";
        memwrite = 0;
        alusrc = 1;
        regwrite = 0;
        cout << string(75, '-') << endl;
        return;
    }

    if (regdst == 1) {
        rdval = stoi(rd, nullptr, 2);
    } 
    else if (regdst == 0) {
        rdval = stoi(rt, nullptr, 2);
    }

}


