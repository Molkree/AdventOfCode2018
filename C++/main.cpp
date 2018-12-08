#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <map>
#include <limits>
#include <chrono>
#include <set>
#include <unordered_set>

using namespace std;

string day1_file = "input.txt";
vector<int> read_frequency_changes(const string & filename)
{
    vector<int> list; list.reserve(999);
    ifstream infile(filename);
    int a;
    while (infile >> a)
    {
        list.emplace_back(a);
    }
    return list;
}

void day1_part2()
{
    vector<int> changes = read_frequency_changes(day1_file);
    int sum = 0;
    unordered_set<int> freqs{ 0 };
    int first_repeat;
    bool found = false;
    while (!found)
    {
        for (const int var : changes)
        {
            sum += var;
            if (!freqs.insert(sum).second)
            {
                first_repeat = sum;
                found = true;
                break;
            }
        }
    }
    cout << "Day 1 part 1 = " << first_repeat << endl;
}

string day2_file = "check_sum.txt";

vector<string> read_ids(const string & filename)
{
    vector<string> list; list.reserve(250);
    ifstream infile(filename);
    string line;
    while (getline(infile, line))
    {
        istringstream iss(line);
        list.emplace_back(line);
    }

    return list;
}

void day2_part1()
{
    auto ids = read_ids(day2_file);
    int two_repetitions = 0, three_repetitions = 0;
    for (auto id : ids)
    {
        unordered_map<char, int> chars;
        for (const char ch : id)
            ++chars[ch];
        bool two_repetitions_found = false, three_repetitions_found = false;
        for (auto it = chars.begin(); it != chars.end(); ++it)
        {
            if (!two_repetitions_found && it->second == 2)
            {
                two_repetitions_found = true;
                ++two_repetitions;
            }
            if (!three_repetitions_found && it->second == 3)
            {
                three_repetitions_found = true;
                ++three_repetitions;
            }
            if (two_repetitions_found && three_repetitions_found)
                break;
        }
    }

    cout << "Day 2 part 1 = " << (two_repetitions * three_repetitions) << endl;
}

void day2_part2()
{
    auto ids = read_ids(day2_file);
    for (int i = 0; i < ids.size(); ++i)
    {
        for (int j = i + 1; j < ids.size(); ++j)
        {
            int differences = 0;
            int difference_ind;
            for (int k = 0; k < ids[i].size(); ++k)
                if (ids[i][k] != ids[j][k])
                {
                    ++differences;
                    difference_ind = k;
                    if (differences > 1)
                        break;
                }
            if (differences == 1)
            {
                ids[i].replace(difference_ind, 1, "");
                cout << "Day 2 part 2 = " << ids[i] << endl;
                return;
            }
        }
    }
}

struct Claim
{
    int id, x, y, width, height;
};

vector<Claim> read_claims(const string & filename)
{
    ifstream infile(filename);
    string line;
    vector<Claim> result;
    while (getline(infile, line))
    {
        stringstream ss(line);
        char hash, at, comma, col, by;
        Claim c;
        ss >> hash >> c.id >> at >> c.x >> comma >> c.y >> col >> c.width >> by >> c.height;
        result.push_back(c);
    }
    return result;
}

string day3_file = "day3_input.txt";
void day3_part1()
{
    vector<Claim> claims = read_claims(day3_file);
    map<pair<int, int>, int> claimed_area;
    for (const Claim & c : claims)
    {
        for (int x = c.x + 1; x < c.x + 1 + c.width; ++x)
            for (int y = c.y + 1; y < c.y + 1 + c.height; ++y)
                ++claimed_area[{x, y}];
    }
    cout << "Day 3 part 1 = " << count_if(claimed_area.cbegin(), claimed_area.cend(), [](const pair<pair<int, int>, int> & p) { return p.second > 1; }) << endl;
}

