#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to run a test case
run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local timeout="$4"

    echo -e "\n${YELLOW}Running Test: $test_name${NC}"
    echo "Command: $command"
    echo "Expected: $expected"

    if [ -n "$timeout" ]; then
        timeout "$timeout" $command
    else
        $command
    fi

    if [ $? -eq 124 ]; then
        echo -e "${YELLOW}Test timed out (expected for indefinite runs)${NC}"
    fi

    echo -e "${GREEN}Test completed. Please verify the output matches the expected behavior.${NC}"
    read -p "Press Enter to continue to the next test..."
}

# Basic Functionality Tests
run_test "Normal Case" "./philo 5 800 200 200" "No philosopher should die, and the simulation should run indefinitely." "10s"
run_test "Death Case" "./philo 4 310 200 100" "A philosopher should die, and the simulation should stop."
run_test "One Philosopher" "./philo 1 800 200 200" "The philosopher should die after approximately 800ms."
run_test "Max Philosophers" "./philo 200 800 200 200" "No philosopher should die, testing the program's ability to handle many threads." "10s"

# Edge Cases
run_test "Instant Death" "./philo 5 0 200 200" "At least one philosopher should die immediately."
run_test "No Eating Time" "./philo 5 800 0 200" "The program should handle this gracefully, either by exiting or running normally."
run_test "No Sleeping Time" "./philo 5 800 200 0" "The program should handle this gracefully, philosophers should only eat and think."

# Optional Argument Tests
run_test "Limited Meals" "./philo 5 800 200 200 7" "The simulation should stop after each philosopher has eaten 7 times."
run_test "One Meal" "./philo 5 800 200 200 1" "The simulation should stop quickly after each philosopher has eaten once."

# Error Handling Tests
run_test "Too Few Arguments" "./philo 5 800 200" "The program should print an error message and exit."
run_test "Too Many Arguments" "./philo 5 800 200 200 7 100" "The program should print an error message and exit."
run_test "Invalid Number of Philosophers" "./philo 0 800 200 200" "The program should print an error message and exit."
run_test "Negative Time" "./philo 5 -800 200 200" "The program should print an error message and exit."
run_test "Non-numeric Input" "./philo 5 800 abc 200" "The program should print an error message and exit."

# Performance Tests
echo -e "\n${YELLOW}Running Long-running Simulation Test${NC}"
echo "Command: ./philo 5 800 200 200"
echo "Let it run for at least 5 minutes. Check for any memory leaks or unexpected behavior."
echo "Press Ctrl+C to stop the test when ready."
./philo 5 800 200 200

echo -e "\n${YELLOW}Running Resource Intensive Test${NC}"
echo "Command: ./philo 100 1000 100 100"
echo "Run this and monitor system resources to ensure your program doesn't consume excessive CPU or memory."
echo "Press Ctrl+C to stop the test when ready."
./philo 100 1000 100 100

echo -e "\n${GREEN}All tests completed.${NC}"
echo "Remember to check for:"
echo "- Correct output formatting"
echo "- No data races (use tools like Valgrind with --tool=helgrind)"
echo "- No memory leaks (use Valgrind)"
echo "- Proper handling of mutexes and threads"
echo "- Accurate timing (especially for death detection)"
echo -e "${RED}Always ensure your program doesn't have any undefined behaviors and handles all potential errors gracefully.${NC}"