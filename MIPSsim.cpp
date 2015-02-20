/*
"On my honor, I have neither given nor received unauthorized aid on this assignment"

	Author : Nishit Sadhwani
	UFID   : 5393-5598  
*/

#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#define INM_LOCK 5
#define INB_LOCK 0
#define ADB_LOCK 1
#define LIB_LOCK 2
#define AIB_LOCK 3
#define REB_LOCK 4

using namespace std;

static int lock_array [6] = {0,0,0,0,0,1};
static int number_of_tokens=0;

// Variable Declarations
vector<string> INM;				// INM : Instruction memory
vector<string> RGF;				// RGF : Register File
vector<string> DAM;				// DAM : Data Memory
vector<string> INB;					// INB : Instruction Buffer
vector<string> LIB;					// LIB : Load Instruction Buffer
vector<string> ADB;					// ADB : Address Buffer
vector<string> REB;					// REB : Result Buffer
vector<string> AIB;					// AIB : Arithmetic Instruction Buffer
string token[8][4];
int token_number = 0;
static int step_count = 0;
string dummy_opcode;
string dummy_destination_register;
string dummy_first_source_operand;
string dummy_second_source_operand;



void take_input();
void write_to_file();
// Checks if both the register sources are available , 
// Returns 1 - If both the operands are available
// Returns 0 - Otherwise
int read_transition(string &source1 , string &source2 , string opcode);

// Will remove the first Instruction and puts it in INB
void decode_transition();

// Will remove token from INB and place it in AIB
void issue1_transition();

// Will remove token from INB and place it in LIB
void issue2_transition();

// Adds or Substracts and puts the value in Result buffer
void asu_transition(string opcode, string destination_register ,string first_source_operand , string second_source_operand);

// Changes the values and Loads
void addr_transition(string destination_register ,string first_source_operand , string second_source_operand);

// Maps and Loads the Value
void load_transition(string destination_register);

//Writes to the Register File from Result buffer
void write_transition(string destination_register);

void calculate();
void change_and_print();
void trigger(int value);

void simulate( 		int pos1 , int pos2 , int pos3 , int pos4 ,
					string opcode , string destination_register ,
					string first_source_operand , string second_source_operand,
					int first_length , int second_length  );

