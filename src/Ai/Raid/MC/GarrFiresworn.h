/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef PLAYERBOTS_GARRFIRESWORN_H
#define PLAYERBOTS_GARRFIRESWORN_H

#include <algorithm>
#include <iterator>
#include <list>
#include <vector>

namespace MoltenCoreHelpers
{
// AI-REVIEW 2026-11-27 [Codex GPT-6/spec]: preserve a held add when another add or tank dies.
template <typename Firesworn, typename Tank>
Firesworn* SelectGarrFiresworn(std::list<Firesworn*>& candidates, std::vector<Tank*> tanks, Tank const* tank,
                               Tank const* garrTank = nullptr)
{
    if (tank == garrTank)
        return nullptr;
    tanks.erase(std::remove(tanks.begin(), tanks.end(), garrTank), tanks.end());

    candidates.remove_if([](Firesworn const* add) { return !add || !add->IsAlive(); });
    candidates.sort([](Firesworn const* left, Firesworn const* right)
                    { return left->GetGUID().GetCounter() < right->GetGUID().GetCounter(); });

    // Give each eligible tank first refusal on one add they already hold.
    for (auto add = candidates.begin(); add != candidates.end();)
    {
        auto* victim = (*add)->GetVictim();
        auto owner = victim ? std::find(tanks.begin(), tanks.end(), victim) : tanks.end();
        if (owner == tanks.end())
        {
            ++add;
            continue;
        }
        if (*owner == tank)
            return *add;

        *owner = nullptr;
        add = candidates.erase(add);
    }

    auto const slot = std::find(tanks.begin(), tanks.end(), tank);
    if (slot == tanks.end())
        return nullptr;

    auto const rank = std::count_if(tanks.begin(), slot, [](Tank const* member) { return member != nullptr; });
    if (static_cast<typename std::list<Firesworn*>::size_type>(rank) >= candidates.size())
        return nullptr;
    return *std::next(candidates.begin(), rank);
}
}  // namespace MoltenCoreHelpers

#endif
