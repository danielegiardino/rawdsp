#!/bin/bash

# ── Colors ──────────────────────────────────────────────
RESET="\033[0m"
BOLD="\033[1m"
DIM="\033[2m"

BLACK="\033[30m"
WHITE="\033[97m"
RED="\033[91m"
GREEN="\033[92m"
YELLOW="\033[93m"
BLUE="\033[94m"
CYAN="\033[96m"
MAGENTA="\033[95m"

BG_BLUE="\033[44m"
BG_GREEN="\033[42m"
BG_RED="\033[41m"
BG_YELLOW="\033[43m"
BG_CYAN="\033[46m"
BG_MAGENTA="\033[45m"

# ── Symbols ──────────────────────────────────────────────
CHECK="✔"
CROSS="✘"
ARROW="▶"
DOT="•"

# ── Bar colors cycle ─────────────────────────────────────
BAR_COLORS=("\033[44m" "\033[45m" "\033[46m" "\033[43m" "\033[42m" "\033[41m")
BAR_NUM=${#BAR_COLORS[@]}

# ── Helpers ──────────────────────────────────────────────
print_divider() {
    local index=$1
    local color=${BAR_COLORS[$((index % BAR_NUM))]}
    local cols=${COLUMNS:-80}
    local bar=""
    for ((i = 0; i < cols; i++)); do bar+=" "; done
    echo -e "${color}${bar}${RESET}"
}

print_header() {
    local cols=${COLUMNS:-80}
    local bar=""
    for ((i = 0; i < cols; i++)); do bar+=" "; done
    echo ""
    echo -e "${BOLD}${BG_BLUE}${WHITE}  ⚡ BATCH RUNNER${bar:16}${RESET}"
    echo -e "${DIM}${BLUE}$(printf '─%.0s' $(seq 1 $cols))${RESET}"
}

print_footer() {
    local passed=$1 failed=$2 total=$3
    local cols=${COLUMNS:-80}
    echo -e "${DIM}${BLUE}$(printf '─%.0s' $(seq 1 $cols))${RESET}"
    echo -e "${BOLD}  Summary:${RESET}  ${GREEN}${CHECK} ${passed} passed ${RESET}  ${DIM}|${RESET}  ${RED}${CROSS} ${failed} failed ${RESET}  ${DIM}|${RESET}  ${CYAN}${total} total${RESET}"
    echo ""
}

print_run_start() {
    local index=$1 name=$2
    echo ""
    echo -e "${BOLD}${BLUE}  [${index}]${RESET} ${BOLD}${name}${RESET}"
    echo -e "  ${DIM}${ARROW} Starting...${RESET}"
}

print_run_end() {
    local name=$1 code=$2 duration=$3
    if [ "$code" -eq 0 ]; then
        echo -e "  ${GREEN}${BG_GREEN}${BLACK} ${CHECK} PASSED ${RESET}${GREEN} exited with code ${code}${RESET}  ${DIM}(${duration}s)${RESET}"
    else
        echo -e "  ${RED}${BG_RED}${WHITE} ${CROSS} FAILED ${RESET}${RED} exited with code ${code}${RESET}  ${DIM}(${duration}s)${RESET}"
    fi
}

# ── Main ─────────────────────────────────────────────────
FOLDER="${1:-.}"

if [ ! -d "$FOLDER" ]; then
    echo -e "\n  ${BG_RED}${WHITE} ERROR ${RESET} ${RED}'${FOLDER}' is not a valid directory.${RESET}\n"
    exit 1
fi

executables=$(find "$FOLDER" -maxdepth 1 -type f -executable)

if [ -z "$executables" ]; then
    echo -e "\n  ${BG_YELLOW}${BLACK} WARN ${RESET} ${YELLOW}No executables found in '${FOLDER}'.${RESET}\n"
    exit 0
fi

total=$(echo "$executables" | wc -l | tr -d ' ')
passed=0
failed=0
index=1

print_header
echo -e "  ${DIM}${DOT} Directory: ${FOLDER}${RESET}"
echo -e "  ${DIM}${DOT} Found ${total} executable(s)${RESET}"

while IFS= read -r exe; do
    name=$(basename "$exe")

    echo ""
    # print_divider "$index"
    echo -e "${BAR_COLORS[$((index % BAR_NUM))]}${WHITE}${BOLD}  ${ARROW} Program ${index}/${total}: ${name}  ${RESET}"
    # print_divider "$index"

    print_run_start "$index" "$name"

    start=$(date +%s)
    "$exe"
    code=$?
    end=$(date +%s)
    duration=$((end - start))

    print_run_end "$name" "$code" "$duration"

    if [ "$code" -eq 0 ]; then
        ((passed++)) || true
    else
        ((failed++)) || true
    fi

    ((index++)) || true
done <<< "$executables"

echo ""
print_divider "0"
print_footer "$passed" "$failed" "$total"