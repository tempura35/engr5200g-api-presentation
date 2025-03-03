#include <iostream>
#include <curl/curl.h>
#include <string>

// Callback function to handle server response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string response_data;

    // Initialize libcurl
    curl = curl_easy_init();
    if (curl) {
        // Set the API URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://httpbin.org/post");

        // Specify that this is a POST request
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // JSON data to send
        std::string json_data = R"({"name": "John Doe", "age": 30, "email": "john@example.com"})";

        // Set the POST fields
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

        // Set content-type header
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set response handling callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Curl request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Server Response:\n" << response_data << std::endl;
        }

        // Cleanup
        curl_slist_free_all(headers); // Free allocated header list
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize libcurl" << std::endl;
    }

    return 0;
}