void day3_part2()
{
    vector<Claim> claims = read_claims(day3_file);
    map<pair<int, int>, int> claimed_area;
    for (const Claim & c : claims)
    {
        for (int x = c.x + 1; x < c.x + 1 + c.width; ++x)
            for (int y = c.y + 1; y < c.y + 1 + c.height; ++y)
                ++claimed_area[{x, y}];
    }
    for (const Claim & c : claims)
    {
        bool no_overlap = true;
        for (int x = c.x + 1; x < c.x + 1 + c.width; ++x)
        {
            for (int y = c.y + 1; y < c.y + 1 + c.height; ++y)
                if (claimed_area[{x, y}] > 1)
                {
                    no_overlap = false;
                    break;
                }
            if (!no_overlap)
                break;
        }
        if (no_overlap)
        {
            cout << "Day 3 part 2 = " << c.id << endl;
            return;
        }
    }

}

string day4_file = "day4_input.txt";
void day4()
{
    ifstream infile(day4_file);
    string line;
    vector<string> lines;
    while (getline(infile, line))
    {
        istringstream iss(line);
        lines.push_back(line);
    }
    sort(lines.begin(), lines.end());

    unordered_map<int, unordered_map<int, int>> guards;
    unordered_map<int, int> minutes_asleep;
    int id;
    for (int i = 0; i < lines.size(); ++i)
    {
        const size_t id_ind = lines[i].find('#');
        if (id_ind != string::npos)
        {
            string s_id = lines[i].substr(id_ind + 1);
            stringstream ss(s_id);
            ss >> id;
            guards.insert({ id, unordered_map<int, int>() });
        }
        else
        {
            const size_t start_ind = lines[i].find(':');
            string s_start_min = lines[i].substr(start_ind + 1);
            stringstream ss(s_start_min);
            int start_min;
            ss >> start_min;

            const size_t end_ind = lines[i + 1].find(':');
            const string s_end_min = lines[i + 1].substr(end_ind + 1);
            ss.str(s_end_min);
            int end_min;
            ss >> end_min;

            for (int j = start_min; j < end_min; ++j)
                ++guards[id][j];
            minutes_asleep[id] += (end_min - start_min);
            ++i;
        }
    }

    int max_mins = -1;
    int worst_guard;
    for (auto guard : minutes_asleep)
        if (guard.second > max_mins)
        {
            max_mins = guard.second;
            worst_guard = guard.first;
        }

    int best_minute;
    int max_days = 0;
    for (auto minute : guards[worst_guard])
        if (minute.second > max_days)
        {
            max_days = minute.second;
            best_minute = minute.first;
        }

    cout << "Day 4 part 1 = " << worst_guard * best_minute << endl;

    max_days = 0;
    best_minute = 0;
    worst_guard = 0;
    for (auto guard : guards)
        for (auto minute : guard.second)
            if (minute.second > max_days)
            {
                max_days = minute.second;
                best_minute = minute.first;
                worst_guard = guard.first;
            }

    cout << "Day 4 part 2 = " << worst_guard * best_minute << endl;
}

int react(const string & s)
{
    vector<char> v;
    for (const char & c : s)
    {
        if (v.empty())
        {
            v.push_back(c);
        }
        else
        {
            const char last = v.back();
            if (abs(last - c) == abs('A' - 'a'))
            {
                v.pop_back();
            }
            else
            {
                v.push_back(c);
            }
        }
    }
    return v.size();
}

string day5_file = "day5_input.txt";
void day5()
{
    ifstream infile(day5_file);
    string line;
    getline(infile, line);

    const int length = react(line);
    cout << "Day 5 part 1 = " << length << endl;

    int min_length = numeric_limits<int>::max();
    for (char c = 'a'; c <= 'z'; ++c)
    {
        string new_try = "";
        for (const char & t : line)
        {
            if (!(t == c || t == c - 32))
            {
                new_try += t;
            }
        }
        min_length = min(min_length, react(new_try));
    }

    cout << "Day 5 part 2 = " << min_length << endl;
}

