#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipe with Built-in cd Command" {
    run "./dsh" <<EOF
cd .. | ls
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ]
}

@test "Pipe with Quoted Arguments" {
    run "./dsh" <<EOF
echo "hello world" | grep "hello"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="helloworlddsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Pipe with Multiple Whitespaces" {
    run "./dsh" <<EOF
   ls    |    grep    dshlib.c   
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dshlib.cdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Error Handling: Non-existent Command" {
    run "./dsh" <<EOF
nonexistentcommand | grep something
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="error:failtoexecuteexternalcommands:Nosuchfileordirectorydsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "A lot of pipe commands" {
    run "./dsh" <<EOF
ls | grep c | wc -l | sort | uniq | cat'
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ]
}

@test "Pipe with Redirected Input" {
    echo "test content" > test_input.txt

    run "./dsh" <<EOF
cat test_input.txt | grep test
EOF

    rm test_input.txt

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="testcontentdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Empty Pipe Command" {
    run "./dsh" <<EOF
 | 
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>warning:nocommandsprovideddsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Exit Command in Piped Commands" {
    run "./dsh" <<EOF
exit | ls
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}