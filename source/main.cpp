#include <iostream>
#include <fstream>
#include <vector>

#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/util/field_comparator.h>
#include "proto/device_irs/customer.pb.h"

uint32_t calculateChecksum(const customer::Customer& customer) {
    std::string data;
    auto comparator = std::make_unique<google::protobuf::util::DefaultFieldComparator>();
    google::protobuf::util::MessageDifferencer differencer;
    differencer.set_field_comparator(comparator.get());
    differencer.IgnoreField(customer::Customer::GetDescriptor()->FindFieldByName("header"));
    differencer.IgnoreField(customer::Customer::GetDescriptor()->FindFieldByName("footer"));
    differencer.ReportDifferencesToString(&data);
    differencer.Compare(customer, customer);

    return std::accumulate(data.begin(), data.end(), 0u);
}

customer::Customer createCustomer() {
    customer::Customer customer;

    customer.mutable_header()->set_id(1);
    customer.mutable_header()->set_timestamp(std::time(nullptr));
    customer.set_name("John Doe");
    customer.set_phone("123456");
    customer.set_email("john.doe@example.com"); // overrides 'phone' field because of oneof usage in proto file
    customer.set_address("123 Main St, Anytown, USA");
    customer.mutable_footer()->set_checksum(calculateChecksum(customer));

    return customer;
}

std::vector<uint8_t> serialize_customer(const customer::Customer& customer) {
    const auto size = customer.ByteSizeLong();
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
    std::cout << "protobuf-cpp" << " v" << APP_VERSION_MAJOR << "." << APP_VERSION_MINOR << "." << APP_VERSION_PATCH << APP_VERSION_DIRTY << std::endl;

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
    std::cout << "---Header---" << std::endl;
    std::cout << "Customer ID: " << deserialized_customer.header().id() << std::endl;
    std::cout << "Timestamp: " << deserialized_customer.header().timestamp() << std::endl;
    std::cout << "---Payload---" << std::endl;
    std::cout << "Name: " << deserialized_customer.name() << std::endl;
    if (deserialized_customer.has_phone()) {
        std::cout << "Phone: " << deserialized_customer.phone() << std::endl;
    }
    if (deserialized_customer.has_email()) {
        std::cout << "Email: " << deserialized_customer.email() << std::endl;
    }
    std::cout << "Address: " << deserialized_customer.address() << std::endl;
    std::cout << "---Footer---" << std::endl;
    std::cout << "Checksum: " << deserialized_customer.footer().checksum() << std::endl;

    // Shutdown protobuf library
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}