// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <functional>


std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here

}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here

}

// Add a new beacon with given ID, name, coordinates and color.
// If the ID already exists, do not modify anything and return false.
// Otherwise, create a BeaconInfo, compute its brightness, initialize
// lightbeam fields, and insert it into:
//  - main storage (beacons_),
//  - name index (beacons_by_name_),
//  - brightness index (beacons_by_brightness_).
bool Datastructures::add_beacon(BeaconID id, const Name& name, Coord xy, Color color)
{
    // Check if the ID already exists
    if (beacons_.find(id) != beacons_.end()) {
        return false;
}
    // Calculate beacon's brightness
    int brightness = 3*color.r + 6*color.g + color.b;

    // Create BeaconInfo for this beacon
    BeaconInfo info;
    info.name = name;
    info.xy = xy;
    info.color = color;
    info.brightness = brightness;

    // Initialize lightbeam emitting
    info.out_edge = NO_BEACON;

    // Insert into the core map
    beacons_.emplace(id, info);

    // Update name index
    beacons_by_name_[name].insert(id);

    // Update brightness index
    beacons_by_brightness_[brightness].insert(id);

    return true;
}

int Datastructures::beacon_count()
{
    //Return number of currently stored beacons.
    return beacons_.size();
}

void Datastructures::clear_beacons()
{
    // Clearing all beacons from three containers
    beacons_.clear();
    beacons_by_name_.clear();
    beacons_by_brightness_.clear();

}


std::vector<BeaconID> Datastructures::all_beacons()
{
    // Creating a list(vector) of the beacons in any order
    std::vector<BeaconID> ids;

    // Reserve memory space first
    ids.reserve(beacons_.size());

    for (const auto& [id, info] : beacons_) {
        ids.push_back(id);
    }

    return ids;
}

// Return the name of beacon with given ID, or NO_NAME if not found.
Name Datastructures::get_name(BeaconID id)
{
    auto it = beacons_.find(id);
    if (it != beacons_.end()) {
        return it->second.name;
    }
    return NO_NAME;
}

// Return coordinates of beacon with given ID, or NO_COORD if not found.
Coord Datastructures::get_coordinates(BeaconID id)
{
    auto it = beacons_.find(id);
    if (it != beacons_.end()) {
        return it->second.xy;
    }
    return NO_COORD;

}

// Return color of beacon with given ID, or NO_COLOR if not found.
Color Datastructures::get_color(BeaconID id)
{
    auto it = beacons_.find(id);
    if (it != beacons_.end()) {
        return it->second.color;
    }
    return NO_COLOR;
}

// Return all beacon IDs sorted alphabetically by name, then by ID.
// We rely on:
//  - std::map<Name, std::set<BeaconID>>:
//      * map keys are sorted by Name,
//      * each set is sorted by BeaconID.
std::vector<BeaconID> Datastructures::beacons_alphabetically()
{
    // Creating a list(vector) of the beacons in any order
    std::vector<BeaconID> result;

    // Reserve memory space first
    result.reserve(beacons_.size());

    // map<Name, set<BeaconID>> gives names in alphabetical order
    for (const auto& [name, idset]: beacons_by_name_) {
        // For each name, add all its IDs (already sorted in set)
        for (const auto& id : idset) {
            result.push_back(id);
        }
    }
    return result;
}

// Return all beacon IDs ordered by increasing brightness.
// For equal brightness, IDs are ordered by numeric/string ID order,
// as given by the std::set<BeaconID>.
std::vector<BeaconID> Datastructures::beacons_brightness_increasing()
{
    // Creating a list(vector) of the beacons in any order
    std::vector<BeaconID> result;

    // Reserve memory space first
    result.reserve(beacons_.size());

    // map<brightness, set<BeaconID>> gives brightness keys in increasing order
    for (const auto& [brightness, idset]: beacons_by_brightness_) {
        // For each name, add all its IDs (already sorted in set)
        for (const auto& id : idset) {
            result.push_back(id);
        }
    }
    return result;
}

// Return ID of a beacon with minimum brightness.
// If there are several, any ID from the lowest-brightness bucket is fine.
BeaconID Datastructures::min_brightness()
{
    // Return NO_BEACON if there is no beacons
    if (beacons_by_brightness_.empty()) {
        return NO_BEACON;
    }

    // Smallest brightness = first element in the map
    auto it = beacons_by_brightness_.begin();

    // The key is the brightness, the value is a set of IDs
    // We only need to take the one (first) ID in the set.
    return *it->second.begin();
}

