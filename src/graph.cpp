// Smart City Route & Infrastructure Planner
// Copyright © 2026 Quad Core. All rights reserved.
//
// graph.cpp - implementation of the Graph class (see graph.h).
//
// Complexity notes: V = number of locations, E = number of roads,
// deg = number of roads at one location. std::map lookups take O(log V).

#include "graph.h"

#include <cctype>
#include <cmath>

// ===== Validation rules (see graph.h) =====

bool isValidDistance(double distance)
{
    // isfinite() is false for infinity and NaN ("not a number").
    return std::isfinite(distance) && distance > 0 && distance <= MAX_ROAD_DISTANCE;
}

std::string trimName(const std::string& name)
{
    const std::string spaces = " \t\r\n";
    size_t start = name.find_first_not_of(spaces);
    if (start == std::string::npos) {
        return "";   // the name contains only spaces
    }
    size_t end = name.find_last_not_of(spaces);
    return name.substr(start, end - start + 1);
}

// Returns the text with every letter in lower case: "BUBT Campus" -> "bubt campus".
// Used to compare names without caring about upper/lower case.
static std::string toLowerCase(const std::string& text)
{
    std::string result = text;
    for (char& c : result) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

// ===== Locations =====

// Time: O(V) because nameExists() checks every name.
int Graph::addLocation(const std::string& name)
{
    std::string cleanName = trimName(name);
    if (cleanName.empty() || nameExists(cleanName)) {
        return -1;
    }

    int id = nextId;
    nextId++;

    names[id] = cleanName;
    adjacency[id] = std::vector<Edge>();   // a new location has no roads yet
    return id;
}

// Time: O(V) because nameExists() checks every name.
bool Graph::addLocationWithId(int id, const std::string& name)
{
    std::string cleanName = trimName(name);
    if (id <= 0 || hasLocation(id) || cleanName.empty() || nameExists(cleanName)) {
        return false;
    }

    names[id] = cleanName;
    adjacency[id] = std::vector<Edge>();

    // Make sure a location added later gets a new, unused ID.
    if (id >= nextId) {
        nextId = id + 1;
    }
    return true;
}

// Time: O(V) because findLocationId() checks every name.
bool Graph::renameLocation(int id, const std::string& newName)
{
    std::string cleanName = trimName(newName);
    if (!hasLocation(id) || cleanName.empty()) {
        return false;
    }
    // The name may not belong to a DIFFERENT location.
    // (The same location may keep its name or change its capital letters.)
    int owner = findLocationId(cleanName);
    if (owner != -1 && owner != id) {
        return false;
    }

    names[id] = cleanName;
    return true;
}

// Time: O(V log V + E) - removing the roads looks at the neighbours' lists,
// and renumbering rebuilds both maps once.
bool Graph::removeLocation(int id)
{
    if (!hasLocation(id)) {
        return false;
    }

    // 1. Remove the location and its roads. Every road is stored in two
    //    lists: the copy in THIS location's list disappears when the list is
    //    erased, but the copy in each neighbour's list must be removed too.
    for (const Edge& edge : adjacency[id]) {
        removeEdge(edge.to, id);
    }
    adjacency.erase(id);
    names.erase(id);

    // 2. Renumber: every ID greater than the deleted one moves down by one.
    //    The maps are rebuilt with the new IDs (a map key cannot be changed),
    //    and every road end is updated with the same rule.
    std::map<int, std::string> newNames;
    std::map<int, std::vector<Edge>> newAdjacency;
    for (const auto& entry : names) {
        newNames[shiftedId(entry.first, id)] = entry.second;
    }
    for (const auto& entry : adjacency) {
        std::vector<Edge> roads = entry.second;   // same roads, same order
        for (Edge& edge : roads) {
            edge.to = shiftedId(edge.to, id);
        }
        newAdjacency[shiftedId(entry.first, id)] = roads;
    }
    names = newNames;
    adjacency = newAdjacency;

    // 3. The next new location gets the next sequential ID.
    nextId = names.empty() ? 1 : names.rbegin()->first + 1;
    return true;
}

// The new ID of location `oldId` after location `deletedId` was removed:
// IDs above the deleted one move down by one, the others stay the same.
int Graph::shiftedId(int oldId, int deletedId)
{
    return oldId > deletedId ? oldId - 1 : oldId;
}

bool Graph::hasLocation(int id) const
{
    return names.count(id) > 0;
}

// Time: O(V) - every name is compared.
int Graph::findLocationId(const std::string& name) const
{
    std::string wanted = toLowerCase(trimName(name));
    // In a std::map, each entry has a key (entry.first) and a value (entry.second).
    for (const auto& entry : names) {
        if (toLowerCase(entry.second) == wanted) {
            return entry.first;
        }
    }
    return -1;
}

bool Graph::nameExists(const std::string& name) const
{
    return findLocationId(name) != -1;
}

std::string Graph::getName(int id) const
{
    if (!hasLocation(id)) {
        return "";
    }
    return names.at(id);
}

std::vector<int> Graph::getLocationIds() const
{
    std::vector<int> ids;
    for (const auto& entry : names) {
        ids.push_back(entry.first);   // std::map keeps keys sorted, so IDs come in order
    }
    return ids;
}

int Graph::locationCount() const
{
    return static_cast<int>(names.size());
}

int Graph::getNextId() const
{
    return nextId;
}

// ===== Roads =====

// Time: O(log V + deg) - the duplicate check scans one adjacency list.
bool Graph::addRoad(int from, int to, double distance)
{
    if (!hasLocation(from) || !hasLocation(to)) {
        return false;
    }
    if (from == to) {
        return false;
    }
    if (!isValidDistance(distance)) {
        return false;
    }
    if (hasRoad(from, to)) {
        return false;
    }

    // The road is two-way, so it is stored in the lists of BOTH locations.
    adjacency[from].push_back({to, distance});
    adjacency[to].push_back({from, distance});
    return true;
}

// Time: O(log V + deg)
bool Graph::updateRoad(int from, int to, double distance)
{
    if (!hasRoad(from, to)) {
        return false;
    }
    if (!isValidDistance(distance)) {
        return false;
    }

    // Change the distance in both copies of the road.
    // `Edge& edge` is a reference, so changing it changes the stored edge.
    for (Edge& edge : adjacency[from]) {
        if (edge.to == to) {
            edge.distance = distance;
        }
    }
    for (Edge& edge : adjacency[to]) {
        if (edge.to == from) {
            edge.distance = distance;
        }
    }
    return true;
}

// Time: O(log V + deg)
bool Graph::removeRoad(int from, int to)
{
    if (!hasRoad(from, to)) {
        return false;
    }

    // Remove both copies of the road.
    removeEdge(from, to);
    removeEdge(to, from);
    return true;
}

void Graph::removeEdge(int from, int to)
{
    std::vector<Edge>& edges = adjacency[from];
    for (size_t i = 0; i < edges.size(); i++) {
        if (edges[i].to == to) {
            edges.erase(edges.begin() + i);
            return;   // there is at most one road between two locations
        }
    }
}

// Time: O(log V + deg)
bool Graph::hasRoad(int from, int to) const
{
    if (!hasLocation(from)) {
        return false;
    }
    for (const Edge& edge : adjacency.at(from)) {
        if (edge.to == to) {
            return true;
        }
    }
    return false;
}

std::vector<Edge> Graph::getNeighbors(int id) const
{
    if (!hasLocation(id)) {
        return std::vector<Edge>();
    }
    return adjacency.at(id);
}

// Time: O(V + E)
std::vector<Road> Graph::getRoads() const
{
    std::vector<Road> roads;
    for (const auto& entry : adjacency) {
        int from = entry.first;
        for (const Edge& edge : entry.second) {
            // Each road is stored twice (A->B and B->A).
            // Keep only the copy where from < to, so every road is listed once.
            if (from < edge.to) {
                roads.push_back({from, edge.to, edge.distance});
            }
        }
    }
    return roads;
}

int Graph::roadCount() const
{
    return static_cast<int>(getRoads().size());
}
