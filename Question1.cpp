#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>
#include <json/json.h> 

const std::string API_KEY = "2b680566f19da128143c30e4a9765860"; 
const std::string BASE_URL = "http://api.openweathermap.org/data/2.5/weather";

// Helper function to write API response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append((char*)contents, totalSize);
    return totalSize;
}

// Function to fetch weather details for a city
std::string getWeatherDetails(const std::string& city) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        std::string url = BASE_URL + "?q=" + city + "&appid=" + API_KEY + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Failed to fetch weather data: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    std::cout << "Response: " << response << std::endl;

    return response;
}

// Function to parse and display weather details
void displayWeatherDetails(const std::string& response) {
    Json::Value jsonData;
    Json::CharReaderBuilder reader;
    std::string errs;

    std::istringstream s(response);
    if (Json::parseFromStream(reader, s, &jsonData, &errs)) {
        if (jsonData["cod"].asInt() == 200) {
            std::cout << "City: " << jsonData["name"].asString() << std::endl;
            std::cout << "Temperature: " << jsonData["main"]["temp"].asFloat() << "°C" << std::endl;
            std::cout << "Weather: " << jsonData["weather"][0]["description"].asString() << std::endl;
            std::cout << "Humidity: " << jsonData["main"]["humidity"].asInt() << "%" << std::endl;
        } else {
            std::cerr << "Error: " << jsonData["message"].asString() << std::endl;
        }
    } else {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
    }
}

// Main Application
int main() {
    std::vector<std::string> favouriteCities;
    std::map<std::string, std::string> weatherCache;
    int choice;

    do {
        std::cout << "\nWeather Management Application\n";
        std::cout << "1. Search for Weather Details of a City\n";
        std::cout << "2. Add a City to Favourites\n";
        std::cout << "3. List Favourite Cities\n";
        std::cout << "4. Update Favourite Cities\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::cin.ignore();
            std::string city;
            std::cout << "Enter city name: ";
            std::getline(std::cin, city);

            std::string response = getWeatherDetails(city);
            displayWeatherDetails(response);
            break;
        }
        case 2: {
            if (favouriteCities.size() >= 3) {
                std::cerr << "Error: You can only have 3 favourite cities. Use option 4 to update them.\n";
            } else {
                std::cin.ignore();
                std::string city;
                std::cout << "Enter city name to add to favourites: ";
                std::getline(std::cin, city);

                favouriteCities.push_back(city);
                std::cout << "City added to favourites.\n";
            }
            break;
        }
        case 3: {
            if (favouriteCities.empty()) {
                std::cout << "No favourite cities.\n";
            } else {
                for (const auto& city : favouriteCities) {
                    if (weatherCache.find(city) == weatherCache.end()) {
                        weatherCache[city] = getWeatherDetails(city);
                    }
                    std::cout << "\nWeather details for " << city << ":\n";
                    displayWeatherDetails(weatherCache[city]);
                }
            }
            break;
        }
        case 4: {
            if (favouriteCities.empty()) {
                std::cout << "No favourite cities to update.\n";
            } else {
                std::cout << "Current favourite cities:\n";
                for (size_t i = 0; i < favouriteCities.size(); ++i) {
                    std::cout << i + 1 << ". " << favouriteCities[i] << "\n";
                }

                int index;
                std::cout << "Enter the index of the city to remove: ";
                std::cin >> index;

                if (index > 0 && index <= (int)favouriteCities.size()) {
                    favouriteCities.erase(favouriteCities.begin() + index - 1);
                    std::cin.ignore();
                    std::string city;
                    std::cout << "Enter city name to add: ";
                    std::getline(std::cin, city);

                    favouriteCities.push_back(city);
                    std::cout << "Favourite cities updated.\n";
                } else {
                    std::cerr << "Invalid index.\n";
                }
            }
            break;
        }
        case 5:
            std::cout << "Exiting the application. Goodbye!\n";
            break;
        default:
            std::cerr << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
