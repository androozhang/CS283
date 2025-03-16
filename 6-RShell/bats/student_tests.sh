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
    expected_output="helloworldlocalmodedsh4>dsh4>cmdloopreturned0"

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
    expected_output="dshlib.clocalmodedsh4>dsh4>cmdloopreturned0"

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
    expected_output="error:failtoexecuteexternalcommands:Nosuchfileordirectorylocalmodedsh4>dsh4>cmdloopreturned0"

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
    expected_output="testcontentlocalmodedsh4>dsh4>cmdloopreturned0"

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
    expected_output="localmodedsh4>warning:nocommandsprovideddsh4>cmdloopreturned0"

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
    expected_output="localmodedsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}



@test "Pipe with Redirected Input Remotely" {
    echo "test content" > test_input.txt

    run "./dsh" -c <<EOF
cat test_input.txt | grep test
EOF

    rm test_input.txt

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Commandcompletedwithcode:1dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Empty Pipe Command Remotely" {
    run "./dsh" -c <<EOF
 | 
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Errorparsingcommanddsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Exit Command works remotely" {
    run "./dsh" -c <<EOF
exit 
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Closingconnectiondsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

# This passes if server with 0.0.0.0 : 7890 is opened
@test "Starting client with custom address and port works" {
    run "./dsh" -c -i 0.0.0.0 -p 7890 <<EOF
exit 
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="localmodedsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "Pipe with Built-in cd Command works Remotely" {
    run "./dsh" -c <<EOF
cd .. | ls
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ]
}

@test "Pipe with Quoted Arguments works Remotely" {
    run "./dsh" -c <<EOF
echo "hello world" | grep "hello"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>helloworldCommandcompletedwithcode:0dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Pipe with Multiple Whitespaces works Remotely" {
    run "./dsh" -c <<EOF
   ls    |    grep    dshlib.c   
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Commandcompletedwithcode:1dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Error Handling: Non-existent Command works Remotely" {
    run "./dsh" -c <<EOF
nonexistentcommand | grep something
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Commandcompletedwithcode:1dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "A lot of pipe commands works Remotely" {
    run "./dsh" -c <<EOF
ls | grep c | wc -l | sort | uniq | cat'
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ]
}