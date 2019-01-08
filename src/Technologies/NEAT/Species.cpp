#include "Species.h"

Species::Species(Genome &base, unsigned id) : specieID(id), bestFitness(base.Fitness()), gensWithoutImprove(0), age(0) {
    this->members.push_back(&base);
    best = base;
}

void Species::addMember(Genome &member) {
    if (member.Fitness() > this->bestFitness) {
        this->bestFitness = member.Fitness();
        gensWithoutImprove = 0;
        best = member;
    }

    members.push_back(&member);
}

void Species::adjustFitness() {
    double totalFitness = 0;
    for (size_t i = 0; i < members.size(); ++i) {
        double fitness = members[i]->Fitness();
        if (age < younThreshold)
            fitness *= youngBonus;

        if (age > oldThreshold)
            fitness *= oldPenalty;

        totalFitness += fitness;
        members[i]->setFitness(fitness / members.size());
    }
}