// Return ID of a beacon with maximum brightness.
// If there are several, any ID from the highest-brightness bucket is fine.
BeaconID Datastructures::max_brightness()
{
    // Return NO_BEACON if there is no beacons
    if (beacons_by_brightness_.empty()) {
        return NO_BEACON;
    }

    // rbegin() gives an iterator to the brightest entry = last element in the map
    auto it = beacons_by_brightness_.rbegin();

    // Take one beacon ID
    return *it->second.begin();
}


// Return all beacon IDs having the given name, ordered ascending by ID.
// If no such name exists, return an empty vector.
std::vector<BeaconID> Datastructures::find_beacons(Name const& name)
{
    // Creating a list(vector) of the beacons in any order
    std::vector<BeaconID> result;

    auto it = beacons_by_name_.find(name);

    if (it != beacons_by_name_.end()) {
        // it->second is std::set<BeaconID>
        result.reserve(it->second.size());
        for (const auto& id: it->second) {
            result.push_back(id);
        }
    }
    return result;
}

bool Datastructures::change_beacon_name(BeaconID id, const Name& newname)
{
    // Check if the beacon exists
    auto it = beacons_.find(id);
    if (it == beacons_.end()) {
        return false;
    }

    // Take the old name from container beacons_
    const Name oldname = it->second.name;

    // If the newname is the same as the old one
    if (oldname == newname) {
        return true;
    }

    // Update the name index: remove from old name's set
    auto name_it = beacons_by_name_.find(oldname);
    if (name_it != beacons_by_name_.end()) {
        name_it->second.erase(id); // remove this ID from the set

        // If no more beacons with this name, erase the key from the map
        if (name_it->second.empty()) {
            beacons_by_name_.erase(name_it);
        }
    }

    // Insert into new name's set
    beacons_by_name_[newname].insert(id);

    // Update the main storage
    it->second.name = newname;

    return true;


}

// Add a lightbeam from sourceid to targetid:
//  - both beacons must exist,
//  - no self-beams,
//  - each beacon may have only one outgoing beam.
// On success, we:
//  - set source.out_edge = targetid,
//  - push sourceid into target.in_edges.
bool Datastructures::add_lightbeam(BeaconID sourceid, BeaconID targetid)
{
    auto source_it = beacons_.find(sourceid);
    auto target_it = beacons_.find(targetid);

    // Check if both IDs exist
    if (source_it == beacons_.end() || target_it == beacons_.end()) {
        return false;
    }

    // Avoid self-beams (source == target)
    if (sourceid == targetid) {
        return false;
    }

    // Check if the source beacon already has an outgoing beam
    BeaconInfo& source_info = source_it->second;
    if (source_info.out_edge != NO_BEACON) {
        return false;
    }

    // Update the source's outgoing beam (O(1))
    source_info.out_edge = targetid;

    // Update the target's incoming beam (O(1) average for vector push_back)
    BeaconInfo& target_info = target_it->second;
    target_info.in_edges.push_back(sourceid);

    return true;
}

// Returns IDs of beacons that transmit their light directly into 'id'.
// If 'id' does not exist, returns {NO_BEACON}.
// Otherwise, returns the incoming source IDs sorted in ascending order.
std::vector<BeaconID> Datastructures::get_lightsources(BeaconID id)
{
    auto it = beacons_.find(id);

    // Check if the beacon ID exists
    if (it == beacons_.end()) {
        return {NO_BEACON};
    }

    std::vector<BeaconID> result = it->second.in_edges;

    // Sort in ascending ID order as required
    std::sort(result.begin(), result.end());   // O(k log k)

    return result;
}

// Follow the outgoing beam chain starting from 'id'.
// The returned vector contains:
//  [id, next, next, ...] until:
//   - there is no outgoing beam (NO_BEACON), or
//   - a cycle is detected.
// If 'id' does not exist, returns {NO_BEACON}.
std::vector<BeaconID> Datastructures::path_outbeam(BeaconID id)
{
    std::vector<BeaconID> path;

    // Used to detect cycles (O(1) average lookup/insertion)
    std::unordered_set<BeaconID> visited;

    // If beacon doesn't exist -> NO_BEACON
    if (beacons_.find(id) == beacons_.end()) {
        path.push_back(NO_BEACON);
        return path;
    }

    BeaconID current_id = id;

    // Follow out_edge chain until no beam or a cycle is detected
    while (current_id != NO_BEACON) {
        // Cycle check: If we have already visited this beacon, stop the path
        if (visited.count(current_id)) {
            break;
        }

        // Add to path and mark as visited
        path.push_back(current_id);
        visited.insert(current_id);

        // Get the outgoing beam target
        BeaconID next_id = beacons_.at(current_id).out_edge;

        // Continue to the next beacon
        current_id = next_id;
    }
    return path;

}

