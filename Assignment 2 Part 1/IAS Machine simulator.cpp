#include <stdio.h>
#include <iostream>
using namespace std;

long long int memory[110];  //memory from 0-100 for instructions, 101-109 for variables
int ac = 0;                 //accumulator
int mq = 0;
int ibr = 0;
int pc = 0;
int ir = 0;
int mar = 0;
long long int mbr = 0;
int endflag = 0;            //useful for implimenting end statements


void printmachinestate(){            //prints all the elements of the IAS machine
    cout <<endl;
    cout <<"Accumulator = "<<ac<<endl;
    cout <<"mq = "<<mq<<endl;
    cout <<"ibr = "<<ibr<<endl;
    cout <<"pc = "<<pc<<endl;
    cout <<"ir = "<<ir<<endl;
    cout <<"mar = "<<mar<<endl;
    cout <<"mbr = "<<mbr<<endl;
    cout <<"Value of c = "<<memory[103]<<endl;
}

void fetch() {                       //fetches the instructions from memory and splits them up into their respective components
    if (ibr == 0) {
        mbr = memory[pc];
        pc++;
        ibr = mbr % 1048576;
        mbr = mbr / 1048576;
        mar = mbr % 4096;
        ir = mbr / 4096;
    } else {
        mar = ibr % 4096;
        ir = ibr / 4096;
        ibr = 0;
    }
}

void decode() {                        //takes the opcodes from ir and uses mar to complete the necessary instructions 
    if (ir == 0b00100001) {//Saving to memory
        cout << "Saving accumulator value "<<ac<<" to " << mar << endl;
        memory[mar] = ac;
    } 
    else if (ir == 0b00000001) {//Loading from memory
        ac = memory[mar];
        cout << "Loading value "<<memory[mar]<<" from memory location " << mar << " into accumulator" << endl;
    } 
    else if (ir == 0b00001101) {//Unconditional jump
        pc = mar-1;
        cout << "Jumping to line " << mar << endl;
    } 
    else if (ir == 0b00001111) {//Conditional jump
        if (ac >= 0){
            pc = mar-1;
            cout << "Conditionally jumping to line " << mar << endl;
        }
        else cout<< "Condition not met" << endl;
    } 
    else if (ir == 0b00000101) {//Adding value from memory to accumulator
        ac += memory[mar];
        cout << "Adding value "<<memory[mar]<<" from " << mar << " to accumulator" << endl;
    } 
    else if (ir == 0b00000110) {//SUbtracting value from memory from accumulator
        ac -= memory[mar];
        cout << "Subtracting value "<<memory[mar]<<" from " << mar << " from accumulator" << endl;
    } 
    else if (ir == 0b00001011) {//Multiplying value from memory from accumulator
        ac *= memory[mar];
        cout << "Multiplying value "<<memory[mar]<<" from " << mar << " with accumulator" << endl;
    } 
    else if (ir == 0b00001100) {//Dividing value from memory from accumulato and storing the quotient in the accumulator and remainder in mq
        if (memory[mar] != 0) {
            mq = ac % memory[mar];
            ac /= memory[mar];
            cout << "Dividing accumulator by value "<<memory[mar]<<" from " << mar << ", remainder stored in MQ" << endl;
        } else {
            cout << "Error: Division by zero attempted." << endl; //some error handling
            endflag = 1;
        }
    } 
    else if (ir == 0b11111111) {//End statement
        endflag = 1;
        cout << "Ending the program" << endl;
    }
}

int main(void) {
    memory[101] = 15;//declaring a and b
    memory[102] = 5;

    memory[0] = 0b0000000100000110010100000110000001100110;//instructions
    memory[1] = 0b0000111100000000010000000000000000000000;
    memory[2] = 0b0000000100000110010100000101000001100110;
    memory[3] = 0b0010000100000110011111111111000000000000;

    while (pc < 10 && !endflag) { //loops the IAS machine upto 10 times(can be changed)
        fetch();
        decode();
    }
    printmachinestate();
    return 0;
}
