#include<iostream>
#include<unordered_map>
#include<string>
using namespace std;
#define ADRESSS_SIZE 4


enum class PORTS{
    PORT1,
    PORT2
};
typedef unordered_map<string, PORTS> AddressTable;

// task 1:  definition of lan frame structure
struct lan_frame
{
    char destination_address[ADRESSS_SIZE + 1];
    char source_address[ADRESSS_SIZE + 1];
    PORTS port;
};

// task 2: definition of Bridge class
class Bridge
{
    private:
        AddressTable map;

        void receive_frame(lan_frame *frame)
        {
            string source_address(frame->source_address);
            if(map.count(source_address)==0){
                cout<< "Find new address: " << frame->source_address << " at port: "<< (int)(frame->port) << endl;
                map[source_address] = frame->port;
            }
        }

        void forward_frame(lan_frame *frame)
        {
            string destination_address(frame->destination_address);
            if(destination_address == "1111"){
                cout<< "Broadcast frame. Flooding the frame." << endl;
            }
            else if(map.count(destination_address)==0){
                cout<< "Unknown destination address: " << frame->destination_address << ". Flooding the frame." << endl;
            }
            else if(map[destination_address] == frame->port){
                cout<< "Frame is received on the same port as the destination address. No forwarding needed." << endl;
            }
            else {
                cout<< "Forwarding frame to port: " << (int)(map[destination_address]) << endl;
            }
        }
    public:
        Bridge(){}

        void frame_handler(lan_frame *frame)
        {
            receive_frame(frame);
            forward_frame(frame);
        }

};

int main()
{
    // test cases
    lan_frame frame1 = {"1011", "1010", PORTS::PORT1};
    lan_frame frame2 = {"1010", "1011", PORTS::PORT2};
    lan_frame frame3 = {"1110", "1010", PORTS::PORT2};
    lan_frame frame4 = {"1010", "1011", PORTS::PORT1}; 
    lan_frame frame5 = {"1111", "1010", PORTS::PORT2};

    Bridge bridge;

    // try
    cout<<"----------------- Test Case Start -----------------" << endl;
    cout<<"Frame 1 test"    << endl;
    bridge.frame_handler(&frame1);
    cout<<"Frame 2 test"    << endl;
    bridge.frame_handler(&frame2);
    cout<<"Frame 3 test"    << endl;
    bridge.frame_handler(&frame3);
    cout<<"Frame 4 test"    << endl;
    bridge.frame_handler(&frame4);
    cout<<"Frame 5 test"    << endl;
    bridge.frame_handler(&frame5);
    cout<<"----------------- Test Case End -----------------" << endl;
    cout<<"Press Enter to continue..." << endl;
    getchar();
    return 0;
}