// -- HELPER FUNCTION FOR path_inbeam_longest:
// For a given curr_id, recursively explore its in_edges
// and return a pair:
// { length_of_longest_chain_ending_at_curr_id, starting_beacon_of_that_chain }.
// 'visiting' is used only as simple cycle protection.
std::pair<int, BeaconID> Datastructures::find_deepest_start(
    BeaconID curr_id,
    std::unordered_set<BeaconID>& visiting)
{
    //Cycle detection: If curr_id is already in the recursion stack
    if (visiting.count(curr_id) != 0) {
        return {1, curr_id}; // Break the cycle
    }

    visiting.insert(curr_id);

    const auto& info = beacons_.at(curr_id);

    // Base case: No incoming beams (Leaf in the reversed tree)
    if (info.in_edges.empty()) {
        // Path length is 1 (just this beacon), and it is the starting node
        return {1, curr_id};
    }

    int max_len = 0;
    BeaconID best_start = curr_id;

    // Recursively check all incoming sources
    for (const auto& source_id : info.in_edges) {

        // Recursive call:
        auto [len, start_node] = find_deepest_start(source_id, visiting);

        // Keep the longest chain
        if (len > max_len) {
            max_len = len;
            best_start = start_node;
        }
        // If lengths are equal, pick smaller ID (optional)
        else if (len == max_len) {
            if (start_node < best_start) {
                best_start = start_node;
            }
        }
    }

    // The path length ending at curr_id is 1 (for curr_id)
    // + the max lengh found in its source
    return {max_len + 1, best_start};

}

// Returns the longest incoming chain that ends at beacon 'id'.
// The result is ordered so that each beacon in the vector sends light
// to the next, and the last is 'id' (if reachable).
// If 'id' does not exist, returns {NO_BEACON}.
std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID id)
{
    std::vector<BeaconID> path;

    // Check if the target beacon exists
    if (beacons_.find(id) == beacons_.end()) {
        return {NO_BEACON};
    }

    // Find the start node of the longest path ending at "id"
    // The helper function uses recursion to find the deepest source
    std::unordered_set<BeaconID> visiting;
    auto [len, start_node] = find_deepest_start(id, visiting);

    // Construct the path by tracing forward from the start_node
    BeaconID curr = start_node;

    // reserve memory space for the result
    path.reserve(len);

    // Follow the single outgoing beam until we reach our target "id"
    while (curr != NO_BEACON) {
        path.push_back(curr);

        if (curr == id) {
            break; // Stop we we reach target "id"
        }
        curr = beacons_.at(curr).out_edge;
    }
    return path;

}


// Compute the total emitted color of beacon 'id':
//  - own color + all incoming beams' total colors,
//  - averaged component-wise.
// This is done recursively along in_edges, with cycle detection
// to avoid infinite recursion
Color Datastructures::total_color(BeaconID id)
{
    // 1. Check if beacon exists
    if (beacons_.find(id) == beacons_.end()) {
        return NO_COLOR;
    }

    // For cycle detection
    std::unordered_set<BeaconID> visiting;

    std::function<Color(BeaconID)> calc_color =
        [&](BeaconID curr_id) -> Color
    {
        // Cycle detection
        if (visiting.count(curr_id) != 0) {
            // Break the cycle: just use own color
            return beacons_.at(curr_id).color;
        }
        visiting.insert(curr_id);

        const auto& info = beacons_.at(curr_id);

        long long sum_r = info.color.r;
        long long sum_g = info.color.g;
        long long sum_b = info.color.b;
        int count = 1; // own contribution

        for (const auto& source_id : info.in_edges) {
            if (beacons_.find(source_id) == beacons_.end()) {
                continue; // skip broken links
            }

            Color incoming = calc_color(source_id);
            if (incoming != NO_COLOR) {
                sum_r += incoming.r;
                sum_g += incoming.g;
                sum_b += incoming.b;
                ++count;
            }
        }

        visiting.erase(curr_id);

        Color result;
        result.r = static_cast<int>(sum_r / count);
        result.g = static_cast<int>(sum_g / count);
        result.b = static_cast<int>(sum_b / count);

        return result;
    };

    return calc_color(id);
}


