#!/bin/sh

#=============== Define color ===============
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

#=============== Local function ===============
println()
{
    echo "$1$2${NC}"
}

#=============== Main Script ===============
PROJECT_DIR=~/eCallgRPC
OUTPUT_DIR="$PROJECT_DIR/output"
N_CPU=$(nproc)

println $BLUE "========== Starting full build and CAN setup =========="

# Step 1: Check if project folder exists
if [ ! -d "$PROJECT_DIR" ]; then
    println $RED "Project folder not found: $PROJECT_DIR"
    exit 1
fi

# Step 2: Run `make` to generate output folder
println $CYAN "Step 1: Running 'make' in $PROJECT_DIR"
cd $PROJECT_DIR || exit 1
make

# Step 3: CMake configuration
println $CYAN "Step 2: Running 'cmake ..'"
cd $OUTPUT_DIR || { println $RED "Output directory missing"; exit 1; }
cmake ..

# Step 4: Clean and Build
println $CYAN "Step 3: make clean"
make clean

println $CYAN "Step 4: Building project with $N_CPU cores"
make -j$N_CPU

# Step 5: Setup CAN interface
println $CYAN "Step 5: Setting up CAN interface"
DEV=$(ls /dev/ttyACM* 2>/dev/null | head -n 1)

if [ -z "$DEV" ]; then
    println $RED "No Device CAN /dev/ttyACM* device found!"
    exit 1
fi

println $GREEN "Using device: $DEV"
sudo slcand -o -c -s6 "$DEV" slcan0
sleep 1
sudo ip link set up slcan0

println $GREEN "Build complete and CAN interface is ready!"
