#include <iostream>
#include <fstream>
#include "proto/device_irs/.customers.pb.h"

void createCustomer() {
    // Create a new Customer object
    Customer customer;
    customer.set_id(1);
    customer.set_name("John Doe");
    customer.set_email("john.doe@example.com");
    customer.set_address("123 Main St, Anytown, USA");

    // Allocate a buffer for the serialized data
    int size = customer.ByteSizeLong();
    std::vector<uint8_t> buffer(size);

    // Serialize the customer to the buffer
    if (!customer.SerializeToArray(buffer.data(), size)) {
        std::cerr << "Failed to serialize customer." << std::endl;
        return;
    }

    // Write the serialized customer to a file
    std::ofstream outputFile("customer.dat", std::ios::out | std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    outputFile.write(reinterpret_cast<const char*>(buffer.data()), size);
    outputFile.close();

    std::cout << "Customer data serialized and written to file." << std::endl;
}

void readCustomer() {
    // Read the serialized customer from a file
    std::ifstream inputFile("customer.dat", std::ios::in | std::ios::binary);
    if (!inputFile) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Deserialize the buffer into a Customer object
    Customer customer;
    if (!customer.ParseFromArray(buffer.data(), buffer.size())) {
        std::cerr << "Failed to deserialize customer." << std::endl;
        return;
    }

    // Display the customer's information
    std::cout << "Customer ID: " << customer.id() << std::endl;
    std::cout << "Name: " << customer.name() << std::endl;
    std::cout << "Email: " << customer.email() << std::endl;
    std::cout << "Address: " << customer.address() << std::endl;
}

int main() {
    std::cout << "protobuf-cpp" << std::endl;

    // Initialize the protobuf library
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create and serialize a customer
    createCustomer();

    // Read and deserialize the customer
    readCustomer();

    // Shutdown protobuf library
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}