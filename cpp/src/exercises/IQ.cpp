/* Write a function that takes an array of integers and another integer and will 
return true if the integer is present in the array. Should be O(1) space with 
minimum compare operations.  */

#include <vector>

bool isPresent(std::vector<int>& integers, int num) {

    int size = integers.size();

    if (size == 0) {
        return false;
    }

    int last_elem = integers[size - 1];

    if (num == last_elem) {
        return true;
    }

    integers[size - 1] = num;

    int i = 0;
    while (integers[i++] != num);

    integers[size - 1] = last_elem;

    return i != size;
}


/* Write a recursive function that will return true if a given int has all of its decimal 
digits ascending. check(1279) will return true. check(3871) will return false */
bool checkHelper(int num, int prev) {
    if (num == 0) {
        return true;
    }

    int digit = num % 10;
    if (digit > prev) {
        return false;
    }

    return checkHelper(num / 10, digit);
}

bool check(int num) {
    return checkHelper(num / 10, num % 10);
}


/* Find duplicate chars in a string */
bool containsDuplicateChar(const std::string& s) {

    if (s.size() < 2) {
        return false;
    }

    std::array<bool, 128> is_present{};

    for (auto const& char : s) {
        if (is_present[char]) {
            return true;
        }
        is_present[char] = true;
    }

    return false;
}

/* Find max subarray with greatest sum*/
int maxSubarraySum(const std::vector<int>& nums) {

    size_t size = nums.size();

    if (size == 0) {
        return 0;
    }

    std::vector<int> dp(size);
    
    dp[0] = nums[0];
    int ret_max = dp[0];

    for (int i = 1; i < size; ++i) {
        int curr_num = nums[i];
        dp[i] = std::max((dp[i - 1] + curr_num), curr_num);
        ret_max = std::max(ret_max, dp[i]);
    }

    return ret_max;
}


/* Balanced parentheses */
bool isBalanced(const std::string& str) {

    std::stack<char> stack{};

    for (auto const& c : str) {
        switch (c)
        {
        case '(':
        case '[':
        case '{':
            stack.push(c);
            break;
        case ')':
            if (stack.empty() || stack.top() != '(') {
                return false;
            }
            stack.pop();
            break;
        case ']':
            if (stack.empty() || stack.top() != '[') {
                return false;
            }
            stack.pop();
            break;
        case '}':
            if (stack.empty() || stack.top() != '{') {
                return false;
            }
            stack.pop();
            break;
        default:
            break;
        }
    }

    if (stack.empty()) {
        return true;
    }

    return false;
}


/* Longest subarray with no dups */
int longestUniqueSubarrayLength(const std::vector<int>& nums) {

    std::unordered_set<int> window{};
    int max_len = 0;
    int left = 0;
    int right = 0;

    while (right < nums.size()) {

        if (!window.contains(nums[right])) {
            window.insert(nums[right]);
            max_len = std::max(max_len, window.size());
            ++right;
        } else {
            window.erase(nums[left]);
            ++left;
        }
    }
    return max_len;
}


/* Number of islands in 0/1 grid nxm */
void dfs(std::vector<std::vector<int>>& grid, int i, int j) {

    if (i < 0 || j < 0 || i > grid.size() || j > grid[0].size() || grid[i][j] != 1) {
        return;
    }

    grid[i][j] = 0;

    dfs(grid, i + 1, j);
    dfs(grid, i - 1, j);
    dfs(grid, i, j - 1);
    dfs(grid, i, j + 1);
}

int numIslands(std::vector<std::vector<int>>& grid) {

    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    int islands = 0;
    
    int n = grid.size();
    int m = grid[0].size();

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            if (grid[i][j] == 1) {
                dfs(grid, i, j);
                ++islands;
            }   
        }
    }

    return islands;
}