// Add an undirected fibre between xpoint1 and xpoint2 with given cost.
// We store it in fibres_graph_ twice (once in each direction) as adjacency lists.
// If the endpoints are equal or the fibre already exists, we do nothing.
bool Datastructures::add_fibre(Coord xpoint1, Coord xpoint2, Cost cost)
{
    if (xpoint1 == xpoint2) {
        return false;
    }

    // Check if the fibre already exist (O(degree) check)
    auto it = fibres_graph_.find(xpoint1);
    if (it != fibres_graph_.end()) {
        for (const auto& neighbor : it->second) {
            if (neighbor.first == xpoint2) {
                return false; // Fibre already exists
            }
        }
    }

    // Add the fibre (Undire ted graph = Two directed edges)
    // Add neighbor to xpoint1's list
    fibres_graph_[xpoint1].push_back({xpoint2, cost});

    // Add neighbor to xpoint2's list
    fibres_graph_[xpoint2].push_back({xpoint1, cost});

    return true;

}


// Return all coordinates from which at least one fibre starts.
// The result is sorted using Coord::operator< (by y, then x).
std::vector<Coord> Datastructures::all_xpoints()
{
    std::vector<Coord> result;
    result.reserve(fibres_graph_.size());

    // Collect all keys
    for (const auto& entry: fibres_graph_) {
        result.push_back(entry.first);
    }

    // Sort in coordinate order using Coord::operator
    std::sort(result.begin(), result.end());

    return result;


}


// Return all outgoing fibres from the given xpoint.
// For each neighbour, we return (Coord, Cost);
// the result is sorted by Coord (y, then x).
// If no fibres leave xpoint, return an empty vector.
std::vector<std::pair<Coord, Cost> > Datastructures::get_fibres_from(Coord xpoint)
{
    // 1. Check if the coordinate exists
    auto it = fibres_graph_.find(xpoint);
    if (it == fibres_graph_.end()) {
        return {}; // No such xpoint
    }

    std::vector<std::pair<Coord, Cost>> result = it->second;

    std::sort(result.begin(), result.end());

    return result;

}


// Return a list of all fibres as pairs (Coord1, Coord2).
// Each undirected fibre appears only once in the result, with the
// smaller endpoint first (Coord1 < Coord2).
// The whole result is sorted lexicographically by (Coord1, Coord2).
std::vector<std::pair<Coord, Coord> > Datastructures::all_fibres()
{
    std::vector<std::pair<Coord, Coord>> result;

    // Iterate through all nodes (V)
    for (const auto& pair : fibres_graph_) {
        Coord start_node = pair.first;
        // Iterate through all edges
        for (const auto& neighbor : pair.second) {
            Coord end_node = neighbor.first;

            // Ensures each fibre is listed only once
            if (start_node < end_node) {
                result.push_back({start_node, end_node});
            }
        }
    }

    // Sort the result
    std::sort(result.begin(), result.end());

    return result;

}


// Remove a fibre between xpoint1 and xpoint2, if it exists.
// Because the graph is undirected, we remove xpoint2 from xpoint1's
// adjacency list and xpoint1 from xpoint2's list.
// If a node ends up with no neighbours, it is removed from fibres_graph_.
bool Datastructures::remove_fibre(Coord xpoint1, Coord xpoint2)
{
    bool removed_one = false;

    // 1. Remove xpoint2 from xpoint1's neighbor list
    auto it1 = fibres_graph_.find(xpoint1);
    if (it1 != fibres_graph_.end()) {
        auto& neighbors1 = it1->second;
        // Search using the .first (Coord) field of the pair
        auto to_remove = std::remove_if(neighbors1.begin(), neighbors1.end(),
                                        [&](const auto& neighbor){ return neighbor.first == xpoint2; });

        if (to_remove != neighbors1.end()) {
            neighbors1.erase(to_remove, neighbors1.end());
            removed_one = true;
            // Clean up xpoint1 if it has no more edges
            if (neighbors1.empty()) {
                fibres_graph_.erase(it1);
            }
        }
    }

    // 2. Remove xpoint1 from xpoint2's neighbor list
    auto it2 = fibres_graph_.find(xpoint2);
    if (it2 != fibres_graph_.end()) {
        auto& neighbors2 = it2->second;
        // Search using the .first (Coord) field of the pair
        auto to_remove = std::remove_if(neighbors2.begin(), neighbors2.end(),
                                        [&](const auto& neighbor){ return neighbor.first == xpoint1; });

        if (to_remove != neighbors2.end()) {
            neighbors2.erase(to_remove, neighbors2.end());
            // Clean up xpoint2 if it has no more edges
            if (neighbors2.empty()) {
                fibres_graph_.erase(it2);
            }
        }
    }

    return removed_one; // True if at least one removal occurred

}

