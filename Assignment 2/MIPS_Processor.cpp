#include <bits/stdc++.h>
using namespace std;

int PC = 0;
string memory[4000] = {""};
string instr;
string opcode, rs, rt, rd, func, shftamt, imm, jmp, writedatas, datas;
string regs[32] = {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"};
int rsvalue = 0, rtvalue = 0, immvalue = 0, rdvalue = 0;
int regdst = 0, branch = 0, memread = 0, memtoreg = 0, memwrite = 0, regwrite = 0, alusrc = 0, zero = 0, alures = 0;
string aluop = "";
string aluin = "";
string binarycode="";
int a=0,b=0,c=0,outint=0;

void parsebinarycode(){
    int i =1000;
    while(binarycode!=""&&i<4000){
        memory[i] = binarycode.substr(0,8);
        binarycode = binarycode.erase(0,8);
        i++;
    }
}

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
    jumpAddress = jumpAddress - 0b10000000000000000000000 ;
    jumpAddress += 1000;
    cout << "Jumping to address: " << jumpAddress << endl;
    PC = jumpAddress;
}

void cond_jump() {
    if(zero==1){
        PC = PC +4 + immvalue;
        cout<<"Jumping forward "<<immvalue/4<<" instructions"<<endl;
    }
}

void fetch() {
    cout << "Fetching instruction ------>" << endl;
    instr = "";
    for (int i = PC; i < PC + 4; i++) {
        instr += memory[i];
    }
    cout << "Instruction : " << instr << endl<<endl;
}

void decode() {
    opcode = instr.substr(0, 6);
    rs = instr.substr(6, 5);
    rt = instr.substr(11, 5);
    rd = instr.substr(16, 5);
    shftamt = instr.substr(21, 5);
    func = instr.substr(26, 6);
    imm = instr.substr(16, 16);
    jmp = instr.substr(6, 26);
    zero = 0;

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
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    }
    else if (opcode == "100011") {
        cout << "LOAD:" << endl;
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp,32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    }
    else if (opcode == "000000") {
        if (func == "100000") {
            cout << "ADD:" << endl;
            rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
            rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
            cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << endl;
        }
        else if (func == "100010") {
            cout << "SUB:" << endl;
            rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
            rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
            cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << endl;
        }
    }
    else if (opcode == "011100") {
        cout << "MUL:" << endl;
        func = "000010";
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << endl;
    }
    else if (opcode == "000100") {
        cout << "BEQ:" << endl;
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp,32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    }
    else if (opcode == "000101") {
        cout << "BNE:" << endl;
        rsvalue = twosComplementToDecimal(regs[stoi(rs, nullptr, 2)]);
        rtvalue = twosComplementToDecimal(regs[stoi(rt, nullptr, 2)]);
        int temp = twosComplementToDecimal(imm);
        imm = decimalToTwosComplement(temp, 32);
        immvalue = twosComplementToDecimal(imm);
        cout << "Value of RS: " << rsvalue << " Value of RT: " << rtvalue << " Value of Imm: " << immvalue << endl;
    }
    else if (opcode == "000010") {
        cout << "JUMP:" << endl;
        execute_jump();
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
    if (opcode != "000010"&&opcode != "000100"&&opcode != "000101")
    cout << "ALU ---->" << endl <<endl ;

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
        if(zero==1)
        cout << "Immediate: " << immvalue << endl;
    }
    cout<<endl;
}

void memory_access() {
    if (memwrite == 0 && memread == 1) {
        string var;
        int i=0;
        while(i<4) {
            var += memory[alures+i];
            i++;
        }
        datas = var;
    }
    else if(memwrite==1 && memread==0){
        cout <<"Writing value "<<rtvalue<<" to memory location: "<<alures<<endl<<endl;
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
        cout << "Rd : " << rdvalue << endl;
        cout << "Memory to Register " << memtoreg << endl;
        if (memtoreg == 1) {
            cout << "Data: " << datas << endl;
            regs[rdvalue] = datas;
        }
        else {
            regs[rdvalue] = decimalToTwosComplement(alures,32);
            cout << "Write Data: " << twosComplementToDecimal(regs[rdvalue]) << endl;
        }
        cout << "Value after write back: " << twosComplementToDecimal(regs[rdvalue]) <<endl<< endl;
    }
}

void run_MIPS(){
    PC = 1000;
    memory[0] = decimalToTwosComplement(a,32);
    memory[4] = decimalToTwosComplement(b,32);
    memory[8] = decimalToTwosComplement(c,32);
    while(PC<=4000&&memory[PC]!=""){
        cout << "--------------------------------------------------------------------------" << endl;
        cout<<"PC : " << PC <<endl;
        fetch();
        decode();
        ctrl_circuit();
        alu_ctrl();
        alu();
        cond_jump();
        memory_access();
        writeback();
        if(opcode != "000010"&&zero!=1)
        PC +=4;
    }
    string output = memory[16]+memory[16+1]+memory[16+2]+memory[16+3];
    outint = twosComplementToDecimal(output);
}

int main(){
    binarycode = "";
    a=0;    //This lods to memory space "0"
    b=0;    //This lods to memory space "4"
    c=0;    //This lods to memory space "8"
    parsebinarycode();
    run_MIPS();
    cout<<"The output = "<<outint<<endl;
}

