#include <bits/stdc++.h>
using namespace std;

int PC = 0;
string memory[4000];
string instr;
string opcode, rs, rt, rd, func, shftamt, imm, jmp, data;
string regs[32] = {"0"};
int rsvalue = 0, rtvalue = 0, immvalue = 0, rdvalue = 0;
int regdst = 0, branch = 0, memread = 0, memtoreg = 0, memwrite = 0, regwrite = 0, alusrc = 0, zero = 0, alures = 0;
string aluop = "";
string aluin = "";

string decimalToTwosComplement(int num, int size) {
    if (num >= 0) {
        return bitset<32>(num).to_string().substr(32 - size);
    } else {
        return bitset<32>(num & ((1 << size) - 1)).to_string().substr(32 - size);
    }
}

int twosComplementToDecimal(const string& binary) {
    if (binary[0] == '0') {
        return stoi(binary, nullptr, 2);
    }
    return stoi(binary, nullptr, 2) - (1 << binary.length());
}

void execute_jump() {
    int jumpAddress = (PC & 0xF0000000) | (stoi(jmp, nullptr, 2) << 2);
    cout << "Jumping to address: " << jumpAddress << endl;
    PC = jumpAddress;
}


void fetch() {
    cout << "Fetching instruction" << endl;
    instr = "";
    for (int i = PC; i < PC + 4; i++) {
        instr += memory[i];
    }
    cout << "Instruction: " << instr << endl;
}

void decode() {
    cout << "Decoding instruction" << endl;
    opcode = instr.substr(0, 6);
    rs = instr.substr(6, 5);
    rt = instr.substr(11, 5);
    rd = instr.substr(16, 5);
    shftamt = instr.substr(21, 5);
    func = instr.substr(26, 6);
    imm = instr.substr(16, 16);
    jmp = instr.substr(6, 26);

    rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
    rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
    immvalue = twosComplementToDecimal(imm);

    if (opcode == "101011") {
        cout << "STOR:" << endl;
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp,32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << "Value of RT: " << rtvalue << "Value of Imm: " << immvalue << endl;
    }
    else if (opcode == "100011") {
        cout << "LOAD:" << endl;
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp,32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << "Value of RT: " << rtvalue << "Value of Imm: " << immvalue << endl;
    }
    else if (opcode == "000000") {
        if (func == "100000") {
            cout << "ADD:" << endl;
            rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
            rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
            cout << "Value of RS: " << rsvalue << "Value of RT: " << rtvalue << endl;
        }
        else if (func == "100010") {
            cout << "SUB:" << endl;
            rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
            rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
            cout << "Value of RS: " << rsvalue << "Value of RT: " << rtvalue << endl;
        }
    }
    else if (opcode == "011100") {
        cout << "MUL:" << endl;
        func = "000010";
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
        cout << "Value of RS: " << rsvalue << "Value of RT: " << rtvalue << endl;
    }
    else if (opcode == "000100") {
        cout << "BEQ:" << endl;
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp,32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << "Value of RT: " << rtvalue << "Value of Imm: " << immvalue << endl;
    }
    else if (opcode == "000101") {
        cout << "BNE:" << endl;
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp, 32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << "Value of RT: " << rtvalue << "Value of Imm: " << immvalue << endl;
    } 
    else if (opcode == "000010") {
        cout << "JUMP:" << endl;
        execute_jump();
        //jmp = "0000" + jmp + "00";
        //immvalue = twosComplementToDecimal(jmp);
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
        if ((rsvalue == rtvalue)) {
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
        if (rsvalue != rtvalue) {
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
        rdvalue = stoi(rd, nullptr, 2);
    }
    else if (regdst == 0) {
        rdvalue = stoi(rt, nullptr, 2);
    }
}

void alu_ctrl() {
    if (aluop == "00") {
        aluin = "010";
    }
    else if (aluop == "10" && func == "100000") {
        aluin = "010";
    }
    else if (aluop == "10" && func == "100010") {
        aluin = "011";
    }
    else if (aluop == "10" && func == "000010") {
        aluin = "111";
    }
    else if (aluop == "01") {
        aluin = "100";
    }
}

void alu() {
    cout << "ALU ---->" << endl;
    if (aluin == "010") {
        if (alusrc == 1) {
            alures = rsvalue + immvalue;
            cout << "After Add: " << alures << endl;
        }
        else if (alusrc == 0) {
            alures = rsvalue + rtvalue;
            cout << "After Add: " << alures << endl;
        }
    }
    else if (aluin == "011") {
        if (alusrc == 0) {
            alures = rsvalue - rtvalue;
            cout << "After Sub: " << alures << endl;
        }
    }
    else if (aluin == "111") {
        if (alusrc == 0) {
            alures = rsvalue*rtvalue;
            cout << "After Mul: " << alures << endl;
        }
    }
    else if (aluin == "100") {
        immvalue = immvalue*4;
        cout << "Immediate: " << immvalue << endl;
    }
}

void memory_access() {
    if (memwrite == 0 && memread == 1) {
        cout << "Memory Access ----->" << endl;
        string var;
        int i=0;
        while(i<4) {
            var += memory[alures+i];
        }
        data = var;
    }
    else if(memwrite==1 && memread==0){
        cout <<"Writing to memory"<<endl;
        string var = decimalToTwosComplement(rtvalue,32);
        memory[alures]=var.substr(0,8);
        memory[alures+1]=var.substr(8,8);
        memory[alures+2]=var.substr(16,8);
        memory[alures+3]=var.substr(24,8);
    }
}

void writeback() {
    if (regwrite == 1) {
        cout << "Writeback ----->" << endl;
        cout << "rd: " << rdvalue << endl;
        cout << memtoreg << endl;
        if (memtoreg == 1) {
            cout << "Data: " << data << endl;
            regs[rdvalue] = data;
        } 
        else {
            regs[rdvalue] = decimalToTwosComplement(alures,32);
            cout << "Write: " << regs[rdvalue] << endl;
        }
        cout << "Value after write back: " << regs[rdvalue] << endl;
    }
}

