#include <iostream>
#include <queue>
using namespace std;

// Define packet structure
struct Packet {
    int data; // Numeric data
};

// Define frame structure
struct Frame {
    int seq;      // Sequence number (0 to 3)
    int ack;      // Acknowledgment number
    Packet packet; // Data
};

// Simulated functions to send and receive packets
void from_network_layer(Packet &p, int packet_num) {
    p.data = packet_num; // Use numeric data
    cout << "Network layer: Packet " << p.data << " ready to send." << endl;
}

void to_physical_layer(const Frame &f) {
    cout << "Physical layer: Frame with seq " << f.seq << " sent (data = " << f.packet.data << ")." << endl;
}

void from_physical_layer(Frame &f) {
    cout << "Physical layer: Frame with seq " << f.seq << " received (data = " << f.packet.data << ")." << endl;
}

void to_network_layer(const Packet &p) {
    cout << "Network layer: Packet " << p.data << " received." << endl;
}

// Sender function for 2-bit sliding window protocol
void sender(int total_packets) {
    int window_size = 2; // Sliding window size
    queue<Frame> window; // Queue to hold frames in the window
    int next_seq = 0;    // Next sequence number to use
    int last_ack = -1;   // Last acknowledgment received

    for (int i = 0; i < total_packets; ++i) {
        // Generate a packet
        Packet p;
        from_network_layer(p, i);

        // Create a frame and add it to the window
        Frame frame;
        frame.seq = next_seq;
        frame.packet = p;
        to_physical_layer(frame);
        window.push(frame);

        // Update sequence number (circular increment for 2 bits: 0-3)
        next_seq = (next_seq + 1) % 4;

        // Simulate acknowledgment
        if (window.size() == window_size || i == total_packets - 1) {
            // Assume acknowledgment is received for the first frame in the window
            Frame ack_frame = window.front();
            window.pop();
            last_ack = ack_frame.seq;

            cout << "Sender: ACK received for seq " << last_ack << "." << endl;
        }
    }
}

// Receiver function for 2-bit sliding window protocol
void receiver(int total_packets) {
    int expected_seq = 0; // Expected sequence number

    for (int i = 0; i < total_packets; ++i) {
        // Simulate receiving a frame
        Frame frame;
        frame.seq = i % 4; // Simulated sequence number (cyclic)
        frame.packet.data = i; // Simulated data
        from_physical_layer(frame);

        // Check if the frame sequence number matches the expected sequence
        if (frame.seq == expected_seq) {
            to_network_layer(frame.packet);
            cout << "Receiver: Packet with seq " << frame.seq << " processed." << endl;

            // Update the expected sequence number (circular increment)
            expected_seq = (expected_seq + 1) % 4;
        } else {
            cout << "Receiver: Out-of-order packet received (seq " << frame.seq << "). Discarding." << endl;
        }

        // Send acknowledgment
        cout << "Receiver: ACK for seq " << ((expected_seq - 1 + 4) % 4) << " sent." << endl;
    }
}

int main() {
    int total_packets;
    cout << "Enter the number of packets to be sent: ";
    cin >> total_packets;

    cout << "Starting 2-Bit Sliding Window Protocol simulation..." << endl;

    sender(total_packets);  // Simulate the sender side
    receiver(total_packets);  // Simulate the receiver side

    return 0;
}

