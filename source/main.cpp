#include <iostream>
#include <fstream>
#include <vector>

#include "proto/device_irs/customer.pb.h"

customer::Customer createCustomer() {
    customer::Customer customer;

    customer.set_id(1);
    customer.set_name("John Doe");
    customer.set_email("john.doe@example.com");
    customer.set_address("123 Main St, Anytown, USA");

    return customer;
}

std::vector<uint8_t> serialize_customer(const customer::Customer& customer) {
    auto size = customer.ByteSizeLong();
    std::vector<uint8_t> buffer(size);

    if (!customer.SerializeToArray(buffer.data(), size)) {
        std::cerr << "Failed to serialize customer." << std::endl;
    }
    return buffer;
}

bool write_serialized_data_to_file(const std::vector<uint8_t>& data, const std::string& filename) {
    std::ofstream outputFile(filename, std::ios::out | std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return true;
    }
    outputFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    outputFile.close();

    return false;
}

customer::Customer read_customer_from_file(const std::string& filename) {
    // Read the serialized customer from a file
    std::ifstream inputFile(filename, std::ios::in | std::ios::binary);
    if (!inputFile) {
        std::cerr << "Failed to open file for reading." << std::endl;
    }

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Deserialize the buffer into a Customer object
    customer::Customer customer;
    if (!customer.ParseFromArray(buffer.data(), buffer.size())) {
        std::cerr << "Failed to deserialize customer." << std::endl;
    }

    return customer;
}

int main() {
    std::cout << "protobuf-cpp" << std::endl;

    // Initialize the protobuf library
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Create a customer
    auto customer = createCustomer();

    // Serialize customer
    const auto serialized_customer = serialize_customer(customer);

    // Write serialized customer to file
    write_serialized_data_to_file(serialized_customer, "customer.dat");

    std::cout << "Customer data serialized and written to file." << std::endl;

    // Read and deserialize the customer
    const auto deserialized_customer = read_customer_from_file("customer.dat");

    // Display the customer's information
    std::cout << "Customer ID: " << deserialized_customer.id() << std::endl;
    std::cout << "Name: " << deserialized_customer.name() << std::endl;
    std::cout << "Email: " << deserialized_customer.email() << std::endl;
    std::cout << "Address: " << deserialized_customer.address() << std::endl;

    // Shutdown protobuf library
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}