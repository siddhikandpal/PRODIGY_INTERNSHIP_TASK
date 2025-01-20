#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "gumbo.h"

using namespace std;

// Function to write the response data to a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}

// Function to extract text from a GumboNode
string extractText(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        string result = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            result += extractText(static_cast<GumboNode*>(children->data[i]));
        }
        return result;
    }
    return "";
}

// Function to search for elements with a specific class name
void searchByClassName(GumboNode* node, const string& className, vector<GumboNode*>& results) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboAttribute* classAttr = gumbo_get_attribute(&node->v.element.attributes, "class");
    if (classAttr && string(classAttr->value).find(className) != string::npos) {
        results.push_back(node);
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        searchByClassName(static_cast<GumboNode*>(children->data[i]), className, results);
    }
}

int main() {
    // URL of the e-commerce website
    string url = "https://www.meesho.com/";

    // Initialize libcurl
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl) {
        cerr << "Failed to initialize libcurl" << endl;
        return 1;
    }

    string htmlData;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlData);

    // Perform the HTTP request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_cleanup(curl);

    // Parse the HTML
    GumboOutput* output = gumbo_parse(htmlData.c_str());
    if (!output) {
        cerr << "Failed to parse HTML" << endl;
        return 1;
    }

    // Search for products
    vector<GumboNode*> products;
    searchByClassName(output->root, "product", products); // Adjust based on HTML structure

    // Open the CSV file
    ofstream csvFile("product_data.csv");
    if (!csvFile.is_open()) {
        cerr << "Failed to open CSV file for writing" << endl;
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        return 1;
    }

    // Write the CSV header
    csvFile << "Product Name,Price,Rating\n";

    // Extract product data
    for (auto* productNode : products) {
        string productName = extractText(productNode); // Adjust based on the structure
        string price = extractText(productNode);       // Adjust based on the structure
        string rating = extractText(productNode);      // Adjust based on the structure

        // Write data to the CSV file
        csvFile << productName << "," << price << "," << rating << "\n";
    }

    cout << "Data has been successfully scraped and saved to product_data.csv" << endl;

    csvFile.close();
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return 0;
}
