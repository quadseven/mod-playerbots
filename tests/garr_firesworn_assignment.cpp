/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

// Run with a C++17 compiler: c++ -std=c++17 -Isrc tests/garr_firesworn_assignment.cpp -o garr-test && ./garr-test

#include <cstdint>
#include <cstdlib>
#include <list>
#include <vector>

#include "Ai/Raid/MC/GarrFiresworn.h"

namespace
{
struct Guid
{
    std::uint32_t counter;
    std::uint32_t GetCounter() const { return counter; }
};

struct Tank
{
};

struct Firesworn
{
    Guid guid;
    bool alive = true;
    Tank* victim = nullptr;
    Guid GetGUID() const { return guid; }
    bool IsAlive() const { return alive; }
    Tank* GetVictim() const { return victim; }
};
}  // namespace

int main()
{
    using MoltenCoreHelpers::SelectGarrFiresworn;

    Tank tankA, tankB, tankC, spare;
    std::vector<Tank*> tanks{&tankA, &tankB, &tankC, &spare};
    Firesworn first{{19}};
    Firesworn second{{41}};
    Firesworn third{{63}};
    Firesworn dead{{3}, false};
    std::list<Firesworn*> fromFirstTank{&third, &dead, &first, &second};
    std::list<Firesworn*> fromSecondTank{&second, &first, &third, &dead};
    std::list<Firesworn*> fromThirdTank{&dead, &second, &third, &first};

    if (SelectGarrFiresworn(fromFirstTank, tanks, &tankA) != &first ||
        SelectGarrFiresworn(fromSecondTank, tanks, &tankB) != &second ||
        SelectGarrFiresworn(fromThirdTank, tanks, &tankC) != &third)
        return EXIT_FAILURE;

    std::list<Firesworn*> surplusTank{&third, &first, &second, &dead};
    std::list<Firesworn*> empty;
    if (SelectGarrFiresworn(surplusTank, tanks, &spare) || SelectGarrFiresworn(empty, tanks, &tankA))
        return EXIT_FAILURE;

    first.victim = &tankA;
    second.victim = &tankB;
    third.victim = &tankC;
    first.alive = false;
    std::list<Firesworn*> afterAddDeath{&third, &first, &second};
    if (SelectGarrFiresworn(afterAddDeath, tanks, &tankA))
        return EXIT_FAILURE;
    afterAddDeath = {&third, &first, &second};
    if (SelectGarrFiresworn(afterAddDeath, tanks, &tankB) != &second)
        return EXIT_FAILURE;

    first.alive = true;
    std::vector<Tank*> afterTankDeath{&tankB, &tankC, &spare};
    std::list<Firesworn*> adds{&third, &first, &second};
    if (SelectGarrFiresworn(adds, afterTankDeath, &tankB) != &second)
        return EXIT_FAILURE;
    adds = {&third, &first, &second};
    if (SelectGarrFiresworn(adds, afterTankDeath, &spare) != &first)
        return EXIT_FAILURE;

    // A living assist tank taking over Garr stays on him, and spare keeps its slot.
    adds = {&first, &second, &third};
    if (SelectGarrFiresworn(adds, tanks, &tankA, &tankA))
        return EXIT_FAILURE;
    adds = {&first, &second, &third};
    if (SelectGarrFiresworn(adds, tanks, &spare, &tankA) != &first)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