void calculate_val(vector<string> place);
int main(){
	take_input();
	return 0;
}
// Function body/definition to output the step wise simulation to a file 
void write_to_file()
{
	

			ofstream output_stream;
			
			if(step_count==0)
			{
				output_stream.open("simulation.txt");
				output_stream<< "STEP "<<step_count<<":"<<"\n";
				output_stream.close();
			}
			else
			{
				output_stream.open("simulation.txt" , ios::app);
				output_stream<<"\n"<< "\nSTEP "<<step_count<<":"<<"\n";
				output_stream.close();
			}
		
			output_stream.open("simulation.txt", ios :: app);
			for( vector<string>::const_iterator i = INM.begin(); i != INM.end() ; i++) // Check Segmentation Fault due to -1
    		{
    	if(i!= INM.begin() && i!= INM.end()-1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
    		}
			output_stream<<"\n";
		
			for( vector<string>::const_iterator i = INB.begin(); i != INB.end(); i++)
    		{
    	if(i!= INB.begin() && i!= INB.end()-1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
    		}
			output_stream<<"\n";
			
			for( vector<string>::const_iterator i = AIB.begin(); i != AIB.end(); i++)
    		{
    	if(i!= AIB.begin() && i!= AIB.end()-1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
    		}
			output_stream<<"\n";
		
			
			for( vector<string>::const_iterator i = LIB.begin(); i != LIB.end(); i++)
    		{
    	if(i!= LIB.begin() && i!= LIB.end()-1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
    		}
			output_stream<<"\n";
		
			
			for( vector<string>::const_iterator i = ADB.begin(); i != ADB.end(); i++)
    		{
    	if(i!= ADB.begin() && i!= ADB.end()-1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
    		}
			output_stream<<"\n";
		
			
			for( vector<string>::const_iterator i = REB.begin(); i != REB.end(); i++)
    		{
    	if(i!= REB.begin() && i!= REB.end()-1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
    		}
			output_stream<<"\n";
		
			
			for( vector<string>::const_iterator i = RGF.begin(); i != RGF.end(); i++)  // Check Segmentation Fault due to -1
    		{
    	if(i!= RGF.begin() && i!= RGF.end()-1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
   			}
			output_stream<<"\n";
		
			for( vector<string>::const_iterator i = DAM.begin(); i != DAM.end(); i++)  // Check Segmentation Fault due to -1
   			{
    	if(i!= DAM.begin() && i!= DAM.end()- 1)
    		output_stream<< *i<<",";
    	else
    		output_stream<< *i;
  			}
			//output_stream<<"\n";
			
			output_stream.close();

			step_count +=1;
			//trigger();
}
void take_input(){
	ifstream input_stream;
	ofstream output_stream;
	string line;

	// The INM, RGF , DAM buffer's can hold only 8 instructions at a time , so
	INM.reserve(9);				// Can store 8 at a time // 9 because INM : will be at [0]th position
	RGF.reserve(9);					// Can store 8 at a time
	DAM.reserve(9);					// Can store 8 at a time
	// Reading From Instructions.txt	
	int data_count=0;
	input_stream.open("instructions.txt");
	INM.push_back("INM:");
	data_count=0;
	while(input_stream >> line)
	{
		if(data_count<8){
			INM.push_back(line);		//(!!!Check!!!) Can include a condition of 8 and wait till one item get's popped out and then 9th enter's
		//INM.push_back(",");
			data_count++;
		}
		else{
			cout<<"\n Error!! More then 8";
		}
		
	}

	input_stream.close();
	data_count=0;
	// Reading From Registers.txt	
	input_stream.open("registers.txt");
	RGF.push_back("RGF:");
	while(input_stream >> line)
	{
		if(data_count<8){
			RGF.push_back(line);
			//RGF.push_back(",");
			data_count++;
		}
		else{
						cout<<"\n Error!! More then 8";
		}
	}		
	input_stream.close();
	data_count=0;
	// Reading From DataMemory.txt	
	input_stream.open("datamemory.txt");
	DAM.push_back("DAM:");
	while(input_stream >> line)
	{
		if(data_count<8){
		DAM.push_back(line);
		//DAM.push_back(",");
		data_count++;
		}
		else{
			cout<<"\n Error!! More then 8";
		}
	}
	input_stream.close();

	INB.push_back("INB:");	// Just for Output format
	AIB.push_back("AIB:");	// Just for Output format
	LIB.push_back("LIB:");	// Just for Output format
	REB.push_back("REB:");	// Just for Output format
	ADB.push_back("ADB:");	// Just for Output format
	write_to_file();
	//cout<<"INM"<<INM.size();
	change_and_print();
}
int read_transition(string &source1 , string &source2 , string opcode){

	int flag1 = 0 , flag2 = 0;
	int pos1 , pos2 , pos3;
	if(opcode=="LD"){
		flag2=1;
		for(int i=0; i< RGF.size() ; i++){
		if(RGF[i].find(source1)==1)
		{
			pos1=RGF[i].find(source1);
			pos2=RGF[i].find(",");
			pos3=RGF[i].find(">");
			//cout<<"\n"<<RGF[i].substr(pos2+1 , pos3-pos2-1)<<"\n";
			source1 = RGF[i].substr(pos2+1 , pos3-pos2-1);		// Actually Changes the value of operands (Call by Reference)
			flag1=1;
		}
	 }
	}
	else{
		for(int i=0; i< RGF.size() ; i++){

		if(RGF[i].find(source1)==1)
		{
			pos1=RGF[i].find(source1);
			pos2=RGF[i].find(",");
			pos3=RGF[i].find(">");
			//cout<<"\n"<<RGF[i].substr(pos2+1 , pos3-pos2-1)<<"\n";
			source1 = RGF[i].substr(pos2+1 , pos3-pos2-1);		// Actually Changes the value of operands (Call by Reference)
			flag1=1;
		}	
		if(RGF[i].find(source2)==1){
			pos1=RGF[i].find(source1);
			pos2=RGF[i].find(",");
			pos3=RGF[i].find(">");
			source2 = RGF[i].substr(pos2+1 , pos3-pos2-1);		// Actually changes the value of operands (Call by reference)
			flag2=1;
		}
	}
}

	if(flag1 && flag2)
		return 1;
	else
		return 0;
}
void calculate(){
	
	if(INM.size() != 1){
		int pos1 , pos2 , pos3 , pos4 ; 			// To find position of commas and separate out operands
		pos1 = INM[1].find(",");			// Position of 1st comma
		pos2 = INM[1].find(",", pos1+1);	// Position of 2nd Comma
		pos3 = INM[1].find("," ,pos2+1);	// Position of 3rd comma
		pos4 = INM[1].find(">");
		//cout<< pos1 << pos2 << pos3 << pos4;
		
		string opcode;
		string destination_register;
		string first_source_operand;
		string second_source_operand;
		int first_length;
		int second_length;

		opcode = INM[1].substr(1,pos1-1);
		destination_register = INM[1].substr(pos1+1,2); //2 tells how many characters to be there in sub
		first_source_operand = INM[1].substr(pos2+1,2);
		second_source_operand = INM[1].substr(pos3+1, pos4-pos3-1); 	
		first_length = first_source_operand.length();
		second_length = second_source_operand.length();

		simulate(	pos1 ,  pos2 ,  pos3 ,  pos4 ,
					opcode , destination_register , first_source_operand , 
					second_source_operand, first_length ,  second_length  );
	}
}

void simulate( 		int pos1 , int pos2 , int pos3 , int pos4 ,
					string opcode , string destination_register ,
					string first_source_operand , string second_source_operand,
					int first_length , int second_length  ){
	
	int read_check;
	read_check = read_transition(first_source_operand , second_source_operand , opcode);
	
		if(read_check==1)
		{
            if  (stoi(first_source_operand) < 10){
                INM[1].replace(pos2+1,first_length,first_source_operand);
                INM[1].replace(pos3,second_length,second_source_operand); // //chnge to 2//Might be a bug, Check
            }
            else
            {
                INM[1].replace(pos2+1,first_length,first_source_operand);
                INM[1].replace(pos3+1,second_length,second_source_operand);
            }
			decode_transition(); 	// Now when you remove the Add operand , then you need to check the INB back to see if other operation are ready
			number_of_tokens++;
		}
		else{
            cout<<"Here";
			//lock_array[INM_LOCK]=0;
		}
}
void decode_transition(){
	INB.push_back(INM[1]);
	INM.erase(INM.begin() + 1);
	//cout<<"Decoded";
	lock_array[INB_LOCK]=1;
	lock_array[INM_LOCK]=1;
	//token_number++;
}

// Will remove token from INB and place it in AIB
void issue1_transition(){
	//cout<<"issue1_transition";
	AIB.push_back(INB[1]);
	INB.erase(INB.begin() + 1);
	lock_array[INB_LOCK]=0;
	lock_array[INM_LOCK]=1;
	lock_array[AIB_LOCK]=1;
}

// Will remove token from INB and place it in LIB
void issue2_transition(){
	LIB.push_back(INB[1]);
	INB.erase(INB.begin() + 1);
	lock_array[INB_LOCK]=0;
	lock_array[INM_LOCK]=1;
	lock_array[LIB_LOCK]=1;
	
}

void asu_transition(string opcode, string destination_register ,string first_source_operand , string second_source_operand){
	int temp;
	if(opcode=="ADD"){
		//cout<<"\n Add";
		//cout<< stoi(first_source_operand,nullptr) + stoi(second_source_operand,nullptr);
		temp = stoi(first_source_operand,nullptr) + stoi(second_source_operand,nullptr);
		REB.push_back("<"+destination_register+","+to_string(temp)+">");
		AIB.erase(AIB.begin() + 1);
	}
	if(opcode=="SUB"){
		temp = stoi(first_source_operand,nullptr) - stoi(second_source_operand,nullptr);
		REB.push_back("<"+destination_register+","+to_string(temp)+">");
		AIB.erase(AIB.begin() + 1);
	}
	lock_array[REB_LOCK]+=1;
	lock_array[AIB_LOCK]=0;
}
void addr_transition(string destination_register ,string first_source_operand , string second_source_operand){
		int temp;
		temp = stoi(first_source_operand,nullptr) + stoi(second_source_operand,nullptr);
		ADB.push_back("<"+destination_register+","+to_string(temp)+">");
		LIB.erase(LIB.begin() + 1);
		lock_array[ADB_LOCK]=1;
		lock_array[LIB_LOCK]=0;
}
void load_transition(string destination_register){
		int pos1 , pos2;
		string temp, temp1;
		pos1 = ADB[1].find(",");
		pos2 = ADB[1].find(">");
		temp = ADB[1].substr(4,pos2-pos1-1);
		//cout<<"\n"<<temp;
		int temp_p1,temp_p2;
		for(int i=1;i<DAM.size();i++){
			temp_p1 = DAM[i].find(",");
			//cout<<"\n"<<DAM[i].substr(1,temp_p1 - 1);
			if(((DAM[i].substr(1,temp_p1 - 1)) == temp)){
				pos1 = DAM[i].find(",");
				pos2 = DAM[i].find(">");
				temp = DAM[i].substr(pos1+1,pos2-pos1-1);
				REB.push_back("<"+destination_register+","+temp+">");
				ADB.erase(ADB.begin() + 1);
			}
		}
		lock_array[REB_LOCK]+=1;
		lock_array[ADB_LOCK]=0;
}
void write_transition(string destination_register){
	// Over-Writes if the value if already there
	int flag1 = 0;
	int pos3 ,pos2 , pos1;
	string temp;
	for(int i=0; i< RGF.size() ; i++){
		
		if(RGF[i].find(destination_register)==1)
		{
			RGF.erase(RGF.begin() + i);
			RGF.push_back(REB[1]);
			REB.erase(REB.begin() + 1);
			sort(RGF.begin()+1,RGF.end());
			flag1=1;
			number_of_tokens--;
			break;

		}
	}
		if(flag1 == 0){
			RGF.push_back(REB[1]);
			REB.erase(REB.begin() + 1);
			sort(RGF.begin()+1,RGF.end());
			number_of_tokens--;
		}
		lock_array[REB_LOCK] -= 1;
}
void calculate_val(vector<string> place){
	//cout<<"Here"<<place[0];
	if(place[0]=="INB:" || place[0]=="AIB:" || place[0]=="LIB:"){
			//cout<<"In IF";
			int pos1 , pos2 , pos3 , pos4 ; 		// To find position of commas and separate out operands
				pos1 = place[1].find(",");			// Position of 1st comma
				pos2 = place[1].find(",", pos1+1);	// Position of 2nd Comma
				pos3 = place[1].find("," ,pos2+1);	// Position of 3rd comma
				pos4 = place[1].find(">");
				//cout<< pos1 << pos2 << pos3 << pos4;
			
				int first_length;
				int second_length;

				dummy_opcode = place[1].substr(1,pos1-1);
				dummy_destination_register = place[1].substr(pos1+1,2); //2 tells how many characters to be there in sub
				dummy_first_source_operand = place[1].substr(pos2+1,2);
				dummy_second_source_operand = place[1].substr(pos3+1, pos4-pos3-1); 
	}
	else
	{
		int pos1 , pos2;
		pos1 = place[1].find(",");
		pos2 = place[1].find(">");
		dummy_destination_register = place[1].substr(1,pos1-1);;
	}
}
void change_and_print(){
		//cout<<"\n Lock Array";
		int temp_array[6];
		for(int i=0;i<6;i++){
			//cout<<lock_array[i];
			temp_array[i]=lock_array[i];
		}	
		
		for(int i=0;i<6;i++)	
		{
				if(temp_array[i]==1 || temp_array[i]>1){
					if(i==5){
						//cout<<INM.size();
						if(INM.size()!=1){
							//cout<<"heree";
							trigger(i);
						}
					}
					else
							trigger(i);
					}
				}

		write_to_file();
		if(REB.size()==1 && INM.size()==1)
			exit(0);
		else
			change_and_print();
	}

void trigger(int value){

	switch(value){
		

		case 0: 
            if (INB.size()>1) {
                calculate_val(INB);
                if((dummy_opcode=="ADD" || dummy_opcode=="SUB")){
                    issue1_transition();
                }
                if((dummy_opcode=="LD" )){
                    issue2_transition();
                }
            }
            

		break;

		case 1: 
							calculate_val(ADB);
							load_transition(dummy_destination_register);
		break;

		

		case 2:	
							calculate_val(LIB);
							addr_transition(dummy_destination_register ,dummy_first_source_operand , dummy_second_source_operand);

		break;

		case 3:		
							calculate_val(AIB);
							asu_transition(dummy_opcode, dummy_destination_register ,dummy_first_source_operand , dummy_second_source_operand);

		break;

		case 4:		if (REB.size()>1)
					{
							calculate_val(REB);
							write_transition(dummy_destination_register);
					}
		break;

		case 5: if(INB.size()==1){
									calculate();
								 	lock_array[INB_LOCK] = 1;
								}
		break;

	}

}
