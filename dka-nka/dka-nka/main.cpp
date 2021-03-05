#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include<algorithm>
#include <queue>
#include <utility>
#include <set>
#include <numeric>


typedef std::string Symbols;
typedef std::string States;
typedef std::unordered_map<States, std::vector<std::string>> TransitionPairs;
typedef std::unordered_map<Symbols, TransitionPairs> Transition;
typedef std::pair<Transition, std::vector<std::string>> TransitionsWithStates;

struct OutputAutomat
{
	std::vector<std::string> allStatesOutput;
	std::vector<std::set<std::string>> newStates;
};


std::vector<std::string> SplitStringToVector(const std::string& str)
{
	std::vector<std::string> res;
	std::stringstream ss(str);
	std::string symbol;
	while (ss >> symbol)
	{
		res.push_back(symbol);
		symbol.clear();
	}
	return res;
}

TransitionPairs MakeTransitionPairs(const std::string& transitions, std::vector<std::string> states)
{
	TransitionPairs pairs;
	std::string transitionToSymbol;
	std::vector<std::string> transitionsTo;
	std::string copyTransitions = transitions;
	boost::split(transitionsTo, copyTransitions, boost::is_any_of(";"));
	
	for (size_t i = 0; i < states.size(); i++)
	{
		std::vector<std::string> avialableStates;
		auto elem = transitionsTo[i];
		boost::trim(elem);
		boost::split(avialableStates, elem, boost::is_any_of(" "));

		std::sort(avialableStates.begin(), avialableStates.end());
		pairs.emplace(states[i], avialableStates);
	}
	return pairs;
}

std::pair<Transition, std::vector<std::string>> ReadFile(std::istream& file)
{
	Transition transition;
	std::string inputSymbols;
	std::string states;
	std::getline(file, inputSymbols);
	auto inputSymbolsVect = SplitStringToVector(inputSymbols);
	std::getline(file, states);
	auto statesVect = SplitStringToVector(states);
	for (size_t i = 0; i < inputSymbolsVect.size(); i++)
	{
		std::string str;
		std::getline(file, str);
		transition[inputSymbolsVect[i]] = MakeTransitionPairs(str, statesVect);
	}

	return std::make_pair(transition, statesVect);
}

std::set<std::string> MakeNewStatesSet(const std::string& state)
{
	std::set<std::string> newStates;
	std::vector<std::string> statesVec;
	boost::split(newStates, state, boost::is_any_of(" "));
	return newStates;
}
template <typename T>
std::string ConcatAndTrimArray(const T& vec)
{
	std::string res = std::accumulate(vec.begin(), vec.end(), std::string(" "));
	boost::trim(res);
	return res;
}

template <typename T>
std::string ConcatWithSpaceAndTrimArray(const T& vec)
{
	std::ostringstream result_stream;
	std::ostream_iterator<std::string> oit(result_stream, " ");
	std::copy(vec.begin(), vec.end(), oit);
	std::string res = result_stream.str();
	boost::trim(res);
	return res;
}

OutputAutomat GetDKA(Transition const& transition, std::vector<std::string>& states)
{
	std::queue<std::string> queue;
	std::set<std::string> allStates;
	std::vector<std::string> allStatesOutput;
	std::vector<std::set<std::string>> newStates;

	queue.push(states[0]);
	allStates.insert(states[0]);

	while (!queue.empty())
	{
		auto newState = queue.front();
		queue.pop();
		std::set<std::string> newStateSet = MakeNewStatesSet(newState);
		newStates.push_back(newStateSet);
		for (const auto& trans: transition)
		{
			std::vector<std::string> nextStateListInThisInputSymbol;
			std::vector<std::string> allStatesOutputTmp;
			for (auto& state : newStateSet)
			{
				auto destinationState = transition.at(trans.first).at(state);
				std::string nextState = ConcatWithSpaceAndTrimArray(destinationState);
				if (nextState != "-")
				{
					nextStateListInThisInputSymbol.push_back(nextState);
					allStatesOutputTmp.push_back(nextState);
				}
			}
			if (ConcatWithSpaceAndTrimArray(allStatesOutputTmp).size() > 0)
			{
				std::vector<std::string> newTmpStates;
				for (auto& it: allStatesOutputTmp)
				{
					auto splitedStr = SplitStringToVector(it);
					for (size_t i = 0; i < splitedStr.size(); i++)
					{
						if (newTmpStates.end() == find(newTmpStates.begin(), newTmpStates.end(), splitedStr[i]))
						{
							newTmpStates.push_back(splitedStr[i]);
						}
					}
				}
				std::sort(newTmpStates.begin(), newTmpStates.end());
				allStatesOutput.push_back(ConcatWithSpaceAndTrimArray(newTmpStates));
			}
			else
			{
				allStatesOutput.push_back("-");
			}
			std::string nextStateQueue = ConcatWithSpaceAndTrimArray(nextStateListInThisInputSymbol);
			auto nextStateQueueSet = MakeNewStatesSet(nextStateQueue);
			nextStateQueue = ConcatWithSpaceAndTrimArray(nextStateQueueSet);
			if (nextStateQueue.size() > 0 && allStates.end() == allStates.find(nextStateQueue))
			{
				queue.push(nextStateQueue);
				allStates.insert(nextStateQueue);
			}	
		}
	}
	return OutputAutomat{ allStatesOutput , newStates };
}

