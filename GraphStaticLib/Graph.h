#pragma once

#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <stack>

template<typename T>
class Graph
{
	using Id = int;
	struct Node
	{
		T value;
		Id id;
		std::set<std::shared_ptr<Node>> neighbors;
		inline Node(Id id, const T& value) noexcept : id(id), value(value) {}

		class iterator
		{
		private:
			std::set<Id> visited;
			std::stack<std::shared_ptr<Node>> toVisit;
			std::shared_ptr<Node> currentNode;
		public:
			explicit iterator(const std::shared_ptr<Node>& node) noexcept : currentNode(node) {}
			iterator& operator++() noexcept
			{
				visited.emplace(currentNode->id);

				for (std::shared_ptr<Node> neighbor : currentNode->neighbors)
				{
					if (!visited.contains(neighbor->id))
					{
						visited.emplace(neighbor->id);
						toVisit.push(neighbor);
					}
				}

				if (!toVisit.empty())
				{
					currentNode = toVisit.top();
					toVisit.pop();
				}
				else
					currentNode = nullptr;

				return *this;
			}
			iterator& operator++(int) noexcept
			{
				auto tmp = *this;
				this->operator++();
				return tmp;
			}
			bool operator!=(const iterator& other) const noexcept { return currentNode != other.currentNode; }
			bool operator==(const iterator& other) const noexcept { return currentNode == other.currentNode; }
			T& operator*() noexcept { return currentNode->value; }
		};
		iterator begin() noexcept { return iterator(std::make_shared<Node>(*this)); }
		iterator end() noexcept { return iterator(nullptr); }
	};

	std::map<Id, std::shared_ptr<Node>> nodes;

public:

	Id getId(const T& value) noexcept
	{
		for (auto iterator = nodes.begin(); iterator != nodes.end(); ++iterator)
		{

			if (value == iterator->second->value)
			{
				return iterator->first;
			}
		}
	}

	bool add(Id id, const T& value) noexcept
	{
		for (auto iterator = nodes.begin(); iterator != nodes.end(); ++iterator)
		{

			if (value == iterator->second->value)
			{
				return false;
			}
		}

		nodes.emplace(id, std::make_shared<Node>(Node(id, value)));

		return true;
	}

	void remove(Id id) noexcept
	{
		if (nodes.contains(id))
		{
			nodes.erase(id);
		}
	}

	void connect(Id id1, Id id2) noexcept
	{
		if (nodes.find(id1) != nodes.end() && nodes.find(id2) != nodes.end() && !nodes[id1]->neighbors.contains(nodes[id2]))
		{
			nodes[id1]->neighbors.emplace(nodes[id2]);
			nodes[id2]->neighbors.emplace(nodes[id1]);
		}
	}

	void disconnect(Id id1, Id id2) noexcept
	{
		if (nodes[id1]->neighbors.contains(nodes[id2]))
			nodes[id1]->neighbors.erase(nodes[id2]);
	}

	class iterator
	{
		decltype(nodes.begin()) mapIterator;
	public:
		iterator(decltype(mapIterator) mapIterator) noexcept : mapIterator(mapIterator) {}
		iterator& operator++() noexcept { ++mapIterator; return *this; }
		iterator operator++(int) noexcept
		{
			auto temp = mapIterator;
			++mapIterator;
			return temp;
		}
		bool operator!=(const iterator& other) const noexcept { return mapIterator != other.mapIterator; }
		bool operator==(const iterator& other) const noexcept { return mapIterator == other.mapIterator; }
		T& operator*() noexcept { return mapIterator->second->value; }
	};
	iterator begin() noexcept { return nodes.begin(); }
	iterator end() noexcept { return nodes.end(); }

	void forEach(const Id& id1, const Id& id2, std::stack<std::string>& stack) noexcept
	{
		for (auto& name : *nodes[id1])
		{
			stack.push(name);

			if (name == nodes[id2]->value)
			{
				break;
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Graph<T>& graph)
	{
		for (auto iterator = graph.nodes.begin(); iterator != graph.nodes.end(); ++iterator)
		{
			os << "Id: " << iterator->first << " " << "Value: " << iterator->second->value << std::endl;
		}

		return os;
	}

	void findShortestPath(const Id& id1, const Id& id2) noexcept
	{
		std::cout << "Shortest path between " << id1 << " and " << id2 << ":\n";

		std::stack<std::string> stack;

		this->forEach(id1, id2, stack);

		std::string tmpName = stack.top();
		stack.pop();



		if (tmpName == nodes[id2]->value)
		{
			std::cout << tmpName;
			while (!stack.empty())
			{
				tmpName = stack.top();
				stack.pop();
				std::cout << " <-- " << tmpName;
			}
		}
		else
		{
			std::cout << "Path between nodes does not exist.";
		}
	}
};
