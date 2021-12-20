
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class TimeStepPrototype_1 {
    uint32_t nages;
    std::vector<double> ages;

public:

    uint32_t nyears;
    uint32_t nseasons;
    double increment;
    
    double first_age;
    double last_age;

    TimeStepPrototype_1(uint32_t nyears, uint32_t nseasons, double first_age, double last_age) :
    nyears(nyears), nseasons(nseasons), first_age(first_age), last_age(last_age) {

        double age_increment = 1.0 / nseasons;
        double age = first_age;

        while (age < last_age) {
            this->ages.push_back(age);
            age += age_increment;
        }


        for (int i = 0; i < this->nyears; i++) {
            std::cout << "year " << i << ":\n";
            for (int a = 0; a < this->ages.size(); a++) {
                std::cout << this->ages[a] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

class TimeStepPrototype_2 {
    uint32_t nages;

    std::map<uint32_t, std::vector<double> > ages;

public:


    uint32_t nyears;
    uint32_t nseasons;
    double first_age;
    double last_age;

    TimeStepPrototype_2(std::map<uint32_t, std::vector<double> > data_time_snapshot, 
    double first_age, double last_age) :
    first_age(first_age), last_age(last_age) {

        this->nyears = data_time_snapshot.size();
        std::map<uint32_t, std::vector<double> >::iterator it;

        for (it = data_time_snapshot.begin(); it != data_time_snapshot.end(); ++it) {

            std::vector<double>& timestamps = (*it).second;
            uint32_t year = (*it).first;
            std::vector<double> ages;
            for (double a = first_age; a < last_age; a++) {
                ages.push_back(a);
                for (int j = 0; j < timestamps.size(); j++) {
                    ages.push_back(a + timestamps[j]);
                }
                ages.push_back(last_age);
                this->ages[year] = ages;
            }

        }

        std::map<uint32_t, std::vector<double> >::iterator ages_iterator;
        for (ages_iterator = this->ages.begin(); ages_iterator != this->ages.end(); ++ages_iterator) {
            uint32_t year = (*ages_iterator).first;
            std::cout << "year " << year << ":\n";
            std::vector<double>& ages = (*ages_iterator).second;
            for (double a = 0; a < ages.size(); a++) {
                std::cout << ages[a] << " ";
            }
            std::cout << std::endl;
        }




    }




};


int main(int argc, char** argv) {

    std::map<uint32_t, std::vector<double> > data_driven_timestamps1;
    std::map<uint32_t, std::vector<double> > data_driven_timestamps2;

    data_driven_timestamps1[0] = {.3333, .6666};
    data_driven_timestamps1[1] = {.3333, .6666};
    data_driven_timestamps1[2] = {.3333, .6666};
    data_driven_timestamps1[3] = {.3333, .6666};
    data_driven_timestamps1[4] = {.3333, .6666};
    data_driven_timestamps1[5] = {.3333, .6666};
    data_driven_timestamps1[5] = {.3333, .6666};
    data_driven_timestamps1[6] = {.3333, .6666};
    

    //data driven timestamps
    data_driven_timestamps2[0] = {.3333, .6666};
    data_driven_timestamps2[1] = {.5};
    data_driven_timestamps2[2] = {.25, .5, .75};
    data_driven_timestamps2[3] = {.3333, .6666};
    data_driven_timestamps2[4] = {.25};
    data_driven_timestamps2[5] = {.3333};
    data_driven_timestamps2[5] = {.6666};
    data_driven_timestamps2[6] = {.5};

    std::cout << "EXAMPLE 1\n\n";
    TimeStepPrototype_1 example1(7, 3, 1, 7);
    TimeStepPrototype_2 example1_2(data_driven_timestamps1, 1, 7);


    std::cout << "\n\nEXAMPLE 2\n\n";
    TimeStepPrototype_1 example2(7, 3, 1, 7);
    TimeStepPrototype_2 example2_2(data_driven_timestamps2, 1, 7);


    return 0;
}

