#include <bits/stdc++.h>
using namespace std;

// car containing car's information ie. registration id and age of the driver
class car{
  public:

  string registration_id;
  int driver_age;

  // Default car i.e no car
  car(){
    registration_id="none";
    driver_age=-1;
  }

  car(string s,int age){
    registration_id=s;
    driver_age=age;
  }

};

// Making parking lot
class parking_lot{

  vector<car>slots;
  // Creating Min Heap which will help us to allot nearest slots
  priority_queue<int,vector<int>,greater<int>>available_slots;
  // Keeping record of the allocated slots
  map<int,map<string,int>>allocated_slots;
  unordered_map<string,int>registered_slots;

  public:

  parking_lot(){

  }

  // Initializing our parking lot of size n with default car and pushing each slot no in available slots
  parking_lot(int n){

    car default_car= car();

    for(int i=0;i<n;i++){
      slots.push_back(default_car);
      available_slots.push(i+1);
    }

    cout<<"Created parking of "<<n<<" slots"<<endl;
  }

  void park_car(string reg_id,int age){
    // If there are no slots available then we will tell the user that parking lot is full
    if(available_slots.empty()){
      cout<<"Parking is full !"<<endl;
      return;
    }

    int curr_slot=available_slots.top();
    available_slots.pop();

    car curr_car=car (reg_id,age);
    // Parking car in nearest slot
    slots[curr_slot-1]=curr_car;
    allocated_slots[age][reg_id]=curr_slot;
    registered_slots[reg_id]=curr_slot;

    cout<<"Car with vehicle registration number \""<<reg_id<<"\" has been parked at slot number "<<curr_slot<<endl;
  }

  void remove_car_from(int slot_no){
    // If slot number is greater than our parking lot size then it will print error message
    if(slot_no>slots.size()){
      cout<<"Invalid slot !"<<endl;
      return;
    }
    // If slot is empty
    if(slots[slot_no-1].driver_age==-1){
      cout<<"Already empty !"<<endl;
      return;
    }

    car default_car=car();
    string remove_reg_id=slots[slot_no-1].registration_id;
    int remove_age=slots[slot_no-1].driver_age;

    // Removing car from given slot
    slots[slot_no-1]=default_car;
    // Adding that slot in available slots
    available_slots.push(slot_no);
    // Removing car from allocated cars also
    allocated_slots[remove_age].erase(remove_reg_id);
    if(allocated_slots[remove_age].size()==0)
      allocated_slots.erase(remove_age);

    registered_slots.erase(remove_reg_id);

    cout<<"Slot number "<<slot_no<<" vacated, the car with vehicle registration number \""<<remove_reg_id<<"\" left the space, the driver of the car was of age "<<remove_age<<endl;
  }

  void get_Vehicle_registration_number_for_driver_of_age(int age){
    // If there are no drivers parked in parking lot of the mentioned age 
    if(allocated_slots.find(age)==allocated_slots.end()){
      cout<<"No drivers available of this age !"<<endl;
      return;
    }

    // Printing registration id's of cars having driver of given age
    bool first_element=true;
    for(auto reg_id:allocated_slots[age]){
      // If only car is present then we dont have to put comma
      if(first_element){
        cout<<reg_id.first;
        first_element=false;
      }
      else
        // Separating registration id's using comma 
        cout<<","<<reg_id.first;
    }
    cout<<endl;

  }

  void get_Slot_number_for_car_with_number(string reg_id){
    // If the mentioned registration id is not present in our database
    if(registered_slots.find(reg_id)==registered_slots.end()){
      cout<<"No car present of this registration id !"<<endl;
      return;
    }

    cout<<registered_slots[reg_id]<<endl;
  }

  void get_Slot_numbers_for_driver_of_age(int age){
    // If no car is parked in parking lot by the drivers of mentioned age
    if(allocated_slots.find(age)==allocated_slots.end()){
      cout<<"No drivers available of this age !"<<endl;
      return;
    }

    // Printing registration id's of cars having driver of given age
    bool first_element=true;
    for(auto reg_id:allocated_slots[age]){
      if(first_element){
        // If only car is present then we dont have to put comma
        cout<<reg_id.second;
        first_element=false;
      }
      else
        // Separating registration id's using comma   
        cout<<","<<reg_id.second;
    }
    cout<<endl;

  }

};


void call_command(parking_lot &p,vector<string>&input){

  // Calling commands corresponding to the input string

  if(input.size()==2){
    if(input[0]=="Create_parking_lot"){
      int n=stoi(input[1]);
      p=parking_lot(n);
    }
    else if(input[0]=="Slot_numbers_for_driver_of_age"){
      int age=stoi(input[1]);
      p.get_Slot_numbers_for_driver_of_age(age);
    }
    else if(input[0]=="Slot_number_for_car_with_number"){
      string reg_id=input[1];
      p.get_Slot_number_for_car_with_number(reg_id);
    }
    else if(input[0]=="Leave"){
      int slot_no=stoi(input[1]);
      p.remove_car_from(slot_no);
    }
    else if(input[0]=="Vehicle_registration_number_for_driver_of_age"){
      int age=stoi(input[1]);
      p.get_Vehicle_registration_number_for_driver_of_age(age);
    }
    else{
      cout<<"Invalid Command !"<<endl;
    }
  }
  else if(input.size()==4){
    if(input[0]=="Park" && input[1].size()==13 && input[2]=="driver_age"){
      string reg_id=input[1];
      int age=stoi(input[3]);
      p.park_car(reg_id,age);
    }
    else{
      cout<<"Invalid Command !"<<endl;
    }
  }
  else{
      cout<<"Invalid Command !"<<endl;
  }

}

vector<string>extract_command(string s){

  // Extracting space separated words from the string

  string temp="";
  vector<string>ans;
  for(int i=0;i<s.size();i++){
    if(s[i]==' '){
      ans.push_back(temp);
      temp="";
    }
    else{
      temp.push_back(s[i]);
    }
  }
  ans.push_back(temp);
  return ans;

}

int main() {
  
  // Declaring an empty parking lot
  parking_lot p;

  // Reading input from sample_tc_input.txt

  fstream newfile;
  newfile.open("input.txt",ios::in);
  if (newfile.is_open()){
    string s;
    while(getline(newfile, s)){

      vector<string>input=extract_command(s);
      call_command(p,input);

    }
    newfile.close();
  }

  return 0;
}