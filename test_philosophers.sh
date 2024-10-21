#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Compile the project
compile_project() {
    echo -e "${YELLOW}Compiling the project...${NC}"
    make re
    if [ $? -ne 0 ]; then
        echo -e "${RED}Compilation failed!${NC}"
        exit 1
    fi
    echo -e "${GREEN}Compilation successful!${NC}"
}

# Run a test case
run_test() {
    local test_name=$1
    local args=$2
    local expected_output=$3
    local max_runtime=$4

    echo -e "\n${YELLOW}Running test: $test_name${NC}"
    echo "Command: ./philo $args"
    
    timeout $max_runtime ./philo $args > test_output.txt 2>&1
    
    if [ $? -eq 124 ]; then
        if [[ $expected_output == "TIMEOUT" ]]; then
            echo -e "${GREEN}Test passed! (Expected timeout occurred)${NC}"
            return 0
        else
            echo -e "${RED}Test timed out after $max_runtime seconds${NC}"
            return 1
        fi
    elif grep -q "$expected_output" test_output.txt; then
        echo -e "${GREEN}Test passed!${NC}"
        return 0
    else
        echo -e "${RED}Test failed!${NC}"
        echo "Expected output not found:"
        echo "$expected_output"
        echo "Actual output:"
        cat test_output.txt
        return 1
    fi
}

# Main test runner
run_tests() {
    compile_project

    local total_tests=0
    local passed_tests=0

    # Test cases
    run_test "Basic test" "4 310 200 100" "is eating" 5
    ((total_tests++))
    [ $? -eq 0 ] && ((passed_tests++))

    run_test "One philosopher" "1 800 200 200" "died" 2
    ((total_tests++))
    [ $? -eq 0 ] && ((passed_tests++))

    run_test "No eating" "5 800 200 200 7" "died" 10
    ((total_tests++))
    [ $? -eq 0 ] && ((passed_tests++))

    run_test "Long times" "4 2000 1000 1000" "TIMEOUT" 5
    ((total_tests++))
    [ $? -eq 0 ] && ((passed_tests++))

    run_test "Invalid input" "4 -5 200 200" "Error" 2
    ((total_tests++))
    [ $? -eq 0 ] && ((passed_tests++))

    run_test "Too many philosophers" "201 800 200 200" "Error" 2
    ((total_tests++))
    [ $? -eq 0 ] && ((passed_tests++))

    run_test "Not enough arguments" "4 310 200" "Error" 2
    ((total_tests++))
    [ $? -eq 0 ] && ((passed_tests++))

    echo -e "\n${YELLOW}Test Results: $passed_tests/$total_tests tests passed${NC}"
}

# Run the tests
run_tests

echo -e "\n${YELLOW}All tests completed!${NC}"