// Remove all fibres and reset the fibre graph.
void Datastructures::clear_fibres()
{
    // O(1)
    fibres_graph_.clear();
}


// Find any route between fromxpoint and toxpoint using BFS.
// The result is a vector of (Coord, cumulative_cost) starting from
// (fromxpoint, 0) and ending at toxpoint, following valid fibres.
// If start == end, returns a single element with cost 0.
// If no route exists, returns an empty vector.
std::vector<std::pair<Coord, Cost> > Datastructures::route_any(Coord fromxpoint, Coord toxpoint)
{
    std::vector<std::pair<Coord, Cost>> result;

    // Check: Return an empty vector if start point = end point
    if (fromxpoint == toxpoint) {
        return {};
    }

    // // Check: if start or end doesn't exist, no route exists
    if (fibres_graph_.find(fromxpoint) == fibres_graph_.end() ||
        fibres_graph_.find(toxpoint) == fibres_graph_.end()) {
        return {};
    }


    // BFS Queue
    std::queue<Coord> q;
    q.push(fromxpoint);

    // Track visited nodes and path info: "Current -> Previous"
    // We need CoordHash because Coord is the key
    std::unordered_map<Coord, Coord, CoordHash> came_from;

    // Track cumulative cost from start to current node
    std::unordered_map<Coord, Cost, CoordHash> cost_so_far;

    // Initialize
    came_from[fromxpoint] = fromxpoint; // Mark start as visited
    cost_so_far[fromxpoint] = 0;

    bool found = false;

    while (!q.empty()) {
        Coord current = q.front();
        q.pop();

        if (current == toxpoint) {
            found = true;
            break;
        }

        // Check neighbors
        for (const auto& edge : fibres_graph_[current]) {
            Coord next = edge.first;
            Cost edge_cost = edge.second;

            // If not visited yet
            if (came_from.find(next) == came_from.end()) {
                q.push(next);
                came_from[next] = current;
                // Accumulate cost
                cost_so_far[next] = cost_so_far[current] + edge_cost;
            }
        }
    }

    // Path Reconstruction (Backtracking)
    if (!found) return {};

    std::vector<std::pair<Coord, Cost>> path;
    Coord curr = toxpoint;

    while (curr != fromxpoint) {
        path.push_back({curr, cost_so_far[curr]});
        curr = came_from[curr];
    }
    // Add start point
    path.push_back({fromxpoint, 0});

    // Reverse to get Start -> End order
    std::reverse(path.begin(), path.end());

    return path;

}


// Finds a path that uses as few xpoints as possible between fromxpoint
// and toxpoint. We can simply reuse route_any(), because BFS naturally
// finds a path with the minimum number of edges in an unweighted graph.
std::vector<std::pair<Coord, Cost>> Datastructures::route_least_xpoints(Coord fromxpoint, Coord toxpoint)
{
    // Re-use BFS because BFS naturally finds the path with the fewest nodes
    return route_any(fromxpoint, toxpoint);
}


