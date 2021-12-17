/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fims_indexing.cpp
 * Author: mattadmin
 *
 * Created on December 17, 2021, 12:42 PM
 */

#include <cstdlib>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

using namespace std;

class model_base {
    static size_t id_g;
public:
    size_t nyears_;
    size_t nseasons_;
    size_t nages_;
    size_t object_id;

    model_base(size_t nyears, size_t nseasons, size_t nages) :
    nyears_(nyears), nseasons_(nseasons), nages_(nages) {
        this->object_id = model_base::id_g++;
    }

    /**
     * Return dimension folded index for time and age;
     * 
     * @param year
     * @param season
     * @param age
     * @return 
     */
    inline const size_t get_index(
            const size_t& year,
            const size_t& season,
            const size_t& age) {

        return year * this->nseasons_ * this->nages_ + season * this->nages_ + age;
    }

    /**
     * Return dimension folded index for time only;
     * 
     * @param year
     * @param season
     * @return 
     */
    inline const size_t get_index(
            const size_t& year,
            const size_t& season) {

        return year * this->nseasons_ * this->nages_ + season;
    }



};

size_t model_base::id_g = 0;

class area : public model_base {
public:

    area(size_t nyears, size_t nseasons, size_t nages) :
    model_base(nyears, nseasons, nages) {
    }

};

class population_base : public model_base {
public:
    std::vector<double> ages;

    population_base(size_t nyears, size_t nseasons, size_t nages, std::vector<double> ages) :
    model_base(nyears, nseasons, nages), ages(ages) {
    }

};

class subpopulation : public population_base {
public:

    std::vector<double> some_derived_quantities;

    subpopulation(size_t nyears, size_t nseasons, size_t nages, std::vector<double> ages) :
    population_base(nyears, nseasons, nages, ages) {
        some_derived_quantities.resize(nyears * nseasons * nages);
    }

    std::shared_ptr<area> area_;

    void calculate_some_life_history_1(size_t index) {
        //        std::cout << "doing some life history stuff at index " << index << std::endl;
        this->some_derived_quantities[index] = index;
    }

    void calculate_some_life_history(size_t index_1, size_t index_2) {
        //        std::cout << "doing some life history stuff 2";
    }

    void finalize() {

        std::cout << "subpopulation " << this->object_id << "\n\n";

        for (size_t y = 0; y < this->nyears_; y++) {
            for (size_t s = 0; s < this->nseasons_; s++) {
                std::cout << "subpopulation " << this->object_id << " " << "year " << y << " " << "season " << s << "\n";
                for (size_t a = 0; a < this->nages_; a++) {
                    size_t index = this->get_index(y, s, a);
                    std::cout << this->some_derived_quantities[index] << "  ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }

};

class population : public population_base {
public:
    std::vector<std::shared_ptr<area> > areas_;


    //given a sex, hold a vector of area specific sub_populations
    std::map<size_t, std::vector<std::shared_ptr<subpopulation> > > subpopulation_;

    size_t nsexes_;

    population(size_t nyears, size_t nseasons, size_t nages, std::vector<double> ages) :
    population_base(nyears, nseasons, nages, ages) {
    }

    /**
     * initialize subpopulations
     * 
     * @param nsexes
     * @param areas
     */
    void initialize_subpopulations(const size_t& nsexes,
            const std::vector<std::shared_ptr<area> >& areas) {
        this->nsexes_ = nsexes;
        this->areas_ = areas;

        for (int i = 0; i < this->nsexes_; i++) {
            for (int j = 0; j < this->areas_.size(); j++) {
                std::shared_ptr<subpopulation> sub_pop = std::make_shared<subpopulation>(this->nyears_, this->nseasons_, this->nages_, this->ages);
                sub_pop->area_ = this->areas_[i];
                this->subpopulation_[i].push_back(sub_pop);
            }
        }

    }

    void evaulate_subpopulations() {

        for (size_t i = 0; i < this->nsexes_; i++) {
            std::vector<std::shared_ptr<subpopulation> >&
                    sub_pops = this->subpopulation_[i];

            for (size_t j = 0; j < sub_pops.size(); j++) {
                for (size_t y = 0; y < this->nyears_; y++) {
                    for (size_t s = 0; s < this->nseasons_; s++) {
                        for (size_t a = 0; a < this->nages_; a++) {

                            size_t index = this->get_index(y, s, a);
                            sub_pops[j]->calculate_some_life_history_1(index);

                        }
                    }
                }

            }
        }
    }

    void finalize() {
        for (size_t i = 0; i < this->nsexes_; i++) {
            std::vector<std::shared_ptr<subpopulation> >&
                    sub_pops = this->subpopulation_[i];

            for (size_t j = 0; j < sub_pops.size(); j++) {
                sub_pops[j]->finalize();
            }
        }
    }
};

/*
 * 
 */
int main(int argc, char** argv) {
    std::vector<double> ages = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t years = 30;
    size_t seasons = 4;

    std::vector<std::shared_ptr<area> > areas = {std::make_shared<area>(years, seasons, ages.size()),
        std::make_shared<area>(years, seasons, ages.size()),
        std::make_shared<area>(years, seasons, ages.size())};

    population pop(years, seasons, ages.size(), ages);
    pop.initialize_subpopulations(2, areas);
    pop.evaulate_subpopulations();
    pop.finalize();

    return 0;
}