void WriteResult(const std::string& outFileName, const Transition& trans, const OutputAutomat& outAutomat)
{
	std::ofstream out(outFileName);
	if (!out.is_open())
		return;
	for (auto& tr: trans)
	{
		out << tr.first << ' ';
	}
	out << '\n';
	auto newStates = outAutomat.newStates;
	auto allStatesOutput = outAutomat.allStatesOutput;
	for (size_t i = 0; i < newStates.size(); i++)
	{
		if (i != newStates.size() - 1)
		{
			out << ConcatAndTrimArray(newStates[i]) << ' ';
		}
		else
		{
			out << ConcatAndTrimArray(newStates[i]) << '\n';
		}
	}

	for (size_t i = 0; i < trans.size(); i++)
	{
		for (size_t j = i; j < allStatesOutput.size(); j += trans.size())
		{
			if (i == j)
			{
				auto state = allStatesOutput[j];
				auto splitedState = SplitStringToVector(state);
				std::sort(splitedState.begin(), splitedState.end());
				out << ConcatAndTrimArray(splitedState);
			}
			else
			{
				auto state = allStatesOutput[j];
				auto splitedState = SplitStringToVector(state);
				std::sort(splitedState.begin(), splitedState.end());
				out << "; " << ConcatAndTrimArray(splitedState);
			}
		}
		out << '\n';
	}

}

void MakeVertex(const std::string& label1, const std::string& label2, std::stringstream & stream)
{
	stream << label1 + " ["
		<< "shape=\"" << "circle" << "\""
		<< "label=\"" + label2 + "\"];\n";
}

void MakeEdge(const std::string& from, const std::string& to, const std::string& label, std::stringstream& stream)
{
	stream << from + "->" << to << " [label=\"" << label << " \"];\n";
}

void VisualizeNKA(const std::string& fileName)
{
	std::ifstream file(fileName);
	std::string inSymbol;
	std::string states;
	std::vector<std::string> inSymbolVec;
	std::vector<std::string> statesVec;
	getline(file, inSymbol);
	boost::trim(inSymbol);
	boost::split(inSymbolVec, inSymbol, boost::is_any_of(" "));
	getline(file, states);
	boost::trim(states);
	boost::split(statesVec, states, boost::is_any_of(" "));
	std::stringstream dotss;
	dotss << "digraph G{\n";
	for (auto& elem : statesVec)
	{
		MakeVertex(elem, elem, dotss);
	}
	for (auto& sym : inSymbolVec)
	{
		std::vector<std::string> splitedStr;
		std::string line;
		getline(file, line);
		boost::trim(line);
		boost::split(splitedStr, line, boost::is_any_of(";"));
		for (size_t i = 0; i < splitedStr.size(); i++)
		{
			boost::trim(splitedStr[i]);
			std::vector<std::string> nextStatesInThisInputSymbol;
			boost::split(nextStatesInThisInputSymbol, splitedStr[i], boost::is_any_of(" "));
			for (auto& state : nextStatesInThisInputSymbol)
			{
				if (state != "-")
				{
					MakeEdge(statesVec[i], state, sym, dotss);
				}
			}
		}
	}
	dotss << "}\n";

	const std::string pathToDot = "../../Graphviz/bin/";
	const std::string dotName = "../../Graphviz/bin/graph.dot";
	const std::string imageName = "graph.png";


	std::ofstream dotFile(dotName);
	dotFile << dotss.str();
	dotFile.close();

	std::string change = "cd " + pathToDot + "\ndot	 -Tpng -o " + imageName + " " + dotName;
	system(change.c_str());

}

int main()
{
	std::string inFileName = "input.txt";
	std::string outFileName = "output.txt";
	std::ifstream file(inFileName);
	if (!file.is_open())
	{
		std::cout << "bad";
	}
	auto pair = ReadFile(file);
	OutputAutomat outputAutomat = GetDKA(pair.first, pair.second);
	WriteResult(outFileName, pair.first, outputAutomat);
	VisualizeNKA(outFileName);
}