// Find the fastest (minimum total cost) route between fromxpoint and toxpoint
// using Dijkstra's algorithm. The result is a vector of (Coord, cumulative_cost)
// from start to end. If no route exists, returns an empty vector.
std::vector<std::pair<Coord, Cost>> Datastructures::route_fastest(Coord fromxpoint, Coord toxpoint)
{

    // Validation
    if (fibres_graph_.find(fromxpoint) == fibres_graph_.end() ||
        fibres_graph_.find(toxpoint) == fibres_graph_.end()) {
        return {};
    }
    if (fromxpoint == toxpoint) {
        return {{fromxpoint, 0}};
    }

    // Data Structures
    // Min-Priority Queue: Stores {Cumulative Cost, Coordinate}
    // 'greater' makes it a Min-Heap (smallest cost on top)
    using Element = std::pair<Cost, Coord>;
    std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;

    // Track shortest distance found so far to each node
    std::unordered_map<Coord, Cost, CoordHash> dist;

    // Track path for reconstruction: Key=Node, Value=Parent
    std::unordered_map<Coord, Coord, CoordHash> came_from;

    // Initialization
    dist[fromxpoint] = 0;
    pq.push({0, fromxpoint});
    came_from[fromxpoint] = fromxpoint; // Marker for start

    bool found = false;

    // Main Loop
    while (!pq.empty()) {
        // Get the node with the smallest cost
        Cost current_cost = pq.top().first;
        Coord current_node = pq.top().second;
        pq.pop();

        // Early Exit: Found the target
        if (current_node == toxpoint) {
            found = true;
            break;
        }

        // If we found a shorter path to this node *after* adding this entry to PQ,
        // the current entry is "stale". Skip it.
        if (current_cost > dist[current_node]) {
            continue;
        }

        // Explore Neighbors
        for (const auto& edge : fibres_graph_[current_node]) {
            Coord neighbor = edge.first;
            Cost edge_weight = edge.second;

            Cost new_dist = current_cost + edge_weight;

            // Relaxation Step:
            // If this new path is shorter than the known path to neighbor (or if neighbor is unvisited)
            if (dist.find(neighbor) == dist.end() || new_dist < dist[neighbor]) {
                dist[neighbor] = new_dist;
                came_from[neighbor] = current_node;
                pq.push({new_dist, neighbor});
            }
        }
    }

    // Path Reconstruction
    if (!found) return {};

    std::vector<std::pair<Coord, Cost>> path;
    Coord curr = toxpoint;

    while (curr != fromxpoint) {
        path.push_back({curr, dist[curr]});
        curr = came_from[curr];
    }
    // Add start point
    path.push_back({fromxpoint, 0});

    // Reverse to get Start -> End order
    std::reverse(path.begin(), path.end());

    return path;
}


// Find a route starting from startxpoint that eventually forms a cycle
// in the fibre network. We use DFS with an explicit recursion stack
// (in_stack) to detect back edges. When we find an edge u→v where v is
// already on the current path, we have found a cycle segment.
//
// We then reconstruct a path from startxpoint to cycle_end (u) using
// the parent map, and finally add cycle_start (v) at the end so that
// the last coordinate is equal to an earlier one in the result vector.
std::vector<Coord> Datastructures::route_fibre_cycle(Coord startxpoint)
{
    std::vector<Coord> route;

    // If start is not in the fibre graph, nothing to do
    auto it = fibres_graph_.find(startxpoint);
    if (it == fibres_graph_.end()) {
        return route; // empty
    }

    std::unordered_map<Coord, Coord, CoordHash> parent;
    std::unordered_set<Coord, CoordHash> visited;
    std::unordered_set<Coord, CoordHash> in_stack;

    Coord cycle_start; // ancestor in the cycle
    Coord cycle_end;   // node that points back to ancestor
    bool found_cycle = false;

    std::function<void(Coord, Coord)> dfs = [&](Coord u, Coord p)
    {
        if (found_cycle) return;

        visited.insert(u);
        in_stack.insert(u);
        parent[u] = p;

        const auto& neighbours = fibres_graph_.at(u);

        for (const auto& edge : neighbours) {
            Coord v = edge.first;

            if (v == p) {
                // Skip the direct edge back to parent
                continue;
            }

            if (!visited.count(v)) {
                dfs(v, u);
                if (found_cycle) return;
            }
            else if (in_stack.count(v)) {
                // Found a back edge u -> v where v is on the current path
                cycle_start = v;
                cycle_end   = u;
                found_cycle = true;
                return;
            }
        }

        in_stack.erase(u);
    };

    dfs(startxpoint, startxpoint);

    if (!found_cycle) {
        return route; // empty
    }

    // Reconstruct path from startxpoint to cycle_end using parent[]
    std::vector<Coord> tmp;
    Coord cur = cycle_end;

    while (true) {
        tmp.push_back(cur);
        if (cur == startxpoint) {
            break;
        }
        cur = parent[cur];
    }

    std::reverse(tmp.begin(), tmp.end()); // [startxpoint, ..., cycle_start, ..., cycle_end]

    // Add the node we "return to" to close the loop
    tmp.push_back(cycle_start);           // cycle_start appears earlier in tmp

    route = std::move(tmp);
    return route;
}

// TEST SUBMISSION