string day6_file = "day6_input.txt";
void day6()
{
    vector<pair<int, int>> coordinates;
    ifstream infile(day6_file);
    string line;
    int x_min = numeric_limits<int>::max(), y_min = x_min,
        x_max = numeric_limits<int>::min(), y_max = x_max;
    while (getline(infile, line))
    {
        istringstream iss(line);
        int x, y;
        char comma;
        iss >> x >> comma >> y;
        coordinates.push_back({ x, y });
        if (x < x_min)
            x_min = x;
        else if (x > x_max)
            x_max = x;
        if (y < y_min)
            y_min = y;
        else if (y > y_max)
            y_max = y;
    }

    map<pair<int, int>, int> inner_coordinates;
    for (auto coord : coordinates)
        if (x_min < coord.first && coord.first < x_max && y_min < coord.second && coord.second < y_max)
            inner_coordinates[coord] = 0;

    for (int x = x_min + 1; x < x_max; ++x)
        for (int y = y_min + 1; y < y_max; ++y)
        {
            int min_distance = numeric_limits<int>::max(), second_min = min_distance;
            pair<int, int> point;
            for (auto coord : inner_coordinates)
            {
                const int distance = abs(x - coord.first.first) + abs(y - coord.first.second);
                if (distance < min_distance)
                {
                    second_min = min_distance;
                    min_distance = distance;
                    point = coord.first;
                    if (min_distance == 0)
                        break;
                }
                else if (distance == min_distance)
                    second_min = min_distance;
            }
            if (min_distance != second_min)
                ++inner_coordinates[point];
        }

    int max_area = 0;
    for (auto coord : inner_coordinates)
        if (coord.second > max_area)
            max_area = coord.second;

    cout << "Day 6 part 1 = " << max_area << endl;

    int safe_area = 0;
    const int max_border = ceil(10000.0 / coordinates.size()) - 1;
    for (int x = x_min - max_border; x < x_max + max_border; ++x)
        for (int y = y_min - max_border; y < y_max + max_border; ++y)
        {
            int distance = 0;
            for (auto coord : coordinates)
                distance += abs(x - coord.first) + abs(y - coord.second);
            if (distance < 10000)
                ++safe_area;
        }

    cout << "Day 6 part 2 = " << safe_area << endl;
}

string day7_file = "day7_input.txt";
void day7()
{
    unordered_map<char, bool> is_root;
    unordered_map<char, set<char>> graph;
    unordered_map<char, set<char>> parents;

    ifstream infile(day7_file);
    string line;
    while (getline(infile, line))
    {
        const char parent = line.at(5), child = line.at(36);
        if (is_root.find(parent) == is_root.end())
            is_root[parent] = true;
        is_root[child] = false;
        parents[child].emplace(parent);
        graph[parent].insert(child);
    }

    set<char> nodes;
    for (auto node : is_root)
        if (node.second)
        {
            nodes.emplace(node.first);
            parents[node.first] = set<char>();
        }
    vector<char> output;
    unordered_set<char> used;
    while (!nodes.empty())
    {
        auto next_node = nodes.cbegin();
        while (!parents[*next_node].empty())
        {
            ++next_node;
        }
        used.emplace(*next_node);
        output.emplace_back(*next_node);
        for (auto it = graph[*next_node].cbegin(); it != graph[*next_node].cend(); ++it)
        {
            if (used.find(*it) == used.cend())
                nodes.emplace(*it);
            parents[*it].erase(parents[*it].find(*next_node));
        }
        nodes.erase(next_node);
    }

    cout << "Day 7 part 1 = ";
    for (const char node : output)
        cout << node;
    cout << endl;
}

int main()
{
    auto start = chrono::steady_clock::now();
    //day1_part2();
    //day2_part1();
    //day2_part2();
    //day3_part1();
    //day3_part2();
    //day4();
    //day5();
    //day6();
    day7();
    auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start);
    cout << "Elapsed time: " << duration.count() / 1000.0 << "s\n";
}