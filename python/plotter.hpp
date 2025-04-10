#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <ROOT/RVec.hxx>
#include <functional>

template <typename InputType, typename Func>
auto MapRVec(const ROOT::RVec<InputType>& inputVec, Func func) {
    using OutputType = decltype(func(std::declval<InputType>()));
    ROOT::RVec<OutputType> outputVec;
    outputVec.reserve(inputVec.size()); // Reserve space for efficiency
    for (const auto& element : inputVec) {
        outputVec.emplace_back(func(element)); // Apply the function and add to the output vector
    }
    return outputVec;
}

auto getP = [](const auto& x) { return x.P(); };
auto getPt = [](const auto& x) { return x.Pt(); };
auto getTheta = [](const auto& x) { return x.Theta(); };
auto getPhi = [](const auto& x) { return x.Phi(); };
auto getE = [](const auto& x) { return x.E(); };
auto getM = [](const auto& x) { return x.M(); };

#endif // PLOTTER_HPP