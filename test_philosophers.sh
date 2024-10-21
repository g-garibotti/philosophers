#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Function to run a test case
run_test() {
    local test_name=$1
    local args=$2
    local expected_output=$3
    local timeout=$4

    echo "Running test: $test_name"
    output=$(timeout $timeout ./philo $args 2>&1)
    exit_code=$?

    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}Test timed out${NC}"
    elif [ $exit_code -ne 0 ]; then
        echo -e "${RED}Test failed with exit code $exit_code${NC}"
    elif echo "$output" | grep -q "$expected_output"; then
        echo -e "${GREEN}Test passed${NC}"
    else
        echo -e "${RED}Test failed. Expected '$expected_output', but got:${NC}"
        echo "$output"
    fi
    echo
}

# Compile the program
make

# Test cases
run_test "Basic test" "5 800 200 200" "is eating" 5
run_test "One philosopher" "1 800 200 200" "has taken a fork" 5
run_test "No eating" "5 800 200 200 0" "is thinking" 5
run_test "Death case" "4 310 200 100" "died" 5
run_test "Invalid arguments" "4 -310 200 100" "Error: Invalid arguments" 5
run_test "Too few arguments" "4 310 200" "Error: Invalid arguments" 5
run_test "Too many arguments" "4 310 200 100 5 extra" "Error: Invalid arguments" 5

# Clean up
make fclean