/* Closest K elements to 'x', sorted input */
int indexOfClosest(const std::vector<int>& sorted, int x) {

    int left = 0;
    int right = sorted.size() - 1;
    int mid = left + (right - left)/2;

    while (left < right) {

        mid = left + (right - left)/2;

        int curr = sorted[mid];
        if (curr == x) {
            break;
        } else if (curr > x) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    if (left > 0 &&
        std::abs(sorted[left - 1] - x) <= std::abs(sorted[left] - x)) {
        return left - 1;
    }

    return left;
}

void fillTail(const std::vector<int>& sorted, std::vector<int>& result, int left, int right) {

    while (left >= 0) {
        result.push_back(sorted[left--]);
    }
    while (right < sorted.size()) {
        result.push_back(sorted[right++]);
    }
}

std::vector<int> closestKSorted(const std::vector<int>& sorted, int x, int k) {

    if (k < 1 || sorted.empty()) {
        return {};
    }

    size_t size = sorted.size();
    std::vector<int> result{};


    int closest_index = indexOfClosest(sorted, x);
    result.push_back(sorted[closest_index]);
    
    int left = closest_index - 1;
    int right = closest_index + 1;

    while (left >= 0 && right < size && result.size() < k) {
        if (std::abs(sorted[left] - x) < std::abs(sorted[right] - x)) {
            result.push_back(sorted[left--]);
        } else {
            result.push_back(sorted[right++]);
        }
    }

    if (result.size() < k) {
        fillTail(sorted, result, left, right);
    }

    return result;
}


/* Some template examples */

/* Filter the input vector to return a new one */
template<typename T, typename Predicate>
std::vector<T> filterVector(const std::vector<T>& input, Predicate pred) {

    std::vector<T> result{};

    for (T const& element : input) {
        if (pred(element)) {
            result.emplace_back(element);
        }
    }

    return result;
}

/* Specialization example */
template<typename T>
struct TypeTraits {
    static void describe() {
        std::cout << "Unknown" << std::endl;
    }
};

template<>
struct TypeTraits<int> {
    static void describe() {
        std::cout << "Integer" << std::endl;
    }
};

template<>
struct TypeTraits<double> {
    static void describe() {
        std::cout << "Double" << std::endl;
    }
};

template<>
struct TypeTraits<std::string> {
    static void describe() {
        std::cout << "String" << std::endl;
    }
};




// Write an efficient function that will take two strings and replace each occurrence
// of a char from the second string in the first string with *.
// char s[] = “hello world”;
// replace(s, “eod”);
// now s contains “h*ll* w*rl*” 


void ReplaceOccurrence(std::string& first, std::string const& second) {

    std::array<bool, 256> appear{};

    for (auto const& c : second) {
        appear[static_cast<unsigned char>(c)] = true;
    }

    for (auto& c : first) {
        if (appear[static_cast<unsigned char>(c)]) {
            c = '*';
        }
    }
}



// Write an efficient function to calculate the nth Fibonacci number

long long fib(long long n) {

    static std::vector<long long> fibs{1, 1};

    if (n <= fibs.size()) {
        return fibs[n - 1];
    }


    for (long long i = fibs.size(); i < n; ++i) {
        fibs.emplace_back(fibs[i - 1] + fibs[i - 2]);
    }

    return fibs[n - 1];
}


/* Write a template function that will do a binary search on a vector<T> of elements
where T implements operator<. */
template<typename T>
int BinarySearch(std::vector<T> const& elements, T const& x) {

    int low = 0;
    int high = elements.size() - 1;
    int mid = low + (high - low)/2;

    while (low <= high) {

        mid = low + (high - low)/2;

        T const& curr = elements[mid];

        if (!(x < curr) && !(curr < x)) {
            return mid;
        } else if (curr < x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}


/* Write a template function that will find the first duplicate element in a given
std::vector<T> */
template<typename T>
std::optional<std::pair<size_t, size_t>> FirstDup(std::vector<T> elements) {

    std::unordered_map<T, size_t> seen{};
    size_t length = elements.size();

    for (size_t i = 0; i < length; ++i) {

        T const& curr = elements[i];
        auto it = seen.find(curr);
        if (it != seen.end()) {
            return std::make_pair(it->second, i);
        }

        seen[curr] = i;
    }
    return nullopt;
}


/* Given a vector of T element write a function to shuffle the element. */
template<typenme T>
void Shuffel(std::vector<T>& elements) {

    size_t size = elements.size();

    int random = rand() % (size - 1);

    while (size - 1 > 0) {

        random = rand() % (size - 1);

        T temp = elements[size - 1];
        elements[size - 1] = elements[random];
        elements[random] = temp;

        --size;
    }
}



/* 31. Write a function that a binary search tree returns true if the tree is a perfect tree.  */


typedef struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;


bool IsPerfectTree(TreeNode const& root) {

    std::queue<TreeNode*> queue{};

    queue.push(root);

    while (!queue.empty()) {

        TreeNode const& curr = queue.front();

        queue.pop();

        if (!curr->left && !curr->right) {
            return queue.size() == ;
        }

    }

    return false;
}


void QuickSortRec(std::vector<int>& vec, int low, int high) {

    if (low >= high) {
        return;
    }

    int pivot = vec[high];

    int i = low - 1;

    for(int j = low; j < high; ++j) {
        if (vec[j] < pivot) {
            ++i;
            std::swap(vec[i], vec[j]);
        }
    }

    std::swap(vec[high], vec[++i]);
    
    QuickSortRec(vec, low, i - 1);
    QuickSortRec(vec, i + 1, high);
}

void QuickSort(std::vector<int>& vec) {
    if (!vec.empty()) {
        QuickSortRec(vec, 0, vec.size() - 1);
    }
}


/* SetAll O(1) */
class SetAllArray {
public:
    SetAllArray();

    void set(int index, int value);
    void setAll(int value);
    int get(int index) const;

private:
    std::unordered_map<int, std::pair<int, int>> _values;
    int _global_val;
    int _timestamp;
};

SetAllArray::SetAllArray()
: _values{}
, _global_val{0}
, _timestamp{0}
{}

void SetAllArray::set(int index, int value) {

    auto it = _values.find(index);
    if (it != _values.end()) {
        it->second.first = value;
        it->second.second = _timestamp;
    } else {
        _values[index] = std::make_pair(value, _timestamp);
    }
}

void SetAllArray::setAll(int value) {
    _global_val = value;
    ++_timestamp;
}

int SetAllArray::get(int index) const {
    auto it = _values.find(index);
    if (it != _values.end()) {

        if (it->second->second == _timestamp) {
            return it->second.first;
        } else {
            return _global_val;
        }

    } else {
        return -1;
    }
}


// All i,j s.t i < j and nums[i] > nums[j]


int numOfCouples(std::vector<int> const& nums) {

    while (i < vec1.size() && j < vec2.size()){ 

    }

    while (j < vec2.size()) {
        
    }

}