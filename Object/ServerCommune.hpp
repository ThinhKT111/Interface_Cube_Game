#ifndef SERVERCOMMUNE_HPP
#define SERVERCOMMUNE_HPP

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <curl/curl.h>

using namespace std;


// Function to handle the response from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

std::string sendData(const std::string& data);

#endif
