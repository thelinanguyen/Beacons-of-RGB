// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <source_location>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>


// Type for beacon IDs
using BeaconID = std::string;
using Name = std::string;

// Return value for cases where required beacon was not found
BeaconID const NO_BEACON= "--NO_BEACON--";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "-- NO_NAME --";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for color (RGB)
struct Color
{
    int r = NO_VALUE;
    int g = NO_VALUE;
    int b = NO_VALUE;
};

// Equality and non-equality comparisons for Colors
inline bool operator==(Color c1, Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }
inline bool operator!=(Color c1, Color c2) { return !(c1==c2); }

// Return value for cases where color was not found
Color const NO_COLOR = {NO_VALUE, NO_VALUE, NO_VALUE};

// Type for light transmission cost (used only in the second assignment)
using Cost = int;

// Return value for cases where cost is unknown
Cost const NO_COST = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    explicit NotImplemented(std::string const msg = "",
                            const std::source_location location = std::source_location::current())
        : msg_{}
    {
        std::string funcname = location.function_name();
        if (auto namestart = funcname.find_last_of(':'); namestart != std::string::npos)
        { funcname.erase(0, namestart+1); }
        if (auto nameend = funcname.find_first_of('('); nameend != std::string::npos)
        { funcname.erase(nameend, std::string::npos); }
        msg_ = (!msg.empty() ? msg+" in " : "")+funcname+"()";
    }
    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // A operations

    // Estimate of performance:
    //   O(log N) average, where N is the number of beacons.
    // Short rationale for estimate:
    //   Unordered lookup in beacons_ is O(1) avg; inserting into two
    //   std::map<…> + std::set<…> indices (by name and brightness) is
    //   O(log N), which dominates.
    bool add_beacon(BeaconID id, Name const& name, Coord xy, Color color);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Returns size() of an std::unordered_map, which is constant time.
    int beacon_count();

    // Estimate of performance: O(N), where N is the number of beacons.
    // Short rationale for estimate: Calls clear() on the core map and both index maps; clearing a
    //   container destroys all its elements linearly.
    void clear_beacons();

    // Estimate of performance:O(N ), where N is the number of beacons.
    // Short rationale for estimate: Iterates through all entries in beacons_ once and pushes each ID
    //   into the result vector.
    std::vector<BeaconID> all_beacons();

    // Estimate of performance: O(1) average
    // Short rationale for estimate: Uses unordered_map for lookup by ID.
    Name get_name(BeaconID id);

    // Estimate of performance: O(1) average
    // Short rationale for estimate: Uses unordered_map for lookup by ID.
    Coord get_coordinates(BeaconID id);

    // Estimate of performance:O(1) average
    // Short rationale for estimate: Uses unordered_map for lookup by ID.
    Color get_color(BeaconID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(N), where N is the number of beacons.
    // Short rationale for estimate: Iterates through beacons_by_name_ (std::map) in key order and
    //   through each std::set<BeaconID>; total number of pushed IDs is N.
    std::vector<BeaconID> beacons_alphabetically();

    // Estimate of performance: O(N), where N is the number of beacons.
    // Short rationale for estimate: Iterates through beacons_by_brightness_ (std::map) in increasing
    //   key order and through each std::set<BeaconID>; total IDs is N.
    std::vector<BeaconID> beacons_brightness_increasing();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Uses std::map::begin() on beacons_by_brightness_ and takes the
    //   first ID from the associated std::set, both constant time.
    BeaconID min_brightness();

    // Estimate of performance: O(1)
    // Short rationale for estimate:  Uses std::map::rbegin() on beacons_by_brightness_ and takes the
    //   first ID from the associated std::set, both constant time.
    BeaconID max_brightness();

    // Estimate of performance: O(log N + K), where N is the number of distinct names and
    //   K is the number of beacons with the given name.
    // Short rationale for estimate: One std::map::find by name is O(log N); then we iterate through
    //   the std::set<BeaconID> for that name and copy K elements.
    std::vector<BeaconID> find_beacons(Name const& name);

    // Estimate of performance: O(log N) average.
    // Short rationale for estimate:  Unordered lookup in beacons_ is O(1) avg; updating beacons_by_name_
    //   requires std::map::find/erase/insert and std::set erase/insert,
    //   both logarithmic in the number of beacons.
    bool change_beacon_name(BeaconID id, Name const& newname);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1) average
    // Short rationale for estimate: Two unordered_map lookups in beacons_; then constant-time updates
    //   of one out_edge and a push_back into an in_edges vector.
    bool add_lightbeam(BeaconID sourceid, BeaconID targetid);

    // Estimate of performance: O(K log K), where K is the number of direct light sources.
    // Short rationale for estimate: Lookup in beacons_ is O(1) avg; copying the in_edges vector is
    //  O(K), then sorting the K IDs is O(K log K).
    std::vector<BeaconID> get_lightsources(BeaconID id);

    // Estimate of performance: O(L), where L is the length of the out_edge chain from id.
    // Short rationale for estimate:  Follows at most one outgoing edge per beacon until NO_BEACON or
    //   a cycle, visiting each beacon in the chain once.
    std::vector<BeaconID> path_outbeam(BeaconID id);

    // B operations

    // Estimate of performance: O(K), where K is the number of beacons reachable via incoming beams.
    // Short rationale for estimate: A recursive helper walks the in_edges graph in a tree-like
    //   structure, visiting each upstream beacon at most once; path
    //   reconstruction is linear in the found path length.
    std::vector<BeaconID> path_inbeam_longest(BeaconID id);

    // Estimate of performance: O(K), where K is the number of upstream beacons contributing light.
    // Short rationale for estimate: A DFS over in_edges visits each contributing beacon once; each
    //   node processes its list of incoming sources once, with O(1) work
    //   per neighbour on average.
    Color total_color(BeaconID id);

    // Estimate of performance: O(D), where D is the degree of xpoint1 in the fibre graph.
    // Short rationale for estimate: Unordered_map lookup is O(1) avg; we linearly scan xpoint1’s
    //   adjacency list to check for an existing fibre, then do two
    //   push_back operations.
    bool add_fibre(Coord xpoint1, Coord xpoint2, Cost cost);

    // Estimate of performance: O(V log V), where V is the number of xpoints.
    // Short rationale for estimate: We collect all keys from the unordered_map once (O(V)), then
    //   sort the resulting vector of coordinates (O(V log V)).
    std::vector<Coord> all_xpoints();

    // Estimate of performance: O(D log D), where D is the degree of the given xpoint.
    // Short rationale for estimate: Lookup in the adjacency map is O(1) avg; copying the neighbours
    //   is O(D), sorting them by Coord is O(D log D).
    std::vector<std::pair<Coord, Cost>> get_fibres_from(Coord xpoint);

    // Estimate of performance: O(E log E), where E is the number of unique fibres.
    // Short rationale for estimate: We iterate over all adjacency lists once, pushing each undirected
    //   fibre only once into a vector (O(E)), then sort that vector by
    //   endpoint coordinates (O(E log E)).
    std::vector<std::pair<Coord, Coord>> all_fibres();

    // Estimate of performance: O(D1 + D2), where D1 and D2 are the degrees of xpoint1 and xpoint2.
    // Short rationale for estimate: For each endpoint, we look up its adjacency list in O(1) avg and
    //   run std::remove_if over the neighbours (linear in that list size).
    bool remove_fibre(Coord xpoint1, Coord xpoint2);

    // Estimate of performance: O(V + E), where V is the number of xpoints and E is the number of fibres.
    // Short rationale for estimate: Clearing the adjacency map destroys all stored adjacency lists,
    //   which is linear in the number of stored nodes and edges.
    void clear_fibres();

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(V + E) in the reachable subgraph.
    // Short rationale for estimate: Breadth-first search on the fibre graph visits each reachable
    //   xpoint and fibre at most once; path reconstruction from parents
    //   is linear in path length.
    std::vector<std::pair<Coord, Cost>> route_any(Coord fromxpoint, Coord toxpoint);

    // C operations

    // Estimate of performance: O(V + E) in the reachable subgraph.
    // Short rationale for estimate: Implemented by reusing route_any (BFS), which already runs in
    //   linear time in the size of the reachable fibre network.
    std::vector<std::pair<Coord, Cost>> route_least_xpoints(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance: O((V + E) log V) in the reachable subgraph.
    // Short rationale for estimate: Uses Dijkstra’s algorithm with a binary heap priority_queue:
    //   each edge relaxation is a heap push/pop, giving O((V+E) log V).
    std::vector<std::pair<Coord, Cost>> route_fastest(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance: O(V + E) in the reachable subgraph.
    // Short rationale for estimate: Depth-first search from the start xpoint explores each reachable
    //   node and fibre at most once; detecting a back edge and
    //   reconstructing one cycle path are both linear in path length.
    std::vector<Coord> route_fibre_cycle(Coord startxpoint);

private:
    // Explain below your rationale for choosing the data structures you use in this class.
    // Add stuff needed for your class implementation below

    // Rationale for data structures:
    // 1. beacons_: Hash map for O(1) average lookup by ID. Core storage.
    // 2. beacons_by_name_: std::map for sorted names (required for sorting) and fast prefix lookups (O(log N)).
    // 3. beacons_by_brightness_: std::map for sorted brightness (required for min/max and iteration by brightness).
    // 4. fibres_graph_: Hash map (Coord -> vector<pair<Coord, Cost>>) for O(1) average lookup of an xpoint
    //    and its neighbours. Ideal for graph algorithms like BFS and Dijkstra. CoordHash is provided.

    // A struct to store each beacon's information: name, coordinate, color, brightness
    // and light sources (beam(s) coming in and a beam coming out)
    struct BeaconInfo {
        Name name;
        Coord xy;
        Color color;
        int brightness;
        std::vector<BeaconID> in_edges;
        BeaconID out_edge;
    };

    // Core storage for all beacons. Efficiency: O(1) average lookup
    // ID -> Info
    std::unordered_map<BeaconID, BeaconInfo> beacons_;

    // Using map to keeps names sorted, find beacon by name and
    // using set to prevents duplicates and is sorted (by ID)
    // Efficiency: Iteration is O(n), Min/Max is O(1)
    // Name ->Set of IDs with that name
    std::map<Name, std::set<BeaconID>> beacons_by_name_;

    // Using map to keep brightness sorted, find min and max brightness, and similarly for set
    // Efficiency: Iteration is O(n), Min/Max is O(1)
    // Brightness -> Set of IDs with that brightness
    std::map<int, std::set<BeaconID>> beacons_by_brightness_;

    // FOR RECURSIVE HELPER FUNCTION
    std::pair<int, BeaconID> find_deepest_start(
        BeaconID curr_id,
        std::unordered_set<BeaconID>& visiting);

    // Data Structure: Adjacency List using Hash Map of Vectors.
    // Key: Start Coord -> Value: Vector of <Destination_Coord, Cost>
    std::unordered_map<Coord, std::vector<std::pair<Coord, Cost>>, CoordHash> fibres_graph_;



};

#endif // DATASTRUCTURES_HH
